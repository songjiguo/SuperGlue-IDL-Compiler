#!/usr/bin/python
#title            :idl_script.py
#description      :This script does two things: 
#                  1) preprocess the IDL language mostly defined in macro 
#                  2) parse the AST and generate the final interface code in C
#author           :Jiguo Song
#date             :20150817
#version          :0.1
#usage            :python idl_script.py
#notes            :
#python_version   :2.6.6
#
#Input : this file should run on the defined macros in cidl_gen.h and cidl.h
#Output: interface recovery code in C 
#==============================================================================

from __future__ import print_function
import sys, os

sys.path.extend(['.', '..'])

find_put_string         = " "

fn_type                 = " "
fn_create               = "CREATE"
fn_terminate            = "TERMINATE"
fn_mutate               = "MUTATE"

# tuple
desc_close_type         = " "
desc_close_C0           = "desc_close_itself"
desc_close_C1           = "desc_close_subtree"
# tuple
desc_dep_create_type    = " "
desc_dep_create_P0      = "desc_dep_create_none"
desc_dep_create_P1      = "desc_dep_create_diff"
desc_dep_create_P2      = "desc_dep_create_same"
# tuple
desc_dep_close_type     = " "
desc_dep_close_Y0       = "desc_dep_close_remv"
desc_dep_close_Y1       = "desc_dep_close_keep"

from pycparser import parse_file, preprocess_file, c_parser, c_ast, c_generator

def print_some_space():
    print("  ")
    print("  ")

def my_modify_ast(ast):
    print_some_space()
    print("*********")

    generator = c_generator.CGenerator()
    print(generator.visit(ast))

    print("*********")
    print_some_space()

def test_block_1(ast):
    global fn_type, desc_close_type
    if (fn_type == 'TERMINATE') and (desc_close_type == 'Y0'):
        out_put_string = 'call desc_dealloc()'
        print(out_put_string)
        
def ptrdecl():        
    generator = c_generator.CGenerator()
    print(generator.visit(ast))
        
def process_para(param_node):
    if hasattr(param_node, 'type'):
        param_node.type.show(nodenames=True, offset=6)
        param_str = param_node.type.__class__.__name__
        if (param_str == 'PtrDecl' or param_str == 'TypeDecl' or
           param_str == 'IdentifierType'
           ):
            vlist = [getattr(param_node.type, n) for n in param_node.type.attr_names]
            attrstr = ', '.join('%s' % v for v in vlist)
            print('<<<attstr: ' + attrstr + ' >>>')
            for (child) in param_node.children():
                process_para(child)

class FuncDefVisitor(c_ast.NodeVisitor):
    def visit_FuncDef(self, node):
        print("****************")  
        print("****************")  
        function_decl = node.decl
        print('%s at %s' % (function_decl.name, function_decl.coord))
        for param_decl in function_decl.type.args.params:
            print(' ')
            print('Arg name: %s' % param_decl.name)
            process_para(param_decl)

def show_func_info(ast):
    ast.show()
    print_some_space()  
    v = FuncDefVisitor()
    print("show some func defs \n")   
    v.visit(ast)


class FuncDefVisitor2(c_ast.NodeVisitor):
    def visit_FuncDef(self, node):
        print('%s at %s' % (node.decl.name, node.decl.coord))


def show_func_defs(ast):
    ast = parse_file('cidl_pre', use_cpp=True)

    v = FuncDefVisitor2()
    ast.show()
    v.visit(ast)  
    
if __name__ == "__main__":
    if len(sys.argv) > 1:
        filename  = sys.argv[1]
    else:
        filename = 'cidl.h'
    
    os.system("gcc -E cidl.h -o cidl_pre");        
    os.system("cat cidl_pre");
    print(" ")
    print(" ")  
    
    print("****************")  
    print("<--- AST --->")  
    print("****************")  
    parser = c_parser.CParser()
    ast = parse_file('cidl_pre', use_cpp=True,
                     cpp_path='cpp', 
                     cpp_args=r'-Iutils/fake_libc_include')
  
#    test_block_1(ast)

    show_func_defs(ast)
