BUILD_DIR := build-deb
EXAMPLES_BUILD_DIR := $(BUILD_DIR)/examples

CMAKE := cmake
CMAKE_FLAGS := \
	       -S . -B $(BUILD_DIR) \
	       -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain-D1.cmake \
	       -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

FEL := ./tools/xfel/xfel
# Should be the same that PZ_MEM_START (see toolchain.cmake)
FEL_ADDR = 0x80000000

RM := rm -f
MAKEFLAGS += --no-print-directory

.PHONY: config
config:
	cmake $(CMAKE_FLAGS)

.PHONY: bins
bins:
	cmake --build $(BUILD_DIR)

.PHONY: fel-exec
# Run with sudo
fel-exec:
	$(FEL) version
	$(FEL) ddr d1
	$(FEL) write $(FEL_ADDR) $(EXAMPLES_BUILD_DIR)/$(NAME).bin
	$(FEL) exec $(FEL_ADDR)

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

.PHONY: re
re:
	$(MAKE) clean
	$(MAKE) bins
