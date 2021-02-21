# SPDX-License-Identifier: BSL-1.0

cmake_minimum_required(VERSION 3.14...3.19 FATAL_ERROR)

project(example.basic)

# Try to find an installed wahl, and if that's not available simply
# include the subdirectory.
find_package(wahl QUIET)
if (NOT wahl_FOUND)
  message(STATUS "wahl is not installed; using local library instead")
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../.." "wahl")
endif ()

add_executable(example.basic main.cpp)
target_link_libraries(example.basic PRIVATE wahl::wahl)
