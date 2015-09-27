#ifndef _cos_idl_gen_h
#define _cos_idl_gen_h

enum { 
	false,
	true,
	itself,
	same,
	kepp,
	creation,
	transition,
	terminal
};

typedef int desc_close;
typedef int desc_dep_create;
typedef int desc_dep_close;
typedef int desc_global;
typedef int desc_block;
typedef int desc_has_data;
typedef int resc_has_data;

typedef int tsplit_sm;
typedef int trelease_sm;
typedef	int treadp_sm;
typedef	int twritep_sm;

typedef int td_t;
typedef int spdid_t;
typedef int tor_flags_t;
typedef int evt_t;
typedef int vaddr_t;
typedef unsigned int u32_t;
typedef unsigned short int us32_t;
typedef unsigned long ul_t;
typedef unsigned long long ull_t;

struct usr_inv_cap {int a;};

#define CVECT_CREATE_STATIC(x)
#define CSLAB_CREATE(x, y)
#define service_global_info struct global_info

#define sm_creation(x) void SM_creation_SM_##x
#define sm_terminal(x) void SM_terminal_SM_##x
#define sm_transition(x, y) void SM_transition_SM_##x##_SM_##y

/* need the second layer of indirection here .... when there is ## in x*/
#define __desc_data_hidden(x)  CD_desc_data_CD_##x
#define __desc_data(x) __desc_data_hidden(x)
#define desc_data(x) __desc_data(x)

#define desc(x) CD_desc_lookup_CD_##x
#define parent_desc(x) parent_desc_CD_##x
#define ret(x) _retval_##x
#define desc_data_retval(x, y)				\
	typedef x CD_desc_data_retval_CD_##x##_CD_##y;	\
	CD_desc_data_retval_CD_##x##_CD_##y
#define desc_terminate(x)	CD_desc_terminate_CD_##x

#define CSTUB_FN(x, y) x y

#endif /* _cos_idl_h */
