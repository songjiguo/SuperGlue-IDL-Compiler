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
import subprocess
from igraph import *


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

########################
##  blocks (interpret the code_template.c for generating blocks)
########################

def build_blk_code(blknode, blkname):
    
    tmp = blkname.lower()
    blkstr = tmp.replace("_", "\_")
    i = 1
    while(i < 10):   # max 10 different (pred, code)
        cmd = 'sed -nr \"/\<'+ blkstr +' pred '+str(i)+' start\>/{:a;n;/'\
              '\<'+ blkstr +' pred '+str(i)+' end\>/b;p;ba} \" code_template.c'
        p = subprocess.Popen([cmd], shell=True, stdout=subprocess.PIPE)
        pred, err = p.communicate() 
        
        #print (pred)
        #pred = pred.replace("\\n\" \\", "\"")        # extract pred
        if not pred:
            break;
        #pred = pred.replace('"', '').strip()        # remove "
        #print(pred)
        cmd = 'sed -nr \"/\<'+ blkstr +' '+str(i)+' start\>/{:a;n;/'\
              '\<'+ blkstr +' '+str(i)+' end\>/b;p;ba} \" code_template.c'
        p = subprocess.Popen([cmd], shell=True, stdout=subprocess.PIPE)
        code, err = p.communicate()
        if not code:
            break;
        preds = pred.split()
        #print (preds)
        #print (code)        
        if (preds == ['']):
            blknode.add_blk([], code, blkname)
        else:
            blknode.add_blk(preds, code, blkname)    

        i = i+1

    cmd = 'sed -nr \"/\<'+ blkstr +' no match start\>/{:a;n;/'\
          '\<'+ blkstr +' no match end\>/b;p;ba} \" code_template.c'
    p = subprocess.Popen([cmd], shell=True, stdout=subprocess.PIPE)
    code, err = p.communicate()
    #print (code)
    pred = ["no match"]  
    blknode.add_blk(pred, code, blkname)

    cmd = 'sed -nr \"/\<'+ blkstr +' func pointer decl start\>/{:a;n;/'\
          '\<'+ blkstr +' func pointer decl end\>/b;p;ba} \" code_template.c'
    p = subprocess.Popen([cmd], shell=True, stdout=subprocess.PIPE)
    code, err = p.communicate()
    #print (code)
    pred = ["fnptr decl"]  
    blknode.add_blk(pred, code, blkname)
    
    cmd = 'sed -nr \"/\<'+ blkstr +' func pointer start\>/{:a;n;/'\
          '\<'+ blkstr +' func pointer end\>/b;p;ba} \" code_template.c'
    p = subprocess.Popen([cmd], shell=True, stdout=subprocess.PIPE)
    code, err = p.communicate()
    #print (code)
    pred = ["fnptr"]  
    blknode.add_blk(pred, code, blkname)

def block_cli_if_recover_upcall_entry():
    BLOCK_CLI_IF_RECOVER_UPCALL_ENTRY = IDLBlock()    
    build_blk_code(BLOCK_CLI_IF_RECOVER_UPCALL_ENTRY, "BLOCK_CLI_IF_RECOVER_UPCALL_ENTRY")
    printc (BLOCK_CLI_IF_RECOVER_UPCALL_ENTRY.list)
    printc ("")
    return BLOCK_CLI_IF_RECOVER_UPCALL_ENTRY       

def block_cli_if_recover_upcall_extern():
    BLOCK_CLI_IF_RECOVER_UPCALL_EXTERN = IDLBlock()    
    build_blk_code(BLOCK_CLI_IF_RECOVER_UPCALL_EXTERN, "BLOCK_CLI_IF_RECOVER_UPCALL_EXTERN")
    printc (BLOCK_CLI_IF_RECOVER_UPCALL_EXTERN.list)
    printc ("")
    return BLOCK_CLI_IF_RECOVER_UPCALL_EXTERN       
    
def block_cli_if_save_data():
    BLOCK_CLI_IF_SAVE_DATA = IDLBlock()    
    build_blk_code(BLOCK_CLI_IF_SAVE_DATA, "BLOCK_CLI_IF_SAVE_DATA")
    printc (BLOCK_CLI_IF_SAVE_DATA.list)
    printc ("")
    return BLOCK_CLI_IF_SAVE_DATA       
    
def block_cli_if_recover_data():
    BLOCK_CLI_IF_RECOVER_DATA = IDLBlock()    
    build_blk_code(BLOCK_CLI_IF_RECOVER_DATA, "BLOCK_CLI_IF_RECOVER_DATA")
    printc (BLOCK_CLI_IF_RECOVER_DATA.list)
    printc ("")
    return BLOCK_CLI_IF_RECOVER_DATA       
       
def block_cli_if_recover_init():
    BLOCK_CLI_IF_RECOVER_INIT = IDLBlock()    
    build_blk_code(BLOCK_CLI_IF_RECOVER_INIT, "BLOCK_CLI_IF_RECOVER_INIT")
    printc (BLOCK_CLI_IF_RECOVER_INIT.list)
    printc ("")
    return BLOCK_CLI_IF_RECOVER_INIT        
       
def block_cli_if_track():
    BLOCK_CLI_IF_TRACK = IDLBlock()    
    build_blk_code(BLOCK_CLI_IF_TRACK, "BLOCK_CLI_IF_TRACK")
    printc (BLOCK_CLI_IF_TRACK.list)
    printc ("")
    return BLOCK_CLI_IF_TRACK    
            
def block_cli_if_recover_subtree():
    BLOCK_CLI_IF_RECOVER_SUBTREE = IDLBlock()    
    build_blk_code(BLOCK_CLI_IF_RECOVER_SUBTREE, "BLOCK_CLI_IF_RECOVER_SUBTREE")
    printc (BLOCK_CLI_IF_RECOVER_SUBTREE.list)
    printc ("")
    return BLOCK_CLI_IF_RECOVER_SUBTREE    

def block_cli_if_recover_upcall():
    BLOCK_CLI_IF_RECOVER_UPCALL = IDLBlock()    
    build_blk_code(BLOCK_CLI_IF_RECOVER_UPCALL, "BLOCK_CLI_IF_RECOVER_UPCALL")
    printc (BLOCK_CLI_IF_RECOVER_UPCALL.list)
    printc ("")
    return BLOCK_CLI_IF_RECOVER_UPCALL
    
def block_cli_if_basic_id():
    BLOCK_CLI_IF_BASIC_ID = IDLBlock()    
    build_blk_code(BLOCK_CLI_IF_BASIC_ID, "BLOCK_CLI_IF_BASIC_ID")
    printc (BLOCK_CLI_IF_BASIC_ID.list)
    printc ("")
    return BLOCK_CLI_IF_BASIC_ID
            
def block_cli_if_recover():
    BLOCK_CLI_IF_RECOVER = IDLBlock()    
    build_blk_code(BLOCK_CLI_IF_RECOVER, "BLOCK_CLI_IF_RECOVER")
    printc (BLOCK_CLI_IF_RECOVER.list)
    printc ("")
    return BLOCK_CLI_IF_RECOVER    

def block_cli_if_invoke_ser_intro():
    BLOCK_CLI_IF_INVOKE_SER_INTRO = IDLBlock()    
    build_blk_code(BLOCK_CLI_IF_INVOKE_SER_INTRO, "BLOCK_CLI_IF_INVOKE_SER_INTRO")
    printc (BLOCK_CLI_IF_INVOKE_SER_INTRO.list)
    printc ("")
    return BLOCK_CLI_IF_INVOKE_SER_INTRO

def block_cli_if_desc_update():
    BLOCK_CLI_IF_DESC_UPDATE = IDLBlock()    
    build_blk_code(BLOCK_CLI_IF_DESC_UPDATE, "BLOCK_CLI_IF_DESC_UPDATE")
    printc (BLOCK_CLI_IF_DESC_UPDATE.list)
    printc ("")
    return BLOCK_CLI_IF_DESC_UPDATE

def block_cli_if_invoke():
    BLOCK_CLI_IF_INVOKE = IDLBlock()    
    build_blk_code(BLOCK_CLI_IF_INVOKE, "BLOCK_CLI_IF_INVOKE")
    printc (BLOCK_CLI_IF_INVOKE.list)
    printc ("")
    return BLOCK_CLI_IF_INVOKE

def read_from_template_code(IFcode):
    cmd = 'sed -nr \"/\<client sm start\>/{:a;n;/'\
          '\<client sm end\>/b;p;ba} \" code_template.c'
    p = subprocess.Popen([cmd], shell=True, stdout=subprocess.PIPE)
    code, err = p.communicate()
    IFcode["sm"] = code    
    
    cmd = 'sed -nr \"/\<client sm_funptr start\>/{:a;n;/'\
          '\<client sm_funptr end\>/b;p;ba} \" code_template.c'
    p = subprocess.Popen([cmd], shell=True, stdout=subprocess.PIPE)
    code, err = p.communicate()
    IFcode["sm_funptr"] = code
        
    cmd = 'sed -nr \"/\<client track start\>/{:a;n;/'\
          '\<client track end\>/b;p;ba} \" code_template.c'
    p = subprocess.Popen([cmd], shell=True, stdout=subprocess.PIPE)
    code, err = p.communicate()
    IFcode["trackds"] = {"code" : code}
    
    cmd = 'sed -nr \"/\<client func decl start\>/{:a;n;/'\
          '\<client func decl end\>/b;p;ba} \" code_template.c'
    p = subprocess.Popen([cmd], shell=True, stdout=subprocess.PIPE)
    code, err = p.communicate()          
    IFcode["internalfn"] = code      
    
    cmd = 'sed -nr \"/\<client cstub start\>/{:a;n;/'\
          '\<client cstub end\>/b;p;ba} \" code_template.c'
    p = subprocess.Popen([cmd], shell=True, stdout=subprocess.PIPE)
    code, err = p.communicate()
    IFcode["cstub"] = code
    
    cmd = 'sed -nr \"/\<client state_fptr start\>/{:a;n;/'\
          '\<client state_fptr end\>/b;p;ba} \" code_template.c'
    p = subprocess.Popen([cmd], shell=True, stdout=subprocess.PIPE)
    code, err = p.communicate()
    IFcode["state_fptr"] = code    
    
    cmd = 'sed -nr \"/\<client state_fptr_typedef start\>/{:a;n;/'\
          '\<client state_fptr_typedef end\>/b;p;ba} \" code_template.c'
    p = subprocess.Popen([cmd], shell=True, stdout=subprocess.PIPE)
    code, err = p.communicate()
    IFcode["state_fptr_typedef"] = code
    
    cmd = 'sed -nr \"/\<client state transition start\>/{:a;n;/'\
          '\<client state transition end\>/b;p;ba} \" code_template.c'
    p = subprocess.Popen([cmd], shell=True, stdout=subprocess.PIPE)
    code, err = p.communicate()
    IFcode["state_transition"] = code    

# pycparser related
typedecl                    = "TypeDecl"
funcdecl                    = "FuncDecl"
ptrdecl                     = "PtrDecl"
        
# each service has a tuple and multiple functions
class IDLServices(object):
    def __init__(self): 
        self.tuple      = []    # system description tuple (see EMSOFT paper)
        self.gvars      = {}    # some global data strutures, and others...
        self.tuple.append(IDLTuple())   # only once        
        
    def add_tuple(self):
        self.tuple.append(IDLTuple())
    
# the keywords must be consistent with ones defined in cidl_gen (macro in cidl_gen)
class IDLTuple(object):
    def __init__(self):
        self.info = {} 
        self.sm_info = {} 
        self.desc_data_fields = []
        self.functions = []         
        #init_tuple_keyword(self)
        #init_tuple_info(self)        
        
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
    node.type                    = "functype" 
    node.sm_state                = "funcsm" 
    node.desc_data               = "desc_data"
    node.desc_data_retval        = "desc_data_retval" #--> in the form of (type, value)
    node.desc_data_remove        = "desc_terminate" #--> in the form of (value)
    node.desc_lookup             = "desc_lookup" #--> in the form of (vale, type)
    node.desc_data_add           = "desc_data_add" #--> in the form of (target_to_add, value, type)
    node.resc_data_add           = "resc_data"     #--> in the form of (desc_id, value, type)

def init_func_info(func):
    func.info[func.name]                = []
    func.info[func.type]                = []
    func.info[func.sm_state]            = []
    func.info[func.desc_data_retval]    = []
    func.info[func.desc_data_remove]    = []
    func.info[func.desc_data]           = []
    func.info[func.desc_lookup]         = []
    func.info[func.desc_data_add]       = []
    func.info[func.resc_data_add]       = []

#===============================================================================
# def init_tuple_keyword(node):
#     node.desc_close              = "desc_close"
#     node.desc_dep_create         = "desc_dep_create"
#     node.desc_dep_close          = "desc_dep_close"
#     node.desc_global             = "desc_global"
#     node.desc_block              = "desc_block" #--> in the form of (desc_block, T/F, [component])
#     node.desc_has_data           = "desc_has_data"
#     node.resc_has_data           = "resc_has_data"    
#     node.sm_create               = "creation"
#     node.sm_mutate               = "transition"
#     node.sm_terminate            = "terminal"
#===============================================================================

#===============================================================================
# def init_tuple_info(tup):
#     tup.info[tup.desc_close]            = []
#     tup.info[tup.desc_dep_create]       = []
#     tup.info[tup.desc_dep_close]        = []
#     tup.info[tup.desc_global]           = []
#     tup.info[tup.desc_block]            = []
#     tup.info[tup.desc_has_data]         = []
#     tup.info[tup.resc_has_data]         = []
#===============================================================================
    
 
 # draw the SM transtion with the faulty path
def  draw_sm_transition(smg):
    #layout = smg.layout_circle()    
    #for node in xrange(smg.vcount()):
    #    print(smg.vs[node]["name"])
    layout = [(10,0), (0,10), (20,10), (10,20), (0,0), (20,20)]
    visual_style = {}
    visual_style["vertex_name"] = smg.vs["name"]
    visual_style["vertex_label_size"] = 20
    visual_style["vertex_size"] = 40
    visual_style["vertex_color"] ="gray"#[color_dict[x] for x in smg.vs["name"]]
    visual_style["vertex_label"] = smg.vs["name"]
    visual_style["layout"] = layout
    visual_style["bbox"] = (500, 500)
    visual_style["margin"] = 80    
    widths = [3] * len(smg.es)
    colors = ["black"]*len(smg.es) 
    for e in smg.es:
        print(e.attributes()["func"])
        if (e.attributes()["retcode"] == "faulty" and e.attributes()["func"]):
            #widths[e.index] = 2
            colors[e.index] = "red"
        elif (e.attributes()["retcode"] == "faulty" and e.attributes()["func"] == ""):
            colors[e.index] = "orange"
    # Update the graph with the color and width for the fault path
    smg.es['width'] = widths
    smg.es['color'] = colors 
    
    #plot(smg, "SM.svg", **visual_style)
    plot(smg, **visual_style)
    
    #===========================================================================
    # # test for duplicated graph
    # parent_smg = smg.copy()
    # tmp_x = [x[0]+200 for x in layout]
    # tmp_y = [x[1]+100 for x in layout]    
    # parent_layout = zip(tmp_x, tmp_y)
    # total_layout = layout + parent_layout
    # visual_style["layout"] = total_layout
    # testg = smg + parent_smg
    # plot(testg, **visual_style)
    #===========================================================================
  
# these are just some util functions  
DEBUG = 0
def printc(s):
    if DEBUG:
        pprint (s)

def repeat_to_length(string_to_expand, length):
   return (string_to_expand * ((length/len(string_to_expand))+1))[:length]
