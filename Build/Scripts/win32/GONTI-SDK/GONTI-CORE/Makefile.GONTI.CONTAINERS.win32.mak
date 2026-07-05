BUILD := Build/bin/win32/GONTI-SDK/GONTI-CORE
OBJ := Build/obj/GONTI-SDK/GONTI-CORE
SRC := GONTI-SDK/GONTI-CORE

ASSEMBLY := GONTI.CONTAINERS
EXTENSION := .dll
COMPILER_FLAGS := -g -Wvarargs -Wall -Werror
INCLUDE_FLAGS := -I"$(SRC)/$(ASSEMBLY)/Source" -IGONTI-SDK/ -I"$(SRC)/GONTI.CORE/Source"
LINKER_FLAGS := -g -shared -luser32 -lGONTI.CORE -L$(BUILD)
DEFINES := -D_DEBUG -DGT_EXPORT -D_CTR_SECURE_NO_WARNINGS

DIR := $(subst /,\,$(WORKSPACE))
BUILD_DIR := $(BUILD)
OBJ_DIR := $(OBJ)/$(ASSEMBLY)

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

SRC_PREFIX := $(SRC)/$(ASSEMBLY)
SCR_FILES := $(call rwildcard,$(SRC_PREFIX)/Source/, *.c)
C_FILES := $(filter %.c, $(SCR_FILES))
DIRECTORIES := $(sort $(dir $(C_FILES)))
OBJ_FILES := $(patsubst $(SRC_PREFIX)/%.c, $(OBJ_DIR)/%.o, $(C_FILES))

all: scaffold compile link

.PHONY: scaffold
scaffold:
	@echo Scaffolding folder structure...
	-@for %%D in ($(subst $(SRC_PREFIX)/,,$(DIRECTORIES))) do @(setlocal enableextensions enabledelayedexpansion && mkdir "$(OBJ_DIR)/%%D" 2>NUL || cd.)
	-@setlocal enableextensions enabledelayedexpansion && mkdir "$(BUILD_DIR)" 2>NUL || cd.
	@echo Done.

.PHONY: link
link: scaffold $(OBJ_FILES)
	@echo Linking $(ASSEMBLY)...
	@clang $(COMPILER_FLAGS) $(OBJ_FILES) -o $(BUILD_DIR)/$(ASSEMBLY)$(EXTENSION) $(LINKER_FLAGS)

.PHONY: compile
compile:
	@echo Compiling...

.PHONY: clean
clean:
	if exist $(BUILD_DIR)\$(ASSEMBLY)$(EXTENSION) del $(BUILD_DIR)\$(ASSEMBLY)$(EXTENSION)
	rmdir /s /q $(OBJ_DIR)\$(ASSEMBLY)

.PHONY: debug
debug:
	@echo ============= DEBUG INFO =============
	@echo SRC_PREFIX:  $(SRC_PREFIX)
	@echo BUILD_DIR: $(BUILD_DIR)
	@echo OBJ_DIR: $(OBJ_DIR)
	@echo SCR_FILES: $(SCR_FILES)
	@echo C_FILES: $(C_FILES)
	@echo OBJ_FILES: $(OBJ_FILES)
	@echo DIRECTORIES: $(DIRECTORIES)
	@echo ====================================

$(OBJ_DIR)/%.o: $(SRC_PREFIX)/%.c
	@echo $<...
	@mkdir "$(dir $@)" 2>NUL || cd.
	@clang $< $(COMPILER_FLAGS) -c -o $@ $(DEFINES) $(INCLUDE_FLAGS)