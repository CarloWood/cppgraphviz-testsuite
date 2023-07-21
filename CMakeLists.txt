cmake_minimum_required(VERSION 3.14...3.26)

project(cppgraphviz-testsuite
        LANGUAGES CXX
        DESCRIPTION "Testsuite for libcppgraphviz")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Begin of gitache configuration.
set(GITACHE_PACKAGES libcwd_r)
include(cwm4/cmake/StableGitache)
# End of gitache configuration.

# This project uses aicxx modules.
include(cwm4/cmake/AICxxProject)

#Exit if someone tries to contaminates the source directory with an in-source build.
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Please do out-of-source builds.\nCleanup: \"rm -rf CMake CMakeCache.txt CMakeFiles/\"")
endif()

include(AICxxSubmodules)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wundef -fconcepts-diagnostics-depth=8 -Wsuggest-override")

add_subdirectory(cppgraphviz)
add_subdirectory(src)
