#!/bin/bash

# Changes to this must be reflected in webpack.config.common.js
EXPORT_NAME=liblp

# Functions starting with this will be exported
PUBLIC_API_NAME=lpapi

echo
echo
echo -------------------------------------------------------------------------------
echo "Building $EXPORT_NAME.js ..."

EXPORTED_FUNCTIONS=$(egrep -o "$PUBLIC_API_NAME""_\w+" src/$PUBLIC_API_NAME.h | tr '\n' ' ' | sed 's/ /","_/g' | sed 's/^/["_/' | sed 's/,"_$/]/')

echo "FUNCTIONS: $EXPORTED_FUNCTIONS"

cat <<'EOF' | sed "s/__name__/$EXPORT_NAME/" > build/$EXPORT_NAME.d.ts
export function __name__(config?: {
  locateFile?(path: string): any,
}): Promise<{
  cwrap: typeof cwrap,
}>
EOF

# if you create more dependencies of fib.cc, simply add them to the end of the below command, like: -o ./fib.js fib.cc add.cc anotherdep.cc and_so_on.cc
em++ -O3 -s WASM=1 \
         -s EXPORTED_RUNTIME_METHODS='ccall,cwrap' \
         -s ALLOW_MEMORY_GROWTH=1 \
         -s MODULARIZE=1 \
         -s EXPORT_NAME=$EXPORT_NAME \
         -s EXPORTED_FUNCTIONS="$EXPORTED_FUNCTIONS" \
         -s ENVIRONMENT=web \
         -o "./build/$EXPORT_NAME.js" \
         -s "SINGLE_FILE" \
         $(find src -iname '*.c' -or -iname '*.cc' -or -iname '*.cpp')
    # -s "FILESYSTEM=0"

echo "DONE...."
echo -------------------------------------------------------------------------------
echo
echo
