#ifndef _COS_IDL_H
#define _COS_IDL_H

// TODO:
// 1) generate strucut desc_data from the annoation
// 2) add something like #START_xxxx_END#

#include "cidl_gen.h"

/* the keywords must be consistent with ones defined in keywords.py */
/*cond_desc_close_itself  = "itself"
  cond_desc_close_subtree  = "subtree"
  cond_desc_create_single  = "nodep"
  cond_desc_create_same  = "same"
  cond_desc_create_diff  = "different"
  cond_desc_close_remove  = "remove"
  cond_desc_close_keep  = "keep"
  cond_desc_global  = "global"
  cond_desc_local  = "local"
  cond_desc_has_data  = "desc_has_data"
  cond_resc_has_data  = "resc_has_data"
  cond_desc_has_no_data  = "desc_has_no_data"
  cond_resc_has_no_data  = "resc_has_no_data"  
*/

//***************************
//  fs
//***************************

struct service_tuple {
	desc_close(itself);
	desc_dep_create(same);
	desc_dep_close(keep);
	desc_global(false);
	desc_block(true, lock_component);
	desc_has_data(true);
	resc_has_data(true);
	
	sm_create(tsplit);
	sm_terminate(trelease);
	sm_mutate(treadp);
	sm_mutate(twritep);
};

/* note: the name of the functoin parameters must be consistent */
struct desc_data {
	td_t tid;
	td_t server_tid;
	
	td_t parent_tid;
	char *param;
	int param_sz;
	spdid_t spdid;
	tor_flags_t tflags;
	evt_t evtid;
	
	int offset;	
	unsigned long long fault_cnt;
};

desc_data_retval(td_t, tid)
tsplit(spdid_t desc_data(spdid),
	   td_t desc_data(parent_desc(parent_tid)),
	   char *desc_data(param),
	   int desc_data(size_of(param, param_sz)),
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



/* //\*************************** */
/* //  lock */
/* //\*************************** */
/* struct service_tuple { */
/* 	desc_close(itself); */
/* 	desc_dep_create(none); */
/* 	desc_dep_close(none); */
/* 	desc_global(false); */
/* 	desc_block(true, scheduler); */
/* 	desc_has_data(none); */
/* 	resc_has_data(none); */
	
/* 	sm_create(lock_component_alloc); */
/* 	sm_terminate(lock_component_free); */
/* 	sm_mutate(lock_component_take); */
/* 	sm_mutate(lock_component_release); */
/* }; */

/* // TODO: generate this */
/* struct desc_data { */
/* 	unsigned long lock_id; */
/* 	spdid_t spdid; */
/* }; */

/* desc_data_retval(ul_t, lock_id) */
/* lock_component_alloc(spdid_t desc_data(spdid)); */

/* int */
/* lock_component_take(spdid_t spd, */
/* 					unsigned long desc(lock_id), */
/* 					unsigned short int thd_id); */

/* int */
/* lock_component_release(spdid_t spd, */
/* 					   unsigned long desc(lock_id)); */

/* desc_data_remove(lock_id) */
/* lock_component_free(spdid_t spdid, unsigned long desc(lock_id)); */

/* //\*************************** */
/* //  event */
/* //\*************************** */

/* struct __tuple_def_service { */
/* 	__tuple_service_name_t			service_name(event); */
	
/* 	__tuple_desc_close_type_t 		desc_close_itself; */
/* 	__tuple_desc_dep_create_type_t 	desc_dep_create_same; */
/* 	__tuple_desc_dep_close_type_t 	desc_dep_close_remv; */
/* 	__tuple_desc_global_type_t		desc_global_true; */
/* 	__tuple_desc_block_type_t		desc_block_true(scheduler_lock); */
/* 	__tuple_desc_data_type_t		desc_data_false; */
/* 	__tuple_resc_data_type_t		resc_data_false; */
/* }; */

/* // TODO: generate this */
/* struct desc_data { */
/* 	long    evtid; */
/* 	long    p_evtid; */
/* 	int     grp; */
/* 	spdid_t spdid; */
/* };	 */

/* desc_data_retval(long, evt_id)  */
/* evt_split(spdid_t desc_data(spdid),  */
/* 		  long desc_data(parent_desc(parent_evt)), */
/* 		  int desc_data(grp)); */

/* void */
/* evt_free(spdid_t spdid, */
/* 		 long desc(evt_id)); */

/* long  */
/* evt_wait(spdid_t spdid,  */
/* 		 long desc(evt_id)); */

/* int  */
/* evt_trigger(spdid_t spdid,  */
/* 			long desc(evt_id)); */

/* //\*************************** */
/* //  mm */
/* //\*************************** */

/* struct __tuple_def_service { */
/* 	__tuple_service_name_t			service_name(mm); */
	
/* 	__tuple_desc_close_type_t 		desc_close_subtree; */
/* 	__tuple_desc_dep_create_type_t 	desc_dep_create_diff; */
/* 	__tuple_desc_dep_close_type_t 	desc_dep_close_remv; */
/* 	__tuple_desc_global_type_t		desc_global_true; */
/* 	__tuple_desc_block_type_t		desc_block_true(null); */
/* 	__tuple_desc_data_type_t		desc_data_true; */
/* 	__tuple_resc_data_type_t		resc_data_true; */
/* }; */

/* // TODO: generate this */
/* struct desc_data { */
/* 	vaddr_t s_addr; */
/* 	vaddr_t d_addr; */
/* 	spdid_t d_spd; */
/* 	int flags; */
/* };	 */


/* desc_data_retval(vaddr_t, s_addr) */
/* mman_get_page(spdid_t desc_data(spd),  */
/* 			  vaddr_t desc_data(addr),  */
/* 			  int desc_data(flags)); */

/* int  */
/* mman_revoke_page(spdid_t spd,  */
/* 				 vaddr_t desc(addr),  */
/* 				 int flags); */

/* vaddr_t  */
/* mman_alias_page(spdid_t s_spd,  */
/* 				vaddr_t desc(s_addr), */
/* 				u32_t desc_data(d_spd), */
/* 				vaddr_t desc_data(d_addr)); */
				
/* //\*************************** */
/* //  scheduler  -- is special, will do this later */
/* //\*************************** */

/* struct __tuple_def_service { */
/* 	__tuple_service_name_t			service_name(sched); */
	
/* 	__tuple_desc_close_type_t 		desc_close_itself; */
/* 	__tuple_desc_dep_create_type_t 	desc_dep_create_none; */
/* 	__tuple_desc_dep_close_type_t 	desc_dep_close_remv; */
/* 	__tuple_desc_global_type_t		desc_global_true; */
/* 	__tuple_desc_block_type_t		desc_block_true(null); */
/* 	__tuple_desc_data_type_t		desc_data_false; */
/* 	__tuple_resc_data_type_t		resc_data_true; */
/* }; */

/* // TODO: generate this */
/* struct desc_data { */
/* 	u32_t thd; */
/* 	u32_t dep_thd; */
/* };	 */

/* int  */
/* sched_wakeup(spdid_t spdid,  */
/* 			 unsigned short int thd_id); */

/* int  */
/* sched_block(spdid_t spdid,  */
/* 			unsigned short int dependency_thd); */

#endif /* _COS_IDL_H */
