#!/bin/bash

cd "$(dirname "$0")" || exit 1

mkdir -p build || exit 1

# Changes to this must be reflected in webpack.config.common.js
EXPORT_NAME=liblp

# Functions starting with this will be exported
PUBLIC_API_NAME=liblp

PUBLIC_API_HEADER="src/$PUBLIC_API_NAME.h"

if [ ! -f "$PUBLIC_API_HEADER" ]; then
    echo "ERROR: $PUBLIC_API_HEADER does not exist"
    exit 1
fi

echo "Building $EXPORT_NAME.js..."

EXPORTED_FUNCTIONS=$(egrep -o "$PUBLIC_API_NAME""_\w+" $PUBLIC_API_HEADER | tr '\n' ' ' | sed 's/ /","_/g' | sed 's/^/["_/' | sed 's/,"_$/]/')

echo "FUNCTIONS: $EXPORTED_FUNCTIONS"

# The warning "treating 'c' input as 'c++' when in C++ mode, this behavior is deprecated" is suppressed
# with "-x c++", from https://gitlab.kitware.com/cmake/cmake/-/issues/18826

# This disables the warning about the find command being word-split, which is not a problem here.
# It does assume that all source files do not contain spaces in their names.

# shellcheck disable=SC2046
em++ -O3  --closure 1  -s WASM=1 \
         -s EXPORTED_RUNTIME_METHODS='ccall,cwrap' \
         -s ALLOW_MEMORY_GROWTH=1 \
         -s MODULARIZE=1 \
         -s EXPORT_NAME=$EXPORT_NAME \
         -s EXPORTED_FUNCTIONS="$EXPORTED_FUNCTIONS" \
         -s ENVIRONMENT=web \
         -o "./build/$EXPORT_NAME.js" \
         -s "SINGLE_FILE" \
         -x c++ \
         $(find src -iname '*.c' -or -iname '*.cc' -or -iname '*.cpp' | grep -v main.cc) \
         || exit 1
    # -s "FILESYSTEM=0"

cat <<'EOF' | sed "s/__name__/$EXPORT_NAME/" > build/$EXPORT_NAME.d.ts
import '@types/emscripten';

export interface EmscriptenModule___name__ extends EmscriptenModule {
  ccall: typeof ccall;
  cwrap: typeof cwrap;
}

const moduleFactory: EmscriptenModuleFactory<EmscriptenModule___name__>;

export default moduleFactory;


EOF

echo "DONE."
