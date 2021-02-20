# SPDX-License-Identifier: BSL-1.0

cmake_minimum_required(VERSION 3.14...3.19 FATAL_ERROR)

project(args_tests)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
add_executable(args_tests ${sources})

set(DOCTEST_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../third-party/doctest")
add_subdirectory("${DOCTEST_SOURCE_DIR}" doctest)

include(CTest)
enable_testing()

# Try to find an installed args, and if that's not available simply
# include the subdirectory.
find_package(args QUIET)
if (NOT args_FOUND)
  message(STATUS "args is not installed; using local library instead")
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/.." "args")
endif ()

target_link_libraries(args_tests PRIVATE args::args doctest::doctest)

include("${DOCTEST_SOURCE_DIR}/scripts/cmake/doctest.cmake")
doctest_discover_tests(args_tests)
