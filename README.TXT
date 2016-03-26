libpsync
--------

https://sourceforge.net/projects/libpsync/

The libpsync library provides lightweight portable synchronization primitives
(thread, mutex, semaphore, etc.) for C/C++ under a non-restrictive license.

This is an open source library, for more information on the licensing of the
code please read LICENSE.TXT (which is a standard new BSD license).


To Do
-----

* Fix warnings related to casting thread user data pointer.
* Add more features to existing APIs.
* Add build files (make? automake? CMake? Visual Studio? Visual SlickEdit? ...)
* Add more synchronization primitives (conditionals, events, etc.)
* Add support for more native synchronization APIs (PS2? PSP? Cocoa?)
* Add more robust test suite.
* Add documentation.


Revision History
----------------

0.1 (2007-09-21)

* Contains very basic APIs for creating threads, mutexes, and semaphores.
* Has implementations for Win32, POSIX, and PS3 (PS3 version available only
  to registered PS3 developers).


Copyright (c) 2007, Benbuck Nason
