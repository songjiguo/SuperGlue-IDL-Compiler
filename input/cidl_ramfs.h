#ifndef _COS_IDL_H
#define _COS_IDL_H

#include "cidl_gen.h"

service_global_info = {
	SERVICE		= ramfs,
        DESC_CLOSE      = itself,
        DESC_DEP_CREATE = same,
        DESC_DEP_CLOSE 	= keep,
        DESC_GLOBAL 	= false,
        DESC_BLOCK 	= true,
        DESC_HAS_DATA 	= true,
        RESC_HAS_DATA 	= true,
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
	   long desc_data(evtid));
	   
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

/* int */
/* trmeta(spdid_t spdid, */
/*        td_t desc(tid), */
/*        const char *key, */
/*        unsigned int klen, */
/*        char *retval, */
/*        unsigned int max_rval_len); */

/* int */
/* twmeta(spdid_t spdid, */
/*        td_t desc(tid), */
/*        const char *key, */
/*        unsigned int klen, */
/*        const char *val, */
/*        unsigned int vlen); */

#endif /* _COS_IDL_H */
