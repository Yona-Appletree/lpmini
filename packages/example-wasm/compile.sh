#!/bin/bash

# EXPORST_NAME means we can import from fib.js as follows:
# import { fib } from './fib.js'
# and fib will contain the module with webassembly

# EXPORTED_FUNCTIONS is the list of functions
# exported from C. Make sure you put them into extern "C" {} block in your c++ code. 
# Then simply prefix the functions you are going to export with an underscore, and add it to the list below

# Uncomment -s "FILESYSTEM=0" if you don't need to use fs

echo
echo
echo "BUILDING LPLIB TO JS...."

EXPORTED_FUNCTIONS=$(egrep -o 'lpapi_\w+' src/lpapi.h | tr '\n' ' ' | sed 's/ /","_/g' | sed 's/^/["_/' | sed 's/,"_$/]/')

echo "FUNCTIONS: $EXPORTED_FUNCTIONS"


# if you create more dependencies of fib.cc, simply add them to the end of the below command, like: -o ./fib.js fib.cc add.cc anotherdep.cc and_so_on.cc
em++ -O3 -s WASM=1 \
         -s EXPORTED_RUNTIME_METHODS='ccall,cwrap' \
         -s ALLOW_MEMORY_GROWTH=1 \
         -s MODULARIZE=1 \
         -s EXPORT_NAME=lp_api \
         -s EXPORTED_FUNCTIONS="$EXPORTED_FUNCTIONS" \
         -s ENVIRONMENT=web \
         -o ./build/lp_api.js \
         -s "SINGLE_FILE" \
         $(find src -iname '*.c' -or -iname '*.cc' -or -iname '*.cpp')
    # -s "FILESYSTEM=0"

echo "DONE...."
echo
echo
