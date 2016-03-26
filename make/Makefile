include defs.mk

EXES += test
SRCS_EXE_test += test/main.cpp
LIBS_EXE_test += libpsync
INCS_EXE_test += ../include

LIBS += libpsync
SRCS_LIB_libpsync += \
	src/$(PLATFORM)/libpsync_$(PLATFORM)_mutex.c \
	src/$(PLATFORM)/libpsync_$(PLATFORM)_semaphore.c \
	src/$(PLATFORM)/libpsync_$(PLATFORM)_thread.c
INCS_LIB_libpsync += ../include

vpath %.c ..
vpath %.cpp ..

include targets.mk