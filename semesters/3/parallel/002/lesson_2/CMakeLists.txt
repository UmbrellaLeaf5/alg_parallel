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

# ---------------------------------      Add executable     --------------------------------
file(GLOB_RECURSE SUBFOLDERS LIST_DIRECTORIES true ".")

foreach(SUBDIR ${SUBFOLDERS})
  IF(IS_DIRECTORY ${SUBDIR})
    get_filename_component(SUBPROJECT_NAME ${SUBDIR} NAME)
    set(SUBPROJECT_NAME "${SUBPROJECT_NAME}_${PROJECT_NAME}")

    file(GLOB_RECURSE SUBPROJECT_SOURCES "${SUBDIR}/*.hpp" "${SUBDIR}/*.cpp")

    add_executable(${SUBPROJECT_NAME} ${SUBPROJECT_SOURCES})
    target_link_libraries(${SUBPROJECT_NAME} PUBLIC MPI::MPI_CXX)
  ELSE()
    CONTINUE()
  ENDIF()
endforeach()