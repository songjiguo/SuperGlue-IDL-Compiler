/*****************************/
/*  ds, inside function etc. used as fake header for now */
/*****************************/
// client header start
struct IDL_desc_track

static volatile unsigned long global_fault_cnt = 0;

// TODO: implement them
static inline void call_update_id(int old_id, int new_id) {}
static inline int call_introspect_creator(int id) {}
static inline void call_recover_upcall(int dest_spd, int id) {}
static inline void call_restore_data(struct desc_track *desc) {}
static inline void call_save_data(int id, void *data) {}
static inline struct desc_track *call_desc_alloc() {}
static inline void call_desc_dealloc(struct desc_track *desc) {}
static inline struct desc_track *call_desc_lookup(int id) {}
static inline struct desc_track *call_desc_update(int id) {}
static inline void call_desc_rec_state(struct desc_track *desc) {}

/* track client id, server id and the parameters here */
// TODO: what else to track??
static inline void call_desc_cons(struct desc_track *desc, int id, int server_id, IDL_desc_saved_params) {}
//static inline void call_desc_cons(struct desc_track *desc, IDL_desc_track_fields) {}

// client header end


// client sm start 

int (* state[])(void) = { IDL_fn_list };
enum state_codes { IDL_state_list };

enum ret_codes { ok, again, fault };
struct transition {
	enum state_codes curr_state;
	enum state_codes next_state;
	enum ret_codes   ret_code;
};

/* transitions */
struct transition state_transitions[] = {
	IDL_transition_rules
};

// client sm end 

///////////////////////////////////////////////
/*****************************/
/* client block_cli_if_invoke */
/*****************************/

// block_cli_if_invoke pred 1 start
desc_dep_create_same|desc_dep_create_diff
creation
// block_cli_if_invoke pred 1 end
// block_cli_if_invoke 1 start
static inline void block_cli_if_invoke_IDL_fname(IDL_parsdecl) {
	struct desc_track *parent_desc = NULL;
	if ((parent_desc = call_desc_lookup(IDL_parent_id))) {
		IDL_parent_id = parent_desc->server_id;
	}
	
	CSTUB_INVOKE(ret, fault, uc, IDL_pars_len, IDL_params);
	if (ret == -1) {   // desc not exist  TODO: change to error code
		block_cli_if_recover(IDL_parent_id);
		CSTUB_INVOKE(ret, fault, uc, IDL_pars_len, IDL_params);
	}
}
// block_cli_if_invoke 1 end

// block_cli_if_invoke pred 2 start
desc_dep_create_single
creation
// block_cli_if_invoke pred 2 end
// block_cli_if_invoke 2 start
static inline void block_cli_if_invoke_IDL_fname(IDL_parsdecl) {
	CSTUB_INVOKE(ret, fault, uc, IDL_pars_len, IDL_params);
}
// block_cli_if_invoke 2 end

// block_cli_if_invoke pred 3 start
transition|terminal
// block_cli_if_invoke pred 3 end
// block_cli_if_invoke 3 start
static inline void block_cli_if_invoke_IDL_fname(IDL_parsdecl) {
	struct desc_track *desc = NULL;
	if ((desc = call_desc_lookup(IDL_id))) {
		if (desc->fault_cnt != global_fault_cnt) {
			desc->fault_cnt = global_fault_cnt;
			block_cli_if_recover(IDL_id);
			block_cli_if_recover_subtree(IDL_id);
		}
		call_update_id(IDL_id, desc->server_id);
		CSTUB_INVOKE(ret, fault, uc, IDL_pars_len, IDL_params);
	} else {  // could be created in different component
		CSTUB_INVOKE(ret, fault, uc, IDL_pars_len, IDL_params);
		if (ret == -1) {   // desc not exist  TODO: change to error code
			block_cli_if_recover(IDL_id);
			CSTUB_INVOKE(ret, fault, uc, IDL_pars_len, IDL_params);
		}
	}
}
// block_cli_if_invoke 3 end

// block_cli_if_invoke no match start
static inline void block_cli_if_invoke_IDL_fname(IDL_parsdecl) {
};
// block_cli_if_invoke no match end

///////////////////////////////////////////////
/*****************************/
/* client block_cli_if_recover */
/*****************************/
// block_cli_if_recover pred 1 start
desc_global_true
// block_cli_if_recover pred 1 end
// block_cli_if_recover 1 start
static inline void block_cli_if_recover(int id) {
	spdid_t creater_component;
	
	assert(id);
	creater_component = call_introspect_creator(id);
	assert(creater_component);
	
	if (creater_component != cos_spd_id()) {
		call_recover_upcall(creater_component, id);
	} else {
		block_cli_if_basic_id(id);
	}
}
// block_cli_if_recover 1 end

// block_cli_if_recover pred 2 start
desc_global_false
// block_cli_if_recover pred 2 end
// block_cli_if_recover 2 start
static inline void block_cli_if_recover(int id) {
	block_cli_if_basic_id(id);
}
// block_cli_if_recover 2 end

// block_cli_if_recover no match start
static inline void block_cli_if_recover(int id) {
};
// block_cli_if_recover no match end
///////////////////////////////////////////////
/*****************************/
/* client block_cli_if_basic_id */
/*****************************/
// block_cli_if_basic_id pred 1 start
desc_dep_create_same
// block_cli_if_basic_id pred 1 end
// block_cli_if_basic_id 1 start
static inline void block_cli_if_basic_id(int id) {
	assert(id);
	struct desc_track *desc = call_desc_lookup(id);
	assert(desc);
	
	int retval = 0;
	retval = IDL_create_fname(IDL_desc_saved_params);

	//TODO: define the error code for non-recovered parent
	if (retval == -99) {
		id = desc->IDL_parent_id;
		block_cli_if_recover(id);
	} else {
		desc->server_id = retval;	
	}
	
	block_cli_if_recover_data(desc);
}
// block_cli_if_basic_id 1 end

// block_cli_if_basic_id pred 2 start
desc_create_single
// block_cli_if_basic_id pred 2 end
// block_cli_if_basic_id 2 start
static inline void block_cli_if_basic_id(int id) {

	assert(id);
	struct desc_track *desc = call_desc_lookup(id);
	assert(desc);
	
	int retval = 0;
	desc->server_id = IDL_fname(IDL_desc_saved_params);
	block_cli_if_recover_data)(desc);
}
// block_cli_if_basic_id 2 end

// block_cli_if_basic_id no match start
static inline void block_cli_if_basic_id(int id) {
};
// block_cli_if_basic_id no match end

///////////////////////////////////////////////
/**************************************/
/* client block_cli_if_recover_upcall */
/**************************************/

// block_cli_if_recover_upcall pred 1 start
desc_global_true
// block_cli_if_recover_upcall pred 1 end

// block_cli_if_recover_upcall 1 start
static inline void block_cli_if_recover_upcall(int id) {
	assert(id);
	block_cli_if_recover(id);
	block_cli_if_recover_subtree(id);
}
// block_cli_if_recover_upcall 1 end

// block_cli_if_recover_upcall no match start
static inline void block_cli_if_recover_upcall(int id) {
};
// block_cli_if_recover_upcall no match end

///////////////////////////////////////////////
/**************************************/
/* client block_cli_if_recover_subtree */
/**************************************/
// block_cli_if_recover_subtree pred 1 start
desc_close_subtree
desc_create_diff
terminal
// block_cli_if_recover_subtree pred 1 end
// block_cli_if_recover_subtree 1 start
static inline void block_cli_if_recover_subtree(int id) {
	assert(id);
	struct desc_track *desc = call_desc_lookup(id);
	assert(desc);
	
	struct desc_track *child_desc_list = desc->child_desc_list;
	
	for ((child_desc) = FIRST_LIST((child_desc_list), next, prev) ;	  
	     (child_desc) != (child_desc_list) ;
	     (child_desc) = FIRST_LIST((child_desc), next, prev)) {
		block_cli_if_basic_id(child_desc->id);
		if (child_desc->dest_spd != cos_spd_id()) {
			call_recover_upcall(child_desc->dest_spd, child_desc->id);
		} else {
			id = child_desc->id;
			block_cli_if_recover_subtree(id);
		}
	}
}
// block_cli_if_recover_subtree 1 end

// block_cli_if_recover_subtree no match start
static inline void block_cli_if_recover_subtree(int id) {}
// block_cli_if_recover_subtree no match end

///////////////////////////////////////////////
/*****************************/
/* client block_cli_if_track */
/*****************************/

// block_cli_if_track pred 1 start
creation
// block_cli_if_track pred 1 end
// block_cli_if_track 1 start
static inline void block_cli_if_track_IDL_fname(int ret, IDL_parsdecl) {
	struct desc_track *desc = call_desc_lookup(ret);
	assert(desc);

	call_desc_save(desc, ret, IDL_params);
}
// block_cli_if_track 1 end

// block_cli_if_track pred 2 start
desc_dep_close_remove
terminal
// block_cli_if_track pred 2 end
// block_cli_if_track 2 start
static inline void block_cli_if_track_IDL_fname(int ret, IDL_parsdecl) {
	struct desc_track *desc = call_desc_lookup(ret);
	assert(desc);
	call_desc_dealloc(desc);
}
// block_cli_if_track 2 end

// block_cli_if_track pred 3 start
desc_dep_close_keep
terminal
// block_cli_if_track pred 3 end
// block_cli_if_track 3 start
static inline void block_cli_if_track_IDL_fname(int ret, IDL_parsdecl) {
	struct desc_track *desc = call_desc_lookup(ret);
	assert(desc);

	call_desc_dealloc(desc);

	// TODO: this needs to be changed
	/* struct desc_track *child_desc_list = desc->child_desc_list;	 */
	/* if (EMPTY_LIST(child_desc_list)) { */
	/* 	call_desc_dealloc(desc); */
	/* } */
}
// block_cli_if_track 3 end

// block_cli_if_track_IDL_fname no match start
static inline void block_cli_if_track_IDL_fname(int ret, IDL_parsdecl) {
}
// block_cli_if_track_IDL_fname no match end

///////////////////////////////////////////////
/************************************/
/* client block_cli_if_recover_data */
/************************************/
// block_cli_if_recover_data pred 1 start
resc_has_data_true
// block_cli_if_recover_data pred 1 end
// block_cli_if_recover_data 1 start
static inline void block_cli_if_recover_data(struct desc_track *desc) {
	assert(desc);
	call_restore_data(desc);
}
// block_cli_if_recover_data 1 end

// block_cli_if_recover_data no match start
static inline void block_cli_if_recover_data(struct desc_track *desc) {
}
// block_cli_if_recover_data no match end

///////////////////////////////////////////////
/************************************/
/* client block_cli_if_save_data    */
/************************************/
// block_cli_if_save_data pred 1 start
desc_has_data_true
// block_cli_if_save_data pred 1 end
// block_cli_if_save_data 1 start
static inline void block_cli_if_save_data(int id, void *data) {
	call_save_data(id, data);
}
// block_cli_if_save_data 1 end

// block_cli_if_save_data no match start
static inline void block_cli_if_save_data(int id, void *data) {
}
// block_cli_if_save_data no match end

///////////////////////////////////////////////
/********************************************/
/* client interface recovery upcall_entry   */
/********************************************/
// block_cli_if_recover_upcall_entry pred 1 start
desc_global_true
// block_cli_if_recover_upcall_entry pred 1 end
// block_cli_if_recover_upcall_entry 1 start
static inline void block_cli_if_recover_upcall_entry(int id) {
	block_cli_if_recover_upcall(id);
}
// block_cli_if_recover_upcall_entry 1 end

// block_cli_if_recover_upcall_entry no match start
static inline void block_cli_if_recover_upcall_entry(int id) {
}
// block_cli_if_recover_upcall_entry no match end
