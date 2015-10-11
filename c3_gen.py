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
from igraph import *

plot_graph = 0
using_main = 1

# transparent to the user (used internally, so no need to dynamically change)
desc_track_server_id = "int IDL_server_id"
desc_track_fault_cnt = "unsigned long long fault_cnt"
desc_track_state = "unsigned int state"
desc_track_next_state = "unsigned int next_state"

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
#
#  EXTRACT THE INFO AND CONSTRUCT THE CODE BLOCKS
#
#===============================================================================
#===============================================================================
#  block is in the form of (predicate, code, blk name)
#  gdescp is in the form of ['desc_close_itself', 'desc_global_global', ...]
#  fdescp is in the form of [function name, normalPara, sm_state, idlRet, idlPara]
#===============================================================================   
def condition_eval(block, (gdescp, fdescp)):
    IFCode = ""
    IFBlkName = ""
    
    list_descp = list(traverse(gdescp)) + list(traverse(fdescp))
    find_any = False
    
    for blk in block.list:
        list_blks = []
        __list_blks = list(traverse(blk[0]))
        for item in __list_blks:
            list_blks.append(item) 
        match = 0;
        for pred in list_blks:
            pred = pred.split('|')
            for desc in list_descp:
                if (desc in pred):
                    match = match + 1;

        if (match == len(list_blks)):
            IFCode = blk[1] + block.list[-1][1]   # last(pred, code) -- [-1][1] is for function pointer
            IFBlkName = blk[2]  
            find_any = True
            break;  # stop once we have found a match TODO: check the consistency

    if (find_any is False):
        for blk in block.list:
            if (blk[0][0] == "no match"):
                return (blk[0][0], blk[1])
    return (IFBlkName, IFCode)
        
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
def replace_params(result, fdesc, code, IFcode, subIFcode, param_list, paramdecl_list):
    name    = fdesc[0]

    code = code.replace("IDL_params", ', '.join(param_list))
    code = code.replace("IDL_pars_len", str(len(param_list)))
    code = code.replace("IDL_parsdecl", ', '.join(paramdecl_list))
    code = code.replace("IDL_fname", name)
    
    if (fdesc[2] != "creation"):
        code = code.replace("IDL_desc_saved_params", subIFcode["paramteres"]["desc_params"])

    for tup in result.tuple:
        for func in tup.functions:
            if (fdesc[0] == func.info["funcname"]):
                code = code.replace("IDL_fntype", func.info["functype"])
    
    return code        

# initial creation -- starting point in SM transition             
def prepare_recreation(fdesc, subIFcode, IFcode):
    code = IFcode["global"]['BLOCK_CLI_IF_BASIC_ID']
    code = code.replace("IDL_desc_saved_params", subIFcode["paramteres"]["desc_params"])
    code = code.replace("IDL_fname", fdesc[0])
    IFcode["global"]['BLOCK_CLI_IF_BASIC_ID'] = code 
    
    code = subIFcode["cstub_fn"]
    code = code.replace("IDL_id", "")
    subIFcode["cstub_fn"] = code
    
    # desc_cons
    code = IFcode["internalfn"]
    tmp = ""
    for f, b in zip(subIFcode["paramteres"]["desc_params"].split(","), 
                    subIFcode["paramteres"]["params"].split(",")):
        tmp = tmp + f + "=" + b + ";\n"
    code = code.replace("IDL_parsdecl", subIFcode["paramteres"]["params_decl"])
    code = code.replace("IDL_desc_cons;", tmp)
    IFcode["internalfn"] = code            

def generate_globalvas(result, IFcode):
    code = IFcode["trackds"]["code"]
    IFcode["trackds"] = {"fields":[]}
   
    tmp = "\n" + code.split()[0] + " " + code.split()[1] + " { \n"
    for item in result.gvars["desc_data"]:
        tmp = tmp + "    " + " ".join(item) + ";\n"
        
    # transparent to the user
    tmp = tmp + "    " + desc_track_state + ";\n"  
    tmp = tmp + "    " + desc_track_next_state + ";\n"  
    tmp = tmp + "    " + desc_track_server_id + ";\n"
    tmp = tmp + "    " + desc_track_fault_cnt + ";"
    tmp = tmp + "\n};\n"  
    
    code = code.replace("struct IDL_desc_track\n", tmp) # only replace a line, not everywhere
    code = code.replace("struct IDL_desc_track", "struct desc_track") # only replace a line, not everywhere

    # all fields of desc_track (to replace in the function params)
    tmp = ""
    for item in result.gvars["desc_data"]:
        tmp = tmp + " ".join(item) + ", "
        IFcode["trackds"]["fields"].append(item[1])
    code = code.replace("IDL_desc_track_fields", tmp[:-2])  # -2 is to remove last ","
    
    # internal function files
    code = r'''#include "cidl_gen.h"''' + "\n" + code 
    
    IFcode["trackds"]["id"] = result.gvars["id"]
    IFcode["trackds"]["parent id"] = result.gvars["parent id"]
    IFcode["trackds"]["code"] = code

def generate_gblocks(globalblocks, IFDesc, IFcode):
    # the global blocks
    IFcode["internalfn_decl"] = ""
    IFcode["extern"] = ""     
    if (globalblocks):
        __IFcode = {}
        for gblk in globalblocks:
            #print (gblk.list)
            name, code= condition_eval(gblk, (IFDesc[0], None))
            if (name and code):    # there should be any params, otherwise it should be function
                # now write out static function skeleton
                if ("extern" in code):
                    IFcode["extern"] = IFcode["extern"] + code
                else:    
                    __IFcode[name] = code   
                    IFcode["internalfn_decl"] = IFcode["internalfn_decl"] + \
                                                code.split('\n', 1)[0][:-2]+";" + "\n"
    IFcode["global"] = __IFcode
    
def generate_fblocks(result, funcblocks, IFDesc, IFcode):
    #no_match_code_list = []
    
    # evaluate function blocks (and also generate cstub code here)
    for fdesc in IFDesc[1]:
        subIFcode, subIFcode["paramteres"],subIFcode["blocks"], subIFcode["state"] = ({} for i in range(4)) 
        param_list, paramdecl_list, paramdesc_list = ([] for i in range(3))

        for para in fdesc[1]:               # fdesc[1] is the list of normal parameters
            param_list.append(para[1])      # para[0] is the type, para[1] is the value
            paramdesc_list.append(para[1])
            paramdecl_list.append(para[0]+" "+para[1])  # this is for the parametes used in the function decl
        subIFcode["paramteres"]["params"] = ', '.join(param_list)
        subIFcode["paramteres"]["params_decl"] = ', '.join(paramdecl_list)  

        for i in xrange(len(param_list)):
            if (param_list[i] in IFcode["trackds"]["fields"]):
                paramdesc_list[i] = "desc->"+ param_list[i]
        subIFcode["paramteres"]["desc_params"] = ', '.join(paramdesc_list)
        #print(subIFcode["paramteres"]["desc_params"])
        
        for fblk in funcblocks:
            #fblk.show()
            name, code = condition_eval(fblk, (IFDesc[0], fdesc)) 
            code = replace_params(result, fdesc, code, IFcode, 
                                  subIFcode, param_list, paramdecl_list)
            
            if (name == "no match"):   # empty body
                #if (code not in no_match_code_list):
                #    no_match_code_list.append(code)
                continue;
            
            if (name and code):
                subIFcode["blocks"][name] = code
                # now write out static function declarations!!!
                IFcode["internalfn_decl"] = IFcode["internalfn_decl"] + code.split('\n', 1)[0][:-2]+";" + "\n"

        code = IFcode["cstub"]
        subIFcode["cstub_fn"] = replace_params(result, fdesc, code, IFcode, 
                                               subIFcode, param_list, paramdecl_list)
        code = "IDL_fname(IDL_desc_saved_params)" # add this function to the edge later
        subIFcode["state"]["state_fn"] = replace_params(result, fdesc, code, IFcode, 
                                                        subIFcode, param_list, paramdecl_list)
        if (fdesc[2] == "creation"):
            prepare_recreation(fdesc, subIFcode, IFcode)

        IFcode[fdesc[0]] = subIFcode
    #pprint (IFcode)
    # this is the non-match list and should be empty static inline function ?? why need this?
    #print (''.join(no_match_code_list))
    
    # Kevin QQ
    #IFcode["internalfn"] = IFcode["internalfn"] + '\n' + ''.join(no_match_code_list)

# construct global/function description
def generate_description(result, funcdescps, globaldescp):
    for tup in result.tuple:
        for key, value in tup.info.iteritems():
            globaldescp.append(key+"_"+value)
            
        for func in tup.functions:
            normalPara = func.normal_para          
            idlRet  = []
            idlPara = []
            for key, value in func.info.iteritems():
                if (key == func.name or key == func.sm_state): # skip function name and state (only IDL stuff) 
                    continue
                elif (value and key is not func.desc_data_retval):
                    idlPara.append((key, value))
                else:
                    (value and key is func.desc_data_retval)
                    idlRet.append((key, value))
            perFunc = (func.info[func.name], normalPara,    #--- this is the funcdescp tuple
                       func.info[func.sm_state], idlRet, idlPara)
            funcdescps.append(perFunc) 

#  init blocks of (predicate, code) 
def init_blocks(globalblocks, funcblocks):
    fblk, gblk = ([] for i in range(2))
    
    fblk.append(keywords.block_cli_if_invoke())
    fblk.append(keywords.block_cli_if_desc_update())
    fblk.append(keywords.block_cli_if_invoke_ser_intro())
    fblk.append(keywords.block_cli_if_recover_subtree())
    fblk.append(keywords.block_cli_if_track())  
    fblk.append(keywords.block_cli_if_recover_init())

    gblk.append(keywords.block_cli_if_recover())
    gblk.append(keywords.block_cli_if_basic_id()) 
    gblk.append(keywords.block_cli_if_recover_upcall())
    gblk.append(keywords.block_cli_if_recover_upcall_extern())  
    gblk.append(keywords.block_cli_if_recover_upcall_entry())  
    gblk.append(keywords.block_cli_if_recover_data())
    gblk.append(keywords.block_cli_if_save_data())
    
    for item in gblk:
        globalblocks.append(item)          
    for item in fblk:
        funcblocks.append(item)
    
#===============================================================================
#
#   CONSTRUCT STATE MACHINE AND TRANSITION WITH THE FAULT/RECOVERY 
#
#===============================================================================
def update_current_state(result, state_list, IFcode):    
    # set the current state
    #pprint(IFcode)
    for tup in result.tuple:
        for func in tup.functions:
            for item in state_list:
                if (func.info[func.name] == item.split("state_")[1]):
                    code = IFcode[func.info[func.name]]["blocks"]["BLOCK_CLI_IF_TRACK"]
                    code = code.replace("IDL_curr_state", str(item))
                    IFcode[func.info[func.name]]["blocks"]["BLOCK_CLI_IF_TRACK"]  = code
 
def construct_sm_graph(from_list, to_list, state_list, IFcode):
    smg = Graph(directed=True)
    smg.add_vertices(state_list)
    smg.add_edges(zip(from_list,to_list))
        
    for e in smg.es:
        fn = smg.vs[e.target]["name"].replace("state_","")
        e["func"] = IFcode[fn]["state"]["state_fn"]

        if (e.source == e.target):
            e["retcode"] = "again"
        else:
            e["retcode"] = "ok"
    
    #pprint(IFcode)
    #exit()
    for item in state_list:
        if (item == "state_null"):
            continue
        fn = state_list[0].replace("state_", "")  # get the actual transition function (recreate)
        smg.add_edge(item, state_list[0], retcode = "faulty", func = IFcode[fn]["state"]["state_fn"])
    return smg

def generate_sm_transition(result, funcblocks, IFcode):     
    fn_list, state_list, \
    transition_list_tuple, transition_list_code, \
    from_list, to_list = ([] for i in range(6))
    
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
        state_list.append("state_null")

        for k, v in tup.sm_info.items():
            for item in v:
                if (item[1] == "end"):  # does not count the end state
                    continue
                from_list.append("state_" + item[0])
                to_list.append("state_" + item[1])
                
                if (item[0] != item[1]):
                    tmp_str = "ok"
                else:
                    tmp_str = "again"
                    
                if (item[1] == "none"):
                    fn_str = "(generic_fp)" + item[0]
                else:
                    fn_str = "(generic_fp)" + item[1]
                    
                transition_list_code.append("{"+ "state_"+ item[0] + ", " + 
                                       "state_" + item[1] + ", " + tmp_str + ", " +  
                                       fn_str + "}" +",\n")
                transition_list_tuple.append((item[0], item[1], tmp_str, fn_str))

    code = IFcode["sm"]
    code = code.replace("IDL_state_list", ', '.join(state_list))
    code = code.replace("IDL_transition_rules", ' '.join(transition_list_code))
    IFcode["sm"] = code 
    
    # update the SM state and transition function for recovery
    update_current_state(result, state_list, IFcode)    
    smg = construct_sm_graph(from_list, to_list, state_list, IFcode)

    # this is the key to the c3 recovery
    recover_sm_transition(state_list, smg, IFcode)
    
    # plot the state transition
    if (plot_graph):      
        keywords.draw_sm_transition(smg)

    return smg
    
# ***********!!!!!!!!*************
# find the state transition path
# ***********!!!!!!!!************
# On the SM graph, the fault can happen when the transition is happenning from a state to another state.
# The red line represents the first transition from the fault state with the transition function  
# For example, the fault state can be transited to the state_tsplit by executing tsplit. 
# Or the fault state can be transited to the previous state by not doing any thing
# Then the "replay" in the C^3 can be applied 
#
# Note: the recover path must be able to reach to the target state. Therefore, smg should be able to support 
#       reachability check (i.e., among all possible paths, select the shortest path)
def recover_sm_transition(state_list, smg, IFcode):
    rec_path = {}
    tmp = ""
    transition_code = IFcode["state_transition"]    
    creation_v = smg.vs.find(name = state_list[0])   # creation node
    for edge in smg.es:
        tmp_dict = {}      
        if (edge["retcode"] == "faulty" or  
            smg.vs(edge.source)["name"][0] == "state_null"):
            continue
        else:
            before = smg.vs(edge.source)["name"][0]
            after  = smg.vs(edge.target)["name"][0] 
            
            # NOTE: this should be taken care by the BLOCK_CLI_IF_RECOVER
            # ********************************************************************
            # first: add the edge to the initial state (recreated by the creation)
            # ********************************************************************
            #rec_edge = smg.es.select(_source = edge.source, _target=creation_v.index)
            #tmp_dict["rec_init"] = (rec_edge["func"], creation_v["name"])
            #print(rec_edge["func"])

            # NOTE: this will any necessary edge (function) along the rec path. 
            #       if the creation can reach the target directly, this can be none 
            # ********************************************************************
            # second: check if there is a reachable path from the initial state
            #         back to the target state (the one failed to transit to)
            # ********************************************************************
            back_edges = smg.get_shortest_paths(creation_v.index, edge.target, 
                                                mode = 'OUTPUT', output="epath")
            # does not include the last function call (will be done by "redo")
            back_edges.pop()
            for _eidx in back_edges:
                if (_eidx):
                    recfn_list = []
                    for eid in _eidx:
                        recfn_list.append((smg.es[eid]["func"], 
                                           smg.vs(smg.es[eid].target)["name"][0]))
                    tmp_dict["rec_steps"] = recfn_list   # TODO: append these together
        
        # now we have all edges to rebuild the state for current state transition (this edge)            
        rec_path[(before, after)] = tmp_dict
        
        # add the creation function (only one creation function)
        _tmp = transition_code.replace("IDL_current_state", before)
        _tmp = _tmp.replace("IDL_next_state", after)
        s = ""
        # then add each function along the rec_path
        if ("rec_steps" in tmp_dict): 
            for item in tmp_dict["rec_steps"]:
                s = s + item[0]
        _tmp = _tmp.replace("IDL_state_transition_call", s)
        tmp = tmp + _tmp
    
    code = IFcode["internalfn"]
    code = code.replace("IDL_state_transition;", tmp)
    IFcode["internalfn"] = code
    
#===============================================================================
#
#  INPUT AND OUTPUT OF CODE GENERATING
#
#===============================================================================
def paste_idl_code(result, IFcode):
    result_code = ""
    tmp, IFresult = ({} for i in range(2))
    
    # reorgonize the code section
    IFresult["GLOBAL"]  = {"global_blks":IFcode["global"],    # multiple blocks
                           "extern_fn":IFcode["extern"],
                           "tracking_ds":IFcode["trackds"]["code"],
                           "internal_fn":IFcode["internalfn"],
                           "internal_fn_decl":IFcode["internalfn_decl"]}
    
    IFresult["SM"]      = {"state_group":IFcode["sm"]} 
    
    for tup in result.tuple:
        for func in tup.functions:
            name = func.info["funcname"]
            tmp[name]= IFcode[name]
    IFresult["FUNCTIONS"] = tmp   
    
    # start pasting the code
    result_code = result_code + IFresult["GLOBAL"]["tracking_ds"]
    result_code = result_code + "\n"
    result_code = result_code + IFresult["SM"]["state_group"]
    result_code = result_code + "\n"    
    result_code = result_code + IFresult["GLOBAL"]["extern_fn"]
    result_code = result_code + "\n"
    result_code = result_code + IFresult["GLOBAL"]["internal_fn_decl"]
    result_code = result_code + "\n"
    result_code = result_code + IFresult["GLOBAL"]["internal_fn"]
    result_code = result_code + "\n"    
    for k, v in IFresult["GLOBAL"]["global_blks"].items():
        result_code = result_code + v
        
    for k, v in IFresult["FUNCTIONS"].items():
        for _k, _v in v["blocks"].items():
            result_code = result_code + _v
    for k, v in IFresult["FUNCTIONS"].items():
        result_code = result_code + v["cstub_fn"]
        
    # finally replace IDL_id, IDL_server_id, IDL_parent_id
    final_id        = IFcode["trackds"]["id"][1]
    final_parent_id = IFcode["trackds"]["parent id"][1]
    result_code = result_code.replace("IDL_id", final_id)
    result_code = result_code.replace("IDL_server_id", "server_" + final_id)
    result_code = result_code.replace("IDL_parent_id", final_parent_id)

        
    # make a fake main function for testing only        
    if (using_main):
        fake_main = r"""
/* this is just a fake main function for testing. Remove it later  */
int main()
{
    return 0;
}
"""
        result_code = result_code + "\n" + fake_main
    
    # write out the result to the file (easier debug)
    output_file = "output.c"
    with open(output_file, "w") as text_file:
        text_file.write("{0}".format(result_code))
    
    os.system("indent -linux output.c") 
    
    #os.system("cat output.c")        
    #exit()
    
    # generate the new ast for the interface code
    parser = c_parser.CParser()
    ast = parse_file(output_file, use_cpp=True,
                     cpp_path='cpp',
                     cpp_args=r'-Iutils/fake_libc_include')    
    #ast.show()
    print ("IDL process is done")

def idl_generate(result, parsed_ast):
    globaldescp, funcdescps, globalblocks, funcblocks = ([] for i in range(4))
    IFcode          = {}
    #===========================================================================
    #pprint (result.tuple[0].info)
    #pprint (result.tuple[0].sm_info)
    #pprint (result.tuple[0].desc_data_fields)
    #pprint (result.gvars)
    #pprint (result.tuple[0].functions[0].info)
    #pprint (result.tuple[0].functions[1].info)
    #pprint (result.tuple[0].functions[2].info)
    #pprint (result.tuple[0].functions[3].info)
    #===========================================================================
    
    keywords.read_from_template_code(IFcode)    
    IFDesc = (globaldescp, funcdescps)
    # build blocks and descriptions
    generate_description(result, funcdescps, globaldescp)  
    init_blocks(globalblocks, funcblocks)
    
    # evaluate the conditions and generate block code
    generate_globalvas(result, IFcode)
    generate_gblocks(globalblocks, IFDesc, IFcode)
    generate_fblocks(result, funcblocks, IFDesc, IFcode)
    
    # add SM transition code
    generate_sm_transition(result, funcblocks, IFcode)

    #pprint (IFcode)
    #pprint(IFcode["global"])
    #print(IFcode["internalfn"])
    #print(IFcode["internalfn_decl"])
    #for item in globalblocks:
    #    item.show()
    #exit()

    paste_idl_code(result, IFcode)   # make some further IFprocess here