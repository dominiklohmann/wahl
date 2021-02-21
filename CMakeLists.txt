cmake_minimum_required(VERSION 3.14...3.19 FATAL_ERROR)

project(
  wahl
  DESCRIPTION "A type-safe argument parser for modern C++."
  VERSION 0.1
  LANGUAGES CXX)

# -- include utlity libraries -------------------------------------------------

include(cmake/InstallProject.cmake)

# -- create library target ----------------------------------------------------

# Glob for source and header files. The project's convention is to use .hpp and
# .cpp, and we will look for no other file endings here.
file(GLOB_RECURSE sources CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

# Create a library target.
add_library(wahl ${sources})

# Building wahl (and linking against it) requires C++17.
target_compile_features(wahl PUBLIC cxx_std_17)

# Enforce standard compliance for MSVC.
target_compile_options(wahl
  PUBLIC
    "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive>")

# Set the include directory properly.
target_include_directories(wahl
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>)

# Enable the install target.
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
  CHANGELOG "${CMAKE_CURRENT_LIST_DIR}/docs/CHANGELOG.md"
  DEPENDENCIES "")
