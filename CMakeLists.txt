# language: CMake

# ---------------------------------     Project setup      ---------------------------------
cmake_minimum_required(VERSION 3.20)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${PROJECT_NAME} C CXX)

# ---------------------------------    Set C++ settings    ---------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -std=c++17 -O2")

# less speed, more warnings = less potential errors in code
# flags using source: https://codeforces.com/blog/entry/15547?locale=ru
# set(CMAKE_CXX_FLAGS " -Wall -Wextra -pedantic -std=c++17 -O2 -Wfloat-equal -Wconversion -Wlogical-op -Wduplicated-cond")

# ---------------------------------   Set CMake settings   ---------------------------------
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# ---------------------------------         Find MPI        --------------------------------
find_package(MPI)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

if(LINUX)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" -fopenmp)
endif()

# ---------------------------------    Add subdirectories   --------------------------------
file(GLOB_RECURSE SUBDIRS LIST_DIRECTORIES true ".")

foreach(DIR ${SUBDIRS})
  IF(IS_DIRECTORY ${DIR})
    IF(${DIR} MATCHES "parallel/lesson_[^/]*$")
      # message(${DIR})
      add_subdirectory(${DIR})
    ENDIF()
  ELSE()
    CONTINUE()
  ENDIF()
endforeach()
