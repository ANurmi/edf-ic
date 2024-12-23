DUT = edf_ic
TB  = $(VERIL_DIR)/$(DUT).cpp

.PHONY: lint
lint:
	verilator \
	--lint-only \
	$(shell bender script flist) \
	--cc $(TB)

.PHONY: verilate
verilate:
	verilator \
	$(shell bender script flist) \
	--cc $(VERIL_DIR)/$(DUT).cpp \
	--build --exe \
	-O3 \
	-sv \
	--timing \
	--trace-fst \
	--trace-structs \
	--trace-params \
	--top-module $(DUT) \
	-Mdir $(BUILD_DIR)/obj_dir

.PHONY: simv
simv:
	$(BUILD_DIR)/obj_dir/V$(DUT)