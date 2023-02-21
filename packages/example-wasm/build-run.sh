#!/usr/bin/env sh

clang++ -std=c++20 -o build/lpcli $(find . -iname '*.c' -or -iname '*.cc' -or -iname '*.cpp') && build/lpcli
