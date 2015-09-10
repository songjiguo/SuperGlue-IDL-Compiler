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
        self.blks = []
        
    def show(self):
        print (self.blks)

    def add_block(self, predicate_tup):
        self.blks.append(predicate_tup)        

curr_tuple = []
curr_func = []
cond = []
#=============================================
# function for evaluate conditions
#=============================================

def __condition_lookup(condition, info_dict):
    for item_k, item_v in info_dict.iteritems():
        if (item_k == condition):
            return item_v
    return False

def __isTupleCondition(key, cond):
    printc ("<<condition:>> %s " % key + cond)
    if (__condition_lookup(key, curr_tuple.info) == cond):
        return True
    else:
        return False

def __retvalTupleCondition(key):
    printc ("<<condition:>> %s " % key)
    ret = __condition_lookup(key, curr_tuple.info)
    return ret

def __isFuncCondition(key, cond):
    printc ("<<condition:>> %s " % key + cond)
    if (__condition_lookup(key, curr_func.info) == cond):
        return True
    else:
        return False

def __retvalFuncCondition(key):
    printc ("<<condition:>> %s " % key)
    ret = __condition_lookup(key, curr_func.info)
    return ret

#===============================================================================
# # test a condition, return true or false
#===============================================================================
def isDescCloseItself():
    return True
#    return __isTupleCondition(curr_tuple.desc_close, 
#                              cond.desc_close_itself)

def isDescCloseSubtree():
    return False
    #return __isTupleCondition(curr_tuple.desc_close, 
     #                         cond.desc_close_subtree)

def isDescDepCrtSingle():
    return True
    return __isTupleCondition(curr_tuple.desc_dep_create, 
                              cond.desc_create_single)

def isDescDepCrtSame():
    return __isTupleCondition(curr_tuple.desc_dep_create, 
                              cond.desc_create_same)

def isDescDepCrtDiff():
    return __isTupleCondition(curr_tuple.desc_dep_create, 
                              cond.desc_create_diff)

def isDescDepCloseRemove():
    return __isTupleCondition(curr_tuple.desc_dep_close, 
                              cond.desc_close_remove)

def isDescDepCloseKeep():
    return __isTupleCondition(curr_tuple.desc_dep_close, 
                              cond.desc_close_keep)

def isDescGlobal():
    return __isTupleCondition(curr_tuple.desc_global, 
                              cond.desc_global)
    
def isDescHasData():
    return __isTupleCondition(curr_tuple.desc_has_data, 
                              cond.desc_has_data)    

def isRescHasData():
    return __isTupleCondition(curr_tuple.resc_has_data, 
                              cond.resc_has_data) 
    
def isFunCreate():       
    return __isFuncCondition(curr_func.sm_state, 
                              cond.func_create) 

def isFunMutate():       
    return __isFuncCondition(curr_func.sm_state, 
                              cond.func_mutate) 

def isFunTerminate():       
    return __isFuncCondition(curr_func.sm_state, 
                              cond.func_terminate) 

#===============================================================================
# # test a condition, return the values if true, otherwise none
#===============================================================================
def retvalDescBlock():
    return __retvalTupleCondition(curr_tuple.desc_block)

def retvalFuncSM():
    return __retvalFuncCondition(curr_func.sm_state)

def retvalFuncName():
    return __retvalFuncCondition(curr_func.name)

def retvalFuncDescData():
    return __retvalFuncCondition(curr_func.desc_data)

def retvalFuncDescDataRetVal():
    return __retvalFuncCondition(curr_func.desc_data_retval)

def retvalFuncDescDataRemove():
    return __retvalFuncCondition(curr_func.desc_data_remove)

def retvalFuncDescLookup():
    return __retvalFuncCondition(curr_func.desc_lookup)

def retvalFuncDescDataAdd():
    return __retvalFuncCondition(curr_func.desc_data_add)

def retvalFuncRescDataAdd():
    return __retvalFuncCondition(curr_func.resc_data_add)

def test_tuple_condition():
    printc (isDescCloseItself())
    printc (isDescCloseSubtree())
    printc (isDescDepCrtSingle())
    printc (isDescDepCrtSame())
    printc (isDescDepCrtDiff())
    printc (isDescDepCloseRemove())
    printc (isDescDepCloseKeep())
    printc (isDescGlobal())
    printc (isDescHasData())
    printc (isRescHasData())
    printc (retvalDescBlock())

def test_function_condition():
    printc (retvalFuncName())
    printc (retvalFuncSM()) 
    printc (isFunCreate())
    printc (isFunMutate())           
    printc (isFunTerminate())           
    printc (retvalFuncDescData())
    printc (retvalFuncDescDataRetVal())
    printc (retvalFuncDescDataRemove())
    printc (retvalFuncDescLookup())
    printc (retvalFuncDescDataAdd())
    printc (retvalFuncRescDataAdd())

DEBUG = 0
def printc(s):
    if DEBUG:
        print (s)

