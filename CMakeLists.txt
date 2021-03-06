cmake_minimum_required(VERSION 2.8)
set(TARGET_NAME simple-logger)
project(${TARGET_NAME})

if("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
   message(FATAL_ERROR "In-source builds are not allowed. Create a directory, and build within it.")
endif()

include_directories(
    "."
    "./patterns" 
)

set(SOURCES 
    "logger.cpp"
    "test-main.cpp"
)

#Do this to visualize the headers on VS solutions or Xcode projects.
set(HEADERS
    "logger.h"
    "util.h"
    "patterns/non-copyable.h"
    "patterns/simple-assert.h"
    "patterns/singleton.h"
)

add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS})

# c++11
if (${CMAKE_VERSION} VERSION_GREATER 3.1.0)
    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 11)
    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
else()
    message(STATUS "Using an older version of CMAKE")
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif()
    if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # If on Apple, use libc++
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    endif()
endif()
