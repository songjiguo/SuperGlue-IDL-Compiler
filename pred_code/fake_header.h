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

#define assert(x) 
#define CSTUB_INVOKE

#endif /* _cos_fake_header_h */
