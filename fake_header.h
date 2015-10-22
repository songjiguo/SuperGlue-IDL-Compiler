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

#define cbuf_alloc(x, y) 0
#define cbuf_free(x) 0

#define assert(x) 
#define unlikely(x) x
#define likely(x) x

#define cvect_add(x, y, z) 0
#define cslab_free_rdservice(x)
#define cslab_alloc_rdservice() 0

#define CSTUB_INVOKE
#define CSTUB_FAULT_UPDATE()

#define MAX_NUM_SPDS 64

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
int lock_component_release(){}

#endif /* _cos_fake_header_h */
