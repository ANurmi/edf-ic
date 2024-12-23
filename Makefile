export TOP_DIR   = $(realpath $(CURDIR))
export VERIL_DIR = $(realpath $(CURDIR))/verilator
export BUILD_DIR = $(realpath $(CURDIR))/build

.PHONY: init
init:
	@mkdir -p $(BUILD_DIR)

.PHONY: lint
lint:
	$(MAKE) -C verilator lint
	@echo "Verilator lint [OK]"

.PHONY: verilate
verilate: clean init
	$(MAKE) -C verilator verilate

.PHONY: simv
simv:
	$(MAKE) -C verilator simv


.PHONY: clean
clean:
	rm -fr $(BUILD_DIR)