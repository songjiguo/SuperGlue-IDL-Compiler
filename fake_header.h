/* This file is temporarily used for gcc debug only. gcc can not
 * understand c3idl key words. It does not matter for parsing. Remove
 * this later. */

#ifndef _cos_fake_header_h
#define _cos_fake_header_h

#include <stdlib.h>
#include <stdio.h>

int fault;
int uc;
int ret;
int param_sz;
int IDL_desc_saved_params;  // TODO: fix this

int tmp;
#define rd_vect tmp
#define cvect_lookup(x, y) tmp
#define cos_map_lookup(x, y) tmp

#define cvect_init_static(x) 

#define ramfs_desc_maps tmp
#define cslab_alloc_ramfs_slab() NULL
#define cslab_free_ramfs_slab(x) NULL

#define lock_desc_maps tmp
#define cslab_alloc_lock_slab() NULL
#define cslab_free_lock_slab(x) NULL

#define evt_desc_maps tmp
#define cslab_alloc_evt_slab() NULL
#define cslab_free_evt_slab(x) NULL

#define sched_desc_maps tmp
#define cslab_alloc_sched_slab() NULL
#define cslab_free_sched_slab(x) NULL

#define mem_mgr_desc_maps tmp
#define cslab_alloc_mem_mgr_slab() NULL
#define cslab_free_mem_mgr_slab(x) NULL

#define periodic_wake_desc_maps tmp
#define cslab_alloc_periodic_wake_slab() NULL
#define cslab_free_periodic_wake_slab(x) NULL

int hahalock = 10;
#define evt_lock hahalock
#define lock_take(x) 
#define lock_release(x) 

#define cos_map_add(x, y) 0
#define cos_map_del(x, y) 0
#define cos_map_init_static(x)

#define cbuf_alloc(x, y) 0
#define cbuf_free(x) 0

#define assert(x) 
#define unlikely(x) x
#define likely(x) x

#define cvect_add(x, y, z) 0
#define cvect_del(x, y) 0
#define cslab_free_rdservice(x)
#define cslab_alloc_rdservice() 0

#define CSTUB_INVOKE
#define CSTUB_INVOKE_NULL
#define CSTUB_INVOKE_3RETS
#define CSTUB_FAULT_UPDATE()

#define MAX_NUM_SPDS 64
#define MAX_NUM_THREADS 32

#define STATIC_INIT_LIST(obj, next, prev)         \
	obj = {                                   \
		.next = &obj,			  \
		.prev = &obj			  \
	}
	      
#define INIT_LIST(obj, next, prev)		  \
	do { (obj)->next = (obj)->prev = (obj); } while (0)

#define ADD_LIST(head, new, next, prev) do {	  \
	(new)->next = (head)->next;		  \
	(new)->prev = (head);			  \
	(head)->next = (new);			  \
	(new)->next->prev = (new); } while (0)

#define APPEND_LIST(last, head, next, prev) do {  \
	(last)->next->prev = (head)->prev;	  \
	(head)->prev->next = (last)->next;	  \
	(last)->next = (head);			  \
	(head)->prev = (last); } while (0)

#define REM_LIST(obj, next, prev) do {		  \
	(obj)->next->prev = (obj)->prev;	  \
	(obj)->prev->next = (obj)->next;	  \
	(obj)->next = (obj)->prev = (obj); } while (0)

#define FIRST_LIST(obj, next, prev)               \
	((obj)->next)

#define LAST_LIST(obj, next, prev)                \
	((obj)->prev)

#define EMPTY_LIST(obj, next, prev)		  \
	((obj)->next == (obj))

#define ADD_END_LIST(head, new, next, prev) 	  \
	ADD_LIST(LAST_LIST(head, next, prev), new, next, prev)


int tsplit(){}
int treadp(){}
int twritep(){}
int trelease(){}

unsigned long lock_component_alloc(){}
int lock_component_free(){}
int lock_component_take(){}
int lock_component_pretake(){}
int lock_component_release(){}

int sched_component_take(){}
int sched_component_release(){}
int cos_sched_lock_take() {}
int cos_sched_lock_release() {}
int sched_block(){}
int sched_wakeup(){}
int sched_timestamp(){}
int sched_restore_ticks(){}

int evt_upcall_creator(){}
int evt_split(){}
int evt_split_exist(){}
int evt_wait(){}
int evt_trigger(){}
int evt_free(){}

int mman_get_page_exist() {}
int __mman_alias_page() {}
void call_recover_upcall(int dest_spd, int id, int type){}
int valloc_upcall() {}

int periodic_wake_create(){}
int periodic_wake_create_exist(){}

#define BUG(); {;}
#define EINVAL 0
#define extern 
int cos_spd_id(){}

int cbuf2buf(){}
#include <string.h>

#define ELOOP 0
#define ECHILD 0
#define COS_UPCALL_RECOVERY 0
#define COS_UPCALL_RECOVERY_SUBTREE 0
#define COS_UPCALL_REMOVE_SUBTREE 0
#define cos_get_thd_id() 0

#endif /* _cos_fake_header_h */
