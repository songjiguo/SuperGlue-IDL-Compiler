// client track start
struct IDL_desc_track

static volatile unsigned long global_fault_cnt = 0;

/* tracking thread state for data recovery */
CVECT_CREATE_STATIC(rd_vect);
CSLAB_CREATE(rdservice, sizeof(struct desc_track));

// client track end

// client sm_funptr start 
typedef int (*generic_fp)(void);
generic_fp state_fn[IDL_fn_list_len] = { 
	IDL_fn_list };
// client sm_funptr end

// client sm start 
enum state_codes { IDL_state_list };
enum ret_codes { ok, again, faulty };
struct transition {
	enum state_codes curr_state;
	enum state_codes next_state;
	enum ret_codes   ret_code;
	generic_fp       transit_fun;
};

/* transitions */
struct transition state_transitions[] = {
	IDL_transition_rules
};
// client sm end

///////////////////////////////////////////////
/*****************************/
/* client block_cli_if_invoke */
/* Note: the block with IDL is related to the function */
///////////////////////////////////////////////

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
	CSTUB_INVOKE(ret, fault, uc, IDL_pars_len, IDL_desc_saved_params);
	if (ret == -1) {   // parent desc not exist  TODO: change to error code
		block_cli_if_recover(IDL_parent_id);
		IDL_parent_id = parent_desc->server_id;
		CSTUB_INVOKE(ret, fault, uc, IDL_pars_len, IDL_desc_saved_params);
	}
}
// block_cli_if_invoke 1 end

// block_cli_if_invoke pred 2 start
desc_dep_create_single
creation
// block_cli_if_invoke pred 2 end
// block_cli_if_invoke 2 start
static inline void block_cli_if_invoke_IDL_fname(IDL_parsdecl) {
	CSTUB_INVOKE(ret, fault, uc, IDL_pars_len, IDL_desc_saved_params);
}
// block_cli_if_invoke 2 end

// block_cli_if_invoke pred 3 start
desc_global_true
transition|terminal
// block_cli_if_invoke pred 3 end
// block_cli_if_invoke 3 start
static inline void block_cli_if_invoke_IDL_fname(IDL_parsdecl) {
	struct desc_track *desc = call_desc_lookup(IDL_id);
	if (desc) {  // might be created in the same component
		IDL_id = desc->server_id;
		CSTUB_INVOKE(ret, fault, uc, IDL_pars_len, IDL_desc_saved_params);
	} else {    // might be created in different component
		CSTUB_INVOKE(ret, fault, uc, IDL_pars_len, IDL_desc_saved_params);
		if (ret == -1) {   // desc not exist  TODO: change to error code
			block_cli_if_recover(IDL_id);// need upcall
			IDL_id = desc->server_id;
			CSTUB_INVOKE(ret, fault, uc, IDL_pars_len, IDL_desc_saved_params);
		}
	}
}
// block_cli_if_invoke 3 end

// block_cli_if_invoke pred 4 start
desc_global_false
transition|terminal
// block_cli_if_invoke pred 4 end
// block_cli_if_invoke 4 start
static inline void block_cli_if_invoke_IDL_fname(IDL_parsdecl) {
	struct desc_track *desc = call_desc_lookup(IDL_id);
	assert(desc);  // must be created in the same component
	IDL_id = desc->server_id;
	CSTUB_INVOKE(ret, fault, uc, IDL_pars_len, IDL_desc_saved_params);
}
// block_cli_if_invoke 4 end

// block_cli_if_invoke no match start
static inline void block_cli_if_invoke_IDL_fname(IDL_parsdecl) {
}
// block_cli_if_invoke no match end



///////////////////////////////////////////////
/*****************************/
/* client block_cli_if_desc_update */
/*****************************/

// block_cli_if_desc_update pred 1 start
transition|terminal
// block_cli_if_desc_update pred 1 end

// block_cli_if_desc_update 1 start
static inline void block_cli_if_desc_update_IDL_fname(int id) {
	call_desc_update(id);
}
// block_cli_if_desc_update 1 end

// block_cli_if_desc_update pred 2 start
creation
// block_cli_if_desc_update pred 2 end

// block_cli_if_desc_update 2 start
static inline void block_cli_if_desc_update_IDL_fname() {
	
}
// block_cli_if_desc_update 2 end

// block_cli_if_desc_update no match start
static inline block_cli_if_desc_update_IDL_fname(int id) {
}
// block_cli_if_desc_update no match end


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
}
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
	retval = IDL_fname(IDL_desc_saved_params);

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
}
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
}
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
static inline void block_cli_if_recover_subtree(int id) {
}
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
	struct desc_track *desc = call_desc_alloc(ret);
	assert(desc);
	call_desc_cons(desc, ret, IDL_params);
	desc->state = IDL_curr_state;
}
// block_cli_if_track 1 end

// block_cli_if_track pred 2 start
desc_dep_close_remove
terminal
// block_cli_if_track pred 2 end
// block_cli_if_track 2 start
static inline void block_cli_if_track_IDL_fname(int ret, IDL_parsdecl) {
	struct desc_track *desc = call_desc_lookup(IDL_id);
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
	struct desc_track *desc = call_desc_lookup(IDL_id);
	assert(desc);
	call_desc_dealloc(desc);

	// TODO: this needs to be changed
	/* struct desc_track *child_desc_list = desc->child_desc_list;	 */
	/* if (EMPTY_LIST(child_desc_list)) { */
	/* 	call_desc_dealloc(desc); */
	/* } */
}
// block_cli_if_track 3 end

// block_cli_if_track pred 4 start
transition
// block_cli_if_track pred 4 end
// block_cli_if_track 4 start
static inline void block_cli_if_track_IDL_fname(int ret, IDL_parsdecl) {
	struct desc_track *desc = call_desc_lookup(IDL_id);
	assert(desc);
	desc->state = IDL_curr_state;
}
// block_cli_if_track 4 end

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


// client func decl start

static inline struct desc_track *call_desc_lookup(int id) {
	return (struct desc_track *)cvect_lookup(&rd_vect, id);
}

static inline struct desc_track *call_desc_alloc(int id) {
	struct desc_track *_desc_track;

	_desc_track = (struct desc_track *)cslab_alloc_rdservice();
	assert(_desc_track);
	if (cvect_add(&rd_vect, _desc_track, id)) {
		assert(0);
	}
	_desc_track->tid = id;
	return _desc_track;
}

static inline void call_desc_dealloc(struct desc_track *desc) {
	assert(desc);
	assert(!cvect_del(&rd_vect, desc->tid));
	cslab_free_rdservice(desc);
}

static inline void call_desc_cons(struct desc_track *desc, int id, IDL_parsdecl) {
	assert(desc);

	desc->tid = id;
	desc->server_id = id;
	IDL_desc_cons;
	return;
}

static inline struct desc_track *call_desc_update(int id) {
	struct desc_track *desc = NULL;
	unsigned int from_state = 0;
	unsigned int to_state = 0;

        desc = call_desc_lookup(id);
	if (unlikely(!desc)) goto done;

	if (likely(desc->fault_cnt == global_fault_cnt)) goto done;
	desc->fault_cnt = global_fault_cnt;

	from_state = desc->state;
	to_state   = desc->next_state;

	// State machine transition under the fault
	block_cli_if_recover(id);

	IDL_state_transition;

done:	
	return desc;
}

static inline int call_introspect_creator(int id) {}
static inline void call_recover_upcall(int dest_spd, int id) {}
static inline void call_restore_data(struct desc_track *desc) {}
static inline void call_save_data(int id, void *data) {}
static inline void call_desc_rec_state(struct desc_track *desc) {}

// client func decl end

/* the template for state transition  */
// client state transition start
if ((from_state == IDL_current_state) && (to_state == IDL_next_state)) {
	IDL_state_transition_call
	goto done;
}      
// client state transition end


// client cstub start
CSTUB_FN(IDL_fntype, IDL_fname) (struct usr_inv_cap *uc, IDL_parsdecl) {
	struct desc_track *desc = NULL;
redo:
	block_cli_if_desc_update_IDL_fname(IDL_id);
	block_cli_if_invoke_IDL_fname(IDL_params); 
        if (unlikely (fault)){
		CSTUB_FAULT_UPDATE();
                goto redo;
        }
	block_cli_if_track_IDL_fname(ret, IDL_params);
 
        return ret;
}
// client cstub end

// client state_fptr_typedef start
typedef int (*ptr_IDL_fname)(IDL_parsdecl);
// client state_fptr_typedef end

// client state_fptr start
(ptr_IDL_fname)(IDL_desc_saved_params);
// client state_fptr end
