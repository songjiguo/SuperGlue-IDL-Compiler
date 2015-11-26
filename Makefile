CC=gcc
PYTHON=python
PARSER=c3_parser.py
TMP_OUTPUT=tmp_result
FAKE_HEADER=fake_header.h

# service name
FS_SERVICE=ramfs
LOCK_SERVICE=lock
EVT_SERVICE=evt
SCHED_SERVICE=sched
MM_SERVICE=mem_mgr
MBOX_SERVICE=mbox
PTE_SERVICE=periodic_wake
SCHED_SERVICE=sched

##############
## everything
##############

all: parse_all compile_all gen_all cp_all plot_all

all_ramfs: parse_ramfs compile_ramfs gen_ramfs cp_ramfs plot_ramfs

all_lock: parse_lock compile_lock gen_lock cp_lock plot_lock

all_evt: parse_evt compile_evt gen_evt cp_evt plot_evt

all_sched: parse_sched compile_sched gen_sched cp_sched plot_sched

all_mem_mgr: parse_mem_mgr compile_mem_mgr gen_mem_mgr cp_mem_mgr plot_mem_mgr

all_periodic_wake: parse_periodic_wake compile_periodic_wake gen_periodic_wake cp_periodic_wake plot_periodic_wake


# everything with the benchmark
all_bench: parse_all compile_all bench_all cp_all

all_ramfs_bench: parse_ramfs compile_ramfs bench_ramfs cp_ramfs

all_lock_bench: parse_lock compile_lock bench_lock cp_lock

all_evt_bench: parse_evt compile_evt bench_evt cp_evt

all_sched_bench: parse_sched compile_sched bench_sched cp_sched

all_mem_mgr_bench: parse_mem_mgr compile_mem_mgr bench_mem_mgr cp_mem_mgr

all_periodic_wake_bench: parse_periodic_wake compile_periodic_wake bench_periodic_wake cp_periodic_wake

# for quick testing
test_all: parse_all compile_all

##############
## parsing
##############
parse_all: parse_ramfs parse_lock parse_evt parse_sched parse_mem_mgr parse_periodic_wake
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

parse_mem_mgr:
	@echo
	@echo "IDL process starting.... <<<"$(MM_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(MM_SERVICE).h

parse_periodic_wake:
	@echo
	@echo "IDL process starting.... <<<"$(PTE_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(PTE_SERVICE).h

##############
## compiling
##############
compile_all: compile_ramfs compile_lock compile_evt compile_sched compile_mem_mgr compile_periodic_wake
	@echo

compile_ramfs:
	@echo
	@echo "Compiling starting.... <<<"$(FS_SERVICE)">>>"
	$(CC)  -Werror  -o output/$(TMP_OUTPUT) output/$(FS_SERVICE)_c_stub.c
	$(CC)  -Werror  -o output/$(TMP_OUTPUT) output/$(FS_SERVICE)_s_cstub.c
	rm output/$(TMP_OUTPUT)

compile_lock:
	@echo
	@echo "Compiling starting.... <<<"$(LOCK_SERVICE)">>>"
	$(CC)  -Werror  -o output/$(TMP_OUTPUT) output/$(LOCK_SERVICE)_c_stub.c
	$(CC)  -Werror  -o output/$(TMP_OUTPUT) output/$(LOCK_SERVICE)_s_cstub.c
	rm output/$(TMP_OUTPUT)

compile_evt:
	@echo
	@echo "Compiling starting.... <<<"$(EVT_SERVICE)">>>"
	$(CC)  -Werror  -o output/$(TMP_OUTPUT) output/$(EVT_SERVICE)_c_stub.c
	$(CC)  -Werror  -o output/$(TMP_OUTPUT) output/$(EVT_SERVICE)_s_cstub.c
	rm output/$(TMP_OUTPUT)

compile_sched:
	@echo
	@echo "Compiling starting.... <<<"$(SCHED_SERVICE)">>>"
	$(CC)  -Werror  -o output/$(TMP_OUTPUT) output/$(SCHED_SERVICE)_c_stub.c
	$(CC)  -Werror  -o output/$(TMP_OUTPUT) output/$(SCHED_SERVICE)_s_cstub.c
	rm output/$(TMP_OUTPUT)

compile_mem_mgr:
	@echo
	@echo "Compiling starting.... <<<"$(MM_SERVICE)">>>"
	$(CC)  -Werror --include $(FAKE_HEADER) -o output/$(TMP_OUTPUT) output/$(MM_SERVICE)_c_stub.c
	$(CC)  -Werror --include $(FAKE_HEADER) -o output/$(TMP_OUTPUT) output/$(MM_SERVICE)_s_cstub.c
	rm output/$(TMP_OUTPUT)

compile_periodic_wake:
	@echo
	@echo "Compiling starting.... <<<"$(PTE_SERVICE)">>>"
	$(CC)  -Werror  -o output/$(TMP_OUTPUT) output/$(PTE_SERVICE)_c_stub.c
	$(CC)  -Werror  -o output/$(TMP_OUTPUT) output/$(PTE_SERVICE)_s_cstub.c
	rm output/$(TMP_OUTPUT)

########################################################
## generate the acutal interface code w/ ubenchmark
########################################################
bench_all: bench_ramfs bench_lock bench_evt bench_sched bench_mem_mgr bench_periodic_wake
	@echo

bench_ramfs:
	@echo $(FINAL_CODE)
	@echo "IDL process starting.... <<<"$(FS_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(FS_SERVICE).h bench

bench_lock:
	@echo
	@echo "IDL process starting.... <<<"$(LOCK_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(LOCK_SERVICE).h bench

bench_evt:
	@echo
	@echo "IDL process starting.... <<<"$(EVT_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(EVT_SERVICE).h bench

bench_sched:
	@echo
	@echo "IDL process starting.... <<<"$(SCHED_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(SCHED_SERVICE).h bench

bench_mem_mgr:
	@echo
	@echo "IDL process starting.... <<<"$(MM_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(MM_SERVICE).h bench

bench_periodic_wake:
	@echo
	@echo "IDL process starting.... <<<"$(PTE_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(PTE_SERVICE).h bench

########################################################
## generate the acutal interface code for Composite
########################################################
gen_all: gen_ramfs gen_lock gen_evt gen_sched gen_mem_mgr gen_periodic_wake
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

gen_sched:
	@echo
	@echo "IDL process starting.... <<<"$(SCHED_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(SCHED_SERVICE).h final

gen_mem_mgr:
	@echo
	@echo "IDL process starting.... <<<"$(MM_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(MM_SERVICE).h final

gen_periodic_wake:
	@echo
	@echo "IDL process starting.... <<<"$(PTE_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(PTE_SERVICE).h final

######################################################################
## copy the generated files to the Compoiste interface(symbolink further)
######################################################################
cp_all: cp_ramfs cp_lock cp_evt cp_sched cp_mem_mgr cp_periodic_wake

cp_ramfs:
	cp output/final_$(FS_SERVICE)_c_stub.c /home/songjiguo/research/composite/src/components/interface/rtorrent/__stubs_rec_ramfs/__IDL_c_stub.c
	cp output/final_$(FS_SERVICE)_s_cstub.c /home/songjiguo/research/composite/src/components/interface/rtorrent/__stubs_rec_ramfs/__IDL_s_cstub.c
	cp output/final_$(FS_SERVICE)_s_stub.S /home/songjiguo/research/composite/src/components/interface/rtorrent/__stubs_rec_ramfs/__IDL_s_stub.S
	cp output/final_$(FS_SERVICE)_idlc3.h /home/songjiguo/research/composite/src/components/interface/rtorrent/__torrent_h_ramfs

cp_lock:
	cp output/final_$(LOCK_SERVICE)_c_stub.c /home/songjiguo/research/composite/src/components/interface/$(LOCK_SERVICE)/__stubs_rec/__IDL_c_stub.c
	cp output/final_$(LOCK_SERVICE)_s_cstub.c /home/songjiguo/research/composite/src/components/interface/$(LOCK_SERVICE)/__stubs_rec/__IDL_s_cstub.c
	cp output/final_$(LOCK_SERVICE)_s_stub.S /home/songjiguo/research/composite/src/components/interface/$(LOCK_SERVICE)/__stubs_rec/__IDL_s_stub.S
	cp output/final_$(LOCK_SERVICE)_idlc3.h /home/songjiguo/research/composite/src/components/interface/$(LOCK_SERVICE)/__lock_h_rec

cp_evt:
	cp output/final_$(EVT_SERVICE)_c_stub.c /home/songjiguo/research/composite/src/components/interface/$(EVT_SERVICE)/__stubs_rec/__IDL_c_stub.c
	cp output/final_$(EVT_SERVICE)_s_cstub.c /home/songjiguo/research/composite/src/components/interface/$(EVT_SERVICE)/__stubs_rec/__IDL_s_cstub.c
	cp output/final_$(EVT_SERVICE)_s_stub.S /home/songjiguo/research/composite/src/components/interface/$(EVT_SERVICE)/__stubs_rec/__IDL_s_stub.S
	cp output/final_$(EVT_SERVICE)_idlc3.h /home/songjiguo/research/composite/src/components/interface/$(EVT_SERVICE)/__evt_h_rec

cp_sched:
	cp output/final_$(SCHED_SERVICE)_c_stub.c /home/songjiguo/research/composite/src/components/interface/$(SCHED_SERVICE)/__stubs_rec/__IDL_c_stub.c
	cp output/final_$(SCHED_SERVICE)_s_cstub.c /home/songjiguo/research/composite/src/components/interface/$(SCHED_SERVICE)/__stubs_rec/__IDL_s_cstub.c
	cp output/final_$(SCHED_SERVICE)_s_stub.S /home/songjiguo/research/composite/src/components/interface/$(SCHED_SERVICE)/__stubs_rec/__IDL_s_stub.S
	cp output/final_$(SCHED_SERVICE)_idlc3.h /home/songjiguo/research/composite/src/components/interface/$(SCHED_SERVICE)/__sched_h_rec

cp_mem_mgr:
	cp output/final_$(MM_SERVICE)_c_stub.c /home/songjiguo/research/composite/src/components/interface/$(MM_SERVICE)/__stubs_rec/__IDL_c_stub.c
	cp output/final_$(MM_SERVICE)_s_cstub.c /home/songjiguo/research/composite/src/components/interface/$(MM_SERVICE)/__stubs_rec/__IDL_s_cstub.c
	cp output/final_$(MM_SERVICE)_s_stub.S /home/songjiguo/research/composite/src/components/interface/$(MM_SERVICE)/__stubs_rec/__IDL_s_stub.S
	cp output/final_$(MM_SERVICE)_idlc3.h /home/songjiguo/research/composite/src/components/interface/$(MM_SERVICE)/__mem_mgr_h_rec

cp_periodic_wake:
	cp output/final_$(PTE_SERVICE)_c_stub.c /home/songjiguo/research/composite/src/components/interface/$(PTE_SERVICE)/__stubs_rec/__IDL_c_stub.c
	cp output/final_$(PTE_SERVICE)_s_cstub.c /home/songjiguo/research/composite/src/components/interface/$(PTE_SERVICE)/__stubs_rec/__IDL_s_cstub.c
	cp output/final_$(PTE_SERVICE)_s_stub.S /home/songjiguo/research/composite/src/components/interface/$(PTE_SERVICE)/__stubs_rec/__IDL_s_stub.S
	cp output/final_$(PTE_SERVICE)_idlc3.h /home/songjiguo/research/composite/src/components/interface/$(PTE_SERVICE)/__periodic_wake_h_rec

#######################
## plot SM transition
#######################
plot_all: plot_ramfs plot_lock plot_evt plot_sched plot_mem_mgr plot_periodic_wake
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

plot_sched:
	@echo
	@echo "Plotting SM graph for.... <<<"$(SCHED_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(SCHED_SERVICE).h graph

plot_mem_mgr:
	@echo
	@echo "Plotting SM graph for.... <<<"$(MM_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(MM_SERVICE).h graph

plot_periodic_wake:
	@echo
	@echo "Plotting SM graph for.... <<<"$(PTE_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(PTE_SERVICE).h graph

###################
## clean all files
###################
clean:
	-rm output/*.c
	-rm output/*.S
	-rm output/*.svg
