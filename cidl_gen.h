#ifndef _cos_idl_gen_h
#define _cos_idl_gen_h

enum { 
	false,
	true,
	/* desc close */
	itself,
	subtree,
	/* dependecy create */
	single,
	diff,
	same,
	/* dependency close */
	keep,
	removal,
	/* state machine */
	creation,
	transition,
	terminal
};

enum {
	sched,
	mem_mgr,
	torrent,  // need differentiate the fs and mbox..... TODO
	lock,
	evt,
	timed_blk,
	mbox
};

struct global_info {
	int service_name;
 	int desc_close;
 	int desc_dep_create;
 	int desc_dep_close;
 	int desc_global;
 	int desc_block;
 	int desc_has_data;
 	int resc_has_data;
};
#define service			.service_name

#define desc_close_self_only    .desc_close_self_only
#define desc_close_subtree      .desc_close_subtree
#define desc_dep_create_diff	.desc_dep_create_diff
#define desc_dep_create_same	.desc_dep_create_same
#define desc_dep_create_none	.desc_dep_create_none
#define desc_dep_close_removal 	.desc_dep_close_removal
#define desc_dep_close_keep 	.desc_dep_close_keep
#define desc_global		.desc_global
#define desc_block		.desc_block
#define desc_has_data		.desc_has_data
#define resc_has_data		.resc_has_data

#define service_global_info struct global_info sgi

typedef int td_t;
typedef int cbuf_t;
typedef int spdid_t;
typedef int tor_flags_t;
typedef int evt_t;
typedef int vaddr_t;
typedef unsigned int u32_t;
typedef unsigned short int us32_t;
typedef unsigned long ul_t;
typedef unsigned long long ull_t;

struct usr_inv_cap {int a;};

#define sm_creation(x)      void SM_creation_SM_##x
#define sm_terminal(x)      void SM_terminal_SM_##x
#define sm_transition(x, y) void SM_transition_SM_##x##_SM_##y
#define sm_block(x)         void SM_block_SM_##x
#define sm_wakeup(x)        void SM_wakeup_SM_##x

/* need the second layer of indirection here .... when there is ## in x*/
#define __desc_data_hidden(x)  CD_desc_data_CD_##x
#define __desc_data(x) __desc_data_hidden(x)
#define desc_data(x) __desc_data(x)

/* need the second layer of indirection here .... when there is ## in x*/
#define __desc_hidden(x)  CD_desc_lookup_CD_##x
#define __desc(x) __desc_hidden(x)
#define desc(x) __desc(x)
//#define desc(x) CD_desc_lookup_CD_##x

#define parent_desc_component(x) parent_desc_component_CD_##x
#define parent_desc(x) parent_desc_CD_##x
#define ret(x) _retval_##x
#define desc_data_retval(x, y)				\
	typedef x CD_desc_data_retval_CD_##x##_CD_##y;	\
	CD_desc_data_retval_CD_##x##_CD_##y
#define desc_terminate(x)	CD_desc_terminate_CD_##x

// this is only for AST paser, remove later!!!!!
#define CVECT_CREATE_STATIC(x) 
#define COS_MAP_CREATE_STATIC(x) 
#define CSLAB_CREATE(x, y)
#define CSTUB_FN(x, y) x y##a

#endif /* _cos_idl_h */
