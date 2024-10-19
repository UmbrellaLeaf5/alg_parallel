cmake_minimum_required(VERSION 3.20)

project(parallel LANGUAGES CXX)

file(GLOB_RECURSE SUBDIRS LIST_DIRECTORIES true ".")

foreach(DIR ${SUBDIRS})
  IF(IS_DIRECTORY ${DIR})
    IF(${DIR} MATCHES "lesson_[^/]*$")
      add_subdirectory(${DIR})
    ENDIF()
  ELSE()
    CONTINUE()
  ENDIF()
endforeach()
