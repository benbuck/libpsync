libpsync
--------

https://sourceforge.net/projects/libpsync/

The libpsync library provides lightweight portable synchronization primitives
(thread, mutex, semaphore, etc.) for C/C++ under a non-restrictive license.

This is an open source library, for more information on the licensing of the
code please read LICENSE.TXT (which is a standard new BSD license).


To Do
-----

For version 1.0:

* Add build files (automake? CMake? Visual SlickEdit? ...)
* Add documentation.
* Review return value handling.

Post 1.0:

* Add more features to existing APIs.
* Add more synchronization primitives (conditionals, events, etc.)
* Add support for more native synchronization APIs (PS2? PSP? Cocoa?)
* Add more robust test suite.


Revision History
----------------

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


Copyright (c) 2007, Benbuck Nason
