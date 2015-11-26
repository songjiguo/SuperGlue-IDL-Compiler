#ifndef _COS_IDL_EVT_H
#define _COS_IDL_EVT_H

#include "cidl_gen.h"

service_global_info = {
	service		       = evt,

        desc_close_self_only   = true,
        desc_dep_create_same   = true,
        desc_dep_close_removal = true,
        desc_global	       = true,
        desc_block	       = true,
        desc_has_data	       = true,
        resc_has_data	       = false,
};

sm_creation(evt_split);
sm_transition(evt_split, evt_wait);
sm_transition(evt_wait, evt_trigger);
sm_transition(evt_trigger, evt_wait);
sm_transition(evt_trigger, evt_free);
sm_transition(evt_split, evt_free);
sm_terminal(evt_free);

sm_block(evt_wait);
sm_wakeup(evt_trigger);

desc_data_retval(long, evtid)
evt_split(spdid_t desc_data(spdid), 
	  long desc_data(parent_desc(parent_evtid)), 
	  int desc_data(grp));

long 
evt_wait(spdid_t spdid, 
	 long desc(evtid));

int
evt_trigger(spdid_t spdid, 
	    long desc(evtid));

int 
evt_free(spdid_t spdid, 
	 long desc(evtid));

#endif /* _COS_IDL_EVT_H */

