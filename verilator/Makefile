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
	--top-module $(DUT) \
	-Mdir $(BUILD_DIR)

.PHONY: simv
simv:
	$(BUILD_DIR)/V$(DUT)