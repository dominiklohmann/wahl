# SPDX-License-Identifier: BSL-1.0

cmake_minimum_required(VERSION 3.14...3.19 FATAL_ERROR)

project(example.group)

# Try to find an installed args, and if that's not available simply
# include the subdirectory.
find_package(args QUIET)
if (NOT args_FOUND)
  message(STATUS "args is not installed; using local library instead")
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../.." "args")
endif ()

add_executable(example.group main.cpp)
target_link_libraries(example.group PRIVATE args::args)
