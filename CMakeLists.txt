cmake_minimum_required(VERSION 3.25)

project(mentor)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

option(BUILD_MENTOR_TUI "" ON)
option(BUILD_MENTOR_TEST "" OFF)

if(BUILD_MENTOR_TEST)
  enable_testing()
endif()

add_subdirectory(lib)

if(BUILD_MENTOR_TUI)
  add_subdirectory(tui)
endif()
