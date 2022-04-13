project(hkldr)
cmake_minimum_required(VERSION 3.0) #lol

### INCLUDE CMEM
include_directories(${CMEM_DIR})

### FOR USE AS A LIBRARY
set(HKLDR_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
set(HKLDR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

FILE(GLOB_RECURSE CLASSES "${$HKLDR_DIR}/formats/*.c")

set(HKLDR_LIBS "${HKLDR_DIR}/formats/hkx/hkx.c ${CLASSES}" PARENT_SCOPE)
