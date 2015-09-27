#ifndef _COS_IDL_H
#define _COS_IDL_H

#include "cidl_gen.h"

service_global_info {
	desc_close         itself;
	desc_dep_create    same;
	desc_dep_close     keep;
	desc_global        false;
	desc_block         true;
	desc_has_data      true;
	resc_has_data      true;
};

sm_creation(tsplit);
sm_transition(tsplit, trelease);
sm_transition(tsplit, treadp);
sm_transition(treadp, treadp);
sm_transition(treadp, trelease);
sm_transition(tsplit, twritep);
sm_transition(twritep, trelease);
sm_terminal(trelease);

desc_data_retval(td_t, tid)
tsplit(spdid_t desc_data(spdid),
	   td_t desc_data(parent_desc(parent_tid)),
	   char *desc_data(param),
	   int desc_data(size_of(param, len)),
	   tor_flags_t desc_data(tflags),
	   evt_t desc_data(evtid));
	   
/* treadp returns cbuf id  */
int
treadp(spdid_t spdid,
       td_t desc(tid),
       int len,
       int *ret(cbuf_off),
       int *desc_data_add(offset, ret(sz)));

/* twritep returns written bytes */
int
twritep(spdid_t spdid,
	td_t desc(tid),
	int resource_data(tid, cbid), 
	int sz);

int
trelease(spdid_t spdid,
	 td_t desc_terminate(tid));

#endif /* _COS_IDL_H */
