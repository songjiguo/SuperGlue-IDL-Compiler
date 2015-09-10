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

#===========================================================================
#     
# desc_close_itself      = "itself"              # C0
# desc_close_subtree     = "subtree"             # C1
# 
# desc_create_single     = "nodep"               # P0
# desc_create_same       = "same"                # P1
# desc_create_diff       = "different"           # P2
# 
# desc_close_remove      = "remove"              # Y0
# desc_close_keep        = "keep"                # Y1
# 
# desc_global            = "global"              # G
# desc_local             = "local"               # 
# desc_has_data          = "desc_has_data"       # D_ir
# resc_has_data          = "resc_has_data"       # D_r
# desc_has_no_data       = "desc_has_no_data"    # 
# resc_has_no_data       = "resc_has_no_data"    #
# func_create            = "create"              
# func_mutate            = "mutate"
# func_terminate         = "terminate"   
#===========================================================================

# pycparser related
typedecl                    = "TypeDecl"
funcdecl                    = "FuncDecl"
ptrdecl                     = "PtrDecl"
        
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
    
  
DEBUG = 0
def printc(s):
    if DEBUG:
        print (s)
