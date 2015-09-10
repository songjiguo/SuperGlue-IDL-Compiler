#ifndef _COS_IDL_H
#define _COS_IDL_H

#define TRUE  1
#define FALSE 0

/* -----> define tuple for a specific service*/
#define COS_IDL_TUPLE_BLOCK        TRUE
#define COS_IDL_TUPLE_DATA_RES     TRUE
#define COS_IDL_TUPLE_GLOBAL_DESC  TRUE
#define COS_IDL_TUPLE_DATA_DESC    TRUE
#define COS_IDL_TUPLE_CLOSE0       TRUE
#define COS_IDL_TUPLE_CLOSE1 	   FALSE
#define COS_IDL_TUPLE_DEP0         FALSE
#define COS_IDL_TUPLE_DEP1         TRUE
#define COS_IDL_TUPLE_DEP2         FALSE
/* define tuple <----- */
#define COS_IDL_TUPLE_DEPENDECY COS_IDL_TUPLE_DEP1|COS_IDL_TUPLE_DEP2

/* -----> annotations about a function */
#define COS_IDL_FUNC_CREATE        TRUE
#define COS_IDL_FUNC_TERMINATE     FALSE
#define COS_IDL_FUNC_MUTATE        FALSE
#define COS_IDL_FUNC_NAME          cos_idl_func_name
#define COS_IDL_FUNC_RET           TRUE
/* annotate about a function <----- */

/* -----> annotations about parameter */
#define COS_IDL_PARA_PARENT_DESC   cos_idl_parent_desc
/* annotate about parameter <----- */

/* -----> annotate IDL block */
#if COS_IDL_TUPLE_DEPENDECY && COS_IDL_FUNC_CREATE
#define COS_IDL_BLOCK_IF_INVOKE(COS_IDL_FUNC_NAME,    	   		\
								COS_IDL_FUNC_RET) 				\
	   {														\
	   		printf("block1\n");									\
	   }
#elif !COS_IDL_TUPLE_DEPENDECY && COS_IDL_FUNC_CREATE
#define COS_IDL_BLOCK_IF_INVOKE(COS_IDL_FUNC_NAME,    	   		\
								COS_IDL_FUNC_RET) 				\
	   {														\
	   		printf("block2\n");									\
	   }
#elif COS_IDL_FUNC_MUTATE || COS_IDL_FUNC_TERMINATE
#define COS_IDL_BLOCK_IF_INVOKE(COS_IDL_FUNC_NAME,    	   		\
								COS_IDL_FUNC_RET) 				\
	   {														\
	   		printf("block3\n");									\
	   }
#endif

// Accept any number of args >= N, but expand to just the Nth one.
// Here, N == 6.
#define _GET_NTH_ARG(_1, _2, _3, _4, _5, N, ...) N
#define COUNT_VARARGS(...) _GET_NTH_ARG(__VA_ARGS__, 5, 4, 3, 2, 1)

// Define some macros to help us create overrides based on the
// arity of a for-each-style macro.
#define _fe_0(_call, ...)
#define _fe_1(_call, x) _call(x)
#define _fe_2(_call, x, ...) _call(x) _fe_1(_call, __VA_ARGS__)
#define _fe_3(_call, x, ...) _call(x) _fe_2(_call, __VA_ARGS__)
#define _fe_4(_call, x, ...) _call(x) _fe_3(_call, __VA_ARGS__)

/**
 * Provide a for-each construct for variadic macros. Supports up
 * to 4 args.
 *
 * Example usage1:
 *     #define FWD_DECLARE_CLASS(cls) class cls;
 *     CALL_MACRO_X_FOR_EACH(FWD_DECLARE_CLASS, Foo, Bar)
 *
 * Example usage 2:
 *     #define START_NS(ns) namespace ns {
 *     #define END_NS(ns) }
 *     #define MY_NAMESPACES System, Net, Http
 *     CALL_MACRO_X_FOR_EACH(START_NS, MY_NAMESPACES)
 *     typedef foo int;
 *     CALL_MACRO_X_FOR_EACH(END_NS, MY_NAMESPACES)
 */
#define CALL_MACRO_X_FOR_EACH(x, ...) \
    _GET_NTH_ARG("ignored", ##__VA_ARGS__, \
    _fe_4, _fe_3, _fe_2, _fe_1, _fe_0)(x, ##__VA_ARGS__)
    
#define PAR_LIST int spd, int COS_IDL_PARA_PARENT_DESC_par1, int par2
int foo(PAR_LIST);








#define CIDL_DESC_DATA(a) a##__cidl_descriptor_data // for cidl generation to ast in idlgen/cidl_gen.h
#define CIDL_DESC_DATA(a) a // for .h file generation to be included by .c files in dothgen/cidl_gen.h 

CIDL_IF_RESOURCE_DATA; // -> int __cidl_interface_resource_hasdata;
CIDL_IF_DESC_CLOSE_TYPE(CIDL_DESC_CLOSE_SUBTREE); // -> int __cidl_interface_closetype_subtree;
struct CIDL_DESC_DATA(desc_data) { // -> struct desc_data__cidl_descriptor_data {
	int offset;
};

CIDL_ADD_RET(int, offset) 
// -> typedef int int__cidl_descriptor_data_addto_offset_t; int__cidl_descriptor_data_addto_offset_t
tread(CIDL_DESC(td), char *buffer, int CIDL_SZ(sz, buffer));

#define TEST_FN(name) call cls;
void code_gen()
{
	COS_IDL_BLOCK_IF_INVOKE(foo, bar);	
	printf("three args: %d\n", COUNT_VARARGS(PAR_LIST));
	CALL_MACRO_X_FOR_EACH(TEST_FN, name1, name2)	
}


#endif /* _COS_IDL_H */twrite
