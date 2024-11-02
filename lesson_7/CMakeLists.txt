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
    set(SUBPROJECT_NAME "${PROJECT_NAME}_${SUBPROJECT_NAME}")

    file(GLOB_RECURSE SUBPROJECT_SOURCES "${SUBDIR}/*.hpp" "${SUBDIR}/*.cpp")

    add_executable(${SUBPROJECT_NAME} ${SUBPROJECT_SOURCES})

    find_library(Lib ../)
    target_link_libraries(${SUBPROJECT_NAME} PUBLIC MPI::MPI_CXX Lib)
  ELSE()
    CONTINUE()
  ENDIF()
endforeach()

# ---------------------------------     Copy extra files    --------------------------------
file(GLOB_RECURSE EXTRA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*")
file(GLOB_RECURSE PROJECT_FILES "*.cpp" "*.h" "*.hpp" "*.sh" "*.o*" "*.e*" ".md")

list(REMOVE_ITEM EXTRA_FILES ${PROJECT_FILES})

file(COPY ${EXTRA_FILES} DESTINATION ${PROJECT_BINARY_DIR})