#include "cidl_gen.h"

struct desc_track {
	spdid_t spdid;
	ul_t lock_id;
	u32_t thd_id;
	unsigned int state;
	unsigned int next_state;
	int server_lock_id;
	unsigned long long fault_cnt;
};

static volatile unsigned long global_fault_cnt = 0;

static int first_map_init = 0;

COS_MAP_CREATE_STATIC(lock_desc_maps);
CSLAB_CREATE(lock_slab, sizeof(struct desc_track));

static inline struct desc_track *call_desc_lookup(int id)
{
	return (struct desc_track *)cos_map_lookup(&lock_desc_maps, id);
}

static inline struct desc_track *call_desc_alloc()
{
	struct desc_track *desc = NULL;
	int map_id = 0;

	while (1) {
		desc = cslab_alloc_lock_slab();
		assert(desc);
		map_id = cos_map_add(&lock_desc_maps, desc);
		desc->lock_id = map_id;
		desc->server_lock_id = -1;	// reset to -1
		if (map_id >= 2)
			break;
	}
	assert(desc && desc->lock_id >= 1);
	return desc;
}

static inline void call_desc_dealloc(struct desc_track *desc)
{
	assert(desc);
	int id = desc->lock_id;
	desc->server_lock_id = -1;	// reset to -1
	assert(desc);
	cslab_free_lock_slab(desc);
	cos_map_del(&lock_desc_maps, id);
	return;
}

enum state_codes { state_lock_component_alloc, state_lock_component_free,
	    state_lock_component_take, state_lock_component_pretake,
	    state_lock_component_release, state_null };

static inline struct desc_track *call_desc_update(int id, int next_state);
static inline void call_map_init();
static inline void block_cli_if_recover(int id);
static inline void block_cli_if_basic_id(int id);
static inline void block_cli_if_upcall_creator(int id);
static inline void block_cli_if_recover_data(struct desc_track *desc);
static inline int block_cli_if_invoke_lock_component_alloc(spdid_t spdid,
							   int ret, long *fault,
							   struct usr_inv_cap
							   *uc);
static inline void block_cli_if_desc_update_lock_component_alloc();
static inline int block_cli_if_desc_update_post_fault_lock_component_alloc();
static inline int block_cli_if_track_lock_component_alloc(int ret,
							  spdid_t spdid);
static inline void call_desc_cons(struct desc_track *desc, int id,
				  spdid_t spdid);
static inline int block_cli_if_invoke_lock_component_take(spdid_t spdid,
							  ul_t lock_id,
							  u32_t thd_id, int ret,
							  long *fault,
							  struct usr_inv_cap
							  *uc);
static inline void block_cli_if_desc_update_lock_component_take(int id);
static inline int block_cli_if_desc_update_post_fault_lock_component_take();
static inline int block_cli_if_track_lock_component_take(int ret, spdid_t spdid,
							 ul_t lock_id,
							 u32_t thd_id);
static inline int block_cli_if_invoke_lock_component_pretake(spdid_t spdid,
							     ul_t lock_id,
							     u32_t thd_id,
							     int ret,
							     long *fault,
							     struct usr_inv_cap
							     *uc);
static inline void block_cli_if_desc_update_lock_component_pretake(int id);
static inline int block_cli_if_desc_update_post_fault_lock_component_pretake();
static inline int block_cli_if_track_lock_component_pretake(int ret,
							    spdid_t spdid,
							    ul_t lock_id,
							    u32_t thd_id);
static inline int block_cli_if_invoke_lock_component_release(spdid_t spdid,
							     ul_t lock_id,
							     int ret,
							     long *fault,
							     struct usr_inv_cap
							     *uc);
static inline void block_cli_if_desc_update_lock_component_release(int id);
static inline int block_cli_if_desc_update_post_fault_lock_component_release();
static inline int block_cli_if_track_lock_component_release(int ret,
							    spdid_t spdid,
							    ul_t lock_id);
static inline int block_cli_if_invoke_lock_component_free(spdid_t spdid,
							  ul_t lock_id, int ret,
							  long *fault,
							  struct usr_inv_cap
							  *uc);
static inline void block_cli_if_desc_update_lock_component_free(int id);
static inline void block_cli_if_desc_update_lock_component_free(int id);
static inline int block_cli_if_desc_update_post_fault_lock_component_free();
static inline int block_cli_if_desc_update_post_fault_lock_component_free();
static inline void block_cli_if_recover_subtree(int id);
static inline int block_cli_if_track_lock_component_free(int ret, spdid_t spdid,
							 ul_t lock_id);

static inline void call_map_init()
{
	if (unlikely(!first_map_init)) {
		first_map_init = 1;
		cos_map_init_static(&lock_desc_maps);
	}
	return;
}

static inline void block_cli_if_recover_data(struct desc_track *desc)
{
}

static inline void call_desc_cons(struct desc_track *desc, int id,
				  spdid_t spdid)
{
	assert(desc);

	desc->server_lock_id = id;

	desc->spdid = spdid;

	desc->fault_cnt = global_fault_cnt;

	return;
}

static inline void block_cli_if_basic_id(int id)
{

	assert(id);
	struct desc_track *desc = call_desc_lookup(id);
	assert(desc);

	int retval = 0;
	retval = lock_component_alloc(desc->spdid);
	assert(retval);

	struct desc_track *new_desc = call_desc_lookup(retval);
	assert(new_desc);

	desc->server_lock_id = new_desc->server_lock_id;
	desc->state = state_lock_component_alloc;	// set the state to the initial state
	desc->fault_cnt = global_fault_cnt;	// set the fault counter to the global

	call_desc_dealloc(new_desc);
	block_cli_if_recover_data(desc);
}

static inline void block_cli_if_recover(int id)
{
	block_cli_if_basic_id(id);
}

static inline void block_cli_if_upcall_creator(int id)
{
}

static inline struct desc_track *call_desc_update(int id, int next_state)
{
	struct desc_track *desc = NULL;
	unsigned int from_state = 0;
	unsigned int to_state = 0;

	if (id == 0)
		return NULL;	/* root id */

	desc = call_desc_lookup(id);
	if (unlikely(!desc)) {
		block_cli_if_upcall_creator(id);
		goto done;
	}

	desc->next_state = next_state;

	if (likely(desc->fault_cnt == global_fault_cnt))
		goto done;
	/* desc->fault_cnt = global_fault_cnt; */

	// State machine transition under the fault
	block_cli_if_recover(id);
	from_state = desc->state;
	to_state = next_state;

	if ((from_state == state_lock_component_alloc)
	    && (to_state == state_lock_component_take)) {
		lock_component_pretake(desc->spdid, desc->lock_id,
				       desc->thd_id);

		goto done;
	}

	if ((from_state == state_lock_component_alloc)
	    && (to_state == state_lock_component_release)) {
		lock_component_pretake(desc->spdid, desc->lock_id,
				       desc->thd_id);
		lock_component_take(desc->spdid, desc->lock_id, desc->thd_id);

		goto done;
	}

 done:
	return desc;
}

static inline int block_cli_if_track_lock_component_pretake(int ret,
							    spdid_t spdid,
							    ul_t lock_id,
							    u32_t thd_id)
{
	struct desc_track *desc = call_desc_lookup(lock_id);
	if (desc) {
	}
	return ret;
}

static inline void block_cli_if_desc_update_lock_component_pretake(int id)
{
	call_desc_update(id, state_lock_component_pretake);
}

static inline int block_cli_if_desc_update_post_fault_lock_component_pretake()
{
	return 1;
}

static inline int block_cli_if_invoke_lock_component_pretake(spdid_t spdid,
							     ul_t lock_id,
							     u32_t thd_id,
							     int ret,
							     long *fault,
							     struct usr_inv_cap
							     *uc)
{
	long __fault = 0;
	struct desc_track *desc = call_desc_lookup(lock_id);

	CSTUB_INVOKE(ret, __fault, uc, 3, spdid, desc->server_lock_id, thd_id);
	*fault = __fault;

	return ret;
}

static inline int block_cli_if_track_lock_component_release(int ret,
							    spdid_t spdid,
							    ul_t lock_id)
{
	struct desc_track *desc = call_desc_lookup(lock_id);
	if (desc) {
	}
	return ret;
}

static inline void block_cli_if_desc_update_lock_component_release(int id)
{
	call_desc_update(id, state_lock_component_release);
}

static inline int block_cli_if_desc_update_post_fault_lock_component_release()
{
	return 1;
}

static inline int block_cli_if_invoke_lock_component_release(spdid_t spdid,
							     ul_t lock_id,
							     int ret,
							     long *fault,
							     struct usr_inv_cap
							     *uc)
{
	long __fault = 0;
	struct desc_track *desc = call_desc_lookup(lock_id);

	CSTUB_INVOKE(ret, __fault, uc, 2, spdid, desc->server_lock_id);
	*fault = __fault;

	return ret;
}

static inline int block_cli_if_track_lock_component_take(int ret, spdid_t spdid,
							 ul_t lock_id,
							 u32_t thd_id)
{
	struct desc_track *desc = call_desc_lookup(lock_id);
	if (desc) {
	}
	return ret;
}

static inline void block_cli_if_desc_update_lock_component_take(int id)
{
	call_desc_update(id, state_lock_component_take);
}

static inline int block_cli_if_desc_update_post_fault_lock_component_take()
{
	return 1;
}

static inline int block_cli_if_invoke_lock_component_take(spdid_t spdid,
							  ul_t lock_id,
							  u32_t thd_id, int ret,
							  long *fault,
							  struct usr_inv_cap
							  *uc)
{
	long __fault = 0;
	struct desc_track *desc = call_desc_lookup(lock_id);

	CSTUB_INVOKE(ret, __fault, uc, 3, spdid, desc->server_lock_id, thd_id);
	*fault = __fault;

	return ret;
}

static inline int block_cli_if_track_lock_component_alloc(int ret,
							  spdid_t spdid)
{
	// if ret does not exist, just return as it is, thinking....
	if (ret == -EINVAL)
		return ret;

	struct desc_track *desc = call_desc_alloc();
	assert(desc);
	call_desc_cons(desc, ret, spdid);
	desc->state = state_lock_component_alloc;

	return desc->lock_id;
}

static inline void block_cli_if_desc_update_lock_component_alloc()
{
}

static inline int block_cli_if_desc_update_post_fault_lock_component_alloc()
{
	return 1;
}

static inline int block_cli_if_invoke_lock_component_alloc(spdid_t spdid,
							   int ret, long *fault,
							   struct usr_inv_cap
							   *uc)
{
	long __fault = 0;
	CSTUB_INVOKE(ret, __fault, uc, 1, spdid);
	*fault = __fault;
	return ret;
}

static inline void block_cli_if_desc_update_lock_component_free(int id)
{
	call_desc_update(id, state_lock_component_free);
}

static inline void block_cli_if_recover_subtree(int id)
{
}

static inline int block_cli_if_desc_update_post_fault_lock_component_free()
{
	return 1;
}

static inline int block_cli_if_invoke_lock_component_free(spdid_t spdid,
							  ul_t lock_id, int ret,
							  long *fault,
							  struct usr_inv_cap
							  *uc)
{
	long __fault = 0;
	struct desc_track *desc = call_desc_lookup(lock_id);

	CSTUB_INVOKE(ret, __fault, uc, 2, spdid, desc->server_lock_id);
	*fault = __fault;

	return ret;
}

static inline int block_cli_if_track_lock_component_free(int ret, spdid_t spdid,
							 ul_t lock_id)
{
	struct desc_track *desc = call_desc_lookup(lock_id);
	if (desc)
		call_desc_dealloc(desc);

	return ret;
}

CSTUB_FN(int, lock_component_pretake)(struct usr_inv_cap * uc, spdid_t spdid,
				      ul_t lock_id, u32_t thd_id) {
	long fault = 0;
	int ret = 0;

	call_map_init();

 redo:
	block_cli_if_desc_update_lock_component_pretake(lock_id);

	ret =
	    block_cli_if_invoke_lock_component_pretake(spdid, lock_id, thd_id,
						       ret, &fault, uc);
	if (unlikely(fault)) {

		CSTUB_FAULT_UPDATE();
		if (block_cli_if_desc_update_post_fault_lock_component_pretake
		    ()) {
			goto redo;
		}
	}
	ret =
	    block_cli_if_track_lock_component_pretake(ret, spdid, lock_id,
						      thd_id);

	if (unlikely(ret == -ELOOP))
		goto redo;

	return ret;
}

CSTUB_FN(int, lock_component_release)(struct usr_inv_cap * uc, spdid_t spdid,
				      ul_t lock_id) {
	long fault = 0;
	int ret = 0;

	call_map_init();

	ret =
	    block_cli_if_invoke_lock_component_release(spdid, lock_id, ret,
						       &fault, uc);
	if (unlikely(fault)) {

		CSTUB_FAULT_UPDATE();
		block_cli_if_desc_update_lock_component_release(lock_id);

		return ret;
	}
	ret = block_cli_if_track_lock_component_release(ret, spdid, lock_id);

	return ret;
}

CSTUB_FN(int, lock_component_take)(struct usr_inv_cap * uc, spdid_t spdid,
				   ul_t lock_id, u32_t thd_id) {
	long fault = 0;
	int ret = 0;

	call_map_init();

	ret =
	    block_cli_if_invoke_lock_component_take(spdid, lock_id, thd_id, ret,
						    &fault, uc);
	if (unlikely(fault)) {

		CSTUB_FAULT_UPDATE();
		block_cli_if_desc_update_lock_component_take(lock_id);

		return ret;
	}
	ret =
	    block_cli_if_track_lock_component_take(ret, spdid, lock_id, thd_id);

	return ret;
}

CSTUB_FN(ul_t, lock_component_alloc) (struct usr_inv_cap * uc, spdid_t spdid) {
	long fault = 0;
	int ret = 0;

	call_map_init();

 redo:
	block_cli_if_desc_update_lock_component_alloc();

	ret = block_cli_if_invoke_lock_component_alloc(spdid, ret, &fault, uc);
	if (unlikely(fault)) {

		CSTUB_FAULT_UPDATE();
		if (block_cli_if_desc_update_post_fault_lock_component_alloc()) {
			goto redo;
		}
	}
	ret = block_cli_if_track_lock_component_alloc(ret, spdid);

	if (unlikely(ret == -ELOOP))
		goto redo;

	return ret;
}

CSTUB_FN(int, lock_component_free)(struct usr_inv_cap * uc, spdid_t spdid,
				   ul_t lock_id) {
	long fault = 0;
	int ret = 0;

	call_map_init();

 redo:
	block_cli_if_desc_update_lock_component_free(lock_id);

	ret =
	    block_cli_if_invoke_lock_component_free(spdid, lock_id, ret, &fault,
						    uc);
	if (unlikely(fault)) {

		CSTUB_FAULT_UPDATE();
		if (block_cli_if_desc_update_post_fault_lock_component_free()) {
			goto redo;
		}
	}
	ret = block_cli_if_track_lock_component_free(ret, spdid, lock_id);

	if (unlikely(ret == -ELOOP))
		goto redo;

	return ret;
}

    /* this is just a fake main function for testing. Remove it later  */
int main()
{
	return 0;
}
