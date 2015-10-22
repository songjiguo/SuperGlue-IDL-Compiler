CC=gcc
PYTHON=python
PARSER=c3_parser.py
FAKE_HEADER=fake_header.h
TMP_OUTPUT=tmp_result

LOCK_SERVICE=lock
FS_SERVICE=fs
SCHED_SERVICE=sched
MM_SERVICE=mm
MBOX_SERVICE=mbox
EVENT_SERVICE=event
TE_SERVICE=te

all: parse_all compile_all plot_all

parse_all: fs_parse lock_parse
	@echo

compile_all: fs_compile lock_compile
	@echo

plot_all: fs_plot lock_plot
	@echo

fs_parse:
	@echo $(FINAL_CODE)
	@echo "IDL process starting.... <<<"$(FS_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(FS_SERVICE).h

fs_plot:
	@echo
	@echo "Plotting SM graph for.... <<<"$(FS_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(FS_SERVICE).h graph

fs_compile:
	@echo
	@echo "Compiling starting.... <<<"$(FS_SERVICE)">>>"
	$(CC)  -Werror -include $(FAKE_HEADER) -o output/$(TMP_OUTPUT) output/$(LOCK_SERVICE)_c_stub.c
	$(CC)  -Werror -include $(FAKE_HEADER) -o output/$(TMP_OUTPUT) output/$(LOCK_SERVICE)_s_cstub.c
	rm output/$(TMP_OUTPUT)

lock_parse:
	@echo
	@echo "IDL process starting.... <<<"$(LOCK_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(LOCK_SERVICE).h

lock_plot:
	@echo
	@echo "Plotting SM graph for.... <<<"$(LOCK_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(LOCK_SERVICE).h graph

lock_compile:
	@echo
	@echo "Compiling starting.... <<<"$(LOCK_SERVICE)">>>"
	$(CC)  -Werror -include $(FAKE_HEADER) -o output/$(TMP_OUTPUT) output/$(LOCK_SERVICE)_c_stub.c
	$(CC)  -Werror -include $(FAKE_HEADER) -o output/$(TMP_OUTPUT) output/$(LOCK_SERVICE)_s_cstub.c
	rm output/$(TMP_OUTPUT)



final: parse_all_final

parse_all_final: fs_parse_final lock_parse_final
	@echo

fs_parse_final:
	@echo $(FINAL_CODE)
	@echo "IDL process starting.... <<<"$(FS_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(FS_SERVICE).h final

lock_parse_final:
	@echo
	@echo "IDL process starting.... <<<"$(LOCK_SERVICE)">>>"
	$(PYTHON) $(PARSER) input/cidl_$(LOCK_SERVICE).h final
