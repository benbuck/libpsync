libpsync
--------

https://sourceforge.net/projects/libpsync/

The libpsync library provides lightweight portable synchronization primitives
(thread, mutex, semaphore) for C/C++ under a non-restrictive license.

This is an open source library, for more information on the licensing of the
code please read LICENSE.TXT (which is a standard new BSD license).


To Do
-----

For version 1.0:

* Add Mach kernel API implementation.
* Add more build files (automake? ...)
* Add documentation.
* Review return value handling.

Post 1.0:

* Add more features to existing APIs.
* Add more synchronization primitives (conditions, events, etc.)
* Add support for more native synchronization APIs (PS2? PSP? Cocoa?)
* Add more robust test suite.


Revision History
----------------

0.7 (2009-04-11)
* Allow copy and assign in C++ mutex and semaphore wrappers.
* Add explicit keyword in C++ wrapper constructors.
* Add platform to output directory name when building with make.
* Add nested threading test.
* Fixed compiler warning in POSIX thread implementation.
* Fixed incorrect POSIX semaphore initialization.
* Fixed crash on double-join in psyncThread class.
* Fixed PS3 tool path handling.
* Increased warning levels for 'make' system.
* Added pthread lib to link when building executables for posix platform.
* Added ignore properties to svn for output files in "make" directory.
* Added ignore properties to svn for "src/ps3" directory.
* Added ignore properties to svn for "vstudio" directory.
* Update copyright notices.

0.6 (2007-11-08)
* Improved POSIX compatibility under Linux and MacOSX.
* Added basic 'CMake' build system.
* Minor improvements to 'make' system.
* Began adding Doxygen support.
* Added another thread test.

0.5 (2007-10-17)
* Added Visual Studio 2005 solution and project files.
* Added basic 'make' build system.
* Changed return type of thread join APIs.
* Began adding a few comments in the header.
* Fixed a few minor compiler warnings exposed by adding -pedantic.
* Improved POSIX compatibility by defining _XOPEN_SOURCE before including pthread.h.

0.4 (2007-09-29)
* Fixed warnings related to casting thread user data pointer and return values.
* Improved thread creation API.
* Rewrote thread priority and stack size code.
* Added additional constructor to mutex classes.

0.3 (2007-09-24)
* Fixed bug in threading test.
* Fixed POSIX mutex to allow recursion.
* Removed redundant null pointer checks in C++ wrappers.
* Added psyncMutexAuto class.

0.2 (2007-09-23)
* Added C++ wrappers for threads, mutexes, and semaphores.

0.1 (2007-09-21)
* Contains very basic APIs for creating threads, mutexes, and semaphores.
* Has implementations for Win32, POSIX, and PS3 (PS3 version available only
  to registered PS3 developers).


Copyright (c) 2007-2009, Benbuck Nason
