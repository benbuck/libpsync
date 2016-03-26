ifeq ($(shell uname), Linux)
	HOST_OS ?= LINUX
else
	HOST_OS ?= WIN32
endif
	
TARGET_OS ?= $(HOST_OS)
ifeq ($(TARGET_OS), LINUX)
	EXE_EXT ?=
	LIB_EXT ?= .a
endif
ifeq ($(TARGET_OS), WIN32)
	EXE_EXT ?= .exe
	LIB_EXT ?= .lib
endif
ifeq ($(TARGET_OS), PS3)
	ifeq ($(HOST_OS), LINUX)
		CELL_HOST_PATH ?= $(CELL_SDK)/host-linux
	else
		CELL_HOST_PATH ?= $(CELL_SDK)/host-win32
	endif
	EXE_EXT ?= .elf
	LIB_EXT ?= .a
	CSTD ?= -std=c99
	COMPILER ?= $(CELL_HOST_PATH)/ppu/bin/ppu-lv2-gcc
	LIBRARIAN ?= $(CELL_HOST_PATH)/ppu/bin/ppu-lv2-ar
	LINKER ?= $(CELL_HOST_PATH)/ppu/bin/ppu-lv2-g++
endif

COMPILER ?= gcc
CSTD ?= -std=c89
CXXSTD ?= -std=c++98
COMPILER_FLAGS ?= \
	-pedantic \
	-W \
	-Wall \
	-Wcomment \
	-Wchar-subscripts \
	-Wdeprecated-declarations \
	-Wendif-labels \
	-Wformat=2 \
	-Wformat-extra-args \
	-Wimplicit \
	-Wimport \
	-Winline \
	-Wmissing-braces \
	-Wparentheses \
	-Wpointer-arith \
	-Wredundant-decls \
	-Wreturn-type \
	-Wshadow \
	-Wsign-compare \
	-Wswitch \
	-Wunknown-pragmas \
	-Wunused \
	-Wwrite-strings \
	-Wswitch-default \
	-Wswitch-enum \
	-Wfloat-equal \
	-Wpacked
LIBRARIAN ?= ar
LIBRARIAN_FLAGS ?= -rcs
LINKER ?= g++
LD_LIBS += c
LINKER_FLAGS ?= \
	$(addprefix -l,$(LD_LIBS))

MODE ?= DEBUG
ifeq ($(MODE), DEBUG)
	COMPILER_FLAGS += -g -O0 -DDEBUG
else
	ifeq ($(MODE), RELEASE)
		COMPILER_FLAGS += -Wuninitialized -O3 -DNDEBUG
	else
		ERROR := $(error unknown MODE=$(MODE))
	endif
endif

ifeq ($(PLATFORM),)
	ifeq ($(TARGET_OS), PS3)
		PLATFORM = ps3
	else
		ifeq ($(TARGET_OS), WIN32)
			PLATFORM = win32
		else
			PLATFORM = posix
		endif
	endif
endif

OUTPUT_DIR ?= $(TARGET_OS)_$(MODE)

ifeq ($(TERM),)
	RM ?= del
	RMDIRP ?= rmdir
	MKDIRP ?= mkdir
else
	RM ?= rm -f
	RMDIRP ?= rmdir --ignore-fail-on-non-empty -p
	MKDIRP ?= mkdir -p
endif
