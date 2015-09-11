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
from pprint import pprint

import keywords
import subprocess

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

########################
##  utils
########################        
def traverse(o, tree_types=(list, tuple)):
    if isinstance(o, tree_types):
        for value in o:
            for subvalue in traverse(value, tree_types):
                yield subvalue
    else:
        yield o
        

def replace_params(name, params, code):
    param_list = []
    paramdecl_list = []
    for para in params:               # fdesc[1] is the list of normal parameters
        param_list.append(para[1])      # para[0] is the type, para[1] is the value
        paramdecl_list.append(para[0]+" "+para[1])  # this is for the parametes used in the function decl
    code = code.replace("params", ', '.join(param_list))
    code = code.replace("par_sz", str(len(param_list)))
    code = code.replace("parsdecl", ', '.join(paramdecl_list))
    code = code.replace("fname", name)
    
    return code

########################
## constructing blocks
######################## 
def construct_blocks(globalblocks, funcblocks):
    
    # transform the predefine code for further processing (e.g., add "\n")
    cmd = 'sed -f pred\_code\/convert.sed pred\_code\/code.c > pred\_code\/tmpcode'
    subprocess.Popen([cmd], shell=True, stdout=subprocess.PIPE)
    
    fblk = []
    gblk = []
    
    fblk.append(keywords.block_cli_if_invoke())
    fblk.append(keywords.block_cli_if_invoke_ser_intro())
    fblk.append(keywords.block_cli_if_recover_subtree())
    fblk.append(keywords.block_cli_if_track())  
    fblk.append(keywords.block_cli_if_recover_init())

    gblk.append(keywords.block_cli_if_recover())
    gblk.append(keywords.block_cli_if_basic_id()) 
    gblk.append(keywords.block_cli_if_recover_upcall()) 
    gblk.append(keywords.block_cli_if_recover_data())
    gblk.append(keywords.block_cli_if_save_data())
    gblk.append(keywords.block_cli_if_recover_upcall_entry())  

    for item in gblk:
        globalblocks.append(item)          
    for item in fblk:
        funcblocks.append(item) 
     
#===============================================================================
#  block is in the form of (predicate, code)
#  gdescp is in the form of ['desc_close_itself', 'desc_global_global', ...]
#  fdescp is in the form of [function name, normalPara, sm_state, idlRet, idlPara]
#
#  This is the evaluation function and return the code for a match
#===============================================================================   
     
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
            #print ("a pred: --> ")
            #print (pred)
            #print (list_descp)
            for desc in list_descp:
                if (desc in pred):
                    match = match + 1;
        if (match == len(list_blks)):
            #print ("found a match")
            IFCode = blk[1] + block.list[-1][1]   # last(pred, code) -- [-1][1] is for function pointer
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
            code = replace_params(fdesc[0], fdesc[1], code)                       
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
    
