BUILD := Build/bin/linux/GONTI-SDK/GONTI-CORE/
OBJ := Build/obj/GONTI-SDK/GONTI-CORE
SRC := GONTI-SDK/GONTI-CORE

ASSEMBLY := GONTI.FILESYSTEM
EXTENSION := .so
COMPILER_FLAGS := -g -Wvarargs -Wall -Werror -fdeclspec -fPIC
INCLUDE_FLAGS := -I"$(SRC)/$(ASSEMBLY)/Source" -IGONTI-SDK/ -I"$(SRC)/GONTI.CORE/Source"
LINKER_FLAGS := -g -shared -lm -L"$(BUILD)" -l:GONTI.CORE.so -Wl,-rpath,'$$ORIGIN'
DEFINES := -D_DEBUG -DGT_EXPORT

BUILD_DIR := $(BUILD)
OBJ_DIR := $(OBJ)/$(ASSEMBLY)
SRC_PREFIX := $(SRC)/$(ASSEMBLY)/Source/

SRC_FILES := $(shell find $(SRC_PREFIX) -name "*.c")
OBJ_FILES := $(patsubst $(SRC_PREFIX)%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

all: scaffold compile link

.PHONY: scaffold
scaffold:
	@echo "Scaffolding folder structure..."
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OBJ_DIR)
	@echo "Done."

.PHONY: link
link: $(OBJ_FILES)
	@echo "Linking $(ASSEMBLY)..."
	@clang $(OBJ_FILES) -o $(BUILD_DIR)/$(ASSEMBLY)$(EXTENSION) $(LINKER_FLAGS)

.PHONY: compile
compile:
	@echo "Compiling..."

.PHONY: clean
clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR)
	@rm -rf $(OBJ_DIR)

.PHONY: debug
debug:
	@echo "============= DEBUG INFO ============="
	@echo "PWD: $(shell pwd)"
	@echo "SRC: $(SRC)"
	@echo "SRC_PREFIX:  $(SRC_PREFIX)"
	@echo "BUILD_DIR:   $(BUILD_DIR)"
	@echo "OBJ_DIR:     $(OBJ_DIR)"
	@echo "SRC_FILES:   $(SRC_FILES)"
	@echo "OBJ_FILES:   $(OBJ_FILES)"
	@echo "======================================"

$(OBJ_DIR)/%.o: $(SRC_PREFIX)%.c
	@echo "Compiling: $<..."
	@mkdir -p $(dir $@)
	@clang $< $(COMPILER_FLAGS) -c -o $@ $(DEFINES) $(INCLUDE_FLAGS)
