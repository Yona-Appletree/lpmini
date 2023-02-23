cmake_minimum_required(VERSION 3.10)

project(lplib)

set(CMAKE_CXX_STANDARD 11)

# Create an executable from the source files
add_executable(
        lpcli
        src/main.cc
        src/lp_context/lp_context.cc
        src/lp_scope/lp_scope.cc
        src/util/lp_duktape.cc
        src/util/noise.cpp
        src/duktape/duktape.c
        src/lp_conn/lp_conn.cc
        src/lp_api.cc
        src/lp_obj/lp_obj.cc
        src/lp_node/lp_node.cc
        src/lp_node/lp_expr_node/lp_expr_node.cc
        src/lp_node/lp_func_node/lp_func_node.cc
        src/lp_node/lp_map_node/lp_map_node.cc
        src/lp_node/lp_noise_node/lp_noise_node.cc
        src/lp_node/lp_time_node/lp_time_node.cc
)
