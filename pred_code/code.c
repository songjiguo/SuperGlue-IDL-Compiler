/* block_cli_if_invoke 1 */

if (parent_desc = desc_lookup(parent_id)) {
	new_parent_id = parent_desc->server_id;
}
invoke_ser_intro();


/* block_cli_if_invoke 2 */
CSTUB_INVOKE(ret, fault, uc, param_sz, params)

/* block_cli_if_invoke 3 */
if (desc = desc_lookup(id)) {
	if (desc->fcnt != global_fault_cnt) {
		desc->fault_cnt = global_fault_cnt;
		client_interface_recover();
		client_interface_recover_subtree();
	}
	update_id(id, desc->server_id);
	CSTUB_INVOKE(ret, fault, uc, param_sz, params);
} else {
	invoke_ser_intro();
}

/* block_cli_if_invoke_ser_intro */
CSTUB_INVOKE(ret, fault, uc, param_sz, params)

if (!desc) {   // some error
	client_interface_recover(id);
	CSTUB_INVOKE(ret, fault, uc, param_sz, params)
}

/* block_cli_if_recover 1*/
spdid_t creater_component;

assert(id);
creater_component = introsepct_creator(id);
assert(creater_component);

if (creater_component != cos_spd_id()) {
	recover_upcall(creater_component, id);
} else {
	client_interface_basic_id(id);
}

/* block_cli_if_recover 2*/
client_interface_basic_id(id);


/* block_cli_if_basic_id 1 */
assert(id);
desc = desc_lookup(id);
assert(desc);

ret = client_interface_recover_init();

if (ret == parent_not_recovered_error) {
	id = desc->parent_id;
	client_interface_recover(id);
}

client_interface_recover_data();

/* block_cli_if_basic_id 2 */
assert(id);
desc = desc_lookup(id);
assert(desc);

ret = client_interface_recover_init();
client_interface_recover_data();

/* block_cli_if_recover_upcall */
assert(id);
client_interface_recover(id);
client_interface_recover_subtree(id);

/* block_cli_if_recover_subtree */
assert(id);
desc = desc_lookup(id);
assert(desc);

child_desc_list = desc->child_desc_list;

for ((child_desc) = FIRST_LIST((child_desc_list), next, prev) ;	  
     (child_desc) != (child_desc_list) ;
     (child_desc) = FIRST_LIST((child_desc), next, prev)) {
	client_interface_basic_id(child_desc->id);
	if (child_desc->dest_spd != cos_spd_id()) {
		recover_upcall(child_desc->dest_spd, child_desc->id);
	} else {
		id = child_desc->id;
		client_interface_recover_subtree(id);
	}
}

/* block_cli_if_track 1*/
desc = desc_alloc(ret);
assert(desc);

desc_save(desc, ret, params);

/* block_cli_if_track 2*/
assert(desc);
desc_alloc(desc);

/* block_cli_if_track 3*/
assert(desc);

child_desc_list = desc->child_desc_list;
if (EMPTY_LIST(child_desc_list)) {
	desc_alloc(desc);
}

/* block_cli_if_rec_init*/
assert(desc);
func_name(desc->saved_params);

/* block_cli_if_rec_data */
assert(desc);
res_data = introspect_data(desc->id);

assert(res_data);
restore_data(res_data);

/* block_cli_if_save_data */
save_data(id, data);
