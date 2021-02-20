cmake_minimum_required(VERSION 3.14...3.19 FATAL_ERROR)

project(
  args
  DESCRIPTION "A type-safe argument parser for modern C++."
  VERSION 0.1
  LANGUAGES CXX)

# -- include utlity libraries -------------------------------------------------

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
include(InstallProject)

# -- create library target ----------------------------------------------------

# Glob for source and header files. The project's convention is to use .hpp and
# .cpp, and we will look for no other file endings here.
file(GLOB_RECURSE sources CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

# Create a library target.
add_library(args ${sources})

# Enforce standard conformance.
set_target_properties(args
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON)

target_compile_options(args
  PUBLIC
    "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive>")

target_include_directories(args
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>)

string(TOLOWER ${PROJECT_NAME}/version.hpp VERSION_HEADER_LOCATION)

InstallProject(
  NAME "${PROJECT_NAME}"
  VERSION "${PROJECT_VERSION}"
  NAMESPACE "${PROJECT_NAME}"
  BINARY_DIR "${PROJECT_BINARY_DIR}"
  INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include"
  INCLUDE_DESTINATION "include/${PROJECT_NAME}-${PROJECT_VERSION}"
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  COMPATIBILITY SameMajorVersion
  LICENSE "${CMAKE_CURRENT_LIST_DIR}/LICENSE"
  README "${CMAKE_CURRENT_LIST_DIR}/docs/README.md"
  DEPENDENCIES "")
