#ifndef _COS_IDL_MM_H
#define _COS_IDL_MM_H

#include "cidl_gen.h"

service_global_info = {
	service		       = mem_mgr,

        desc_close_subtree     = true,
        desc_dep_create_diff   = true,
        desc_dep_close_removal = true,
        desc_global	       = true,
        desc_block	       = false,
        desc_has_data	       = true,
        resc_has_data	       = true,
};

sm_creation(__mman_alias_page);
sm_transition(__mman_alias_page, __mman_alias_page);
sm_terminal(mman_revoke_page);

vaddr_t
mman_get_page(spdid_t desc_data(spd), 
	      vaddr_t desc_data(addr), 
	      int desc_data(flags));

int
mman_release_page(spdid_t spd, 
		  vaddr_t desc(addr), 
		  int flags);

int 
mman_revoke_page(spdid_t spd, 
		 vaddr_t desc(addr), 
		 int flags); 

desc_data_retval(vaddr_t, d_addr)
__mman_alias_page(spdid_t spd, 
		  vaddr_t desc(parent_desc(addr)), 
		  u32_t desc_data(d_spd_flags), 
		  vaddr_t desc_data(d_addr));

#endif /* _COS_IDL_MM_H */
