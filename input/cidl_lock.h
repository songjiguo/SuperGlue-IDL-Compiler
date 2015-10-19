#ifndef _COS_IDL_H
#define _COS_IDL_H

#include "cidl_gen.h"

service_global_info = {
        DESC_CLOSE 		= itself,
        DESC_DEP_CREATE = single,
        DESC_DEP_CLOSE 	= none,
        DESC_GLOBAL 	= false,
        DESC_BLOCK 		= true,
        DESC_HAS_DATA 	= false,
        RESC_HAS_DATA 	= false,
};

sm_creation(lock_component_alloc);
sm_transition(lock_component_alloc, lock_component_free);
sm_transition(lock_component_alloc, lock_component_take);
sm_transition(lock_component_take, lock_component_release);
sm_transition(lock_component_release, lock_component_take);
sm_transition(lock_component_release, lock_component_free);
sm_terminal(lock_component_free);

desc_data_retval(ul_t, lock_id)
lock_component_alloc(spdid_t desc_data(spd));

int 
lock_component_take(spdid_t spd, 
					ul_t desc(sm_block(lock_id)), 
					u32_t thd_id);

int 
lock_component_release(spdid_t spd, 
					   ul_t desc(sm_wakeup(lock_id)));

int 
lock_component_free(spdid_t spd, 
					ul_t desc_terminate(lock_id));

/*desc_data_retval(td_t, tid)
tsplit(spdid_t desc_data(spdid),
	   td_t desc_data(parent_desc(parent_tid)),
	   char *desc_data(param),
	   int desc_data(size_of(param, len)),
	   tor_flags_t desc_data(tflags),
	   evt_t desc_data(evtid));
*/	   
	   
/* treadp returns cbuf id  */
/*
int
treadp(spdid_t spdid,
       td_t desc(sm_block(tid)),
       int *ret(cbuf_off),
       int *desc_data_add(offset, ret(sz)));
*/
/* twritep returns written bytes */
/*
int
twritep(spdid_t spdid,
	td_t desc(sm_wakeup(tid)),
	int resource_data(tid, cbid), 
	int sz);

int
trelease(spdid_t spdid,
	 td_t desc_terminate(tid));
*/
#endif /* _COS_IDL_H */
