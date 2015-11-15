/*   ___| (_) ___ _ __ | |_  (_)_ __ | |_ ___ _ __ / _| __ _  ___ ___  */
/*  / __| | |/ _ \ '_ \| __| | | '_ \| __/ _ \ '__| |_ / _` |/ __/ _ \ */
/* | (__| | |  __/ | | | |_  | | | | | ||  __/ |  |  _| (_| | (_|  __/ */
/*  \___|_|_|\___|_| |_|\__| |_|_| |_|\__\___|_|  |_|  \__,_|\___\___| */

// client sm start 
enum state_codes { IDL_state_list };
// client sm end

// client track start
struct IDL_desc_track

static volatile unsigned long global_fault_cnt = 0;

static int first_map_init = 0;
// client track end


/*******************************************/
/* client block_cli_if_call_desc_update    */
/*******************************************/
// block_cli_if_call_desc_update pred 1 start
desc_global_false
// block_cli_if_call_desc_update pred 1 end
// block_cli_if_call_desc_update 1 start
static inline struct desc_track *call_desc_update(int id, int next_state) {
	struct desc_track *desc = NULL;
	unsigned int from_state = 0;
	unsigned int to_state = 0;

	if (id == IDL_root_id) return NULL;  /* root id*/

        desc = call_desc_lookup(id);
	if (unlikely(!desc)) {
		block_cli_if_upcall_creator(id);
		goto done;
	}

	desc->next_state = next_state;

	if (likely(desc->fault_cnt == global_fault_cnt)) goto done;
	/* desc->fault_cnt = global_fault_cnt; */

	// State machine transition under the fault
	block_cli_if_recover(id);
	from_state       = desc->state;
	to_state         = next_state;

	IDL_state_transition;

done:	
	return desc;
}
// block_cli_if_call_desc_update 1 end

// block_cli_if_call_desc_update pred 2 start
desc_global_true&desc_dep_create_diff
// block_cli_if_call_desc_update pred 2 end
// block_cli_if_call_desc_update 2 start
static inline struct desc_track *call_desc_update(int id, int next_state) {
	struct desc_track *desc = NULL;
	unsigned int from_state = 0;
	unsigned int to_state = 0;

	if (id == IDL_root_id) return NULL;  /* root id*/

        desc = call_desc_lookup(id);
	if (unlikely(!desc)) {
		block_cli_if_upcall_creator(id);
		goto done;
	}

	desc->next_state = next_state;

	if (likely(desc->fault_cnt == global_fault_cnt)) goto done;
	/* desc->fault_cnt = global_fault_cnt; */

	// State machine transition under the fault
	block_cli_if_recover(id);
	from_state       = desc->state;
	to_state         = next_state;

	IDL_state_transition;

done:	
	return desc;
}
// block_cli_if_call_desc_update 2 end

// block_cli_if_call_desc_update pred 3 start
desc_global_true&desc_dep_create_same
// block_cli_if_call_desc_update pred 3 end
// block_cli_if_call_desc_update 3 start
static inline struct desc_track *call_desc_update(int id, int next_state) {
	struct desc_track *desc = NULL;
	unsigned int from_state = 0;
	unsigned int to_state = 0;

	if (id == IDL_root_id) return NULL;  /* root id*/

        desc = call_desc_lookup(id);
	if (unlikely(!desc)) {
		block_cli_if_upcall_creator(id);
		goto done;
	}

	desc->next_state = next_state;

	if (likely(desc->fault_cnt == global_fault_cnt)) goto done;
	/* desc->fault_cnt = global_fault_cnt; */

	// State machine transition under the fault
	block_cli_if_recover(id);
	from_state       = desc->state;
	to_state         = next_state;

	IDL_state_transition;

done:	
	return desc;
}
// block_cli_if_call_desc_update 3 end

// block_cli_if_call_desc_update pred 4 start
desc_global_true&desc_dep_create_none
// block_cli_if_call_desc_update pred 4 end
// block_cli_if_call_desc_update 4 start
static inline struct desc_track *call_desc_update(int id, int next_state) {
	struct desc_track *desc = NULL;
	unsigned int from_state = 0;
	unsigned int to_state = 0;

        desc = call_desc_lookup(cos_get_thd_id());
	if (unlikely(!desc)) {
		desc = call_desc_alloc(cos_get_thd_id());
		desc->fault_cnt = global_fault_cnt;
	}

	desc->next_state = next_state;

	if (likely(desc->fault_cnt == global_fault_cnt)) goto done;
	desc->fault_cnt = global_fault_cnt;
done:	
	return desc;
}
// block_cli_if_call_desc_update 4 end

// block_cli_if_call_desc_update no match start
// block_cli_if_call_desc_update no match end
/*****************************/
/* client block_cli_if_tracking_map_ds */
/*****************************/
// Note:
// cvect knows id then return desc, id conversion is done at the client interface
// cosmap returns id for a desc, for the global case, id conversion is done through 3rd party spd

// block_cli_if_tracking_map_ds pred 1 start
desc_global_false
// block_cli_if_tracking_map_ds pred 1 end
// block_cli_if_tracking_map_ds 1 start
COS_MAP_CREATE_STATIC(IDL_service_desc_maps);
CSLAB_CREATE(IDL_service_slab, sizeof(struct desc_track));
// block_cli_if_tracking_map_ds 1 end

// block_cli_if_tracking_map_ds pred 2 start
desc_global_true
// block_cli_if_tracking_map_ds pred 2 end
// block_cli_if_tracking_map_ds 2 start
CVECT_CREATE_STATIC(IDL_service_desc_maps);
CSLAB_CREATE(IDL_service_slab, sizeof(struct desc_track));
// block_cli_if_tracking_map_ds 2 end

// block_cli_if_tracking_map_ds no match start
// block_cli_if_tracking_map_ds no match end

/*****************************/
/* client block_cli_if_map_init */
/*****************************/
// block_cli_if_map_init pred 1 start
not desc_global or (desc_global and not desc_dep_create_none)
// block_cli_if_map_init pred 1 end
// block_cli_if_map_init 1 start
static inline void call_map_init() {
	if (unlikely(!first_map_init)) {
		first_map_init = 1;
		cos_map_init_static(&IDL_service_desc_maps);
	}
	return;
}
// block_cli_if_map_init 1 end

// block_cli_if_map_init pred 4 start
desc_global_true and desc_dep_create_none
// block_cli_if_map_init pred 4 end
// block_cli_if_map_init 4 start
static inline void call_map_init() {
}
// block_cli_if_map_init 4 end

// block_cli_if_map_init no match start
// block_cli_if_map_init no match end

/********************************************/
/* client block_cli_if_tracking_map_fn */
/********************************************/
// block_cli_if_tracking_map_fn pred 1 start
desc_global_false
// block_cli_if_tracking_map_fn pred 1 end
// block_cli_if_tracking_map_fn 1 start
static inline struct desc_track *call_desc_lookup(int id) {
	 return (struct desc_track *)cos_map_lookup(&IDL_service_desc_maps, id);
}

static inline struct desc_track *call_desc_alloc() {
	struct desc_track *desc = NULL;
	int map_id = 0;

	while(1) {
		desc = cslab_alloc_IDL_service_slab();
		assert(desc);	
		map_id = cos_map_add(&IDL_service_desc_maps, desc);
		desc->IDL_id        = map_id;
		desc->IDL_server_id = -1;  // reset to -1
		if (map_id >= 2) break;
	}
	assert(desc && desc->IDL_id >= 1);
	return desc;	
}

static inline void call_desc_dealloc(struct desc_track *desc) {
	assert(desc);
	int id = desc->IDL_id;
	desc->IDL_server_id = -1;  // reset to -1
	assert(desc);
	cslab_free_IDL_service_slab(desc);
	cos_map_del(&IDL_service_desc_maps, id);
	return;
}
// block_cli_if_tracking_map_fn 1 end

// block_cli_if_tracking_map_fn pred 2 start
desc_global_true
// block_cli_if_tracking_map_fn pred 2 end
// block_cli_if_tracking_map_fn 2 start
static inline struct desc_track *call_desc_lookup(int id) {
	 return (struct desc_track *)cvect_lookup(&IDL_service_desc_maps, id);
}

static inline struct desc_track *call_desc_alloc(int id) {
	struct desc_track *desc = NULL;
	desc = cslab_alloc_IDL_service_slab();
	assert(desc);
	desc->IDL_id  = id;
	cvect_add(&IDL_service_desc_maps, desc, id);
	return desc;
}

static inline void call_desc_dealloc(struct desc_track *desc) {
	assert(desc);
	if (cvect_del(&IDL_service_desc_maps, desc->IDL_id)) assert(0);
	cslab_free_IDL_service_slab(desc);
	return;
}
// block_cli_if_tracking_map_fn 2 end

// block_cli_if_tracking_map_fn no match start
// block_cli_if_tracking_map_fn no match end

/*****************************/
/* client block_cli_if_invoke */
/* Note: the block with IDL is related to the function */
/*****************************/
// block_cli_if_invoke pred 1 start
desc_dep_create_same
creation
// block_cli_if_invoke pred 1 end
// block_cli_if_invoke 1 start
static inline int block_cli_if_invoke_IDL_fname(IDL_parsdecl, int ret, long *fault, struct usr_inv_cap *uc) {
	struct desc_track *parent_desc = NULL;
	if ((IDL_parent_id > 1) && (parent_desc = call_desc_lookup(IDL_parent_id))) {
		IDL_parent_id = parent_desc->IDL_server_id;
	} /* else {  	// td_root, or in a different component */
	/* 	IDL_parent_id = IDL_parent_id; */
	/* } */

	long __fault = 0;
	CSTUB_INVOKE(ret, __fault, uc, IDL_pars_len, IDL_params);
	*fault = __fault;

	return ret;
}
// block_cli_if_invoke 1 end


// block_cli_if_invoke pred 2 start
desc_dep_create_diff
creation
// block_cli_if_invoke pred 2 end
// block_cli_if_invoke 2 start
static inline int block_cli_if_invoke_IDL_fname(IDL_parsdecl, int ret, long *fault, struct usr_inv_cap *uc) {
	struct desc_track *parent_desc = NULL;
	if ((IDL_parent_id > 1) && (parent_desc = call_desc_lookup(IDL_parent_id))) {
		IDL_parent_id = parent_desc->IDL_server_id;
	} /* else {  	// td_root, or in a different component */
	/* 	IDL_parent_id = IDL_parent_id; */
	/* } */

	long __fault = 0;
	CSTUB_INVOKE(ret, __fault, uc, IDL_pars_len, IDL_params);
	*fault = __fault;

	return ret;
}
// block_cli_if_invoke 2 end

// block_cli_if_invoke pred 3 start
desc_dep_create_none
creation
// block_cli_if_invoke pred 3 end
// block_cli_if_invoke 3 start
static inline int block_cli_if_invoke_IDL_fname(IDL_parsdecl, int ret, long *fault, struct usr_inv_cap *uc) {
	long __fault = 0;
	CSTUB_INVOKE(ret, __fault, uc, IDL_pars_len, IDL_params);
	*fault = __fault;
	return ret;
}
// block_cli_if_invoke 3 end

// block_cli_if_invoke pred 4 start
desc_global_true&desc_dep_create_diff
transition|terminal
// block_cli_if_invoke pred 4 end
// block_cli_if_invoke 4 start
static inline int block_cli_if_invoke_IDL_fname(IDL_parsdecl, int ret, long *fault, struct usr_inv_cap *uc) {
	struct desc_track *desc = call_desc_lookup(IDL_id);
	long __fault = 0;
	if (desc) {  // might be created in the same component
		CSTUB_INVOKE(ret, __fault, uc, IDL_pars_len, IDL_server_id_params);
	} else {    // might be created in different component
		CSTUB_INVOKE(ret, __fault, uc, IDL_pars_len, IDL_params);
		if (ret == -1) {   // desc not exist  TODO: change to error code
			block_cli_if_recover(IDL_id);// need upcall
			assert((desc = call_desc_lookup(IDL_id)));
			CSTUB_INVOKE(ret, __fault, uc, IDL_pars_len, IDL_params);
		}
	}
	*fault = __fault;

	return ret;
}
// block_cli_if_invoke 4 end

// block_cli_if_invoke pred 5 start
desc_global_true&desc_dep_create_same
transition|terminal
// block_cli_if_invoke pred 5 end
// block_cli_if_invoke 5 start
static inline int block_cli_if_invoke_IDL_fname(IDL_parsdecl, int ret, long *fault, struct usr_inv_cap *uc) {
	struct desc_track *desc = call_desc_lookup(IDL_id);
	long __fault = 0;
	if (desc) {  // might be created in the same component
		CSTUB_INVOKE(ret, __fault, uc, IDL_pars_len, IDL_server_id_params);
	} else {    // might be created in different component
		CSTUB_INVOKE(ret, __fault, uc, IDL_pars_len, IDL_params);
		if (ret == -1) {   // desc not exist  TODO: change to error code
			block_cli_if_recover(IDL_id);// need upcall
			assert((desc = call_desc_lookup(IDL_id)));
			CSTUB_INVOKE(ret, __fault, uc, IDL_pars_len, IDL_params);
		}
	}
	*fault = __fault;

	return ret;
}
// block_cli_if_invoke 5 end

// block_cli_if_invoke pred 6 start
desc_global_true&desc_dep_create_none
transition|terminal
// block_cli_if_invoke pred 6 end
// block_cli_if_invoke 6 start
static inline int block_cli_if_invoke_IDL_fname(IDL_parsdecl, int ret, long *fault, struct usr_inv_cap *uc) {
	long __fault = 0;
	CSTUB_INVOKE(ret, __fault, uc, IDL_pars_len, IDL_params);
	*fault = __fault;
	return ret;
}
// block_cli_if_invoke 6 end

// block_cli_if_invoke pred 7 start
desc_global_false
transition|terminal
// block_cli_if_invoke pred 7 end
// block_cli_if_invoke 7 start
static inline int block_cli_if_invoke_IDL_fname(IDL_parsdecl, int ret, long *fault, struct usr_inv_cap *uc) {
	long __fault = 0;
	CSTUB_INVOKE(ret, __fault, uc, IDL_pars_len, IDL_server_id_params);
	*fault = __fault;

	return ret;
}
// block_cli_if_invoke 7 end

// block_cli_if_invoke no match start
static inline int block_cli_if_invoke_IDL_fname(IDL_parsdecl, int ret, long *fault, struct usr_inv_cap *uc) {
}
// block_cli_if_invoke no match end

/*****************************/
/* client block_cli_if_desc_update_pre */
/*****************************/
// block_cli_if_desc_update_pre pred 1 start
desc_dep_create_none
creation
// block_cli_if_desc_update_pre pred 1 end
// block_cli_if_desc_update_pre 1 start
static inline void block_cli_if_desc_update_pre_IDL_fname() {
}
// block_cli_if_desc_update_pre 1 end


// block_cli_if_desc_update_pre pred 2 start
desc_dep_create_same|desc_dep_create_diff
creation
// block_cli_if_desc_update_pre pred 2 end
// block_cli_if_desc_update_pre 2 start
static inline void block_cli_if_desc_update_pre_IDL_fname(int id) {
	call_desc_update(id, state_IDL_fname);
}
// block_cli_if_desc_update_pre 2 end


// block_cli_if_desc_update_pre pred 3 start
desc_global_false
transition|terminal
// block_cli_if_desc_update_pre pred 3 end
// block_cli_if_desc_update_pre 3 start
static inline void block_cli_if_desc_update_pre_IDL_fname(int id) {
	call_desc_update(id, state_IDL_fname);
}
// block_cli_if_desc_update_pre 3 end


// block_cli_if_desc_update_pre pred 4 start
desc_global_true
transition
// block_cli_if_desc_update_pre pred 4 end
// block_cli_if_desc_update_pre 4 start
static inline void block_cli_if_desc_update_pre_IDL_fname() {
}
// block_cli_if_desc_update_pre 4 end

// block_cli_if_desc_update_pre pred 5 start
terminal
// block_cli_if_desc_update_pre pred 5 end
// block_cli_if_desc_update_pre 5 start
static inline void block_cli_if_desc_update_pre_IDL_fname(int id) {
	call_desc_update(id, state_IDL_fname);
}
// block_cli_if_desc_update_pre 5 end


// block_cli_if_desc_update_pre no match start
static inline block_cli_if_desc_update_pre_IDL_fname(int id) {
}
// block_cli_if_desc_update_pre no match end


/*****************************/
/* client block_cli_if_desc_update_post_fault */
/*****************************/
// block_cli_if_desc_update_post_fault pred 1 start
creation|terminal
// block_cli_if_desc_update_post_fault pred 1 end
// block_cli_if_desc_update_post_fault 1 start
static inline int block_cli_if_desc_update_post_fault_IDL_fname(int id) {
	return 1;
}
// block_cli_if_desc_update_post_fault 1 end

// block_cli_if_desc_update_post_fault pred 2 start
desc_global_false
transition|terminal
// block_cli_if_desc_update_post_fault pred 2 end
// block_cli_if_desc_update_post_fault 2 start
static inline int block_cli_if_desc_update_post_fault_IDL_fname(int id) {
	return 1;
	
}
// block_cli_if_desc_update_post_fault 2 end


// block_cli_if_desc_update_post_fault pred 3 start
desc_global_true
transition
// block_cli_if_desc_update_post_fault pred 3 end
// block_cli_if_desc_update_post_fault 3 start
static inline int block_cli_if_desc_update_post_fault_IDL_fname(int id) {
	call_desc_update(id, state_IDL_fname);
	return 0;
}
// block_cli_if_desc_update_post_fault 3 end

// block_cli_if_desc_update_post_fault no match start
static inline int block_cli_if_desc_update_post_fault_IDL_fname(int id) {
}
// block_cli_if_desc_update_post_fault no match end

/*****************************/
/* client block_cli_if_recover */
/*****************************/
// block_cli_if_recover pred 1 start
desc_global_true
// block_cli_if_recover pred 1 end
// block_cli_if_recover 1 start
static inline void block_cli_if_recover(int id) {
	/* spdid_t creater_component; */
	
	/* assert(id); */
	/* creater_component = call_introspect_creator(id); */
	/* assert(creater_component); */
	
	/* if (creater_component != cos_spd_id()) { */
	/* 	call_recover_upcall(creater_component, id); */
	/* } else { */
	/* 	block_cli_if_basic_id(id); */
	/* } */
	block_cli_if_basic_id(id);
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

/*****************************/
/* client block_cli_if_basic_id */
/*****************************/
// block_cli_if_basic_id pred 1 start
desc_dep_create_same&desc_global_false
// block_cli_if_basic_id pred 1 end
// block_cli_if_basic_id 1 start
static inline void block_cli_if_basic_id(int id) {
	assert(id);
	struct desc_track *desc = call_desc_lookup(id);
	assert(desc);
	
	int retval = 0;
again:
	retval = IDL_fname(IDL_desc_saved_params);
	//TODO: define the error code for non-recovered parent
	// thinking...1111111
	if (retval == -EINVAL) {
		id = desc->IDL_parent_id;
		call_desc_update(id, state_IDL_fname);
		goto again;
	} 

	assert(retval);
	desc->state = IDL_init_state;       // set the state to the initial state
	desc->fault_cnt = global_fault_cnt; // set the fault counter to the global
	block_cli_if_recover_data(desc);
	return;
}
// block_cli_if_basic_id 1 end

// block_cli_if_basic_id pred 2 start
desc_dep_create_same&desc_global_true
// block_cli_if_basic_id pred 2 end
// block_cli_if_basic_id 2 start
static inline void block_cli_if_basic_id(int id) {
	assert(id);
	struct desc_track *desc = call_desc_lookup(id);
	assert(desc);
	
	int retval = 0;
again:
	retval = IDL_fname_exist(IDL_desc_saved_params, desc->IDL_server_id);
	//TODO: define the error code for non-recovered parent
	// thinking...2222
	if (retval == -EINVAL) {
		id = desc->IDL_parent_id;
		call_desc_update(id, state_IDL_fname);
		goto again;
	} 

	assert(retval);
	desc->state = IDL_init_state;       // set the state to the initial state
	desc->fault_cnt = global_fault_cnt; // set the fault counter to the global
	block_cli_if_recover_data(desc);
}
// block_cli_if_basic_id 2 end

// block_cli_if_basic_id pred 3 start
desc_dep_create_none&desc_global_false
// block_cli_if_basic_id pred 3 end
// block_cli_if_basic_id 3 start
static inline void block_cli_if_basic_id(int id) {

	assert(id);
	struct desc_track *desc = call_desc_lookup(id);
	assert(desc);
	
	int retval = 0;
	retval = IDL_fname(IDL_desc_saved_params);
	assert(retval);

	struct desc_track *new_desc = call_desc_lookup(retval);
	assert(new_desc);
	
	desc->IDL_server_id = new_desc->IDL_server_id;
	desc->state = IDL_init_state;       // set the state to the initial state
	desc->fault_cnt = global_fault_cnt; // set the fault counter to the global

	call_desc_dealloc(new_desc);
	block_cli_if_recover_data(desc);
}
// block_cli_if_basic_id 3 end

// block_cli_if_basic_id pred 4 start
desc_dep_create_none&desc_global_true
// block_cli_if_basic_id pred 4 end
// block_cli_if_basic_id 4 start
static inline void block_cli_if_basic_id(int id) {
}
// block_cli_if_basic_id 4 end

// block_cli_if_basic_id no match start
static inline void block_cli_if_basic_id(int id) {
}
// block_cli_if_basic_id no match end


/*****************************/
/* client block_cli_if_upcall_creator */
/*****************************/
// block_cli_if_upcall_creator pred 1 start
desc_global_true
// block_cli_if_upcall_creator pred 1 end
// block_cli_if_upcall_creator 1 start
static inline void block_cli_if_upcall_creator(int id) {
	IDL_service_upcall_creator(cos_spd_id(), id);
}
// block_cli_if_upcall_creator 1 end

// block_cli_if_upcall_creator pred 2 start
desc_global_false
// block_cli_if_upcall_creator pred 2 end
// block_cli_if_upcall_creator 2 start
static inline void block_cli_if_upcall_creator(int id) {
}
// block_cli_if_upcall_creator 2 end

// block_cli_if_upcall_creator no match start
static inline void block_cli_if_upcall_creator(int id) {
}
// block_cli_if_upcall_creator no match end


/**************************************/
/* client block_cli_if_recover_upcall */
/**************************************/
// block_cli_if_recover_upcall pred 1 start
desc_global_true&desc_dep_create_same
// block_cli_if_recover_upcall pred 1 end
// block_cli_if_recover_upcall 1 start
static inline void block_cli_if_recover_upcall(int id) {
	assert(id);
	block_cli_if_recover(id);
	block_cli_if_recover_subtree(id);
}
// block_cli_if_recover_upcall 1 end

// block_cli_if_recover_upcall pred 2 start
desc_global_true&desc_dep_create_diff
// block_cli_if_recover_upcall pred 2 end
// block_cli_if_recover_upcall 2 start
static inline void block_cli_if_recover_upcall(int id) {
	assert(id);
	block_cli_if_recover(id);
	block_cli_if_recover_subtree(id);
}
// block_cli_if_recover_upcall 2 end

// block_cli_if_recover_upcall no match start
// block_cli_if_recover_upcall no match end

/**************************************/
/* client block_cli_if_recover_subtree */
/**************************************/
// block_cli_if_recover_subtree pred 1 start
desc_create_diff&desc_close_subtree
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

// block_cli_if_recover_subtree pred 2 start
desc_close_itself
terminal
// block_cli_if_recover_subtree pred 2 end
// block_cli_if_recover_subtree 2 start
static inline void block_cli_if_recover_subtree(int id) {
}
// block_cli_if_recover_subtree 2 end

// block_cli_if_recover_subtree no match start
static inline void block_cli_if_recover_subtree(int id) {
}
// block_cli_if_recover_subtree no match end

/*****************************/
/* client block_cli_if_track */
/*****************************/

// block_cli_if_track pred 1 start
desc_global_false
creation
// block_cli_if_track pred 1 end
// block_cli_if_track 1 start
static inline int block_cli_if_track_IDL_fname(int ret, IDL_parsdecl) {
	// if ret does not exist, just return as it is, thinking....
	if (ret == -EINVAL) return ret;

	struct desc_track *desc = call_desc_alloc();
	assert(desc);
	call_desc_cons(desc, ret, IDL_params);
	IDL_curr_state;

	return desc->IDL_id;
}
// block_cli_if_track 1 end

// block_cli_if_track pred 2 start
desc_global_true
creation
// block_cli_if_track pred 2 end
// block_cli_if_track 2 start
static inline int block_cli_if_track_IDL_fname(int ret, IDL_parsdecl) {
	// if ret does not exist, just return as it is, thinking....
	if (ret == -EINVAL) return ret;

	struct desc_track *desc = call_desc_alloc(ret);
	assert(desc);
	call_desc_cons(desc, ret, IDL_params);
	IDL_curr_state;

	return desc->IDL_id;
}
// block_cli_if_track 2 end

// block_cli_if_track pred 3 start
desc_dep_close_removal
terminal
// block_cli_if_track pred 3 end
// block_cli_if_track 3 start
static inline int block_cli_if_track_IDL_fname(int ret, IDL_parsdecl) {
	struct desc_track *desc = call_desc_lookup(IDL_id);
	if (desc) call_desc_dealloc(desc);

	return ret;
}
// block_cli_if_track 3 end

// block_cli_if_track pred 4 start
desc_dep_close_keep
terminal
// block_cli_if_track pred 4 end
// block_cli_if_track 4 start
static inline int block_cli_if_track_IDL_fname(int ret, IDL_parsdecl) {
	struct desc_track *desc = call_desc_lookup(IDL_id);
	if (desc) call_desc_dealloc(desc);

	return ret;
	// TODO: this needs to be changed
	/* struct desc_track *child_desc_list = desc->child_desc_list;	 */
	/* if (EMPTY_LIST(child_desc_list)) { */
	/* 	call_desc_dealloc(desc); */
	/* } */
}
// block_cli_if_track 4 end

// block_cli_if_track pred 5 start
terminal
// block_cli_if_track pred 5 end
// block_cli_if_track 5 start
static inline int block_cli_if_track_IDL_fname(int ret, IDL_parsdecl) {
	struct desc_track *desc = call_desc_lookup(IDL_id);
	if(desc) call_desc_dealloc(desc);

	return ret;
}
// block_cli_if_track 5 end

// block_cli_if_track pred 6 start
desc_global_false
transition
// block_cli_if_track pred 6 end
// block_cli_if_track 6 start
static inline int block_cli_if_track_IDL_fname(int ret, IDL_parsdecl) {
	struct desc_track *desc = call_desc_lookup(IDL_id);
	if (desc) { IDL_curr_state; }
	return ret;
}
// block_cli_if_track 6 end


// block_cli_if_track pred 7 start
desc_global_true
transition
// block_cli_if_track pred 7 end
// block_cli_if_track 7 start
static inline int block_cli_if_track_IDL_fname(int ret, IDL_parsdecl) {
	struct desc_track *desc = call_desc_lookup(IDL_id);
	if (desc) { IDL_curr_state; }

	if (ret == -EINVAL) {
		call_desc_update(IDL_id, state_IDL_fname);
		ret = -ELOOP;
	}

	return ret;
}
// block_cli_if_track 7 end


// block_cli_if_track_IDL_fname no match start
static inline int block_cli_if_track_IDL_fname(int ret, IDL_parsdecl) {
}
// block_cli_if_track_IDL_fname no match end

/************************************/
/* client block_cli_if_recover_data */
/************************************/
// block_cli_if_recover_data pred 1 start
resc_has_data_true
// block_cli_if_recover_data pred 1 end
// block_cli_if_recover_data 1 start
static inline void call_restore_data(struct desc_track *desc) {}
static inline void block_cli_if_recover_data(struct desc_track *desc) {
	assert(desc);
	call_restore_data(desc);
}
// block_cli_if_recover_data 1 end

// block_cli_if_recover_data no match start
static inline void block_cli_if_recover_data(struct desc_track *desc) {
}
// block_cli_if_recover_data no match end

/************************************/
/* client block_cli_if_save_data    */
/************************************/
// block_cli_if_save_data pred 1 start
desc_has_data_true
// block_cli_if_save_data pred 1 end
// block_cli_if_save_data 1 start
static inline void call_save_data(int id, void *data) {}

static inline void block_cli_if_save_data(int id, void *data) {
	call_save_data(id, data);
}
// block_cli_if_save_data 1 end

// block_cli_if_save_data no match start
static inline void block_cli_if_save_data(int id, void *data) {
}
// block_cli_if_save_data no match end

/********************************************/
/* client interface recovery upcall_entry   */
/********************************************/
// block_cli_if_recover_upcall_entry pred 1 start
desc_global_true
// block_cli_if_recover_upcall_entry pred 1 end
// block_cli_if_recover_upcall_entry 1 start
void IDL_service_cli_if_recover_upcall_entry(int id) {
	block_cli_if_recover_upcall(id);
}
// block_cli_if_recover_upcall_entry 1 end

// block_cli_if_recover_upcall_entry no match start
// block_cli_if_recover_upcall_entry no match end

/********************************************/
/* client interface call recovery_upcall_extern   */
/********************************************/
// block_cli_if_recover_upcall_extern pred 1 start
desc_global_true&desc_create_diff
// block_cli_if_recover_upcall_extern pred 1 end
// block_cli_if_recover_upcall_extern 1 start
extern void call_recover_upcall(int dest_spd, int id);
// block_cli_if_recover_upcall_extern 1 end

// block_cli_if_recover_upcall_extern no match start
// block_cli_if_recover_upcall_extern no match end

// client func decl start
static inline void call_desc_cons(struct desc_track *desc, int id, IDL_parsdecl) {
	assert(desc);

	desc->IDL_server_id = id;

	IDL_desc_cons;

	desc->fault_cnt = global_fault_cnt;

	return;
}

// client func decl end

/* the template for state transition  */
// client state transition start
if ((from_state == IDL_current_state) && (to_state == IDL_next_state)) {
	IDL_state_transition_call
	goto done;
}      
// client state transition end

/*****************************/
/* client interface cstub fn */
/*****************************/
/* // client cstub startbak */
/* CSTUB_FN(IDL_fntype, IDL_fname) (struct usr_inv_cap *uc, IDL_parsdecl) { */
/* 	long fault = 0; */
/* 	int ret = 0; */
	
/* 	call_map_init(); */
/* redo: */
/* 	block_cli_if_desc_update_IDL_fname(IDL_id); */

/* 	IDL_benchmark_end */

/* 	ret = block_cli_if_invoke_IDL_fname(IDL_params, ret, &fault, uc);  */
/*         if (unlikely (fault)){ */

/* 		IDL_benchmark_start */

/* 		CSTUB_FAULT_UPDATE(); */
/*                 goto redo; */
/*         } */
/* 	ret = block_cli_if_track_IDL_fname(ret, IDL_params); */
 
/*         return ret; */
/* } */
/* // client cstub endbak */


// client cstub start
CSTUB_FN(IDL_fntype, IDL_fname) (struct usr_inv_cap *uc, IDL_parsdecl) {
	long fault = 0;
	int ret = 0;
	
	call_map_init();

redo:
	block_cli_if_desc_update_pre_IDL_fname(IDL_idIDL_parent_id);

	IDL_benchmark_end

	ret = block_cli_if_invoke_IDL_fname(IDL_params, ret, &fault, uc); 
        if (unlikely (fault)){

		IDL_benchmark_start

		CSTUB_FAULT_UPDATE();
		if (block_cli_if_desc_update_post_fault_IDL_fname(IDL_idIDL_parent_id)) {
			goto redo;
		}
        }
	ret = block_cli_if_track_IDL_fname(ret, IDL_params);

	if (unlikely(ret == -ELOOP)) goto redo;

        return ret;
}
// client cstub end

// client cstub no redo start
CSTUB_FN(IDL_fntype, IDL_fname) (struct usr_inv_cap *uc, IDL_parsdecl) {
	long fault = 0;
	int ret = 0;
	
	call_map_init();

	ret = block_cli_if_invoke_IDL_fname(IDL_params, ret, &fault, uc); 
        if (unlikely (fault)){

		IDL_benchmark_start

		CSTUB_FAULT_UPDATE();
		block_cli_if_desc_update_IDL_fname(IDL_id);		

		IDL_benchmark_end

		return ret;
        }
	ret = block_cli_if_track_IDL_fname(ret, IDL_params);
 
        return ret;
}
// client cstub no redo end

/*  ___  ___ _ ____   _____ _ __  (_)_ __ | |_ ___ _ __ / _| __ _  ___ ___  */
/* / __|/ _ \ '__\ \ / / _ \ '__| | | '_ \| __/ _ \ '__| |_ / _` |/ __/ _ \ */
/* \__ \  __/ |   \ V /  __/ |    | | | | | ||  __/ |  |  _| (_| | (_|  __/ */
/* |___/\___|_|    \_/ \___|_|    |_|_| |_|\__\___|_|  |_|  \__,_|\___\___| */

// server track start
struct track_block {
	int IDL_id;
	struct track_block *next, *prev;
};
struct track_block tracking_block_list[MAX_NUM_SPDS]; 

// server track end

/***********************************/
/* server block_ser_if_block_track */
/***********************************/
// block_ser_if_block_track pred 1 start
server_block
// block_ser_if_block_track pred 1 end
// block_ser_if_block_track 1 start
static inline int block_ser_if_block_track_IDL_fname(IDL_parsdecl) {
	int ret = 0;
	struct track_block tb;  // track on stack

	IDL_TAKE;
	
	if (unlikely(!tracking_block_list[IDL_from_spd].next)) {
		INIT_LIST(&tracking_block_list[IDL_from_spd], next, prev);
	}
	INIT_LIST(&tb, next, prev);
	tb.IDL_id = IDL_id;
	ADD_LIST(&tracking_block_list[IDL_from_spd], &tb, next, prev);

	IDL_RELEASE;

	ret = IDL_fname(IDL_params);

	IDL_TAKE;

	REM_LIST(&tb, next, prev);

	IDL_RELEASE;

	return ret;
}

IDL_fntype __ser_IDL_fname(IDL_parsdecl) {
	return block_ser_if_block_track_IDL_fname(IDL_params);
}

// block_ser_if_block_track 1 end

// block_ser_if_block_track no match start
static inline int block_ser_if_block_track_IDL_fname(IDL_parsdecl) {
}
// block_ser_if_block_track no match end

/***********************************/
/* server block_ser_if_client_fault_notification */
/***********************************/
// block_ser_if_client_fault_notification pred 1 start
server_wakeup
// block_ser_if_client_fault_notification pred 1 end
// block_ser_if_client_fault_notification 1 start
static inline void block_ser_if_client_fault_notification(int IDL_from_spd) {
	struct track_block *tb;	
	
	IDL_TAKE;

	if (!tracking_block_list[IDL_from_spd].next) goto done;
	if (EMPTY_LIST(&tracking_block_list[IDL_from_spd], next, prev)) goto done;

	for (tb = FIRST_LIST(&tracking_block_list[IDL_from_spd], next, prev);
	     tb != &tracking_block_list[IDL_from_spd];
	     tb = FIRST_LIST(tb, next, prev)) {

		IDL_RELEASE;

		IDL_fname(IDL_params);

		IDL_TAKE;
	}

done:
	IDL_RELEASE;

	return;
}

void __ser_IDL_service_client_fault_notification(int IDL_from_spd) {
	block_ser_if_client_fault_notification(IDL_from_spd);
	return;
}

// block_ser_if_client_fault_notification 1 end

// block_ser_if_client_fault_notification no match start
static inline void block_ser_if_client_fault_notification(spdid_t spdid) {
}
// block_ser_if_client_fault_notification no match end

/***********************************/
/* server block_ser_if_recreate_exist */
/***********************************/
// block_ser_if_recreate_exist pred 1 start
desc_global_true
// block_ser_if_recreate_exist pred 1 end
// block_ser_if_recreate_exist 1 start
IDL_fntype __ser_IDL_fname_exist(IDL_parsdecl, int existing_id) {
	IDL_fntype ret = 0;
	ret = IDL_fname_exist(IDL_params, existing_id);
	return ret;
}
// block_ser_if_recreate_exist 1 end

// block_ser_if_recreate_exist pred 2 start
desc_global_false
// block_ser_if_recreate_exist pred 2 end
// block_ser_if_recreate_exist 2 start
IDL_fntype __ser_IDL_recreate_fname_exist(IDL_parsdecl, int existing_id) {
}
// block_ser_if_recreate_exist 2 end

// block_ser_if_recreate_exist no match start
// block_ser_if_recreate_exist no match end

/***********************************/
/* server block_ser_if_upcall_creator */
/***********************************/
// block_ser_if_upcall_creator pred 1 start
desc_global_true
// block_ser_if_upcall_creator pred 1 end
// block_ser_if_upcall_creator 1 start
extern int ns_upcall(spdid_t spdid, int id);
int __ser_IDL_service_upcall_creator(spdid_t spdid, int id) {
	int ret = 0;
	ns_upcall(spdid, id);
	return ret;
}
// block_ser_if_upcall_creator 1 end

// block_ser_if_upcall_creator pred 2 start
desc_global_false
// block_ser_if_upcall_creator pred 2 end
// block_ser_if_upcall_creator 2 start
int __ser_IDL_service_upcall_creator(spdid_t spdid, int id) {
}
// block_ser_if_upcall_creator 2 end

// block_ser_if_upcall_creator no match start
// block_ser_if_upcall_creator no match end

/*                           _           _ _ _              */
/*  _ __ ___   __ _ _ __ ___| |__   __ _| | (_)_ __   __ _  */
/* | '_ ` _ \ / _` | '__/ __| '_ \ / _` | | | | '_ \ / _` | */
/* | | | | | | (_| | |  \__ \ | | | (_| | | | | | | | (_| | */
/* |_| |_| |_|\__,_|_|  |___/_| |_|\__,_|_|_|_|_| |_|\__, | */
/*                                                   |___/  */

// marshalling ds start

// assumption: marshalled function is not same as the block/wakeup function
struct __ser_IDL_fname_marshalling {
	IDL_marshalling_parsdecl;
	char data[0];
};
// marshalling ds end

/*****************************/
/* client interface cstub fn  (marshalling version)*/
/*****************************/
// client cstub marshalling start
CSTUB_FN(IDL_fntype, IDL_fname) (struct usr_inv_cap *uc, IDL_parsdecl) {
	long fault = 0;
	int ret = 0;

	call_map_init();

        struct __ser_IDL_fname_marshalling *md = NULL;
	cbuf_t cb = 0;
	int sz  = IDL_data_len + sizeof(struct __ser_IDL_fname_marshalling);

redo:
	block_cli_if_desc_update_IDL_fname(IDL_id);

        md = (struct __ser_IDL_fname_marshalling *)cbuf_alloc(sz, &cb);
	assert(md);  // assume we always get cbuf for now

	IDL_benchmark_end

	ret = block_cli_if_marshalling_invoke_IDL_fname(IDL_params, ret, &fault, uc, md, sz, cb);

        if (unlikely (fault)){

		IDL_benchmark_start

		CSTUB_FAULT_UPDATE();
		cbuf_free(cb);
                goto redo;
        }
	cbuf_free(cb);

	ret = block_cli_if_track_IDL_fname(ret, IDL_params);
 
        return ret;
}
// client cstub marshalling end

/**************************************************/
/* client block_cli_if_invoke  marshalling version (from previous)*/
/****************************************************/
// block_cli_if_marshalling_invoke pred 1 start
desc_dep_create_same
creation
// block_cli_if_marshalling_invoke pred 1 end
// block_cli_if_marshalling_invoke 1 start
static inline int block_cli_if_marshalling_invoke_IDL_fname(IDL_parsdecl, int ret, long *fault, struct usr_inv_cap *uc, struct __ser_IDL_fname_marshalling *md, int sz, cbuf_t cb) {
	struct desc_track *parent_desc = NULL;
	// thinking....
	if ((IDL_parent_id > 1) && (parent_desc = call_desc_lookup(IDL_parent_id))) {
		IDL_parent_id = parent_desc->IDL_server_id;
	}

	IDL_marshalling_cons;
	
	long __fault = 0;
	CSTUB_INVOKE(ret, __fault, uc, 3, IDL_from_spd, cb, sz);
	*fault = __fault;
	
	return ret;
}
// block_cli_if_marshalling_invoke 1 end

// block_cli_if_marshalling_invoke pred 2 start
desc_dep_create_diff
creation
// block_cli_if_marshalling_invoke pred 2 end
// block_cli_if_marshalling_invoke 2 start
static inline int block_cli_if_marshalling_invoke_IDL_fname(IDL_parsdecl, int ret, long *fault, struct usr_inv_cap *uc, struct __ser_IDL_fname_marshalling *md, int sz, cbuf_t cb) {
	struct desc_track *parent_desc = NULL;
	// thinking....
	if ((IDL_parent_id > 1) && (parent_desc = call_desc_lookup(IDL_parent_id))) {
		IDL_parent_id = parent_desc->IDL_server_id;
	}

	IDL_marshalling_cons;
	
	long __fault = 0;
	CSTUB_INVOKE(ret, __fault, uc, 3, IDL_from_spd, cb, sz);
	*fault = __fault;
	
	return ret;
}
// block_cli_if_marshalling_invoke 2 end

// block_cli_if_marshalling_invoke pred 3 start
desc_dep_create_none
creation
// block_cli_if_marshalling_invoke pred 3 end
// block_cli_if_marshalling_invoke 3 start
static inline int block_cli_if_marshalling_invoke_IDL_fname(IDL_parsdecl, int ret, long *fault, struct usr_inv_cap *uc, struct __ser_IDL_fname_marshalling *md, int sz, cbuf_t cb) {

	IDL_marshalling_cons;

	long __fault = 0;
	CSTUB_INVOKE(ret, __fault, uc, 3, IDL_from_spd, cb, sz);
	*fault = __fault;

	return ret;
}
// block_cli_if_marshalling_invoke 3 end

// block_cli_if_marshalling_invoke pred 4 start
desc_global_true
transition|terminal
// block_cli_if_marshalling_invoke pred 4 end
// block_cli_if_marshalling_invoke 4 start
static inline int block_cli_if_marshalling_invoke_IDL_fname(IDL_parsdecl, int ret, long *fault, struct usr_inv_cap *uc, struct __ser_IDL_fname_marshalling *md, int sz, cbuf_t cb) {
	struct desc_track *desc = call_desc_lookup(IDL_id);

	long __fault = 0;
	if (desc) {  // might be created in the same component
		IDL_marshalling_desc_cons;
		CSTUB_INVOKE(ret, fault, uc, 3, cb, sz);
	} else {    // might be created in different component
		IDL_marshalling_cons;
		CSTUB_INVOKE(ret, fault, uc, 3, cb, sz);
		if (ret == -1) {   // desc not exist  TODO: change to error code
			block_cli_if_recover(IDL_id);// need upcall
			assert((desc = call_desc_lookup(IDL_id)));
			CSTUB_INVOKE(ret, fault, uc, 3, cb, sz);
		}
	}
	*fault = __fault;

	return ret;
}
// block_cli_if_marshalling_invoke 4 end

// block_cli_if_marshalling_invoke pred 5 start
desc_global_false
transition|terminal
// block_cli_if_marshalling_invoke pred 5 end
// block_cli_if_marshalling_invoke 5 start
static inline int block_cli_if_marshalling_invoke_IDL_fname(IDL_parsdecl, int ret, long *fault, struct usr_inv_cap *uc, struct __ser_IDL_fname_marshalling *md, int sz, cbuf_t cb) {
	struct desc_track *desc = call_desc_lookup(IDL_id);
	assert(desc);  // must be created in the same component
	IDL_marshalling_desc_cons;

	long __fault = 0;
	CSTUB_INVOKE(ret, fault, uc, 3, cb, sz);
	*fault = __fault;

	return ret;
}
// block_cli_if_marshalling_invoke 5 end

// block_cli_if_marshalling_invoke no match start
static inline int block_cli_if_marshalling_invoke_IDL_fname(IDL_parsdecl, int ret, long *fault, struct usr_inv_cap *uc, struct __ser_IDL_fname_marshalling *md, int sz) {
}
// block_cli_if_marshalling_invoke no match end

/**************************************************/
/* server block_ser_if_invoke marshalling fn
/****************************************************/
// server marshalling_invoke start
IDL_fntype __ser_IDL_fname(IDL_decl_from_spd,  cbuf_t cbid, int len) {
	struct __ser_IDL_fname_marshalling *md = NULL;
	
	md = (struct __ser_IDL_fname_marshalling *)cbuf2buf(cbid, len);
	assert(md);

	/* // for IDL now, ignore these checking */
	/* if (unlikely(md->len[0] != 0)) return -2;  */
	/* if (unlikely(md->len[0] > d->len[1])) return -3; */
	/* if (unlikely(((int)(md->len[1] + sizeof(struct __ser_tsplit_data))) != len)) return -4; */
	/* if (unlikely(md->tid == 0)) return -EINVAL; */
	
	return IDL_fname(IDL_marshalling_finalpars);
}
// server marshalling_invoke end
