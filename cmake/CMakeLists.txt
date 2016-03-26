PROJECT (libpsync)

INCLUDE_DIRECTORIES (../include)

IF (NOT DEFINED PLATFORM)
	IF (UNIX)
		SET (PLATFORM posix)
	ELSEIF (WIN32 OR MINGW)
		SET (PLATFORM win32)
	ENDIF (UNIX)
ENDIF (NOT DEFINED PLATFORM)

ADD_LIBRARY (
	psync
	../src/${PLATFORM}/libpsync_${PLATFORM}_mutex.c
	../src/${PLATFORM}/libpsync_${PLATFORM}_semaphore.c
	../src/${PLATFORM}/libpsync_${PLATFORM}_thread.c
)

ADD_EXECUTABLE (test ../test/main.cpp)
TARGET_LINK_LIBRARIES (test psync)