#! /bin/bash

rm -rf $(find . -name CMakeCache.txt)
rm -rf $(find . -name CMakeFiles)
rm -rf $(find . -name cmake_install.cmake)
rm -rf $(find . -name CTestTestfile.cmake)
rm -rf $(find . -name Makefile)
