#ifndef _COS_IDL_SCHED_H
#define _COS_IDL_SCHED_H

#include "cidl_gen.h"

service_global_info = {
	service		     = sched,

        desc_close_self_only = true,
        desc_dep_create_none = true,
        desc_global	     = true,
        desc_block	     = false,
        desc_has_data	     = false,
        resc_has_data	     = true,
};

sm_creation(sched_create_thd);
sm_transition(sched_create_thd, sched_block);
sm_transition(sched_block, sched_wakeup);
sm_transition(sched_wakeup, sched_block);

sm_block(sched_block);
sm_wakeup(sched_wakeup);

desc_data_retval(int, thdid)
sched_create_thd(spdid_t spdid, 
		 u32_t sched_param0, 
		 u32_t sched_param1, 
		 u32_t sched_param2);

int 
sched_block(spdid_t spdid, 
	    unsigned short int desc_data(dependency_thd));

int 
sched_wakeup(spdid_t spdid, 
	     unsigned short int thdid);

int
sched_component_take(spdid_t spdid);

int
sched_component_release(spdid_t spdid);

int
sched_timeout(spdid_t spdid, 
	      unsigned long amnt);

/* unsigned long */
/* sched_timestamp(); */

#endif /* _COS_IDL_SCHED_H */
