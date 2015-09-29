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

import keywords, sys, os

# transparent to the user (used internally, so no need to dynamically change)
desc_track_server_id = "int server_id"
desc_track_fault_cnt = "unsigned long long fault_cnt"
desc_track_state = "unsigned int state"

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
        
#===============================================================================
#  block is in the form of (predicate, code, blk name)
#  gdescp is in the form of ['desc_close_itself', 'desc_global_global', ...]
#  fdescp is in the form of [function name, normalPara, sm_state, idlRet, idlPara]
#
#  This is the evaluation function and return the code for a match
#===============================================================================   
def condition_eval(block, (gdescp, fdescp)):
    #print ("\n <<<<<<< eval starts! >>>>>>>>")
    IFCode = ""
    IFBlkName = ""
    
    list_descp = list(traverse(gdescp)) + list(traverse(fdescp))
    #print ("list descp ->") 
    #print (list_descp)
    #print ("<- list descp") 
    
    find_any = False
    
    for blk in block.list:
        list_blks = []
        __list_blks = list(traverse(blk[0]))
        for item in __list_blks:
            list_blks.append(item) 
        #print ("\na ist of blks: sz --> ", len(list_blks))
        
            #print (blk[0])
            #print (blk[0])
            #print (list_blks)
        
        match = 0;
        for pred in list_blks:
            pred = pred.split('|')
            #print ("a pred: --> ")
            #print (pred)
            #print (list_descp)
            for desc in list_descp:
                if (desc in pred):
                    #print ("found one")
                    #print (desc)
                    match = match + 1;

        if (match == len(list_blks)):
            #print ("found a match")
            IFCode = blk[1] + block.list[-1][1]   # last(pred, code) -- [-1][1] is for function pointer
            IFBlkName = blk[2]  
            find_any = True
            break;  # stop once we have found a match TODO: check the consistency

    if (find_any is False):
        for blk in block.list:
            if (blk[0][0] == "no match"):
                return (blk[0][0], blk[1])
    return (IFBlkName, IFCode)
    #print ("<<<<<<< eval ends! >>>>>>>>\n")
        
def traverse(o, tree_types=(list, tuple)):
    if isinstance(o, tree_types):
        for value in o:
            for subvalue in traverse(value, tree_types):
                yield subvalue
    else:
        yield o

# this is to repalce for each function, not global
# fdesc[0] is the func name, fdesc[1] is the normal pars
# fdesc[4] is the IDL-ed pars  
def replace_params(result, fdesc, code):
    name    = fdesc[0]
    params  = fdesc[1]
    #pprint (result.tuple[0].functions[0].info)
    #print (name)
    #print(fdesc[4])
    
    parent_id = ""
    id = ""
    tmp_list = list(traverse(fdesc[4]))
    for item in tmp_list:
        if (item == "desc_lookup" or item == "desc_terminate"):
            id = tmp_list[tmp_list.index(item)+2]
        if (item == "parent_desc"):
            parent_id = tmp_list[tmp_list.index(item)+2]
    #print (name)
    #print (id)
    #print (parent_id)
    
    param_list = []
    paramdecl_list = []
    for para in params:               # fdesc[1] is the list of normal parameters
        param_list.append(para[1])      # para[0] is the type, para[1] is the value
        paramdecl_list.append(para[0]+" "+para[1])  # this is for the parametes used in the function decl
    #print (', '.join(paramdecl_list))
    code = code.replace("IDL_params", ', '.join(param_list))
    code = code.replace("IDL_pars_len", str(len(param_list)))
    code = code.replace("IDL_parsdecl", ', '.join(paramdecl_list))
    code = code.replace("IDL_fname", name)

    for tup in result.tuple:
        for func in tup.functions:
            if (fdesc[0] == func.info["funcname"]):
                #print(func.info["funcname"])
                #print(func.info["functype"])
                code = code.replace("IDL_fntype", func.info["functype"])
    
    if (id):
        code =  code.replace("IDL_id", id)
    if (parent_id):
        code =  code.replace("IDL_parent_id", parent_id)
    
    return code        


# this is to repalce in global
# some post processing (pop up the saved params, find create function etc...)  
def post_process(result, IFcode, IFDesc):
    
    for fdesc in IFDesc[1]:
        params  = fdesc[1]
        
        #print(fdesc[1])
        #print(fdesc[2])
        
        parent_id = ""
        tmp_list = list(traverse(fdesc[4]))
        for item in tmp_list:
            if (item == "parent_desc"):
                parent_id = tmp_list[tmp_list.index(item)+2]
        param_list = []
        paramdecl_list = []
        for para in params:               # fdesc[1] is the list of normal parameters
            param_list.append(para[1])      # para[0] is the type, para[1] is the value
            paramdecl_list.append(para[0] + " " + para[1])  # this is for the parametes used in the function decl
        #print (', '.join(paramdecl_list))

        # replace some create function for recover purpose  
        if (fdesc[2] == "creation"):
            
            code = IFcode["global"]['BLOCK_CLI_IF_BASIC_ID']
            code = code.replace("IDL_create_fname", fdesc[0])
            for i in xrange(len(param_list)):
                param_list[i] = "desc->"+ param_list[i]
            code = code.replace("IDL_desc_saved_params", ', '.join(param_list))
            code = code.replace("IDL_parent_id", parent_id)
            IFcode["global"]['BLOCK_CLI_IF_BASIC_ID'] = code

            code = IFcode["internalfn"]
            code = code.replace("IDL_desc_saved_params", ', '.join(paramdecl_list))
            IFcode["internalfn"] = code

            # ensure the same name here
            tmp = ""
            for item in paramdecl_list:
                tmp = tmp + "desc->" + item.split(" ")[-1] + " = " + item.split(" ")[-1] + ";\n"
     
            code = IFcode["internalfn"]
            code = code.replace("IDL_desc_cons;", tmp)
            IFcode["internalfn"] = code

########################
# generating functions
########################
def generate_globalvas(result, IFcode):
    
    code = IFcode["trackds"]

    tmp = "\n" + code.split()[0] + " " + code.split()[1] + " { \n"
    for item in result.gvars["desc_data"]:
        tmp = tmp + "    " + " ".join(item) + ";\n"
        
    # transparent to the user
    tmp = tmp + "    " + desc_track_state + ";\n"  
    tmp = tmp + "    " + desc_track_server_id + ";\n"
    tmp = tmp + "    " + desc_track_fault_cnt + ";"
    tmp = tmp + "\n};\n"  
    
    code = code.replace("struct IDL_desc_track\n", tmp) # only replace a line, not everywhere
    code = code.replace("struct IDL_desc_track", "struct desc_track") # only replace a line, not everywhere

    # all fields of desc_track (to replace in the function params)
    tmp = ""
    for item in result.gvars["desc_data"]:
        tmp = tmp + " ".join(item) + ", "
    code = code.replace("IDL_desc_track_fields", tmp[:-2])  # -2 is to remove last ","

    # internal function files
    code = r'''#include "cidl_gen.h"''' + "\n" + code 

    IFcode["trackds"] = code

# the function pointer decl blocks    
def generate_fnptr(funcblocks, globalblocks, IFcode):
    __IFcode = {}        
    for fblk in funcblocks:
        #fblk.show()
        for item in fblk.list:
            if (item[0] and item[0][0] == 'fnptr decl'):
                name = item[2]
                code = item[1]
                __IFcode[name] = code
    for gblk in globalblocks:
        #gblk.show()
        for item in gblk.list:
            if (item[0] and item[0][0] == 'fnptr decl'):
                name = item[2]
                code = item[1]
                __IFcode[name] = code
    IFcode["funptr"] = __IFcode

def generate_gblocks(globalblocks, IFDesc, IFcode):
    # the global blocks
    IFcode["internalfn"] = IFcode["internalfn"] + "//group: block function declarations \n"
    if (globalblocks):
        __IFcode = {}
        for gblk in globalblocks:
            #print (gblk.list)
            name, code= condition_eval(gblk, (IFDesc[0], None))
            if (name and code):    # there should be any params, otherwise it should be function
                
                __IFcode[name] = code
                # now write out static function declarations!!!
                #print (code.split('\n', 1)[0])
                IFcode["internalfn"] = IFcode["internalfn"] + code.split('\n', 1)[0][:-2]+";" + "\n"
    IFcode["global"] = __IFcode
    #pprint(IFcode['global']["BLOCK_CLI_IF_RECOVER_DATA"])  
    
def generate_fblocks(result, funcblocks, IFDesc, IFcode):
    no_match_code_list = []
 
#    read_from_code_template(IFcode)
    
    # evaluate function blocks (and also generate cstub code here)
    cstub_code   = ""
    fptr_typedef = ""
    fptr_dict    = {}    
    for fdesc in IFDesc[1]:
        __IFcode = {}   
        for fblk in funcblocks:
            #fblk.show()
            name, code = condition_eval(fblk, (IFDesc[0], fdesc)) 
            code = replace_params(result, fdesc, code)

            if (name == "no match"):   # empty body
                if (code not in no_match_code_list):
                    no_match_code_list.append(code)
                continue;
            
            if (name and code):
                __IFcode[name] = code
                # now write out static function declarations!!!
                IFcode["internalfn"] = IFcode["internalfn"] + code.split('\n', 1)[0][:-2]+";" + "\n"
        IFcode[fdesc[0]] = __IFcode
    
        code = IFcode["cstub"]
        cstub_code = cstub_code + replace_params(result, fdesc, code)
        code = IFcode["state_fptr_typedef"]
        fptr_typedef = fptr_typedef + replace_params(result, fdesc, code)
        code = IFcode["state_fptr"]
        fptr_dict["state_"+fdesc[0]] = replace_params(result, fdesc, code)
        
    # construct cstub warpper for each function 
    IFcode["cstub"] = cstub_code
    IFcode["state_fptr_typedef"] = fptr_typedef
    IFcode["state_fptr"] = fptr_dict
   
    # this is the non-match list and should be empty static inline function
    IFcode["internalfn"] = IFcode["internalfn"] + "\n//group: empty block function implementation"    
    IFcode["internalfn"] = IFcode["internalfn"] + '\n' + ''.join(no_match_code_list)

    #pprint(IFcode)
    #exit()    

# construct global/function description
def generate_description(result, funcdescps, globaldescp):
    for tup in result.tuple:
        #pprint (tup.sm_info)
        #pprint (tup.info)
        #pprint (tup.desc_data_fields)
        
        for key, value in tup.info.iteritems():
            globaldescp.append(key+"_"+value)
            
        for func in tup.functions:
            #print (func.normal_para)  
            #pprint(func.info["funcname"])
            normalPara = func.normal_para          
            #pprint (func.info)
            idlRet  = []
            idlPara = []
            for key, value in func.info.iteritems():
                if (key == func.name or key == func.sm_state): # skip function name and state (only IDL stuff) 
                    continue
                elif (value and key is not func.desc_data_retval):
                    #print(func.desc_data_retval)
                    idlPara.append((key, value))
                else:
                    (value and key is func.desc_data_retval)
                    idlRet.append((key, value))
            # Kevin Andy
            perFunc = (func.info[func.name], normalPara,    #--- this is the funcdescp tuple
                       func.info[func.sm_state], idlRet, idlPara)
            #pprint (perFunc)
            funcdescps.append(perFunc) 
        #exit()
#  init blocks of (predicate, code) 
def generate_blocks(globalblocks, funcblocks):
    
    fblk = []
    gblk = []
    
    fblk.append(keywords.block_cli_if_invoke())
    fblk.append(keywords.block_cli_if_desc_update())
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
# the main functoin to paste and generate the fina code/ast
#===============================================================================
def paste_idl_code(IFcode):
    
    result_code = IFcode['trackds'] + "\n"    
    result_code = result_code + IFcode['sm'] + "\n"
    result_code = result_code + IFcode['state_fptr_typedef'] + "\n"
    result_code = result_code + IFcode['internalfn'] + "\n"
    
    tmp_dict = IFcode['global']
    for k, v in tmp_dict.iteritems():
        #print(v)        
        result_code = result_code + v
        result_code = result_code + "\n"
        
    for kname, vdict in IFcode.iteritems():
        if (kname == "funptr" or kname == "global" or kname == "cstub" or kname == "sm_funptr" or
            kname == "internalfn" or kname == "sm" or kname == "trackds" or 
            kname == "state_fptr" or kname == "state_fptr_typedef"):
            continue
        for k, v in vdict.iteritems():
            #print(v)
            result_code = result_code + v
            result_code = result_code + "\n"       
    
    result_code = result_code + IFcode['cstub'] + "\n" 
    result_code = result_code + IFcode['sm_funptr'] + "\n"       
    
    # make a fake main function for testing only
    fake_main = r"""
/* this is just a fake main function for testing. Remove it later  */
int main()
{
    return 0;
}
"""
    result_code = result_code + "\n" + fake_main
    
    #exit()
    
    # write out the result to the file (easier debug)
    output_file = "output.c"
    with open(output_file, "w") as text_file:
        text_file.write("{0}".format(result_code))
    
    os.system("indent -kr output.c") 
    
    #os.system("cat output.c")        
    #exit()
    
    # generate the new ast for the interface code
    parser = c_parser.CParser()
    ast = parse_file(output_file, use_cpp=True,
                     cpp_path='cpp',
                     cpp_args=r'-Iutils/fake_libc_include')    
    #ast.show()

    print ("IDL process is done")
    

def intersect(a, b):
    return list(set(a) & set(b))
 
def difference(a,b): 
    return list(set(a) - set(b))

def build_fault_transition(ft_list, item1, item2):
    ft_list.append("{"+ item1 + ", " + item2 + ", " 
                                 + "faulty" + "}" +",\n") 

def find_recover_state_transition(fault_transition_list, from_list, to_list, to_item):
    idx = 0;
    ret = ""
    for item in to_list:
        if (item == to_item):
            if (from_list[idx] not in to_list):  # ignore the one that also on the to_list
                ret = from_list[idx]           
        idx = idx + 1
    if (ret):
        build_fault_transition(fault_transition_list, to_item, ret)
 
def construct_fault_transition(from_list, to_list):
    fault_transition_list = []
        
    # assumption: only 1 creation
    tmp = difference(from_list, to_list)  # only in from_list (must be init and creation)
    if (tmp):
        build_fault_transition(fault_transition_list, tmp[0], tmp[0])
    
    #exit()
    # assumption: only 1 terminal
    tmp = difference(to_list, from_list)  # only in to_list (must be terminal)
    find_recover_state_transition(fault_transition_list, from_list, to_list, tmp[0])
    
    
    tmp = intersect(from_list, to_list)   # in both from_list and to_list
    for new_item in tmp:
        find_recover_state_transition(fault_transition_list, from_list, to_list, new_item)

    return fault_transition_list

def construct_current_state(result, state_list, IFcode):    
    # set the current state
    for tup in result.tuple:
        for func in tup.functions:
            for item in state_list:
                if (func.info[func.name] == item.split("state_")[1]):
                    code = IFcode[func.info[func.name]]["BLOCK_CLI_IF_TRACK"]
                    code = code.replace("IDL_curr_state", str(item))
                    IFcode[func.info[func.name]]["BLOCK_CLI_IF_TRACK"]  = code
                    
def generate_sm_transition(result, funcblocks, IFcode):    
    #print("constructing SM transition")
    
    fn_list = []
    state_list = []
    transition_list = []
    
    from_list = []
    to_list = []
    
    for tup in result.tuple:
        # here we fix the start and end to be 1st and 2nd in the list
        for func in tup.functions:
            if (func.info[func.sm_state] == "creation"):
                fn_list.insert(0, func.info[func.name])
                state_list.insert(0, "state_"+func.info[func.name])
            elif (func.info[func.sm_state] == "terminal"):
                fn_list.insert(1, func.info[func.name])
                state_list.insert(1, "state_"+func.info[func.name])
            else:
                fn_list.append(func.info[func.name])
                state_list.append("state_"+func.info[func.name])
                 
        for item in tup.sm_info["transition"]:
            from_list.append("state_" + item[0])
            to_list.append("state_" + item[1])
            
            tmp_str = ""
            if (item[0] != item[1]):
                tmp_str = "ok"
            else:
                tmp_str = "again"
            transition_list.append("{"+ "state_"+ item[0] + ", " + 
                                   "state_" + item[1] + ", " + 
                                   tmp_str + "}" +",\n")
            
    # now construct the transition under the fault situation (from 2 lists)
    fault_transition = construct_fault_transition(from_list, to_list)
    for item in fault_transition:
        transition_list.append(item) 
        

    code = IFcode["sm_funptr"]    
    gen_fn_list = map(lambda x:"(generic_fp)"+x, fn_list)
    #print(fn_list)     
    code = code.replace("IDL_fn_list_len", str(len(gen_fn_list)))
    code = code.replace("IDL_fn_list", ', '.join(gen_fn_list))
    IFcode["sm_funptr"] = code

    code = IFcode["sm"]
    #state_list = [x.upper() for x in state_list]
    code = code.replace("IDL_state_list", ', '.join(state_list))
    
    #pprint(transition_list)
    #exit()
    code = code.replace("IDL_transition_rules", ' '.join(transition_list))
    IFcode["sm"] = code  
    # update the SM state and transition function for recovery
    construct_current_state(result, state_list, IFcode)

    # this is the key to the c3 recovery
    recover_sm_transition(fn_list, transition_list,state_list, IFcode)
    
#===============================================================================
# This is the main function to generate the state transition for recovery    
#===============================================================================
def recover_sm_transition(fn_list, transition_list, state_list, IFcode):
    
    #pprint(IFcode["state_fptr"])
    code = IFcode["internalfn"]
    
    tmp = "switch (state) {\n"
    
    for item in state_list:
        _code = IFcode["state_fptr"][item]
        print(_code)
        tmp = tmp + "case " + item + ":\n" + _code + "break;\n"
        
    tmp = tmp + "default " +  ":\n" + "assert(0);\n }"

    code = code.replace("IDL_state_list", tmp)
    IFcode["internalfn"] = code
    
#===============================================================================
# the function to process passed in result and generate block code
#===============================================================================
def idl_generate(result, parsed_ast):

    globaldescp     = []
    funcdescps      = []
    globalblocks    = []
    funcblocks      = []
    IFcode          = {}
    
    #===========================================================================
    # pprint (result.tuple[0].info)
    # pprint (result.tuple[0].sm_info)
    # pprint (result.tuple[0].desc_data_fields)
    # pprint (result.gvars)
    # pprint (result.tuple[0].functions[0].info)
    # pprint (result.tuple[0].functions[1].info)
    # pprint (result.tuple[0].functions[2].info)
    # pprint (result.tuple[0].functions[3].info)
    #===========================================================================

    keywords.read_from_template_code(IFcode)    
    IFDesc = (globaldescp, funcdescps)
    # build blocks and descriptions
    generate_description(result, funcdescps, globaldescp)  
    generate_blocks(globalblocks, funcblocks)

    # evaluate the conditions and generate block code
    generate_globalvas(result, IFcode)
    generate_gblocks(globalblocks, IFDesc, IFcode)
    generate_fblocks(result, funcblocks, IFDesc, IFcode)
    
    # post process
    post_process(result, IFcode, IFDesc)
    
    # add SM transition code
    generate_sm_transition(result, funcblocks, IFcode)
   
    #pprint (IFcode)
    #exit()
    paste_idl_code(IFcode)   # make some further process here
