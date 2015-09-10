#ifndef _COS_IDL_GEN_NORM_H
#define _COS_IDL_GEN_NORM_H

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


#endif /* _COS_IDL_NORM_H */
