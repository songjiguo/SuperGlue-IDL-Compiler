#------------------------------------------------------------------------------
# pycparser: c-to-c.py
#
# Example of using pycparser.c_generator, serving as a simplistic translator
# from C to AST and back to C.
#
# Copyright (C) 2008-2015, Eli Bendersky
# License: BSD
#------------------------------------------------------------------------------
from __future__ import print_function
from pycparser import parse_file, preprocess_file
from pycparser import c_parser, c_ast, c_generator, c_parser, c_generator
from c3_parser import print_logo
from pprint import pprint
from keywords import printc

import keywords, predicates
import copy
import subprocess
from reportlab.pdfbase.pdfform import GLOBALFONTSDICTIONARY
from pydoc import describe

class MyCode():
    def __init__(self):
        self.ifcode = {}
        
    def add_interface(self, if_name):
        self.ifcode[if_name] = InterfaceCode(if_name)

class InterfaceCode(object):
    def __init__(self, interface_name):
        self.blocks = {}
        self.name   = interface_name
    
    def add_block(self, block_name):
        self.blocks[block_name] = BlockCode(block_name)

class BlockCode(object):
    def __init__(self, block_name):
        self.rules = []
        self.name   = block_name
        self.block_code = ""
        
    def gen_code(self, arg1, *args):   # pass the conditions in
        #print ("Python does not support function overloading")
        #self.set_name(arg1)
        
        if args:
            #for item in args:
            #    print (item)
            self.block_code = "hello world" 
        else:
            print (arg1)
            
    def set_name(self, block_name):
        name = block_name

    def show_code(self):
        print (self.block_code)  
        
def traverse(o, tree_types=(list, tuple)):
    if isinstance(o, tree_types):
        for value in o:
            for subvalue in traverse(value, tree_types):
                yield subvalue
    else:
        yield o
        
########################
##  blocks
########################

def block_cli_if_save_data():
    #print ("[[BLOCK_CLI_IF_SAVE_DATA]]")
    BLOCK_CLI_IF_SAVE_DATA = keywords.IDLBlock()
    
    pred = ["desc_has_data_true"]
    code = "save_data(id, data);\n" \

    BLOCK_CLI_IF_SAVE_DATA.add_blk(pred, code, "BLOCK_CLI_IF_SAVE_DATA")
    
    return BLOCK_CLI_IF_SAVE_DATA

def block_cli_if_rec_data():
    #print ("[[BLOCK_CLI_IF_REC_DATA]]")
    BLOCK_CLI_IF_REC_DATA = keywords.IDLBlock()
    
    pred = ["resc_has_data_true"]
    code = "assert(desc);\n" \
"data = introspect_data(desc->id);\n" \
"\n" \
"assert(data);\n" \
"restore_data(data);\n" \

    BLOCK_CLI_IF_REC_DATA.add_blk(pred, code, "BLOCK_CLI_IF_REC_DATA")
    
    return BLOCK_CLI_IF_REC_DATA

def block_cli_if_rec_init():
   #print ("[[BLOCK_CLI_IF_REC_INIT]]")
    BLOCK_CLI_IF_REC_INIT = keywords.IDLBlock()
    
    pred = ["create"]
    code = "assert(desc);\n" \
"func_name(desc->saved_params);\n" \

    BLOCK_CLI_IF_REC_INIT.add_blk(pred, code, "BLOCK_CLI_IF_REC_INIT")
    
    return BLOCK_CLI_IF_REC_INIT
   
def block_cli_if_track():
    #print ("[[BLOCK_CLI_IF_TRACK]]")    
    BLOCK_CLI_IF_TRACK = keywords.IDLBlock()
    
    pred = ["create"]
    code = "desc = desc_alloc(ret);\n" \
"assert(desc);\n" \
"\n" \
"desc_save(desc, ret, params);\n" \

    BLOCK_CLI_IF_TRACK.add_blk(pred, code, "BLOCK_CLI_IF_TRACK")
        
    pred = ["desc_dep_close_remove",
            "terminate"]
    code = "assert(desc);\n" \
"desc_dealloc(desc);\n" \

    BLOCK_CLI_IF_TRACK.add_blk(pred, code, "BLOCK_CLI_IF_TRACK")
    
    pred = ["desc_dep_close_keep",
            "terminate"]
    code = "assert(desc);\n" \
"\n" \
"child_desc_list = desc->child_desc_list;\n" \
"if (EMPTY_LIST(child_desc_list)) {\n" \
"    desc_dealloc(desc);\n" \
"}\n" \

    BLOCK_CLI_IF_TRACK.add_blk(pred, code, "BLOCK_CLI_IF_TRACK")
    
    return BLOCK_CLI_IF_TRACK              
            

def block_cli_if_recover_subtree():
    #print ("[[BLOCK_CLI_IF_RECOVER_SUBTREE]]")
    BLOCK_CLI_IF_RECOVER_SUBTREE = keywords.IDLBlock()
    
    pred = ["desc_close_subtree", 
            "desc_create_diff",
            "terminate"]
    code = "assert(id);\n" \
"desc = desc_lookup(id);\n" \
"assert(desc);\n" \
"\n" \
"child_desc_list = desc->child_desc_list;\n" \
"\n" \
"for ((child_desc) = FIRST_LIST((child_desc_list), next, prev) ;      \n" \
"     (child_desc) != (child_desc_list) ;\n" \
"     (child_desc) = FIRST_LIST((child_desc), next, prev)) {\n" \
"    client_interface_basic_id(child_desc->id);\n" \
"    if (child_desc->dest_spd != cos_spd_id()) {\n" \
"        recover_upcall(child_desc->dest_spd, child_desc->id);\n" \
"    } else {\n" \
"        id = child_desc->id;\n" \
"        client_interface_recover_subtree(id);\n" \
"    }\n" \
"}\n" \
    
    BLOCK_CLI_IF_RECOVER_SUBTREE.add_blk(pred, code, "BLOCK_CLI_IF_RECOVER_SUBTREE")
         
    return BLOCK_CLI_IF_RECOVER_SUBTREE

def block_cli_if_recover_upcall():
    #print ("[[BLOCK_CLI_IF_RECOVER_UPCALL]]")
    BLOCK_CLI_IF_RECOVER_UPCALL = keywords.IDLBlock()
    
    pred = ["desc_global_global"] 
    code = "assert(id);\n" \
"client_interface_recover(id);\n" \
"client_interface_recover_subtree(id);\n" \
    
    BLOCK_CLI_IF_RECOVER_UPCALL.add_blk(pred, code, "BLOCK_CLI_IF_RECOVER_UPCALL")
         
    return BLOCK_CLI_IF_RECOVER_UPCALL

def block_cli_if_basic_id():
    #print ("[[BLOCK_CLI_IF_BASIC_ID]]")
    BLOCK_CLI_IF_BASIC_ID = keywords.IDLBlock()
    
    pred = ["desc_dep_create_same"] 
    code = "assert(id);\n" \
"desc = desc_lookup(id);\n" \
"assert(desc);\n" \
"\n" \
"ret = client_interface_recover_init();\n" \
"\n" \
"if (ret == parent_not_recovered_error) {\n" \
"    id = desc->parent_id;\n" \
"    client_interface_recover(id);\n" \
"}\n" \

    BLOCK_CLI_IF_BASIC_ID.add_blk(pred, code, "BLOCK_CLI_IF_BASIC_ID")

    pred = ["desc_create_single"] 
    code = "assert(id);\n" \
"desc = desc_lookup(id);\n" \
"assert(desc);\n" \
"\n" \
"ret = client_interface_recover_init();\n" \
"client_interface_recover_data();\n" \
    
    BLOCK_CLI_IF_BASIC_ID.add_blk(pred, code, "BLOCK_CLI_IF_BASIC_ID")
         
    return BLOCK_CLI_IF_BASIC_ID
        
def block_cli_if_recover():
    #print ("[[BLOCK_CLI_IF_RECOVER]]")
    BLOCK_CLI_IF_RECOVER = keywords.IDLBlock()
    
    pred = ["desc_global_global"] 
    code = "spdid_t creater_component;\n" \
"\n" \
"assert(id);\n" \
"creater_component = introspect_creator(id);\n" \
"assert(creater_component);\n" \
"\n" \
"if (creater_component != cos_spd_id()) {\n" \
"    recover_upcall(creater_component, id);\n" \
"} else {\n" \
"    client_interface_basic_id(id);\n" \
"}\n" \

    BLOCK_CLI_IF_RECOVER.add_blk(pred, code, "BLOCK_CLI_IF_RECOVER")

    pred = ["desc_local"] 
    code = "client_interface_basic_id(id);\n" \

    BLOCK_CLI_IF_RECOVER.add_blk(pred, code, "BLOCK_CLI_IF_RECOVER")
         
    return BLOCK_CLI_IF_RECOVER
                

def block_cli_if_invoke_ser_intro():
    #print ("[[BLOCK_CLI_IF_INVOKE_SER_INTRO]]")
    BLOCK_CLI_IF_INVOKE_SER_INTRO = keywords.IDLBlock()
    
    pred = [] 
    code  = "CSTUB_INVOKE(ret, fault, uc, par_sz, params)\n" \
"\n" \
"if (!desc) {   // some error\n" \
"    client_interface_recover();\n" \
"    CSTUB_INVOKE(ret, fault, uc, par_sz, params)\n" \
"}\n" \

    BLOCK_CLI_IF_INVOKE_SER_INTRO.add_blk(pred, code, "BLOCK_CLI_IF_INVOKE_SER_INTRO")
         
    return BLOCK_CLI_IF_INVOKE_SER_INTRO
        
      
def block_cli_if_invoke():
    #print ("[[BLOCK_CLI_IF_INVOKE]]")
    BLOCK_CLI_IF_INVOKE = keywords.IDLBlock()
    
    pred = ["desc_dep_create_same|desc_dep_create_diff",
            "create"]
    code =  "if (parent_desc = desc_lookup(id)) { \n"   \
            "    id = paren_desc.server_id; \n"         \
            "}\n"                                       \
            "invoke_ser_intro(); \n"                    
                        
    BLOCK_CLI_IF_INVOKE.add_blk(pred, code, "BLOCK_CLI_IF_INVOKE")
    
    
    pred = ["desc_dep_create_single",
            "create"]
    code =  "CSTUB_INVOKE(ret, fault, uc, par_sz, params)\n"    
    BLOCK_CLI_IF_INVOKE.add_blk(pred, code, "BLOCK_CLI_IF_INVOKE")

    # TODO: automatically generate these code
    #===========================================================================
    # p = subprocess.Popen(['sed -f convert.sed pred_code/code_tmplate.c'], shell=True, stdout=subprocess.PIPE)
    # code, err = p.communicate()
    # print (code)
    # p = subprocess.Popen(['sed  \'1d;$d\' pred_code/block_cli_if_invoke.c'], shell=True, stdout=subprocess.PIPE)
    # pred, err = p.communicate()
    # print (pred)    
    #===========================================================================

    pred = ["mutate|terminate"]
    code =  "if (desc = desc_lookup(id)) {\n" \
"    if (desc->fcnt != global_fault_cnt) {\n" \
"        desc->fault_cnt = global_fault_cnt;\n" \
"        client_interface_recover();\n" \
"        client_interface_recover_subtree();\n" \
"    }\n" \
"    update_id(id, desc->server_id);\n" \
"    CSTUB_INVOKE(ret, fault, uc, par_sz, params);\n" \
"} else {\n" \
"    invoke_ser_intro();\n" \
"}\n" \
    
    BLOCK_CLI_IF_INVOKE.add_blk(pred, code, "BLOCK_CLI_IF_INVOKE")    
    return BLOCK_CLI_IF_INVOKE          

#===============================================================================
#  block is in the form of (predicate, code)
#  gdescp is in the form of ['desc_close_itself', 'desc_global_global', ...]
#  fdescp is in the form of [function name, normalPara, sm_state, idlRet, idlPara]
#
#  This is the evaluation function and return the code for a match
#===============================================================================   
def construct_blocks(globalblocks, funcblocks):
    fblk = []
    gblk = []
    
    fblk.append(block_cli_if_invoke())
    fblk.append(block_cli_if_invoke_ser_intro())
    fblk.append(block_cli_if_recover_subtree())
    fblk.append(block_cli_if_track())
    fblk.append(block_cli_if_rec_init())
       
    gblk.append(block_cli_if_recover())
    gblk.append(block_cli_if_basic_id())
    gblk.append(block_cli_if_recover_upcall())
    gblk.append(block_cli_if_rec_data())
    gblk.append(block_cli_if_save_data())  

    for item in gblk:
        globalblocks.append(item)          
    for item in fblk:
        funcblocks.append(item) 

     
def ast_eval(block, (gdescp, fdescp)):
    #print ("\n <<<<<<< eval starts! >>>>>>>>")
    IFCode = ""
    IFBlkName = ""
    
    list_descp = list(traverse(gdescp)) + list(traverse(fdescp)) 
    
    for blk in block.list:
        list_blks = []
        __list_blks = list(traverse(blk[0]))
        for item in __list_blks:
            list_blks.append(item) 
        #print ("\na ist of blks: --> ")
        #print (list_blks)
        
        match = 0;
        for pred in list_blks:
            pred = pred.split('|')
            #===================================================================
            # print ("a pred: --> ")
            # print (pred)
            # print (list_descp)
            #===================================================================
            for desc in list_descp:
                if (desc in pred):
                    match = match + 1;
        if (match == len(list_blks)):
            #print ("found a match")
            IFCode = blk[1]
            IFBlkName = blk[2]
            
            break;  # TODO: check the consistency
    
    return (IFBlkName, IFCode)
    #print ("<<<<<<< eval ends! >>>>>>>>\n")
     
#===============================================================================
# This is the main function to generate new AST 
#===============================================================================
def idl_generate(result, parsed_ast):

    globaldescp     = []
    funcdescps      = []
        
    globalblocks    = []
    funcblocks      = []

    ##########################################
    #### construct global/function description
    ##########################################    

    for tup in result.tuple:
        #pprint (tup.info)
        for key, value in tup.info.iteritems():
            if (key is not tup.sm_create and
                key is not tup.sm_mutate and
                key is not tup.sm_terminate):
                globaldescp.append(key+"_"+value)
        for func in tup.functions:
            #print (func.normal_para)  
            normalPara = func.normal_para          
            #pprint (func.info)
            idlRet  = []
            idlPara = []
            for key, value in func.info.iteritems():
                if (key == func.name or key == func.sm_state):
                    continue
                if (value and key is not func.desc_data_retval):
                    idlPara.append((key, value))
                if (value and key is func.desc_data_retval):
                    idlRet.append((key, value))
            #print (tmpRet)
            perFunc = (func.info[func.name], normalPara,    #--- this is the funcdescp tuple
                       func.info[func.sm_state], idlRet, idlPara)
            #pprint (perFunc)
            #print ()
            funcdescps.append(perFunc)
    
    ##########################################
    ####  construct blocks of (predicate, code)
    ##########################################  
      
    construct_blocks(globalblocks, funcblocks)
        
    ##########################################
    #### evaluate and match
    ##########################################   
    parser = c_parser.CParser()
    IFDesc = (globaldescp, funcdescps)
   
    IFcode = {}
    
    if (globalblocks):
        __IFcode = {}
        for gblk in globalblocks: 
            #print (gblk.list)
            name, code= ast_eval(gblk, (IFDesc[0], None))
            if (name and code):    # there should be any params, otherwise it should be function
                __IFcode[name] = code
    IFcode["global"] = __IFcode
        
    for fdesc in IFDesc[1]:
        __IFcode = {}        
        for fblk in funcblocks:
            #fblk.show()
            name, code = ast_eval(fblk, (IFDesc[0], fdesc))
            param_list = []
            for para in fdesc[1]:
                param_list.append(para[1])    # para[0] is the type, para[1] is the value
            code = code.replace("params", ', '.join(param_list))
            code = code.replace("par_sz", str(len(param_list)))
            if (name and code):
                __IFcode[name] = code            
        #=======================================================================
        # for key, value in __IFcode.iteritems():
        #     if (key):
        #         print ("<<<<" + key + ">>>>")
        #         print (value)
        #=======================================================================                
        IFcode[fdesc[0]] = __IFcode      
        
    #pprint (IFcode)
    print ()
    
    
    for kname, vdict in IFcode.iteritems():
        print ("**********************")
        print (kname)
        print ()
        for blkname, blkcode in vdict.iteritems():
            print ("******")
            print (blkname)
            print (blkcode)
             
