define BIN_def
	TARGET_$(2)_$(1) = $(OUTPUT_DIR)/$(1)$$($(2)_EXT)
	TARGETS += $$(TARGET_$(2)_$(1))
	C_SRCS_$(2)_$(1) = $$(filter %.c, $$(SRCS_$(2)_$(1)))
	C_OBJS_$(2)_$(1) = $$(patsubst %, $$(OUTPUT_DIR)/$(2)_$(1)/%.o, $$(basename $$(C_SRCS_$(2)_$(1))))
	C_DEPS_$(2)_$(1) = $$(patsubst %, %.d, $$(basename $$(C_OBJS_$(2)_$(1))))
	CPP_SRCS_$(2)_$(1) = $$(filter %.cpp, $$(SRCS_$(2)_$(1)))
	CPP_OBJS_$(2)_$(1) = $$(patsubst %, $$(OUTPUT_DIR)/$(2)_$(1)/%.o, $$(basename $$(CPP_SRCS_$(2)_$(1))))
	CPP_DEPS_$(2)_$(1) = $$(patsubst %, %.d, $$(basename $$(CPP_OBJS_$(2)_$(1))))
	OBJS_$(2)_$(1) = $$(C_OBJS_$(2)_$(1)) $$(CPP_OBJS_$(2)_$(1))
	DEPS_$(2)_$(1) = $$(C_DEPS_$(2)_$(1)) $$(CPP_DEPS_$(2)_$(1))
	OBJS += $$(OBJS_$(2)_$(1))
	DEPS += $$(DEPS_$(2)_$(1))
endef
$(foreach bin,$(EXES),$(eval $(call BIN_def,$(bin),EXE)))
$(foreach bin,$(LIBS),$(eval $(call BIN_def,$(bin),LIB)))

define EXE_def
ifeq ($$(TARGET_OS),PS3)
	TARGET_SELF_$(1) = $$(patsubst %.elf, %.self, $$(TARGET_EXE_$(1)))
	TARGETS += $$(TARGET_SELF_$(1))
endif
	TARGET_LIBS_EXE_test = $$(foreach lib,$$(LIBS_EXE_$(1)),$$(TARGET_LIB_$$(lib)))
endef
$(foreach exe,$(EXES),$(eval $(call EXE_def,$(exe))))

.PHONY: all
all: $(TARGETS)

.PHONY:debug
debug:
	@echo EXES=$(EXES)
	@echo LIBS=$(LIBS)
	@echo DEBUG=$(DEBUG)
	@echo TARGETS=$(TARGETS)
	@echo TARGET_LIBS_EXE_test=$(TARGET_LIBS_EXE_test)
	@echo TARGET_SELF_test=$(TARGET_SELF_test)

define EXE_target
ifeq ($$(TARGET_OS),PS3)
$$(TARGET_SELF_$(1)): $$(TARGET_EXE_$(1))
	$$(CELL_HOST_PATH)/bin/make_fself $$< $$@ 
endif
$$(TARGET_EXE_$(1)): $$(OBJS_EXE_$(1)) $$(TARGET_LIBS_EXE_test)
	@echo Building $$@
	-@$$(MKDIRP) "$$(dir $$(@))"
	$$(LINKER) $$(LINKER_FLAGS) $$(OBJS_EXE_$(1)) $$(TARGET_LIBS_EXE_test) -o $$@
endef
$(foreach exe,$(EXES),$(eval $(call EXE_target,$(exe))))

define LIB_target
$$(TARGET_LIB_$(1)): $$(OBJS_LIB_$(1))
	@echo Building $$@
	-@$$(MKDIRP) "$$(dir $$(@))"
	$$(LIBRARIAN) $$(LIBRARIAN_FLAGS) $$@ $$(OBJS_LIB_$(1))
endef
$(foreach lib,$(LIBS),$(eval $(call LIB_target,$(lib))))

define BIN_target
$$(CPP_OBJS_$(2)_$(1)): $$(OUTPUT_DIR)/$(2)_$(1)/%.o: %.cpp
	-@$$(MKDIRP) "$$(dir $$(@))"
	$(addprefix -I,$(INCLUDE_DIRS))
	$$(COMPILER) -c $$(CXXSTD) $$(COMPILER_FLAGS) $$(addprefix -I,$$(INCS_$(2)_$(1))) -Wp,-MMD,$$(@:.o=.d),-MT,$$@ -o $$@ $$<
	
$$(C_OBJS_$(2)_$(1)): $$(OUTPUT_DIR)/$(2)_$(1)/%.o: %.c
	-@$$(MKDIRP) "$$(dir $$(@))"
	$$(COMPILER) -c $$(CSTD) $$(COMPILER_FLAGS) $$(addprefix -I,$$(INCS_$(2)_$(1))) -Wp,-MMD,$$(@:.o=.d),-MT,$$@ -o $$@ $$<
endef
$(foreach bin,$(EXES),$(eval $(call BIN_target,$(bin),EXE)))
$(foreach bin,$(LIBS),$(eval $(call BIN_target,$(bin),LIB)))

clean:
	-$(RM) $(addprefix ",$(addsuffix ",$(OBJS) $(DEPS) $(TARGETS)))
	-$(RMDIRP) $(addprefix ",$(addsuffix ",$(sort $(foreach item,$(OBJS) $(DEPS) $(TARGETS),$(dir $(item))))))

-include $(DEPS)
