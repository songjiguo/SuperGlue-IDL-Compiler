#include "cidl_gen.h"

struct desc_track {
	spdid_t spdid;
	td_t parent_tid;
	char *param;
	int len;
	tor_flags_t tflags;
	evt_t evtid;
	td_t tid;
	int offset;
	unsigned int state;
	unsigned int next_state;
	int server_tid;
	unsigned long long fault_cnt;
};

static volatile unsigned long global_fault_cnt = 0;

/* tracking thread state for data recovery */
CVECT_CREATE_STATIC(rd_vect);
CSLAB_CREATE(rdservice, sizeof(struct desc_track));

// assumption: at most one pointer is passed at a time
struct __sg_tsplit_marshalling {
	spdid_t spdid;
	td_t parent_tid;
	char *param;
	int len;
	tor_flags_t tflags;
	evt_t evtid;
	char data[0];
};

enum state_codes { state_tsplit, state_trelease, state_treadp, state_twritep,
	    state_null };

static inline void block_cli_if_recover(int id);
static inline void block_cli_if_basic_id(int id);
static inline void call_restore_data(struct desc_track *desc);
static inline void call_save_data(int id, void *data);
static inline void block_cli_if_invoke_tsplit(spdid_t spdid, td_t parent_tid,
					      char *param, int len,
					      tor_flags_t tflags, evt_t evtid);
static inline void block_cli_if_marshalling_invoke_tsplit(spdid_t spdid,
							  td_t parent_tid,
							  char *param, int len,
							  tor_flags_t tflags,
							  evt_t evtid,
							  struct
							  __sg_tsplit_marshalling
							  *md, int sz,
							  cbuf_t cb);
static inline void block_cli_if_desc_update_tsplit();
static inline void block_cli_if_track_tsplit(int ret, spdid_t spdid,
					     td_t parent_tid, char *param,
					     int len, tor_flags_t tflags,
					     evt_t evtid);
static inline void block_cli_if_invoke_treadp(spdid_t spdid, td_t tid,
					      int *_retval_cbuf_off,
					      int *_retval_sz);

static inline void block_cli_if_desc_update_treadp(int id);
static inline void block_cli_if_track_treadp(int ret, spdid_t spdid, td_t tid,
					     int *_retval_cbuf_off,
					     int *_retval_sz);
static inline void block_cli_if_invoke_twritep(spdid_t spdid, td_t tid,
					       int cbid, int sz);

static inline void block_cli_if_desc_update_twritep(int id);
static inline void block_cli_if_track_twritep(int ret, spdid_t spdid, td_t tid,
					      int cbid, int sz);
static inline void block_cli_if_invoke_trelease(spdid_t spdid, td_t tid);

static inline void block_cli_if_desc_update_trelease(int id);
static inline void block_cli_if_track_trelease(int ret, spdid_t spdid,
					       td_t tid);

static inline struct desc_track *call_desc_lookup(int id)
{
	return (struct desc_track *)cvect_lookup(&rd_vect, id);
}

static inline struct desc_track *call_desc_alloc(int id)
{
	struct desc_track *_desc_track;

	_desc_track = (struct desc_track *)cslab_alloc_rdservice();
	assert(_desc_track);
	if (cvect_add(&rd_vect, _desc_track, id)) {
		assert(0);
	}
	_desc_track->tid = id;
	return _desc_track;
}

static inline void call_desc_dealloc(struct desc_track *desc)
{
	assert(desc);
	assert(!cvect_del(&rd_vect, desc->tid));
	cslab_free_rdservice(desc);
}

static inline void call_desc_cons(struct desc_track *desc, int id,
				  spdid_t spdid, td_t parent_tid, char *param,
				  int len, tor_flags_t tflags, evt_t evtid)
{
	assert(desc);

	desc->tid = id;
	desc->server_tid = id;
	desc->spdid = spdid;
	desc->parent_tid = parent_tid;
	desc->param = param;
	desc->len = len;
	desc->tflags = tflags;
	desc->evtid = evtid;

	return;
}

static inline struct desc_track *call_desc_update(int id, int next_state)
{
	struct desc_track *desc = NULL;
	unsigned int from_state = 0;
	unsigned int to_state = 0;

	desc = call_desc_lookup(id);
	if (unlikely(!desc))
		goto done;

	from_state = desc->state;
	to_state = next_state;
	desc->next_state = next_state;

	if (likely(desc->fault_cnt == global_fault_cnt))
		goto done;
	desc->fault_cnt = global_fault_cnt;

	// State machine transition under the fault
	block_cli_if_recover(id);

	if ((from_state == state_tsplit) && (to_state == state_trelease)) {

		goto done;
	}
	if ((from_state == state_tsplit) && (to_state == state_treadp)) {

		goto done;
	}
	if ((from_state == state_treadp) && (to_state == state_treadp)) {

		goto done;
	}
	if ((from_state == state_treadp) && (to_state == state_trelease)) {

		goto done;
	}
	if ((from_state == state_tsplit) && (to_state == state_twritep)) {

		goto done;
	}
	if ((from_state == state_twritep) && (to_state == state_trelease)) {

		goto done;
	}

 done:
	return desc;
}

static inline void call_restore_data(struct desc_track *desc)
{
}

static inline void block_cli_if_recover_data(struct desc_track *desc)
{
	assert(desc);
	call_restore_data(desc);
}

static inline void block_cli_if_basic_id(int id)
{
	assert(id);
	struct desc_track *desc = call_desc_lookup(id);
	assert(desc);

	int retval = 0;
	retval =
	    tsplit(desc->spdid, desc->parent_tid, desc->param, desc->len,
		   desc->tflags, desc->evtid);

	//TODO: define the error code for non-recovered parent
	if (retval == -99) {
		id = desc->parent_tid;
		//block_cli_if_recover(id);
		call_desc_update(id, state_tsplit);
	} else {
		desc->server_tid = retval;
	}

	block_cli_if_recover_data(desc);
}

static inline void block_cli_if_recover(int id)
{
	block_cli_if_basic_id(id);
}

static inline void call_save_data(int id, void *data)
{
}

static inline void block_cli_if_save_data(int id, void *data)
{
	call_save_data(id, data);
}

static inline void block_cli_if_track_tsplit(int ret, spdid_t spdid,
					     td_t parent_tid, char *param,
					     int len, tor_flags_t tflags,
					     evt_t evtid)
{
	struct desc_track *desc = call_desc_alloc(ret);
	assert(desc);
	call_desc_cons(desc, ret, spdid, parent_tid, param, len, tflags, evtid);
	desc->state = state_tsplit;
}

static inline void block_cli_if_desc_update_tsplit()
{
}

static inline void block_cli_if_marshalling_invoke_tsplit(spdid_t spdid,
							  td_t parent_tid,
							  char *param, int len,
							  tor_flags_t tflags,
							  evt_t evtid,
							  struct
							  __sg_tsplit_marshalling
							  *md, int sz,
							  cbuf_t cb)
{
	struct desc_track *parent_desc = NULL;
	if ((parent_desc = call_desc_lookup(parent_tid))) {
		parent_tid = parent_desc->server_tid;
	}

	md->spdid = spdid;
	md->parent_tid = parent_tid;
	md->param = param;
	md->len = len;
	md->tflags = tflags;
	md->evtid = evtid;
	CSTUB_INVOKE(ret, fault, uc, 3, spdid, cb, sz);
}

static inline void block_cli_if_track_trelease(int ret, spdid_t spdid, td_t tid)
{
	struct desc_track *desc = call_desc_lookup(tid);
	assert(desc);
	call_desc_dealloc(desc);

	// TODO: this needs to be changed
	/* struct desc_track *child_desc_list = desc->child_desc_list;   */
	/* if (EMPTY_LIST(child_desc_list)) { */
	/*      call_desc_dealloc(desc); */
	/* } */
}

static inline void block_cli_if_desc_update_trelease(int id)
{
	call_desc_update(id, state_trelease);
}

static inline void block_cli_if_invoke_trelease(spdid_t spdid, td_t tid)
{
	struct desc_track *desc = call_desc_lookup(tid);
	assert(desc);		// must be created in the same component
	CSTUB_INVOKE(ret, fault, uc, 2, desc->spdid, desc->tid);
}

static inline void block_cli_if_track_twritep(int ret, spdid_t spdid, td_t tid,
					      int cbid, int sz)
{
	struct desc_track *desc = call_desc_lookup(tid);
	assert(desc);
	desc->state = state_twritep;
}

static inline void block_cli_if_desc_update_twritep(int id)
{
	call_desc_update(id, state_twritep);
}

static inline void block_cli_if_invoke_twritep(spdid_t spdid, td_t tid,
					       int cbid, int sz)
{
	struct desc_track *desc = call_desc_lookup(tid);
	assert(desc);		// must be created in the same component
	CSTUB_INVOKE(ret, fault, uc, 4, desc->spdid, desc->tid, cbid, sz);
}

static inline void block_cli_if_track_treadp(int ret, spdid_t spdid, td_t tid,
					     int *_retval_cbuf_off,
					     int *_retval_sz)
{
	struct desc_track *desc = call_desc_lookup(tid);
	assert(desc);
	desc->state = state_treadp;
}

static inline void block_cli_if_desc_update_treadp(int id)
{
	call_desc_update(id, state_treadp);
}

static inline void block_cli_if_invoke_treadp(spdid_t spdid, td_t tid,
					      int *_retval_cbuf_off,
					      int *_retval_sz)
{
	struct desc_track *desc = call_desc_lookup(tid);
	assert(desc);		// must be created in the same component
	CSTUB_INVOKE(ret, fault, uc, 4, desc->spdid, desc->tid,
		     _retval_cbuf_off, _retval_sz);
}

CSTUB_FN(td_t, tsplit) (struct usr_inv_cap * uc, spdid_t spdid, td_t parent_tid,
			char *param, int len, tor_flags_t tflags, evt_t evtid) {
	struct desc_track *desc = NULL;
	struct __sg_tsplit_marshalling *md = NULL;
	cbuf_t cb = 0;
	int sz = len + sizeof(struct __sg_tsplit_marshalling);
 redo:
	block_cli_if_desc_update_tsplit();

	md = (struct __sg_tsplit_marshalling *)cbuf_alloc(sz, &cb);
	assert(md);		// assume we always get cbuf for now

	block_cli_if_marshalling_invoke_tsplit(spdid, parent_tid, param, len,
					       tflags, evtid, md, sz, cb);

	if (unlikely(fault)) {
		CSTUB_FAULT_UPDATE();
		cbuf_free(cb);
		goto redo;
	}
	cbuf_free(cb);

	block_cli_if_track_tsplit(ret, spdid, parent_tid, param, len, tflags,
				  evtid);

	return ret;
}

CSTUB_FN(int, trelease)(struct usr_inv_cap * uc, spdid_t spdid, td_t tid) {
	struct desc_track *desc = NULL;
 redo:
	block_cli_if_desc_update_trelease(tid);
	block_cli_if_invoke_trelease(spdid, tid);
	if (unlikely(fault)) {
		CSTUB_FAULT_UPDATE();
		goto redo;
	}
	block_cli_if_track_trelease(ret, spdid, tid);

	return ret;
}

CSTUB_FN(int, twritep)(struct usr_inv_cap * uc, spdid_t spdid, td_t tid,
		       int cbid, int sz) {
	struct desc_track *desc = NULL;
 redo:
	block_cli_if_desc_update_twritep(tid);
	block_cli_if_invoke_twritep(spdid, tid, cbid, sz);
	if (unlikely(fault)) {
		CSTUB_FAULT_UPDATE();
		goto redo;
	}
	block_cli_if_track_twritep(ret, spdid, tid, cbid, sz);

	return ret;
}

CSTUB_FN(int, treadp)(struct usr_inv_cap * uc, spdid_t spdid, td_t tid,
		      int *_retval_cbuf_off, int *_retval_sz) {
	struct desc_track *desc = NULL;
 redo:
	block_cli_if_desc_update_treadp(tid);
	block_cli_if_invoke_treadp(spdid, tid, _retval_cbuf_off, _retval_sz);
	if (unlikely(fault)) {
		CSTUB_FAULT_UPDATE();
		goto redo;
	}
	block_cli_if_track_treadp(ret, spdid, tid, _retval_cbuf_off,
				  _retval_sz);

	return ret;
}

/* this is just a fake main function for testing. Remove it later  */
int main()
{
	return 0;
}
