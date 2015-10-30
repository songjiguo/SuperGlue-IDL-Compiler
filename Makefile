CC=gcc
PYTHON=python
PARSER=c3_parser.py
FAKE_HEADER=fake_header.h
TMP_OUTPUT=tmp_result


# service name should be same as the interfacre in the Composite
LOCK_SERVICE=lock
FS_SERVICE=ramfs
SCHED_SERVICE=sched
MM_SERVICE=mm
MBOX_SERVICE=mbox
EVENT_SERVICE=event
TE_SERVICE=te

##############
## everything
##############

all: parse_all compile_all gen_all cp_all

all_lock: parse_lock compile_lock gen_lock cp_lock plot_lock

all_ramfs: parse_ramfs compile_ramfs gen_ramfs cp_ramfs plot_ramfs


all_bench: parse_all compile_all gen_all_bench cp_all

all_lock_bench: parse_lock compile_lock gen_lock_bench cp_lock plot_lock

all_ramfs_bench: parse_ramfs compile_ramfs gen_ramfs_bench cp_ramfs plot_ramfs

##############
## parsing
##############
parse_all: parse_ramfs parse_lock
	@echo

parse_ramfs:
	@echo $(FINAL_CODE)
	@echo "IDL process starting.... <<<"$(FS_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(FS_SERVICE).h

parse_lock:
	@echo
	@echo "IDL process starting.... <<<"$(LOCK_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(LOCK_SERVICE).h

##############
## compiling
##############
compile_all: compile_ramfs compile_lock
	@echo

compile_ramfs:
	@echo
	@echo "Compiling starting.... <<<"$(FS_SERVICE)">>>"
	$(CC)  -Werror -include $(FAKE_HEADER) -o output/$(TMP_OUTPUT) output/$(FS_SERVICE)_c_stub.c
	$(CC)  -Werror -include $(FAKE_HEADER) -o output/$(TMP_OUTPUT) output/$(FS_SERVICE)_s_cstub.c
	rm output/$(TMP_OUTPUT)

compile_lock:
	@echo
	@echo "Compiling starting.... <<<"$(LOCK_SERVICE)">>>"
	$(CC)  -Werror -include $(FAKE_HEADER) -o output/$(TMP_OUTPUT) output/$(LOCK_SERVICE)_c_stub.c
	$(CC)  -Werror -include $(FAKE_HEADER) -o output/$(TMP_OUTPUT) output/$(LOCK_SERVICE)_s_cstub.c
	rm output/$(TMP_OUTPUT)

########################################################
## generate the acutal interface code w/ ubenchmark
########################################################
gen_all_bench: gen_ramfs_bench gen_lock_bench
	@echo

gen_ramfs_bench:
	@echo $(FINAL_CODE)
	@echo "IDL process starting.... <<<"$(FS_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(FS_SERVICE).h bench

gen_lock_bench:
	@echo
	@echo "IDL process starting.... <<<"$(LOCK_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(LOCK_SERVICE).h bench

########################################################
## generate the acutal interface code for Composite
########################################################
gen_all: gen_ramfs gen_lock
	@echo

gen_ramfs:
	@echo $(FINAL_CODE)
	@echo "IDL process starting.... <<<"$(FS_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(FS_SERVICE).h final

gen_lock:
	@echo
	@echo "IDL process starting.... <<<"$(LOCK_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(LOCK_SERVICE).h final

######################################################################
## copy the generated files to the Compoiste interface(symbolink further)
######################################################################
cp_all: cp_ramfs cp_lock

cp_ramfs:
	cp output/final_$(FS_SERVICE)_c_stub.c /home/songjiguo/research/composite/src/components/interface/rtorrent/stubs/__IDL_c_stub.c
	cp output/final_$(FS_SERVICE)_s_cstub.c /home/songjiguo/research/composite/src/components/interface/rtorrent/stubs/__IDL_s_cstub.c
	cp output/final_$(FS_SERVICE)_s_stub.S /home/songjiguo/research/composite/src/components/interface/rtorrent/stubs/__IDL_s_stub.S

cp_lock:
	cp output/final_$(LOCK_SERVICE)_c_stub.c /home/songjiguo/research/composite/src/components/interface/$(LOCK_SERVICE)/stubs/__IDL_c_stub.c
	cp output/final_$(LOCK_SERVICE)_s_cstub.c /home/songjiguo/research/composite/src/components/interface/$(LOCK_SERVICE)/stubs/__IDL_s_cstub.c
	cp output/final_$(LOCK_SERVICE)_s_stub.S /home/songjiguo/research/composite/src/components/interface/$(LOCK_SERVICE)/stubs/__IDL_s_stub.S

#######################
## plot SM transition
#######################
plot_all: plot_ramfs plot_lock
	@echo

plot_ramfs:
	@echo
	@echo "Plotting SM graph for.... <<<"$(FS_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(FS_SERVICE).h graph

plot_lock:
	@echo
	@echo "Plotting SM graph for.... <<<"$(LOCK_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(LOCK_SERVICE).h graph


###################
## clean all files
###################
clean:
	rm output/*.c
	rm output/*.S
	rm output/*.svg
