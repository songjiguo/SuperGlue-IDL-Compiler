#ifndef _cos_idl_gen_h
#define _cos_idl_gen_h

enum { 
	false,
	true,
	itself,
	same,
	kepp,
	create,
	mutate,
	terminate
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

/* need the second layer of indirection here .... when there is ## in x*/
#define __desc_data_hidden(x)  CD_desc_data_CD_##x
#define __desc_data(x) __desc_data_hidden(x)
#define desc_data(x) __desc_data(x)

#define desc(x) CD_desc_lookup_CD_##x
#define parent_desc(x) parent_desc_CD_##x
#define ret(x) _retval_##x
#define desc_data_retval(x, y) 									\
	typedef x CD_desc_data_retval_CD_##x##_CD_##y;	\
	CD_desc_data_retval_CD_##x##_CD_##y
#define desc_data_remove(x)	CD_desc_data_remove_CD_##x
//#define desc_data_remove(x) 				\
//	typedef void CD_desc_data_remove_CD_##x;\
//	CD_desc_data_remove_CD_##x

		
//#define size_of(x, y) _size_of_##x##_CD_##y
	
// tuple
/* #define desc_close(x)   	int TP_desc_close_TP_##x	// C0:itself, C1:subtree */
/* #define desc_dep_create(x) 	int TP_desc_dep_create_TP_##x   // P0:none, P1: diff, P2:same */
/* #define desc_dep_close(x)  	int TP_desc_dep_close_TP_##x   // Y0:remv, Y1:keep */
/* #define desc_global(x)   	int TP_desc_global_TP_##x  // true or false */
/* #define desc_block(x, y)	int TP_desc_block_TP_##x##_TP_##y  // true or false, and which spd block in */
/* #define desc_has_data(x)   	int TP_desc_has_data_TP_##x  // true or false */
/* #define resc_has_data(x)  	int TP_resc_has_data_TP_##x */

/* // function state machine */
/* #define sm_create(x)   		int TP_create_TP_##x	// C0:itself, C1:subtree */
/* #define sm_mutate(x) 		int TP_mutate_TP_##x   // P0:none, P1: diff, P2:same */
/* #define sm_terminate(x)  	int TP_terminate_TP_##x   // Y0:remv, Y1:keep */


#endif /* _cos_idl_h */
