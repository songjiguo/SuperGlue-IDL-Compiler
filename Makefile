CC=gcc
PYTHON=python
PARSER=c3_parser.py
FAKE_HEADER=fake_header.h
TMP_OUTPUT=tmp_result

# service name
FS_SERVICE=ramfs
LOCK_SERVICE=lock
EVT_SERVICE=evt
SCHED_SERVICE=sched
MM_SERVICE=mm
MBOX_SERVICE=mbox
TE_SERVICE=te
SCHED_SERVICE=sched

##############
## everything
##############

all: parse_all compile_all gen_all cp_all

all_ramfs: parse_ramfs compile_ramfs gen_ramfs cp_ramfs plot_ramfs

all_lock: parse_lock compile_lock gen_lock cp_lock plot_lock

all_evt: parse_evt compile_evt gen_evt cp_evt plot_evt


# everything with the benchmark
all_bench: parse_all compile_all gen_all_bench cp_all

all_ramfs_bench: parse_ramfs compile_ramfs gen_ramfs_bench cp_ramfs plot_ramfs

all_lock_bench: parse_lock compile_lock gen_lock_bench cp_lock plot_lock

all_evt_bench: parse_evt compile_evt gen_evt_bench cp_evt plot_evt

##############
## parsing
##############
parse_all: parse_ramfs parse_lock parse_evt
	@echo

parse_ramfs:
	@echo $(FINAL_CODE)
	@echo "IDL process starting.... <<<"$(FS_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(FS_SERVICE).h

parse_lock:
	@echo
	@echo "IDL process starting.... <<<"$(LOCK_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(LOCK_SERVICE).h

parse_evt:
	@echo
	@echo "IDL process starting.... <<<"$(EVT_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(EVT_SERVICE).h

parse_sched:
	@echo
	@echo "IDL process starting.... <<<"$(SCHED_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(SCHED_SERVICE).h

##############
## compiling
##############
compile_all: compile_ramfs compile_lock compile_evt compile_sched
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

compile_evt:
	@echo
	@echo "Compiling starting.... <<<"$(EVT_SERVICE)">>>"
	$(CC)  -Werror -include $(FAKE_HEADER) -o output/$(TMP_OUTPUT) output/$(EVT_SERVICE)_c_stub.c
	$(CC)  -Werror -include $(FAKE_HEADER) -o output/$(TMP_OUTPUT) output/$(EVT_SERVICE)_s_cstub.c
	rm output/$(TMP_OUTPUT)

compile_sched:
	@echo
	@echo "Compiling starting.... <<<"$(SCHED_SERVICE)">>>"
	$(CC)  -Werror -include $(FAKE_HEADER) -o output/$(TMP_OUTPUT) output/$(SCHED_SERVICE)_c_stub.c
	$(CC)  -Werror -include $(FAKE_HEADER) -o output/$(TMP_OUTPUT) output/$(SCHED_SERVICE)_s_cstub.c
	rm output/$(TMP_OUTPUT)

########################################################
## generate the acutal interface code w/ ubenchmark
########################################################
gen_all_bench: gen_ramfs_bench gen_lock_bench gen_evt_bench
	@echo

gen_ramfs_bench:
	@echo $(FINAL_CODE)
	@echo "IDL process starting.... <<<"$(FS_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(FS_SERVICE).h bench

gen_lock_bench:
	@echo
	@echo "IDL process starting.... <<<"$(LOCK_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(LOCK_SERVICE).h bench

gen_evt_bench:
	@echo
	@echo "IDL process starting.... <<<"$(EVT_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(EVT_SERVICE).h bench

########################################################
## generate the acutal interface code for Composite
########################################################
gen_all: gen_ramfs gen_lock gen_evt
	@echo

gen_ramfs:
	@echo $(FINAL_CODE)
	@echo "IDL process starting.... <<<"$(FS_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(FS_SERVICE).h final

gen_lock:
	@echo
	@echo "IDL process starting.... <<<"$(LOCK_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(LOCK_SERVICE).h final

gen_evt:
	@echo
	@echo "IDL process starting.... <<<"$(EVT_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(EVT_SERVICE).h final

######################################################################
## copy the generated files to the Compoiste interface(symbolink further)
######################################################################
cp_all: cp_ramfs cp_lock cp_evt

cp_ramfs:
	cp output/final_$(FS_SERVICE)_c_stub.c /home/songjiguo/research/composite/src/components/interface/rtorrent/__stubs_rec_ramfs/__IDL_c_stub.c
	cp output/final_$(FS_SERVICE)_s_cstub.c /home/songjiguo/research/composite/src/components/interface/rtorrent/__stubs_rec_ramfs/__IDL_s_cstub.c
	cp output/final_$(FS_SERVICE)_s_stub.S /home/songjiguo/research/composite/src/components/interface/rtorrent/__stubs_rec_ramfs/__IDL_s_stub.S

cp_lock:
	cp output/final_$(LOCK_SERVICE)_c_stub.c /home/songjiguo/research/composite/src/components/interface/$(LOCK_SERVICE)/__stubs_rec/__IDL_c_stub.c
	cp output/final_$(LOCK_SERVICE)_s_cstub.c /home/songjiguo/research/composite/src/components/interface/$(LOCK_SERVICE)/__stubs_rec/__IDL_s_cstub.c
	cp output/final_$(LOCK_SERVICE)_s_stub.S /home/songjiguo/research/composite/src/components/interface/$(LOCK_SERVICE)/__stubs_rec/__IDL_s_stub.S

cp_evt:
	cp output/final_$(EVT_SERVICE)_c_stub.c /home/songjiguo/research/composite/src/components/interface/$(EVT_SERVICE)/__stubs_rec/__IDL_c_stub.c
	cp output/final_$(EVT_SERVICE)_s_cstub.c /home/songjiguo/research/composite/src/components/interface/$(EVT_SERVICE)/__stubs_rec/__IDL_s_cstub.c
	cp output/final_$(EVT_SERVICE)_s_stub.S /home/songjiguo/research/composite/src/components/interface/$(EVT_SERVICE)/__stubs_rec/__IDL_s_stub.S

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

plot_evt:
	@echo
	@echo "Plotting SM graph for.... <<<"$(EVT_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(EVT_SERVICE).h graph

###################
## clean all files
###################
clean:
	rm output/*.c
	rm output/*.S
	rm output/*.svg
