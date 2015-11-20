#ifndef _COS_IDL_PTE_H
#define _COS_IDL_PTE_H

#include "cidl_gen.h"

service_global_info = {
	service		       = periodic_wake,

        desc_close_self_only   = true,
        desc_dep_create_none   = true,
        desc_global	       = false,
        desc_block	       = true,
        desc_has_data	       = true,
        resc_has_data	       = false,
};

sm_creation(periodic_wake_create);
sm_transition(periodic_wake_create, periodic_wake_wait);
sm_transition(periodic_wake_wait, periodic_wake_wait);
sm_transition(periodic_wake_wait, periodic_wake_remove);
sm_transition(periodic_wake_create, periodic_wake_remove);
sm_terminal(periodic_wake_remove);

int
periodic_wake_create(spdid_t desc_data(spdid),
		     unsigned int desc_data(period));

int
periodic_wake_wait(spdid_t spdid);

int
periodic_wake_remove(spdid_t spdid, 
		     unsigned short int desc(thdid));

#endif  // _COS_IDL_PTE_H
