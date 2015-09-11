///////////////////////////////////////////////
/*****************************/
/* client block_cli_if_invoke */
/*****************************/

// block_cli_if_invoke pred 1 start
desc_dep_create_same|desc_dep_create_diff
create
// block_cli_if_invoke pred 1 end
// block_cli_if_invoke 1 start
void *__block_cli_if_invoke_fname(void *data, parsdecl) {
	if (parent_desc = desc_lookup(parent_id)) {
		new_parent_id = parent_desc->server_id;
	}
	if (block_cli_if_invoke_ser_intro) {
		(*block_cli_if_invoke_ser_intro_fname)(data, params);
	}
}
// block_cli_if_invoke 1 end

// block_cli_if_invoke pred 2 start
desc_dep_create_single
create
// block_cli_if_invoke pred 2 end
// block_cli_if_invoke 2 start
void *__block_cli_if_invoke_fname(void *data, parsdecl) {
	CSTUB_INVOKE(ret, fault, uc, param_sz, params);
}
// block_cli_if_invoke 2 end

// block_cli_if_invoke pred 3 start
mutate|terminate
// block_cli_if_invoke pred 3 end
// block_cli_if_invoke 3 start
void *__block_cli_if_invoke_fname(void *data, parsdecl) {
	if (desc = desc_lookup(id)) {
		if (desc->fcnt != global_fault_cnt) {
			desc->fault_cnt = global_fault_cnt;
			if (block_cli_if_recover) {
				(*block_cli_if_recover)(data, id);
			}
			if (block_cli_if_recover_subtree) {
				(*block_cli_if_recover_subtree)(data, id);
			}
		}
		update_id(id, desc->server_id);
		CSTUB_INVOKE(ret, fault, uc, param_sz, params);
	} else {
		if (block_cli_if_invoke_ser_intro) {
			(*block_cli_if_invoke_ser_intro)(data, params);
		}
	}
}
// block_cli_if_invoke 3 end

// block_cli_if_invoke no match start
void *__block_cli_if_invoke_fname = NULL;
// block_cli_if_invoke no match end

// block_cli_if_invoke func pointer start
void (*block_cli_if_invoke)(void *data, ...);
block_cli_if_invoke = (void *)__block_cli_if_invoke_fname;
// block_cli_if_invoke func pointer end

///////////////////////////////////////////////
/*****************************/
/* client block_cli_if_invoke_ser_intro */
/*****************************/
// block_cli_if_invoke_ser_intro pred 1 start

// block_cli_if_invoke_ser_intro pred 1 end

// block_cli_if_invoke_ser_intro 1 start
void *__block_cli_if_invoke_ser_intro_fname(void *data, parsdecl) {
	CSTUB_INVOKE(ret, fault, uc, param_sz, params);
	
	if (!desc) {   // some error
		if (block_cli_if_recover) {
			(*block_cli_if_recover)(data, id);
		}
		CSTUB_INVOKE(ret, fault, uc, param_sz, params);
	}
}
// block_cli_if_invoke_ser_intro 1 end

// block_cli_if_invoke_ser_intro no match start
void* __block_cli_if_invoke_ser_intro_fname = NULL;
// block_cli_if_invoke_ser_intro no match end

// block_cli_if_invoke_ser_intro func pointer start
void (*block_cli_if_invoke_ser_intro)(void *data, ...);
block_cli_if_invoke_ser_intro = (void *)__block_cli_if_invoke_ser_intro_fname;
// block_cli_if_invoke_ser_intro func pointer end

///////////////////////////////////////////////
/*****************************/
/* client block_cli_if_recover */
/*****************************/
// block_cli_if_recover pred 1 start
desc_global_true
// block_cli_if_recover pred 1 end
// block_cli_if_recover 1 start
void *__block_cli_if_recover(int id) {
	spdid_t creater_component;
	
	assert(id);
	creater_component = introspect_creator(id);
	assert(creater_component);
	
	if (creater_component != cos_spd_id()) {
		recover_upcall(creater_component, id);
	} else {
		if (block_cli_if_basic_id) {
			(*block_cli_if_basic_id)(id);
		}
	}
}
// block_cli_if_recover 1 end

// block_cli_if_recover pred 2 start
desc_global_false
// block_cli_if_recover pred 2 end
// block_cli_if_recover 2 start
void *__block_cli_if_recover(int id) {
	client_interface_basic_id(id);
}
// block_cli_if_recover 2 start

// block_cli_if_recover no match start
void *__block_cli_if_recover = NULL;
// block_cli_if_recover no match end

// block_cli_if_recover func pointer start
void (*block_cli_if_invoke)(void *data, ...);
block_cli_if_recover = (void *)__block_cli_if_recover;
// block_cli_if_recover func pointer end

///////////////////////////////////////////////
/*****************************/
/* client block_cli_if_basic_id */
/*****************************/
// block_cli_if_basic_id pred 1 start
desc_dep_create_same
// block_cli_if_basic_id pred 1 end
// block_cli_if_basic_id 1 start
void *__block_cli_if_basic_id(int id){
	assert(id);
	struct desc_track *desc = desc_lookup(id);
	assert(desc);
	
	int retval = 0;
	if (block_cli_if_recover_init) {
		retval = client_interface_recover_init();
	}
	
	if (retval == parent_not_recovered_error) {
		id = desc->parent_id;
		client_interface_recover(id);
	}
	
	client_interface_recover_data();
}
// block_cli_if_basic_id 1 end

// block_cli_if_basic_id pred 2 start
desc_create_single
// block_cli_if_basic_id pred 2 end

// block_cli_if_basic_id 2 start
void *__block_cli_if_basic_id(int id){
	assert(id);
	desc = desc_lookup(id);
	assert(desc);
	
	ret = client_interface_recover_init();
	client_interface_recover_data();
}
// block_cli_if_basic_id 2 end

// block_cli_if_basic_id no match start
void* __block_cli_if_basic_id = NULL;
// block_cli_if_basic_id no match end

// block_cli_if_basic_id func pointer start
void (*block_cli_if_basic_id)(int id)(void *data, ...);
block_cli_if_basic_id = (void *)__block_cli_if_basic_id;
// block_cli_if_basic_id func pointer end

///////////////////////////////////////////////
/**************************************/
/* client block_cli_if_recover_upcall */
/**************************************/

// block_cli_if_recover_upcall pred 1 start
desc_global_true
// block_cli_if_recover_upcall pred 1 end

// block_cli_if_recover_upcall 1 start
void *__block_cli_if_recover_upcall(int id) {
	assert(id);
	client_interface_recover(id);
	client_interface_recover_subtree(id);
}
// block_cli_if_recover_upcall 1 end

// block_cli_if_recover_upcall no match start
void *__block_cli_if_recover_upcall = NULL;
// block_cli_if_recover_upcall no match end

// block_cli_if_recover_upcall func pointer start
void (*block_cli_if_recover_upcall)(void *data, ...);
block_cli_if_recover_upcall = (void *)__block_cli_if_recover_upcall;
// block_cli_if_recover_upcall func pointer end

///////////////////////////////////////////////
/**************************************/
/* client block_cli_if_recover_subtree */
/**************************************/
// block_cli_if_recover_subtree pred 1 start
desc_close_subtree
desc_create_diff
terminate
// block_cli_if_recover_subtree pred 1 end
// block_cli_if_recover_subtree 1 start
void *__block_cli_if_recover_subtree(int id){
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
}
// block_cli_if_recover_subtree 1 end

// block_cli_if_recover_subtree no match start
void* __block_cli_if_recover_subtree = NULL;
// block_cli_if_recover_subtree no match end

// block_cli_if_recover_subtree func pointer start
void (*block_cli_if_recover_subtree)(void *data, ...); 
block_cli_if_recover_subtree = (void *)__block_cli_if_recover_subtree;
// block_cli_if_recover_subtree func pointer end

///////////////////////////////////////////////
/*****************************/
/* client block_cli_if_track */
/*****************************/

// block_cli_if_track pred 1 start
create
// block_cli_if_track pred 1 end
// block_cli_if_track 1 start
void *__block_cli_if_track(int ret, parsdecl){
	desc = desc_alloc(ret);
	assert(desc);

	desc_save(desc, ret, params);
}
// block_cli_if_track 1 end

// block_cli_if_track pred 2 start
desc_dep_close_remove
terminate
// block_cli_if_track pred 2 end
// block_cli_if_track 2 start
void *__block_cli_if_track(int ret, parsdecl){
	assert(desc);
	desc_alloc(desc);
}
// block_cli_if_track 2 end

// block_cli_if_track pred 3 start
desc_dep_close_keep
terminate
// block_cli_if_track pred 3 end
// block_cli_if_track 3 start
void *__block_cli_if_track(int ret, parsdecl){
	assert(desc);
	
	child_desc_list = desc->child_desc_list;
	if (EMPTY_LIST(child_desc_list)) {
		desc_alloc(desc);
	}
}
// block_cli_if_track 3 end

// block_cli_if_track no match start
void *__block_cli_if_track = NULL;
// block_cli_if_track no match end

// block_cli_if_track func pointer start
void (*block_cli_if_track)(void *data, ...);
block_cli_if_track = (void *)__block_cli_if_track;
// block_cli_if_track func pointer end

///////////////////////////////////////////////
/************************************/
/* client block_cli_if_recover_init */
/************************************/
// block_cli_if_recover_init pred 1 start
create
// block_cli_if_recover_init pred 1 end
// block_cli_if_recover_init 1 start
void *__block_cli_if_recover_init(struct desc_track *desc) {
	assert(desc);
	func_name(desc->saved_params);
}
// block_cli_if_recover_init 1 end

// block_cli_if_recover_init no match start
void* __block_cli_if_recover_init = NULL;
// block_cli_if_recover_init no match end

// block_cli_if_recover_init func pointer start
void (*block_cli_if_recover_init)(void *data, ...);
block_cli_if_recover_init = (void *)__block_cli_if_recover_init;
// block_cli_if_recover_init func pointer start

///////////////////////////////////////////////
/************************************/
/* client block_cli_if_recover_data */
/************************************/
// block_cli_if_recover_data pred 1 start
resc_has_data_true
// block_cli_if_recover_data pred 1 end
// block_cli_if_recover_data 1 start
void *__block_cli_if_recover_data(struct desc_track *desc) {
	assert(desc);
	res_data = introspect_data(desc->id);
	
	assert(res_data);
	restore_data(res_data);
}
// block_cli_if_recover_data 1 end

/* no match */
// block_cli_if_recover_data no match start
void* __block_cli_if_recover_data = NULL;
// block_cli_if_recover_data no match end

/* basic function pointer */
// block_cli_if_recover_data func pointer start
void (*block_cli_if_recover_data)(void *data, ...);
block_cli_if_recover_data = (void *)__block_cli_if_recover_data;
// block_cli_if_recover_data func pointer end

///////////////////////////////////////////////
/************************************/
/* client block_cli_if_save_data    */
/************************************/
// block_cli_if_save_data pred 1 start
desc_has_data_true
// block_cli_if_save_data pred 1 end
// block_cli_if_save_data 1 start
void* __block_cli_if_save_data(int id, (void *)data) {
	save_data(id, data);
}
// block_cli_if_save_data 1 end

// block_cli_if_save_data no match start
void* __block_cli_if_save_data = NULL;
// block_cli_if_save_data no match end

// block_cli_if_save_data func pointer start
void (*block_cli_if_save_data)(void *data, ...);
block_cli_if_save_data = (void *)__block_cli_if_save_data;
// block_cli_if_save_data func pointer end

///////////////////////////////////////////////
/********************************************/
/* client interface recovery upcall_entry   */
/********************************************/
// block_cli_if_recover_upcall_entry pred 1 start
desc_global_true
// block_cli_if_recover_upcall_entry pred 1 end
// block_cli_if_recover_upcall_entry 1 start
void* __block_cli_if_recover_upcall_entry(int id) {
	if (block_cli_if_recover_upcall) {
		(*block_cli_if_recover_upcall)(id);
	}
}
// block_cli_if_recover_upcall_entry 1 end

// block_cli_if_recover_upcall_entry no match start
void* __block_cli_if_recover_upcall_entry = NULL;
// block_cli_if_recover_upcall_entry no match end

// block_cli_if_recover_upcall_entry func pointer start
void (*block_cli_if_recover_upcall_entry)(void *data, ...);
block_cli_if_recover_upcall_entry = (void *)__block_cli_if_recover_upcall_entry;
// block_cli_if_recover_upcall_entry func pointer end





