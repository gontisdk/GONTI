BUILD := Build/bin/linux/GONTI-SDK/GONTI-RENDER
OBJ := Build/obj/GONTI-SDK/GONTI-RENDER
SRC := GONTI-SDK/GONTI-RENDER

GONTI_CORE_I := GONTI-SDK/GONTI-CORE
GONTI_CORE_L := Build/bin/linux/$(GONTI_CORE_I)

GONTI_RENDER_VK_I := GONTI-SDK/GONTI-RENDER/GONTI-RENDER-VK
GONTI_RENDER_VK_L := Build/bin/linux/$(GONTI_RENDER_VK_I)

ASSEMBLY := GONTI.RENDER
EXTENSION := .so
COMPILER_FLAGS := -g -Wvarargs -Wall -Werror -fdeclspec -fPIC
INCLUDE_FLAGS := -I"$(VULKAN_SDK)/include" -IGONTI-SDK/ -I"$(SRC)/$(ASSEMBLY)/Source" -I$(GONTI_CORE_I)/GONTI.CORE/Source -I$(GONTI_RENDER_VK_I)/GONTI.RENDER.CORE.VK/Source -I$(GONTI_RENDER_VK_I)/GONTI.RENDER.SHADERS.VK/Source -I$(GONTI_CORE_I)/GONTI.MATH/Source
LINKER_FLAGS := -g -shared -lxcb -L"$(VULKAN_SDK)/lib" -lvulkan -L$(GONTI_CORE_L) -l:GONTI.CORE.so -l:GONTI.MATH.so -L$(GONTI_RENDER_VK_L) -l:GONTI.RENDER.CORE.VK.so -l:GONTI.RENDER.VK.so -l:GONTI.RENDER.SHADERS.VK.so -Wl,-rpath,'$$ORIGIN'
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
	@echo "SRC_PREFIX:  $(SRC_PREFIX)"
	@echo "BUILD_DIR:   $(BUILD_DIR)"
	@echo "OBJ_DIR:     $(OBJ_DIR)"
	@echo "SRC_FILES:   $(SRC_FILES)"
	@echo "C_FILES:     $(C_FILES)"
	@echo "OBJ_FILES:   $(OBJ_FILES)"
	@echo "DIRECTORIES: $(DIRECTORIES)"
	@echo "======================================"

$(OBJ_DIR)/%.o: $(SRC_PREFIX)%.c
	@echo "Compiling: $<..."
	@mkdir -p $(dir $@)
	@clang $< $(COMPILER_FLAGS) -c -o $@ $(DEFINES) $(INCLUDE_FLAGS)
