#!/usr/bin/env sh

# This is a wrapper script for building the liblp library for the web.
# It will use em++ if it is available, otherwise it will use docker.

cd "$(dirname "$0")" || exit 1

# Check for em++
if command -v em++ &> /dev/null
then
  echo "Building with local emscripten..."
  ./build-emscripten.sh || exit 1
else
  # Check for docker
  if command -v docker-compose &> /dev/null
  then
    echo "emscripten not found, using docker for build. This is probably slower."
    docker-compose run --rm builder || exit 1
  else
    echo "Neither em++ or docker-compose could be found."
    echo "Please install emscripten:"
    echo "    brew install emscripten"
    echo
    echo "Or ensure Docker is available"
    echo
    exit 1
  fi
fi
