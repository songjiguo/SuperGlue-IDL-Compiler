#ifndef _COS_IDL_EVT_H
#define _COS_IDL_EVT_H

#include "cidl_gen.h"

service_global_info = {
	SERVICE		= evt,
        DESC_CLOSE      = itself,
        DESC_DEP_CREATE = same,
        DESC_DEP_CLOSE 	= removal,
        DESC_GLOBAL 	= true,
        DESC_BLOCK 	= true,
        DESC_HAS_DATA 	= true,
        RESC_HAS_DATA 	= false,
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
