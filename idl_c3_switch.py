#!/usr/bin/python

# how to use this file --
# exmaple: python idl_c3_switch.py cos evt
# exmaple: python idl_c3_switch.py idl lock
# exmaple: python idl_c3_switch.py man sched

import os, sys, argparse

ramfs_if_path   = "/home/songjiguo/research/composite/src/components/interface/rtorrent/"
ramfs_path      = ramfs_if_path + "stubs/"
lock_if_path    = "/home/songjiguo/research/composite/src/components/interface/lock/"
lock_path       = lock_if_path + "stubs/"
evt_if_path     = "/home/songjiguo/research/composite/src/components/interface/evt/"
evt_path        = evt_if_path + "stubs/"
sched_if_path   = "/home/songjiguo/research/composite/src/components/interface/sched/"
sched_path      = sched_if_path + "stubs/"
mem_mgr_if_path = "/home/songjiguo/research/composite/src/components/interface/mem_mgr/"
mem_mgr_path    = mem_mgr_if_path + "stubs/"
periodic_wake_if_path  = "/home/songjiguo/research/composite/src/components/interface/periodic_wake/"
periodic_wake_path      = periodic_wake_if_path + "stubs/"

path = ""

STUB_DIR = "stubs"
COS_STUB_DIR = ""  # uased as the baseline
REFLECTION_S_STUB_DIR = ""

if (sys.argv[2] == "ramfs"):
    REFLECTION_S_STUB_DIR = lock_path
elif (sys.argv[2] == "evt"):
    REFLECTION_S_STUB_DIR = lock_path
elif (sys.argv[2] == "lock"):
    REFLECTION_S_STUB_DIR = sched_path
else:
    REFLECTION_S_STUB_DIR = ""

REFLECTION_S_STUB_DIR = ""   

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

def set_server_file(mode, service_name):
    if (mode == "idl" or mode == "man"):
        if (service_name == "lock"):
            path = "/home/songjiguo/research/composite/src/components/implementation/lock/two_phase/"
            os.chdir(path)
            if os.path.exists("lock.c"):
                os.system("unlink lock.c")
            os.system("ln -s __lock_rec lock.c")
        elif (service_name == "evt"):
            path = "/home/songjiguo/research/composite/src/components/implementation/evt/edge_grp/"
            os.chdir(path)
            if os.path.exists("evt.c"):
                os.system("unlink evt.c")
            os.system("ln -s __evt_rec evt.c")
            path = "/home/songjiguo/research/composite/src/components/implementation/evt/edge/"
            os.chdir(path)
            if os.path.exists("evt.c"):
                os.system("unlink evt.c")
            os.system("ln -s __evt_rec evt.c")
        elif (service_name == "ramfs"):
            path = "/home/songjiguo/research/composite/src/components/implementation/torrent/ramfs/"
            os.chdir(path)
            if os.path.exists("ramfs.c"):
                os.system("unlink ramfs.c")
            os.system("ln -s __ramfs_rec ramfs.c")
        elif (service_name == "sched"):
            path = "/home/songjiguo/research/composite/src/components/implementation/sched/"
            os.chdir(path)
            if os.path.exists("cos_sched_base.c"):
                os.system("unlink cos_sched_base.c")
            os.system("ln -s __cos_sched_base_rec cos_sched_base.c")
        elif (service_name == "mem_mgr"):
            path = "/home/songjiguo/research/composite/src/components/implementation/mem_mgr/naive/"
            os.chdir(path)
            if os.path.exists("mem_man.c"):
                os.system("unlink mem_man.c")
            os.system("ln -s __mem_man_rec mem_man.c")
        elif (service_name == "periodic_wake"):
            path = "/home/songjiguo/research/composite/src/components/implementation/timed_blk/timed_evt/"
            os.chdir(path)
            if os.path.exists("timed_event.c"):
                os.system("unlink timed_event.c")
            os.system("ln -s __timed_event_rec timed_event.c")
        else:
            print("idl or man: wrong service name or path")
    elif (mode == "cos"):
        if (service_name == "lock"):
            path = "/home/songjiguo/research/composite/src/components/implementation/lock/two_phase/"
            os.chdir(path)
            if os.path.exists("lock.c"):
                os.system("unlink lock.c")
            os.system("ln -s __lock lock.c")
        elif (service_name == "evt"):
            path = "/home/songjiguo/research/composite/src/components/implementation/evt/edge_grp/"
            os.chdir(path)
            if os.path.exists("evt.c"):
                os.system("unlink evt.c")
            os.system("ln -s __evt evt.c")
            path = "/home/songjiguo/research/composite/src/components/implementation/evt/edge/"
            os.chdir(path)
            if os.path.exists("evt.c"):
                os.system("unlink evt.c")
            os.system("ln -s __evt evt.c")
        elif (service_name == "ramfs"):
            path = "/home/songjiguo/research/composite/src/components/implementation/torrent/ramfs/"
            os.chdir(path)
            if os.path.exists("ramfs.c"):
                os.system("unlink ramfs.c")
            os.system("ln -s __ramfs ramfs.c")
        elif (service_name == "sched"):
            path = "/home/songjiguo/research/composite/src/components/implementation/sched/"
            os.chdir(path)
            if os.path.exists("cos_sched_base.c"):
                os.system("unlink cos_sched_base.c")
            os.system("ln -s __cos_sched_base cos_sched_base.c")
        elif (service_name == "mem_mgr"):
            path = "/home/songjiguo/research/composite/src/components/implementation/mem_mgr/naive/"
            os.chdir(path)
            if os.path.exists("mem_man.c"):
                os.system("unlink mem_man.c")
            os.system("ln -s __mem_man mem_man.c")
        elif (service_name == "periodic_wake"):
            path = "/home/songjiguo/research/composite/src/components/implementation/timed_blk/timed_evt/"
            os.chdir(path)
            if os.path.exists("timed_event.c"):
                os.system("unlink timed_event.c")
            os.system("ln -s __timed_event timed_event.c")
        else:
            print("cos: wrong service name or path")
    else:
        print("wrong mode")

if __name__ == '__main__':
    if (sys.argv[1] != "cos" and sys.argv[1] != "idl" and sys.argv[1] != "man"):
        print ("wrong mode!!!")
        exit()

    if (sys.argv[2] != "ramfs" and sys.argv[2] != "lock" and sys.argv[2] != "evt" and sys.argv[2] != "sched" and sys.argv[2] != "mem_mgr" and sys.argv[2] != "periodic_wake"):
        print ("wrong service!!!")
        exit()

    if (sys.argv[1] == "cos"):
        COS_STUB_DIR = "__stubs"
        if (sys.argv[2] == "lock"):
            path = lock_if_path
        elif (sys.argv[2] == "ramfs"):
            path = ramfs_if_path
        elif (sys.argv[2] == "evt"):
            path = evt_if_path
        elif (sys.argv[2] == "sched"):
            path = sched_if_path
        elif (sys.argv[2] == "mem_mgr"):
            path = mem_mgr_if_path
        elif (sys.argv[2] == "periodic_wake"):
            path = periodic_wake_if_path
        else:
            path = ""
    elif (sys.argv[1] == "idl" or sys.argv[1] == "man"):
        if (sys.argv[2] == "lock"):
            COS_STUB_DIR = "__stubs_rec"
            path = lock_if_path
        elif (sys.argv[2] == "ramfs"):
            COS_STUB_DIR = "__stubs_rec_ramfs"
            path = ramfs_if_path
        elif (sys.argv[2] == "evt"):
            COS_STUB_DIR = "__stubs_rec"
            path = evt_if_path
        elif (sys.argv[2] == "sched"):
            COS_STUB_DIR = "__stubs_rec"
            path = sched_if_path
        elif (sys.argv[2] == "mem_mgr"):
            COS_STUB_DIR = "__stubs_rec"
            path = mem_mgr_if_path
        elif (sys.argv[2] == "periodic_wake"):
            COS_STUB_DIR = "__stubs_rec"
            path = periodic_wake_if_path
        else:
            path = ""
    
    if not path:
        exit()
    if not COS_STUB_DIR:
        exit()
    
    print("\n[[[ "+ sys.argv[2] + " in "+ sys.argv[1] + " mode ]]]\n")
    #print("stub path :" + path)
    #print("stub for the mode " + sys.argv[1] + " :"  + COS_STUB_DIR)
    #print("reflection on stub: "+ REFLECTION_S_STUB_DIR)
    #exit()
    
    os.chdir(path)

    # choose header    
    if (sys.argv[1] == "cos"):
        if (sys.argv[2] == "lock"):
            if os.path.exists("lock.h"):
                os.system("unlink lock.h")
            os.system("ln -s __lock_h lock.h")
        elif (sys.argv[2] == "ramfs"):
            #print("this is ramfs file")
            print
        elif (sys.argv[2] == "evt"):
            if os.path.exists("evt.h"):
                os.system("unlink evt.h")
            os.system("ln -s __evt_h evt.h")
        elif (sys.argv[2] == "sched"):
            if os.path.exists("sched.h"):
                os.system("unlink sched.h")
            os.system("ln -s __sched_h sched.h")
        elif (sys.argv[2] == "mem_mgr"):
            if os.path.exists("mem_mgr.h"):
                os.system("unlink mem_mgr.h")
            os.system("ln -s __mem_mgr_h mem_mgr.h")
        elif (sys.argv[2] == "periodic_wake"):
            if os.path.exists("periodic_wake.h"):
                os.system("unlink periodic_wake.h")
            os.system("ln -s __periodic_wake_h periodic_wake.h")
        else:
            print("wrong!!!! here")
            exit()
    elif (sys.argv[1] == "idl" or sys.argv[1] == "man"):
        if (sys.argv[2] == "lock"):
            if os.path.exists("lock.h"):
                os.system("unlink lock.h")
            os.system("ln -s __lock_h_rec lock.h")
        elif (sys.argv[2] == "ramfs"):
            print("this is ramfs file")
        elif (sys.argv[2] == "evt"):
            if os.path.exists("evt.h"):
                os.system("unlink evt.h")
            os.system("ln -s __evt_h_rec evt.h")
        elif (sys.argv[2] == "sched"):
            if os.path.exists("sched.h"):
                os.system("unlink sched.h")
            os.system("ln -s __sched_h_rec sched.h")
        elif (sys.argv[2] == "mem_mgr"):
            if os.path.exists("mem_mgr.h"):
                os.system("unlink mem_mgr.h")
            os.system("ln -s __mem_mgr_h_rec mem_mgr.h")
        elif (sys.argv[2] == "periodic_wake"):
            if os.path.exists("periodic_wake.h"):
                os.system("unlink periodic_wake.h")
            os.system("ln -s __periodic_wake_h_rec periodic_wake.h")
        else:
            print("wrong!!!! here rec")
            exit()
    
    
    
    if (sys.argv[1] == "idl"):
        #reflect_and_unlink()
        if os.path.exists(path+STUB_DIR):
            os.system("unlink " + STUB_DIR)
            os.system("ln -s " + COS_STUB_DIR + " " + STUB_DIR)

        os.chdir(path+STUB_DIR)
        if os.path.exists("s_cstub.c"):
            os.system("unlink "  + SCSTUB)
            os.system("ln -s " + IDL_SCSTUB + " " + SCSTUB)
        if os.path.exists("c_stub.c"):
            os.system("unlink "  + CSTUB)
            os.system("ln -s " + IDL_CSTUB  + " " + CSTUB)
        if os.path.exists("s_stub.S"):
            os.system("unlink "  + SSTUB)
            os.system("ln -s " + IDL_SSTUB  + " " + SSTUB)
    elif (sys.argv[1] == "man"):
        #reflect_and_unlink()
        if os.path.exists(path+STUB_DIR):
            os.system("unlink " + STUB_DIR)
            os.system("ln -s " + COS_STUB_DIR + " " + STUB_DIR)

        os.chdir(path+STUB_DIR)
        if os.path.exists("s_cstub.c"):
            os.system("unlink "  + SCSTUB)
            os.system("ln -s " + MAN_SCSTUB + " " + SCSTUB)
        if os.path.exists("c_stub.c"):
            os.system("unlink "  + CSTUB)
            os.system("ln -s " + MAN_CSTUB  + " " + CSTUB)
        if os.path.exists("s_stub.S"):
            os.system("unlink "  + SSTUB)
            os.system("ln -s " + MAN_SSTUB  + " " + SSTUB)
    elif (sys.argv[1] == "cos"):
        #=======================================================================
        # if (REFLECTION_S_STUB_DIR):
        #     os.chdir(REFLECTION_S_STUB_DIR)
        #     if (os.path.exists(REFLECTION_S_STUB_DIR+"s_cstub.c")):
        #         os.system("unlink s_cstub.c")
        #     os.chdir(path)
        #=======================================================================
        
        #print("asdsa")
        #print(path+STUB_DIR)
        #print(path+COS_STUB_DIR)
        if os.path.exists(path+STUB_DIR):
            os.system("unlink " + STUB_DIR)
            os.system("ln -s " + COS_STUB_DIR + " " + STUB_DIR)
    else:
        print("wrong mode!!\n")
        exit()

    set_server_file(sys.argv[1], sys.argv[2])
    
