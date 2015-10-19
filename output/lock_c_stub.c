#include "cidl_gen.h"

struct desc_track {
	spdid_t spd;
	ul_t lock_id;
	unsigned int state;
	unsigned int next_state;
	int server_lock_id;
	unsigned long long fault_cnt;
};

static volatile unsigned long global_fault_cnt = 0;

/* tracking thread state for data recovery */
CVECT_CREATE_STATIC(rd_vect);
CSLAB_CREATE(rdservice, sizeof(struct desc_track));

enum state_codes { state_lock_component_alloc, state_lock_component_free,
	    state_lock_component_take, state_lock_component_release,
	    state_null };

static inline void block_cli_if_recover(int id);
static inline void block_cli_if_basic_id(int id);
static inline void block_cli_if_recover_data(struct desc_track *desc);
static inline void block_cli_if_save_data(int id, void *data);
static inline void block_cli_if_invoke_lock_component_alloc(spdid_t spd);
static inline void block_cli_if_desc_update_lock_component_alloc();
static inline void block_cli_if_track_lock_component_alloc(int ret,
							   spdid_t spd);
static inline void block_cli_if_invoke_lock_component_take(spdid_t spd,
							   ul_t lock_id,
							   u32_t thd_id);
static inline void block_cli_if_desc_update_lock_component_take(int id);
static inline void block_cli_if_track_lock_component_take(int ret, spdid_t spd,
							  ul_t lock_id,
							  u32_t thd_id);
static inline void block_cli_if_invoke_lock_component_release(spdid_t spd,
							      ul_t lock_id);
static inline void block_cli_if_desc_update_lock_component_release(int id);
static inline void block_cli_if_track_lock_component_release(int ret,
							     spdid_t spd,
							     ul_t lock_id);
static inline void block_cli_if_invoke_lock_component_free(spdid_t spd,
							   ul_t lock_id);
static inline void block_cli_if_desc_update_lock_component_free(int id);
static inline void block_cli_if_track_lock_component_free(int ret, spdid_t spd,
							  ul_t lock_id);

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
	_desc_track->lock_id = id;
	return _desc_track;
}

static inline void call_desc_dealloc(struct desc_track *desc)
{
	assert(desc);
	assert(!cvect_del(&rd_vect, desc->lock_id));
	cslab_free_rdservice(desc);
}

static inline void call_desc_cons(struct desc_track *desc, int id, spdid_t spd)
{
	assert(desc);

	desc->lock_id = id;
	desc->server_lock_id = id;
	desc->spd = spd;

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

	if ((from_state == state_lock_component_alloc)
	    && (to_state == state_lock_component_free)) {

		goto done;
	}
	if ((from_state == state_lock_component_alloc)
	    && (to_state == state_lock_component_take)) {

		goto done;
	}
	if ((from_state == state_lock_component_take)
	    && (to_state == state_lock_component_release)) {

		goto done;
	}
	if ((from_state == state_lock_component_release)
	    && (to_state == state_lock_component_take)) {

		goto done;
	}
	if ((from_state == state_lock_component_release)
	    && (to_state == state_lock_component_free)) {

		goto done;
	}

 done:
	return desc;
}

static inline void block_cli_if_basic_id(int id)
{

	assert(id);
	struct desc_track *desc = call_desc_lookup(id);
	assert(desc);

	int retval = 0;
	desc->server_lock_id = lock_component_alloc(desc->spd);
	block_cli_if_recover_data(desc);
}

static inline void block_cli_if_recover(int id)
{
	block_cli_if_basic_id(id);
}

static inline void block_cli_if_recover_data(struct desc_track *desc)
{
}

static inline void block_cli_if_save_data(int id, void *data)
{
}

static inline void block_cli_if_track_lock_component_release(int ret,
							     spdid_t spd,
							     ul_t lock_id)
{
	struct desc_track *desc = call_desc_lookup(lock_id);
	assert(desc);
	desc->state = state_lock_component_release;
}

static inline void block_cli_if_desc_update_lock_component_release(int id)
{
	call_desc_update(id, state_lock_component_release);
}

static inline void block_cli_if_invoke_lock_component_release(spdid_t spd,
							      ul_t lock_id)
{
	struct desc_track *desc = call_desc_lookup(lock_id);
	assert(desc);		// must be created in the same component
	CSTUB_INVOKE(ret, fault, uc, 2, desc->spd, desc->lock_id);
}

static inline void block_cli_if_track_lock_component_take(int ret, spdid_t spd,
							  ul_t lock_id,
							  u32_t thd_id)
{
	struct desc_track *desc = call_desc_lookup(lock_id);
	assert(desc);
	desc->state = state_lock_component_take;
}

static inline void block_cli_if_desc_update_lock_component_take(int id)
{
	call_desc_update(id, state_lock_component_take);
}

static inline void block_cli_if_invoke_lock_component_take(spdid_t spd,
							   ul_t lock_id,
							   u32_t thd_id)
{
	struct desc_track *desc = call_desc_lookup(lock_id);
	assert(desc);		// must be created in the same component
	CSTUB_INVOKE(ret, fault, uc, 3, desc->spd, desc->lock_id, thd_id);
}

static inline void block_cli_if_track_lock_component_alloc(int ret, spdid_t spd)
{
	struct desc_track *desc = call_desc_alloc(ret);
	assert(desc);
	call_desc_cons(desc, ret, spd);
	desc->state = state_lock_component_alloc;
}

static inline void block_cli_if_desc_update_lock_component_alloc()
{
}

static inline void block_cli_if_invoke_lock_component_alloc(spdid_t spd)
{
	CSTUB_INVOKE(ret, fault, uc, 1, spd);
}

static inline void block_cli_if_track_lock_component_free(int ret, spdid_t spd,
							  ul_t lock_id)
{
	struct desc_track *desc = call_desc_lookup(lock_id);
	assert(desc);
	call_desc_dealloc(desc);
}

static inline void block_cli_if_desc_update_lock_component_free(int id)
{
	call_desc_update(id, state_lock_component_free);
}

static inline void block_cli_if_invoke_lock_component_free(spdid_t spd,
							   ul_t lock_id)
{
	struct desc_track *desc = call_desc_lookup(lock_id);
	assert(desc);		// must be created in the same component
	CSTUB_INVOKE(ret, fault, uc, 2, desc->spd, desc->lock_id);
}

CSTUB_FN(int, lock_component_release) (struct usr_inv_cap * uc, spdid_t spd,
				       ul_t lock_id) {
	struct desc_track *desc = NULL;
 redo:
	block_cli_if_desc_update_lock_component_release(lock_id);
	block_cli_if_invoke_lock_component_release(spd, lock_id);
	if (unlikely(fault)) {
		CSTUB_FAULT_UPDATE();
		goto redo;
	}
	block_cli_if_track_lock_component_release(ret, spd, lock_id);

	return ret;
}

CSTUB_FN(int, lock_component_take)(struct usr_inv_cap * uc, spdid_t spd,
				   ul_t lock_id, u32_t thd_id) {
	struct desc_track *desc = NULL;
 redo:
	block_cli_if_desc_update_lock_component_take(lock_id);
	block_cli_if_invoke_lock_component_take(spd, lock_id, thd_id);
	if (unlikely(fault)) {
		CSTUB_FAULT_UPDATE();
		goto redo;
	}
	block_cli_if_track_lock_component_take(ret, spd, lock_id, thd_id);

	return ret;
}

CSTUB_FN(ul_t, lock_component_alloc) (struct usr_inv_cap * uc, spdid_t spd) {
	struct desc_track *desc = NULL;
 redo:
	block_cli_if_desc_update_lock_component_alloc();
	block_cli_if_invoke_lock_component_alloc(spd);
	if (unlikely(fault)) {
		CSTUB_FAULT_UPDATE();
		goto redo;
	}
	block_cli_if_track_lock_component_alloc(ret, spd);

	return ret;
}

CSTUB_FN(int, lock_component_free)(struct usr_inv_cap * uc, spdid_t spd,
				   ul_t lock_id) {
	struct desc_track *desc = NULL;
 redo:
	block_cli_if_desc_update_lock_component_free(lock_id);
	block_cli_if_invoke_lock_component_free(spd, lock_id);
	if (unlikely(fault)) {
		CSTUB_FAULT_UPDATE();
		goto redo;
	}
	block_cli_if_track_lock_component_free(ret, spd, lock_id);

	return ret;
}

/* this is just a fake main function for testing. Remove it later  */
int main()
{
	return 0;
}
