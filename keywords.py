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
import subprocess, re, time
from igraph import *
import xml.etree.ElementTree as ET   # used for parsing xml-like code template

IDL_ver = "IDL generated code ver 0.1"

service_name = ""
header       = ""
plot_graph   = True
final_output = False
bench = False
cos_path = '/home/songjiguo/research/composite/src/'   
cos_if_path = '/home/songjiguo/research/composite/src/components/interface/'   

# define the root id for all services (even the service does not have dependency)
rootid = {"ramfs":"1", "evt":"0", "lock":"0", "mem_mgr":"0", "periodic_wake":"0"}
nameserver = {"evt":"ns_upcall", "mem_mgr":"valloc_upcall"}

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

def init_service_name(s_name):
    global service_name
    service_name = s_name
   
def plot_sm_graph():
    global plot_graph
    plot_graph = True

def final_code():
    global final_output
    final_output = True

def bench_code():
    global bench;
    bench = True

########################
##  blocks (interpret the code_template.c for generating blocks)
########################
def build_blk_code(cfblk, sfblk, gblk, marshallingblk, gblk_nonfunc):
    tree = ET.parse('code_template.xml')
    idl = tree.getroot()
    #print (idl.tag)
    for _interface in idl:
        #print _interface.attrib["name"]
        for _block in _interface:
            #print _block.attrib["name"]
            for _preds in _block:
                #print _preds.attrib["name"]
                preds = _preds.attrib["name"]
                for _code in _preds:
                    blknode = IDLBlock()
                    #print(_code.text)
                    code =_code.text
                    blknode.add_blk(preds, code, _block.attrib["name"].upper())
                    #blknode.show()
                    if ("client" in _interface.attrib["name"]):
                        if ("marshalling" in _preds.attrib["name"]):
                            marshallingblk.append(blknode)
                        elif ("creation"    in _preds.attrib["name"] or
                            "transition"    in _preds.attrib["name"] or
                            "terminal"      in _preds.attrib["name"] or
                            "temporal"      in _preds.attrib["name"] or
                            "server_block"  in _preds.attrib["name"] or
                            "server_wakeup" in _preds.attrib["name"]):
                            cfblk.append(blknode)
                        elif ("always_appear" in _preds.attrib["name"]):
                            gblk_nonfunc.append(blknode)
                        else:
                            gblk.append(blknode)
                    elif ("server" in _interface.attrib["name"]):
                        if ("marshalling" in _preds.attrib["name"]):
                            marshallingblk.append(blknode)
                        else:                        
                            sfblk.append(blknode)
    

def get_lock_function(IFcode, service_name):
    global cos_path    
    if (service_name == "periodic_wake" or service_name == "ramfs"):
        return
    
    if (service_name != "sched"): 
        cmd = 'find ' + cos_path + " -name " + service_name + ".c"
        p = subprocess.Popen([cmd], shell=True, stdout=subprocess.PIPE)
        path, err = p.communicate()
        
    if (service_name == "evt"):  
        for item in path.split("\n"):
            if ("_grp" in item): # hack for edge and edge_grp (start with this now)
                path = item
    elif (service_name == "sched"):
        path = cos_path + "components/implementation/sched/cos_sched_base.c"
    else:
        path = path.split("\n")[0]  # there might be more than one file (e.g., evt.c)

    cmd = 'sed -nr \"/\<lock take start\>/{:a;n;/'\
          '\<lock take end\>/b;p;ba} \" ' + path
    p = subprocess.Popen([cmd], shell=True, stdout=subprocess.PIPE)
    code, err = p.communicate()
    IFcode["lock_take_func"] = code.replace("\\","")

    cmd = 'sed -nr \"/\<lock release start\>/{:a;n;/'\
          '\<lock release end\>/b;p;ba} \" ' + path
    p = subprocess.Popen([cmd], shell=True, stdout=subprocess.PIPE)
    code, err = p.communicate()
    IFcode["lock_release_func"] = code.replace("\\","")

    cmd = 'sed -nr \"/\<lock name start\>/{:a;n;/'\
          '\<lock name end\>/b;p;ba} \" ' + path
    p = subprocess.Popen([cmd], shell=True, stdout=subprocess.PIPE)
    code, err = p.communicate()
    IFcode["lock_name"] = code.replace("\\","")

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
        self.ser_block_track = {} 
        self.tsm_info = {}    
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
    node.resc_data               = "resc_data"     #--> in the form of (desc_id, value, type)

def init_func_info(func):
    func.info[func.name]                = []
    func.info[func.type]                = []
    func.info[func.sm_state]            = []
    func.info[func.desc_data_retval]    = []
    func.info[func.desc_data_remove]    = []
    func.info[func.desc_data]           = []
    func.info[func.desc_lookup]         = []
    func.info[func.desc_data_add]       = []
    func.info[func.resc_data]           = []


 # draw the SM transtion with the faulty path
def  draw_sm_transition(smg):
    #layout = smg.layout_circle()    
    #for node in xrange(smg.vcount()):
    #    print(smg.vs[node]["name"])
    
    layout = [(0,0), (100,100), (100,0), (0,100), (25,25), (25,75), (75,25), (-50,0), (25,25), (40, 40)]
    visual_style = {}
    visual_style["vertex_name"] = smg.vs["name"]
    visual_style["vertex_label_size"] = 20
    visual_style["vertex_size"] = 40
    visual_style["vertex_color"] ="gray"#[color_dict[x] for x in smg.vs["name"]]
    visual_style["vertex_label"] = smg.vs["name"]
    visual_style["layout"] = layout
    visual_style["bbox"] = (600, 600)
    visual_style["margin"] = 150    
    widths = [3] * len(smg.es)
    colors = ["black"]*len(smg.es) 
    for e in smg.es:
        #print(e.attributes()["func"])
        if (e.attributes()["retcode"] == "faulty" and e.attributes()["func"]):
            #widths[e.index] = 2
            colors[e.index] = "red"
        elif (e.attributes()["retcode"] == "faulty" and e.attributes()["func"] == ""):
            colors[e.index] = "orange"
    # Update the graph with the color and width for the fault path
    smg.es['width'] = widths
    smg.es['color'] = colors 
    #sname = re.findall(r'cidl_(.*?).h',service_name + "_c_stub.c")[0]
    sname = service_name
    plot(smg, "output/SM_"+sname+".svg", **visual_style)
    #plot(smg, **visual_style)
    
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

sched_timestamp_track_str = '''
if (last_system_ticks > ret) {
    sched_restore_ticks(last_system_ticks);
    ret = last_system_ticks;
} else last_system_ticks = ret;
'''

slab_alloc_str = '''
extern void *alloc_page(void);
extern void free_page(void *ptr);

#define CSLAB_ALLOC(sz)   alloc_page()
#define CSLAB_FREE(x, sz) free_page(x)
#include <cslab.h>

#define CVECT_ALLOC() alloc_page()
#define CVECT_FREE(x) free_page(x)
#include <cvect.h>
'''

init_map_str = '''
if (unlikely(!IDL_service_desc_maps.data.depth)) {
cos_map_init_static(&IDL_service_desc_maps);
}
'''

para_save_str = '''
static char *
param_save(char *param, int param_len)
{
        char *l_param;
        
        if (param_len <= 0) return NULL;
        assert(param);
        l_param = malloc(param_len);
        assert(l_param);
        strncpy(l_param, param, param_len);
        l_param[param_len] = '\\0\';   // zero out any thing left after the end                                                                                                                  
        return l_param;
}
'''

benchmark_meas_start = '''
IDL_fname_ubenchmark_flag = 1;
rdtscll(ubenchmark_start);
'''
benchmark_meas_end = '''
rdtscll(ubenchmark_end);
if (IDL_fname_ubenchmark_flag) {
    IDL_fname_ubenchmark_flag = 0;
    printc(\"IDL_fname:recover per object end-end cost: %llu\\n", ubenchmark_end - ubenchmark_start);
    }
'''
benchmark_vars = '''
volatile unsigned long long ubenchmark_start, ubenchmark_end;
'''

ser_treadp_str='''
int
__ser_treadp(spdid_t spdid, int tid, int len, int __pad0, int *off_len)
{  
        int ret = 0;
        ret = treadp(spdid, tid, len, &off_len[0], &off_len[1]);  
        return ret;
}
'''

def add_c_header(h_name):
    header = ""
    header = header + r'''#include <cos_component.h>''' + "\n"
    header = header + r'''#include <sched.h>''' + "\n"
    header = header + r'''#include <print.h>''' + "\n"
    header = header + r'''#include <cos_debug.h>''' + "\n"
    header = header + r'''#include <cos_map.h>''' + "\n"
    header = header + r'''#include <cos_list.h>''' + "\n"
    header = header + r'''#include <cstub.h>''' + "\n"
    header = header + r'''#include <'''+ h_name  + ".h>\n"
    header = header + slab_alloc_str + "\n"
    header = header + "\n"
    return header

def add_s_header(h_name):
    header = ""
    header = header + r'''#include <cos_component.h>''' + "\n"
    header = header + r'''#include <sched.h>''' + "\n"
    header = header + r'''#include <print.h>''' + "\n"
    header = header + r'''#include <cos_debug.h>''' + "\n"
    header = header + r'''#include <cos_map.h>''' + "\n"
    header = header + r'''#include <cos_list.h>''' + "\n"
    header = header + r'''#include <cstub.h>''' + "\n"
    header = header + r'''#include <'''+ h_name  + ".h>\n"
    header = header + "\n"
    return header

cos_header_dict = {
"sched": cos_if_path+"sched/__sched_h",               
"mem_mgr": cos_if_path+"mem_mgr/__mem_mgr_h",
"lock": cos_if_path+"lock/__lock_h",
"evt": cos_if_path+"evt/__evt_h",
"ramfs": cos_if_path+"rtorrent/__torrent_h",
"periodic_wake":cos_if_path+"periodic_wake/__periodic_wake_h"
}

original_sstub = "/__stubs/s_stub.S"
cos_asm_dict = {
"sched": cos_if_path+"sched"+original_sstub,               
"mem_mgr": cos_if_path+"mem_mgr"+original_sstub,
"lock": cos_if_path+"lock"+original_sstub,
"evt": cos_if_path+"evt"+original_sstub,
"ramfs": cos_if_path+"rtorrent"+original_sstub,
"periodic_wake":cos_if_path+"periodic_wake"+original_sstub
}
  
# these are just some util functions  
DEBUG = 0
def printc(s):
    if DEBUG:
        pprint (s)

def repeat_to_length(string_to_expand, length):
   return (string_to_expand * ((length/len(string_to_expand))+1))[:length]