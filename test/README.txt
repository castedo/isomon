This directory contains unit tests.

The test/c subdirectory contains a simple Makefile to compile unit tests for
the C functions using a C99 compiler with no C++.

The test subdirectory contains C++ unit tests using the Boost unit testing
framework and CMake. You can do an "out-of-source" build in any directory
but you probably want to do the following from this directory:
  mkdir build
  cd build
  cmake ..
  make

