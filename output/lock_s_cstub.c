#include "cidl_gen.h"

struct track_block {
	int lock_id;
	struct track_block *next, *prev;
};
struct track_block tracking_block_list[MAX_NUM_SPDS];

static inline int block_ser_if_block_track_lock_component_take(spdid_t spd,
							       ul_t lock_id,
							       u32_t thd_id)
{
	int ret = 0;
	struct track_block tb;	// track on stack

	if (unlikely(!tracking_block_list[spd].next)) {
		INIT_LIST(&tracking_block_list[spd], next, prev);
	}
	INIT_LIST(&tb, next, prev);
	tb.lock_id = lock_id;
	ADD_LIST(&tracking_block_list[spd], &tb, next, prev);
	ret = lock_component_take(spd, lock_id, thd_id);
	REM_LIST(&tb, next, prev);

	return ret;
}

int __ser_lock_component_take(spdid_t spd, ul_t lock_id, u32_t thd_id)
{
	return block_ser_if_block_track_lock_component_take(spd, lock_id,
							    thd_id);
}

static inline void block_ser_if_client_fault_notification(int spd)
{
	struct track_block *tb;

	// TAKE LOCK

	for (tb = FIRST_LIST(&tracking_block_list[spd], next, prev);
	     tb != &tracking_block_list[spd]; tb = FIRST_LIST(tb, next, prev)) {
		lock_component_release(spd, tb->lock_id);
	}

	// RELEASE LOCK

	return;
}

void __ser_client_fault_notification(int spd)
{
	return block_ser_if_client_fault_notification(spd);
}

    /* this is just a fake main function for testing. Remove it later  */
int main()
{
	return 0;
}
