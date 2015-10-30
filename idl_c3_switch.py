#!/usr/bin/python

import os, sys, argparse

# parser = argparse.ArgumentParser(
#     description = "This is the python code to switch between C^3 and IDL code",
#     epilog = '''
#     python idl_man_switch.py idl lock -or- python idl_man_switch.py man lock \n
#     python idl_man_switch.py cos lock\n

# output = parser.parse_args()

#print 'no of args:', len(sys.argv),'arguments.'
#print 'Argument list:', str(sys.argv)
#exit()

ramfs_if_path   = "/home/songjiguo/research/composite/src/components/interface/rtorrent/"
ramfs_path      = ramfs_if_path + "stubs/"
lock_if_path    = "/home/songjiguo/research/composite/src/components/interface/lock/"
lock_path       = lock_if_path + "stubs/"
sched_if_path    = "/home/songjiguo/research/composite/src/components/interface/sched/"
sched_path      = sched_if_path + "stubs/"

path = ""

STUB_DIR = "stubs"
COS_STUB_DIR = ""  # uased as the baseline
REFLECTION_S_STUB_DIR = ""

if (sys.argv[2] == "ramfs"):
    REFLECTION_S_STUB_DIR = lock_path
elif (sys.argv[2] == "lock"):
    REFLECTION_S_STUB_DIR = sched_path
else:
    REFLECTION_S_STUB_DIR = ""
    
if (sys.argv[1] != "cos" and sys.argv[1] != "idl" and sys.argv[1] != "man"):
    print ("wrong mode!!!")
    exit()

if (sys.argv[2] != "ramfs" and sys.argv[2] != "lock"):
    print ("wrong service!!!")
    exit()

if (sys.argv[1] == "cos"):
    COS_STUB_DIR = "__stubs"
    if (sys.argv[2] == "lock"):
        path = lock_if_path
    elif (sys.argv[2] == "ramfs"):
        path = ramfs_if_path
    else:
        path = ""
elif (sys.argv[1] == "idl" or sys.argv[1] == "man"):
    if (sys.argv[2] == "lock"):
        COS_STUB_DIR = "__stubs_rec"
        path = lock_if_path
    elif (sys.argv[2] == "ramfs"):
        COS_STUB_DIR = "__stubs_rec_ramfs"
        path = ramfs_if_path
    else:
        path = ""

if not path:
    exit()
if not COS_STUB_DIR:
    exit()

print("[[[ "+ sys.argv[2] + " in "+ sys.argv[1] + " mode ]]]")

#print(path)
#print(COS_STUB_DIR)
#print(REFLECTION_S_STUB_DIR)
os.chdir(path)
#exit()

IDL_SCSTUB = "__IDL_s_cstub.c"
IDL_CSTUB  = "__IDL_c_stub.c"
IDL_SSTUB  = "__IDL_s_stub.S"
MAN_SCSTUB  = "__s_cstub.c"
MAN_CSTUB   = "__c_stub.c"
MAN_SSTUB   = "__s_stub.S"

SCSTUB  = "s_cstub.c"
CSTUB   = "c_stub.c"
SSTUB   = "s_stub.S"

def unlink_all():
    if os.path.exists("s_cstub.c"):
        os.system("unlink "  + SCSTUB)
    if os.path.exists("c_stub.c"):
        os.system("unlink "  + CSTUB)
    if os.path.exists("s_stub.S"):
        os.system("unlink "  + SSTUB)

def reflect_and_unlink():
    if (REFLECTION_S_STUB_DIR):
        os.chdir(REFLECTION_S_STUB_DIR)
        if (os.path.exists(REFLECTION_S_STUB_DIR+"s_cstub.c")):
            os.system("unlink s_cstub.c")
        os.system("ln -s ../__stubs_rec/s_cstub.c s_cstub.c")
        os.chdir(path)
    if os.path.exists(path+STUB_DIR):
        os.system("unlink " + STUB_DIR)
    os.system("ln -s " + COS_STUB_DIR + " " + STUB_DIR)
    os.chdir(path+STUB_DIR)
    unlink_all()

if (sys.argv[1] == "idl"):
    reflect_and_unlink()
    os.system("ln -s " + IDL_SCSTUB + " " + SCSTUB)
    os.system("ln -s " + IDL_CSTUB  + " " + CSTUB)
    os.system("ln -s " + IDL_SSTUB  + " " + SSTUB)
elif (sys.argv[1] == "man"):
    reflect_and_unlink()
    os.system("ln -s " + MAN_SCSTUB + " " + SCSTUB)
    os.system("ln -s " + MAN_CSTUB  + " " + CSTUB)
    os.system("ln -s " + MAN_SSTUB  + " " + SSTUB)
elif (sys.argv[1] == "cos"):
    if (REFLECTION_S_STUB_DIR):
        os.chdir(REFLECTION_S_STUB_DIR)
        if (os.path.exists(REFLECTION_S_STUB_DIR+"s_cstub.c")):
            os.system("unlink s_cstub.c")
        os.chdir(path)
        
    if os.path.exists(path+STUB_DIR):
        os.system("unlink " + STUB_DIR)
    os.system("ln -s " + COS_STUB_DIR + " " + STUB_DIR)
else:
    print("wrong mode!!\n")

