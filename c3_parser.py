#!/usr/bin/python
# title            :c3_parser.py
# description      :This script preprocesses the IDL language mostly defined in macro 
#                  and send to the 2nd stage to generagte the AST and the final interface code in C
# author           :Jiguo Song
# date             :20150817
# version          :0.1
# notes            :
# python_version   :2.6.6
#
# Input : this file should run on the defined macros in cidl_gen.h and cidl.h
# Output: interface recovery code in C 
#==============================================================================

# Rules For Constructing Variable Name
#
#  Characters Allowed :
#     Underscore(_)
#     Capital Letters (A-Z)
#     Small Letters (a-z)
#     Digits (0-9)
#
#  Blanks & Commas are not allowed
#  No Special Symbols other than underscore(_) are allowed
#  First Character should be alphabet or Underscore
#  Variable name Should not be Reserved Word

# We define the following delimiter:
#  SCD -- Start of C3 Delimiter
#  ECD -- End of C3 Delimiter
#  CD --  C3 Delimiter

from __future__ import print_function
import sys, os, re 
#from pprint import pprint
from pycparser import parse_file, preprocess_file
from pycparser import c_parser, c_ast, c_generator
sys.path.extend(['.', '..'])

import c3_gen, keywords
from pprint import pprint

#get_desc_track_struct(result, node);

idl_parse_func_result = []  # global string to save parsed func info
idl_parse_tuple_result = []  # global string to save parsed tuple info

def get_class_name(node):
    return node.__class__.__name__
       
def get_type_name(node):
    return node.type.type.type.names

def get_dec_type_name(node):
    return node.type.type.names
       
def parse_idl_str(delimiter, mystr):
    ret = []    

    pattern = "(?<=" + delimiter + ")(?:(?!" + \
              delimiter + ").){5,}(?=" + delimiter + ")"

    last_str = ""
    for match in re.finditer(pattern, mystr, re.I):
        result_str = match.group()
        result_str = result_str[1:-1]
        ret.append(result_str)
        last_str = mystr[match.end() + len(delimiter):]
        last_str = last_str[1:]
    ret.append(last_str)    
    return ret
    
#===============================================================================
# # Struct: [name, decls**]
# # struct is node.decls  --> decl list (node is Struct)
# 
# # Decl: [name, quals, storage, funcspec, type*, init*, bitsize*]
# # field is Decl  
#   
# # TypeDecl: [declname, quals, type*]  
# # field.type is TypeDecl  
# 
# # IdentifierType: [names]
# # field.type.tpye is IdentifierType  
#===============================================================================
class StructVisitor(c_ast.NodeVisitor):
    def visit_Struct(self, node):
        #global result
        #result.add_tuple()
        ret = parse_structure_info(node)
        
def parse_structure_info(node):
    struct  = node.decls
    #print (node.name)
    for field in struct:
        tmp = get_class_name(field.type)
        if (tmp == keywords.ptrdecl):
            key = "".join(field.type.type.type.names) + " *"
        elif (tmp == keywords.typedecl):
            key = "".join(field.type.type.names)
        elif (tmp == keywords.funcdecl):
            print ("keywords is function decl")
        val = field.name
        if (node.name == "tuple"):
            result.tuple[-1].info[key] = val
            result.gvars[node.name] = result.tuple[-1].info
        elif (node.name == "func_sm"):
            result.tuple[-1].sm_info[key] = val
            result.gvars[node.name] = result.tuple[-1].sm_info
        elif (node.name == "desc_data"):
            result.tuple[-1].desc_data_fields.append([key,val])
            result.gvars[node.name] = result.tuple[-1].desc_data_fields
    #pprint(result.gvars)
    
#===============================================================================
# # FuncDecl: [args*, type*]
# # node is FuncDecl
# #
# # ParamList: [params**]
# # node.args is the ParamList
# # Decl: [name, quals, storage, funcspec, type*, init*, bitsize*]
# # an item in the ParamList is a Decl   
#
# # TypeDecl: [declname, quals, type*]  
# # ndoe.type is TypeDecl  
# 
# # IdentifierType: [names]
# # ndoe.type.tpye is IdentifierType  
#===============================================================================
class FuncDeclVisitor(c_ast.NodeVisitor):
    def visit_FuncDecl(self, node):
        global result
        result.tuple[-1].add_function()
        parse_func(node)
        
def parse_func(node):
    global idl_parse_result 
    func_params = [] 

    ##### begin of a function #####
    fun = result.tuple[-1].functions[-1]   # last added tuple and last added functoin
    fun_info = fun.info
    # set func name and sm state for a function
    fun_info[fun.name] = node.type.declname
    for k, v in result.tuple[-1].sm_info.iteritems():
        if (re.sub('\_sm$', '', k) == node.type.declname):
            #fun_info[v] = re.sub('\_sm$', '', k) 
            fun_info[fun.sm_state] = v
            break;   # only one matched function allowed    
    #pprint (fun_info)

    #### parameters of a function #####
    for param_decl in node.args.params:
        func_params = parse_parameters(param_decl)
        #print (func_params)
        # swap the type and value for para (last one is the pycparser type, eg.g, PtrDecl)
        tmp = func_params[-2]
        func_params[-2] = func_params[-3]
        func_params[-3] = tmp
           
        # for normal parameters
        fun.normal_para.append((func_params[-3], func_params[-2]))
           
        # if there is a match 
        if (func_params[0] in fun_info):
            if isinstance(fun_info[func_params[0]], basestring):
                print (">>>>>  " + func_params[0])
                fun_info[func_params[0]] = func_params[1:]
            elif isinstance(fun_info[func_params[0]], list):
                fun_info[func_params[0]].append(func_params[1:])
          
    #### return of a function ####        
    func_return = parse_idl_str('CD', str(get_dec_type_name(node)[0]))
    if (not func_return[0]):
        func_return[0] = node.type.type.names[0]
    fun_info[func_return[0]] = func_return[1:]
    
    #print (result.tuple[-1].functions[-1].normal_para)

# # Decl: [name, quals, storage, funcspec, type*, init*, bitsize*]
# # node is a Decl
def parse_parameters(node):
    func_params = parse_idl_str('CD', node.name)
    if (not func_params[0]):
        func_params[0] = node.name  # normal para 
        
    param_str = get_class_name(node.type)
    if (param_str == keywords.ptrdecl):
        func_params.append(get_type_name(node)[0]+" *")
    if (param_str == keywords.typedecl):
        func_params.append(node.type.type.names[0])
    if (param_str == keywords.funcdecl):
        for param_funcdecl in node.type.args.params:
            func_params.append(param_funcdecl.name)
        sub_return = get_class_name(node.type.type)
        if (sub_return == keywords.ptrdecl):
            ret_tp = node.type.type.type.type.names
            ret_tp[0] = ret_tp[0]+" *"
        if (sub_return == keywords.typedecl):    
            ret_tp = node.type.type.type.names                    
        func_params.append(ret_tp[0])                

    func_params.append(param_str)  # add pycparser type
    #print(func_params)
    return func_params
  
#===============================================================================
# # main function - parsing the input information
#===============================================================================
def parse_func_decl(ast):
    v = FuncDeclVisitor()
    v.visit(ast)
    
def parse_struct(ast):
    v = StructVisitor()
    v.visit(ast)
    
#===============================================================================
# def parse_global_vars(ast):
#     v = GlobalVarsVisitor()
#     v.visit(ast)
#===============================================================================

if __name__ == "__main__":
    if len(sys.argv) > 1:
        filename = sys.argv[1]
    else:
        filename = 'cidl.h'
    
    os.system("gcc -E cidl.h -o cidl_pre");        
    #os.system("cat cidl_pre");
    
    # print_some_space() 
    # print_logo('<--- AST --->')    
    parser = c_parser.CParser()
    ast = parse_file('cidl_pre', use_cpp=True,
                     cpp_path='cpp',
                     cpp_args=r'-Iutils/fake_libc_include')
    #ast.show()
    
    result = keywords.IDLServices()
    parse_struct(ast)
    parse_func_decl(ast)
  
#===============================================================================
#     # pprint (result.tuple[0].info)
#     # pprint (result.tuple[0].sm_info)
#     # pprint (result.tuple[0].desc_data_fields)
#     pprint (result.gvars)
# 
#     print("")
#     pprint (result.tuple[0].functions[0].info)
#     print("")
#     pprint (result.tuple[0].functions[1].info)
#     print("")
#    pprint (result.tuple[0].functions[2].info)
#     print("")
#     pprint (result.tuple[0].functions[3].info)
#     print("")
#    exit()
#===============================================================================
  
    c3_gen.idl_generate(result, ast)
    
    