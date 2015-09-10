#!/usr/bin/python
#title            :keywords.py
#description      :This script defines the dictionary class for
#                     -- function
#                     -- tuple
#author           :Jiguo Song
#date             :20150817
#version          :0.1
#notes            :
#python_version   :2.6.6
#
#==============================================================================

from pprint import pprint

# -----> ignore the below. rewrite the description for global and function, and 
# blocks (predicate, code) etc

# pycparser related
typedecl                    = "TypeDecl"
funcdecl                    = "FuncDecl"
ptrdecl                     = "PtrDecl"

# basic condition
class IDLRule(object):
    def __init__(self, cond):
        self.rule_list = []
        init_rule_keyword(self, cond)

# interface
class IDLInterface(object):
    def __init__(self):
        self.interf_list = []
        init_interface_keyword(self)
   
# the keywords must be consistent with ones defined in cidl_gen (macro in cidl_gen)
class IDLBlock(object):
    def __init__(self):
        self.list  = []

    def add_blk(self, pred, code, blkname):
        self.list.append((pred, code, blkname))

    def show(self):
        for item in self.list:
            print (item[2])     #--- blk name            
            print (item[0])     #--- pred
            print (item[1])     #--- code
            
# basic functional block for generating the interface
class IDLBlocks(object):
    def __init__(self):
        self.block_list = []
        init_block_keyword(self) 
        
# each service has a tuple and multiple functions
class IDLServices(object):
    def __init__(self): 
        self.tuple = []
        
    def add_tuple(self):
        self.tuple.append(IDLTuple())
    
# the keywords must be consistent with ones defined in cidl_gen (macro in cidl_gen)
class IDLTuple(object):
    def __init__(self):
        init_tuple_keyword(self)
        self.info = {} 
        init_tuple_info(self)        
        self.functions = []
        
    def add_function(self):
        self.functions.append(IDLFunction())

# function class
class IDLFunction():
    def __init__(self):
        init_function_keyword(self)
        self.info = {} 
        init_func_info(self)
        
        self.normal_para = []

def init_function_keyword(node):
    node.name                    = "funcname"
    node.sm_state                = "funcsm" 
    node.desc_data               = "desc_data"
    node.desc_data_retval        = "desc_data_retval" #--> in the form of (type, value)
    node.desc_data_remove        = "desc_data_remove" #--> in the form of (value)
    node.desc_lookup             = "desc_lookup" #--> in the form of (vale, type)
    node.desc_data_add           = "desc_data_add" #--> in the form of (target_to_add, value, type)
    node.resc_data_add           = "resc_data"     #--> in the form of (desc_id, value, type)


def init_tuple_keyword(node):
    node.desc_close              = "desc_close"
    node.desc_dep_create         = "desc_dep_create"
    node.desc_dep_close          = "desc_dep_close"
    node.desc_global             = "desc_global"
    node.desc_block              = "desc_block" #--> in the form of (desc_block, T/F, [component])
    node.desc_has_data           = "desc_has_data"
    node.resc_has_data           = "resc_has_data"    
    node.sm_create               = "create"
    node.sm_mutate               = "mutate"
    node.sm_terminate            = "terminate"
  
def init_pred_code():
    # global 
    desc_close              = "desc_close"
    desc_dep_create         = "desc_dep_create"
    desc_dep_close          = "desc_dep_close"
    desc_global             = "desc_global"
    desc_block              = "desc_block" #--> in the form of (desc_block, T/F, [component])
    desc_has_data           = "desc_has_data"
    resc_has_data           = "resc_has_data"    
    sm_create               = "create"
    sm_mutate               = "mutate"
    sm_terminate            = "terminate"
    
    # fnlobal 
    name                    = "funcname"
    sm_state                = "funcsm" 
    desc_data               = "desc_data"
    desc_data_retval        = "desc_data_retval" #--> in the form of (type, value)
    desc_data_remove        = "desc_data_remove" #--> in the form of (value)
    desc_lookup             = "desc_lookup" #--> in the form of (vale, type)
    desc_data_add           = "desc_data_add" #--> in the form of (target_to_add, value, type)
    resc_data_add           = "resc_data"     #--> in the form of (desc_id, value, type)
    
        
    desc_close_itself      = "itself"              # C0
    desc_close_subtree     = "subtree"             # C1
    
    desc_create_single     = "nodep"               # P0
    desc_create_same       = "same"                # P1
    desc_create_diff       = "different"           # P2
    
    desc_close_remove      = "remove"              # Y0
    desc_close_keep        = "keep"                # Y1
    
    desc_global            = "global"              # G
    desc_local             = "local"               # 
    desc_has_data          = "desc_has_data"       # D_ir
    resc_has_data          = "resc_has_data"       # D_r
    desc_has_no_data       = "desc_has_no_data"    # 
    resc_has_no_data       = "resc_has_no_data"    #
    func_create            = "create"              
    func_mutate            = "mutate"
    func_terminate         = "terminate"   

# define the block rules                         
def init_rule_keyword(node,cond):
    # client interface
    node.cli_if_invoke                 = []
    node.cli_if_invoke_ser_intro       = []
    node.cli_if_recover                = []
    node.cli_if_basic_id               = []
    node.cli_if_recover_upcal          = []
    node.cli_if_recover_subtree        = []
    node.cli_if_track                  = [[cond.desc_close_remove, cond.func_terminate],
                                          [cond.desc_close_keep, cond.func_terminate],
                                          [cond.func_create]]
    node.cli_if_recover_init           = []
    node.cli_if_recover_data           = []
    node.cli_if_save_data              = []
    # server interface
    node.ser_if_reflection_track       = []
    node.ser_if_reflection_unblock     = []
    node.ser_if_save_data              = []
    node.ser_if_recover_upcall         = []
    # server
    node.ser_reflectoin                = []
    node.ser_introspection             = []
    node.ser_lookup                    = []
    
    # add all above into a list
    for tmp in vars(node):
        if (tmp != "rule_list"):
            node.rule_list.append(tmp)
    
def init_func_info(func):
    func.info[func.name]                = []
    func.info[func.sm_state]            = []
    func.info[func.desc_data_retval]    = []
    func.info[func.desc_data_remove]    = []
    func.info[func.desc_data]           = []
    func.info[func.desc_lookup]         = []
    func.info[func.desc_data_add]       = []
    func.info[func.resc_data_add]       = []

def init_tuple_info(tup):
    tup.info[tup.desc_close]            = []
    tup.info[tup.desc_dep_create]       = []
    tup.info[tup.desc_dep_close]        = []
    tup.info[tup.desc_global]           = []
    tup.info[tup.desc_block]            = []
    tup.info[tup.desc_has_data]         = []
    tup.info[tup.resc_has_data]         = []
    tup.info[tup.sm_create]             = []
    tup.info[tup.sm_mutate]             = []
    tup.info[tup.sm_terminate]          = []
    
  
def init_interface_keyword(node):
    node.client                  = "client"
    node.server                  = "server"
    node.kernel                  = "kernel"
    
    # add all above into a list
    for tmp in vars(node):
        if (tmp != "interf_list"):
            node.interf_list.append(tmp)

def init_block_keyword(node):
    # client interface
    node.cli_if_invoke              = "BLOCK_CLI_IF_INVOKE"
    node.cli_if_invoke_ser_intro    = "BLOCK_CLI_IF_INVOKE_SER_INTRO"
    node.cli_if_recover             = "BLOCK_CLI_IF_RECOVER"
    node.cli_if_basic_id            = "BLOCK_CLI_IF_BASIC_ID"
    node.cli_if_recover_upcall      = "BLOCK_CLI_IF_RECOVER_UPCALL"
    node.cli_if_recover_subtree     = "BLOCK_CLI_IF_RECOVER_SUBTREE"
    node.cli_if_track               = "BLOCK_CLI_IF_TRACK"
    node.cli_if_recover_init        = "BLOCK_CLI_IF_RECOVER_INIT"
    node.cli_if_recover_data        = "BLOCK_CLI_IF_RECOVER_DATA"
    node.cli_if_save_data           = "BLOCK_CLI_IF_SAVE_DATA"
    # server interface
    node.ser_if_reflection_track    = "BLOCK_SER_IF_REFLECTION_TRACK"
    node.ser_if_reflection_unblock  = "BLOCK_SER_IF_REFLECTION_UNBLOCK"
    node.ser_if_save_data           = "BLOCK_SER_IF_REFLECTION_SAVE_DATA"
    node.ser_if_recover_upcall      = "BLOCK_SER_IF_REFLECTION_RECOVER_UPCALL"
    # server
    node.ser_reflectoin             = "BLOCK_SER_REFLECTION"
    node.ser_introspection          = "BLOCK_SER_INTROSPECTION"
    node.ser_lookup                 = "BLOCK_SER_LOOKUP"

    # add all above into a list
    for tmp in vars(node):
        if (tmp != "block_list"):
            node.block_list.append(tmp)    
        
DEBUG = 0
def printc(s):
    if DEBUG:
        print (s)
