cmake_minimum_required(VERSION 3.10)

project(lplib)

set(CMAKE_CXX_STANDARD 11)

# Get a list of all the .cc files in the src/ directory
file(GLOB SOURCES "src/*.cc")

# Create an executable from the source files
# Find all C, C++, and .cc files in the src directory
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")

# Create an executable from the source files
add_executable(lpcli ${SOURCES} src/util/lp_duktape.cc src/lpapi.cc)
