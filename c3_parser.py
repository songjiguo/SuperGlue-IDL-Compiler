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

class GlobalVarsVisitor(c_ast.NodeVisitor):
    def visit_Struct(self, node):
        global result
        parse_globalvars_info(result.gvars, node)

class StructVisitor(c_ast.NodeVisitor):
    def visit_Struct(self, node):
        global result
        result.add_tuple()
        ret = parse_structure_info('TP', node.decls)
        if (ret == -1):
            result.tuple.pop()  # this must be some other normal structure
            return ret
        parse_structure_info('SM', node.decls)

        # debug
        #if not result.tuple[-1].info[result.tuple[-1].desc_close]:
        #    return            
        #pprint (result.tuple[-1].info)
    
class FuncDeclVisitor(c_ast.NodeVisitor):
    def visit_FuncDecl(self, node):
        global result
        result.tuple[-1].add_function()
        parse_func(node, "funcdecl")
        #pprint (result.tuple[-1].functions[-1].info)
         
class PtrDeclVisitor(c_ast.NodeVisitor):
    def visit_PtrDecl(self, node):
        print_logo('pointer')
        print('%s' % (node.quals))
        print('%s' % (node.type.declname))
        print('%s' % (node.type.type.names))

class StructRefVisitor(c_ast.NodeVisitor):
    def visit_StructRef(self, node):
        print_logo('struct ref field')
        print('%s' % (node.field))
         
class FuncDefVisitor(c_ast.NodeVisitor):
    def visit_FuncDef(self, node):
        global idl_parse_result 
        
        idl_result = []      
        func_name = []
        func_params = []        
        decl = node.decl

        # begin of a function
        func_name.append(decl.name)
        idl_result.append(func_name)
        
        # parameters of a function        
        for param_decl in decl.type.args.params:
            func_params = parse_parameters(param_decl)
            idl_result.append(func_params)
        
        # return of a function        
        func_return = parse_idl_str('CD', str(get_type_name(decl)[0]))
        if (not func_return[0]):
            func_return[0] = decl.type.type.type.names[0]
        idl_result.append(func_return)
        
        idl_parse_func_result.append(idl_result)        
           
           
class ParamListVisitor(c_ast.NodeVisitor):
    def visit_ParamList(self, node):
        print_logo('paramlist')
        print('%s' % (node.params))
        for param_decl in node.params:
            print(' ')
            print('Arg name: %s' % param_decl.name)
            param_str = param_decl.type.__class__.__name__
            print('node type: %s' % param_str)
            if (param_str == 'PtrDecl'):
                print('ptrdecl: %s' % param_decl.type.type.type.names)
            if (param_str == 'FuncDecl'):
                for param_decl in param_decl.type.args.params:
                    print('Arg name: %s' % param_decl.name)
         
def print_logo(name):
    print("************************")  
    print("   " + name + "     ")  
    print("************************")
    print()    
    
def explain_c_decl(c_decl):

    parser = c_parser.CParser()

    try:
        node = parser.parse(c_decl, filename='<stdin>')
    except c_parser.ParseError:
        e = sys.exc_info()[1]
        return "Parse error:" + str(e)

    if (not isinstance(node, c_ast.FileAST) or
        not isinstance(node.ext[-1], c_ast.Decl)
        ):
        return "Not a valid declaration"

    return _explain_decl_node(node.ext[-1])


def _explain_decl_node(decl_node):
    storage = ' '.join(decl_node.storage) + ' ' if decl_node.storage else ''

    return (decl_node.name + 
            " is a " + 
            storage + 
            _explain_type(decl_node.type))
                    
def _explain_type(decl):
    """ Recursively goes through a type decl node
    """
    typ = type(decl)

    if typ == c_ast.TypeDecl:
        quals = ' '.join(decl.quals) + ' ' if decl.quals else ''
        return quals + _explain_type(decl.type)
    elif typ == c_ast.Typename or typ == c_ast.Decl:
        return _explain_type(decl.type)
    elif typ == c_ast.IdentifierType:
        return ' '.join(decl.names)
    elif typ == c_ast.PtrDecl:
        quals = ' '.join(decl.quals) + ' ' if decl.quals else ''
        return quals + 'pointer to ' + _explain_type(decl.type)
    elif typ == c_ast.ArrayDecl:
        arr = 'array'
        if decl.dim: arr += '[%s]' % decl.dim.value

        return arr + " of " + _explain_type(decl.type)

    elif typ == c_ast.FuncDecl:
        if decl.args:
            params = [_explain_type(param) for param in decl.args.params]
            args = ', '.join(params)
        else:
            args = ''

        return ('function(%s) returning ' % (args) + 
                _explain_type(decl.type))
        
def get_class_name(node):
    return node.__class__.__name__
       
def get_type_name(node):
    return node.type.type.type.names

def get_dec_type_name(node):
    return node.type.type.names
       
def find_in_string(target_str, str):
    match = re.search(r"[^a-zA-Z](" + target_str + ")[^a-zA-Z]", str) 
    if match is None:
        return None
    return match.string[match.start(1):match.end(1)]    

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

def set_sm_state(func_name):
    # get the state info from the tuple structure here (easier)
    #tmp_fun_info[tmp_fun.sm_state] = result.tuple[-1].info[]
    tmp_fun = result.tuple[-1].functions[-1]
    tmp_fun_info = result.tuple[-1].functions[-1].info
    for tmp_k, tmp_v in result.tuple[-1].info.iteritems():
        if isinstance(tmp_v, list):
            for tmp_item in tmp_v:
                if (tmp_item == func_name):
                    tmp_fun_info[tmp_fun.sm_state] = tmp_k
                    break
        elif (tmp_v == func_name):
            tmp_fun_info[tmp_fun.sm_state] = tmp_k
            break;   
        
#***************************             
#***************************
#** IDL parsing functions **
#***************************
#***************************
def parse_globalvars_info(gvars_list, node):
    # tuple needs to be processed differently 
    if (node.name == "service_tuple"):
        return        
    #node.show()
    tmp = []
    for field in node.decls:
        #print (field.name)
        #print (field.type.type.names)
        param_str = get_class_name(field.type)
        #print (param_str)
        if (param_str == keywords.ptrdecl):
            tmp_str = ' '.join(field.type.type.type.names)+" *"
        elif (param_str == keywords.typedecl):
            tmp_str = ' '.join(field.type.type.names)
        elif (param_str == keywords.funcdecl):
            print ("Not defined yet!")  
        #print (tmp_str)
        tmp.append(tmp_str + " " + field.name)
    gvars_list[node.name] = tmp
    
def parse_structure_info(pattern, struct):
    for field in struct:
        tmp = parse_idl_str(pattern, field.name)
        if not tmp[0]:
            return -1
        tmp_tup = result.tuple[-1]
        tmp_tup_info = tmp_tup.info
        if not tmp_tup_info[tmp[0]]:
            tmp_tup_info[tmp[0]] = tmp[-1]
        else:
            tmp_tup_info[tmp[0]] = [tmp_tup_info[tmp[0]], tmp[-1]]
    return 1
   
def parse_parameters(decl_func_node):
    func_params = parse_idl_str('CD', decl_func_node.name)
    #print (func_params)
    if (not func_params[0]):
        func_params[0] = decl_func_node.name  # normal para 
    if(func_params == ['']):
        print ("no para to save")
        
    param_str = get_class_name(decl_func_node.type)
    if (param_str == keywords.ptrdecl):
        func_params.append(get_type_name(decl_func_node)[0]+" *")
    if (param_str == keywords.typedecl):
        func_params.append(decl_func_node.type.type.names[0])
    if (param_str == keywords.funcdecl):
        for param_funcdecl in decl_func_node.type.args.params:
            #===============================================================
            # tmp = parse_idl_str('CD', param_funcdecl.name)
            # if (tmp):
            #     func_params.append(tmp[-1])
            # else:
            #     func_params.append(param_funcdecl.name)
            #===============================================================
            func_params.append(param_funcdecl.name)
            
        sub_return = get_class_name(decl_func_node.type.type)
        if (sub_return == keywords.ptrdecl):
            ret_tp = decl_func_node.type.type.type.type.names
            ret_tp[0] = ret_tp[0]+" *"
        if (sub_return == keywords.typedecl):    
            ret_tp = decl_func_node.type.type.type.names                    
        func_params.append(ret_tp[0])                

    func_params.append(param_str)  # add pycparser type
    return func_params

def parse_func(node, mytype):
    global idl_parse_result 
       
    func_params = [] 
              
    if (mytype == "funcdecl"):               
        decl = node
    if (mytype == "funcdef"):               
        decl = node.decl

    ##### begin of a function #####
        
    tmp_fun = result.tuple[-1].functions[-1]
    tmp_fun_info = result.tuple[-1].functions[-1].info
    tmp_fun_info[tmp_fun.name] = decl.type.declname
       
    set_sm_state(decl.type.declname)
                
    #### parameters of a function #####
    for param_decl in decl.args.params:
        func_params = parse_parameters(param_decl)
        #print (func_params[0])
        # swap the type and value for para (last one is the pycparser type)
        tmp = func_params[-2]
        func_params[-2] = func_params[-3]
        func_params[-3] = tmp
           
        # for any parameter
        result.tuple[-1].functions[-1].normal_para.append((func_params[-3], func_params[-2]))
           
        # if there is a match 
        if (func_params[0] in result.tuple[-1].functions[-1].info):
            if isinstance(result.tuple[-1].functions[-1].info[func_params[0]], basestring):
                print (">>>>>  " + func_params[0])
                result.tuple[-1].functions[-1].info[func_params[0]] = func_params[1:]
            elif isinstance(result.tuple[-1].functions[-1].info[func_params[0]], list):
                result.tuple[-1].functions[-1].info[func_params[0]].append(func_params[1:])
          
    #### return of a function ####        
    func_return = parse_idl_str('CD', str(get_dec_type_name(decl)[0]))
    if (not func_return[0]):
        if (mytype == "funcdecl"):
            func_return[0] = decl.type.type.names[0]
        if (mytype == "funcdef"):
            func_return[0] = decl.type.type.type.names[0]
    result.tuple[-1].functions[-1].info[func_return[0]] = func_return[1:]
    #print (result.tuple[-1].functions[-1].normal_para)
 
def parse_func_decl(ast):
    v = FuncDeclVisitor()
    v.visit(ast)
    
def parse_func_def(ast):
    v = FuncDefVisitor()
    v.visit(ast)    
    
def parse_tuple(ast):
    v = StructVisitor()
    v.visit(ast) 
    
def parse_global_vars(ast):
    v = GlobalVarsVisitor()
    v.visit(ast)

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
    # ast.show()

    result = keywords.IDLServices()
    parse_tuple(ast)
    parse_func_decl(ast)
    parse_global_vars(ast)
  
    #pprint (result.tuple[0].info)
    #pprint (result.tuple[0].functions[0].info)
    #pprint (result.gvars)
    #exit()
  
    c3_gen.idl_generate(result, ast)
    
    