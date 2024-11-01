# language: CMake

# ---------------------------------     Project setup      ---------------------------------
cmake_minimum_required(VERSION 3.20)

project(Lib LANGUAGES CXX)

# ---------------------------------    Set C++ settings    ---------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -std=c++11 -O2")

# less speed, more warnings = less potential errors in code
# flags using source: https://codeforces.com/blog/entry/15547?locale=ru
# set(CMAKE_CXX_FLAGS " -Wall -Wextra -pedantic -std=c++11 -O2 -Wfloat-equal -Wconversion -Wlogical-op -Wduplicated-cond")

# ---------------------------------       Add library       --------------------------------
file(GLOB_RECURSE SOURCE_LIST "*.cpp" "*.hpp")

add_library(Lib ${SOURCE_LIST})

set_target_properties(Lib PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
