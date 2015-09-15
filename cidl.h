#ifndef _COS_IDL_H
#define _COS_IDL_H

#include "cidl_gen.h"

//struct service_tuple {
	//desc_close(itself);
	//desc_dep_create(same);
	//desc_dep_close(keep);
	//desc_global(false);
	//desc_block(true, lock_component);
	//desc_has_data(true);
	//resc_has_data(true);
	
	//sm_create(tsplit);
	//sm_terminate(trelease);
	//sm_mutate(treadp);
	//sm_mutate(twritep);
	
//}

struct tuple {
	desc_close         itself;
	desc_dep_create    same;
	desc_dep_close     keep;
	desc_global        false;
	desc_block         true;
	desc_has_data      true;
	resc_has_data      true;
};

struct func_sm {
	tsplit_sm      create;
	trelease_sm	   terminate;
	treadp_sm	   mutate;
	twritep_sm	   mutate;
};

struct desc_data {
	td_t tid;
	td_t parent_tid;
	char *param;
	int len;
	spdid_t spdid;
	tor_flags_t tflags;
	evt_t evtid;
	int offset;	
};

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
 	   int *desc_data_add(offset, ret(sz))){;} 

/* twritep returns written bytes */
int
twritep(spdid_t spdid,
		td_t desc(tid),
 		int resc_data(tid, cbid),
 		int sz){;}
	   
void
trelease(spdid_t spdid,
 		 td_t desc_data_remove(tid)){;}

#endif /* _COS_IDL_H */
