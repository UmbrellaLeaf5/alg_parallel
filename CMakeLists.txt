# language: CMake

# ---------------------------------     Project setup      ---------------------------------
cmake_minimum_required(VERSION 3.20)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${PROJECT_NAME} LANGUAGES CXX)

# ---------------------------------    Set C++ settings    ---------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -std=c++11 -O2")

# less speed, more warnings = less potential errors in code
# flags using source: https://codeforces.com/blog/entry/15547?locale=ru
# set(CMAKE_CXX_FLAGS " -Wall -Wextra -pedantic -std=c++11 -O2 -Wfloat-equal -Wconversion -Wlogical-op -Wduplicated-cond")

# ---------------------------------   Set CMake settings   ---------------------------------
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# ---------------------------------         Find MPI        --------------------------------
find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

find_package(OpenMP REQUIRED)
string(APPEND CMAKE_CXX_FLAGS " -fopenmp")

# ---------------------------------    Add subdirectories   --------------------------------
file(GLOB_RECURSE SUBDIRS LIST_DIRECTORIES true ".")

foreach(DIR ${SUBDIRS})
  if(IS_DIRECTORY ${DIR})
    if(${DIR} MATCHES "${PROJECT_NAME}/lesson_[^/]*$")
      # message(${DIR})
      add_subdirectory(${DIR})
    endif()
  ELSE()
    CONTINUE()
  endif()
endforeach()

# Lib
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib)