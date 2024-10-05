# language: CMake

# ---------------------------------     Project setup      ---------------------------------
cmake_minimum_required(VERSION 3.20)

project(lesson3 LANGUAGES CXX)

# ---------------------------------    Set C++ settings    ---------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -std=c++17 -O2")

# less speed, more warnings = less potential errors in code
# flags using source: https://codeforces.com/blog/entry/15547?locale=ru
# (commented because of qcustomplot warnings)
# set(CMAKE_CXX_FLAGS " -Wall -Wextra -pedantic -std=c++17 -O2 -Wfloat-equal -Wconversion -Wlogical-op -Wduplicated-cond")

# ---------------------------------   Set CMake settings   ---------------------------------
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# ---------------------------------         Find MPI        --------------------------------
find_package(MPI)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

if(LINUX)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" -fopenmp)
endif()

# file(GLOB_RECURSE PROJECT_SOURCES "*.hpp" "*.cpp")
add_executable(size_rank_lesson3 size_rank/size_rank.cpp)
target_link_libraries(size_rank_lesson3 PUBLIC MPI::MPI_CXX)

add_executable(send_lesson3 send/send.cpp)
target_link_libraries(send_lesson3 PUBLIC MPI::MPI_CXX)