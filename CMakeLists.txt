cmake_minimum_required(VERSION 3.14...3.26)

project(cppgraphviz
        LANGUAGES CXX
        DESCRIPTION "A collection of classes that allow to chart the relationship between C++ objects as a graph.")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Begin of gitache configuration.
set(GITACHE_PACKAGES libcwd_r)
# FetchContent was added to 3.14.
cmake_minimum_required(VERSION 3.14)

include(FetchContent)

# If a local gitache submodule is present then use that rather than downloading one.
if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/gitache/.git)
  # This will disable the use of the GIT_REPOSITORY/GIT_TAG below, and disable the
  # FetchContent- download and update step. Instead, use the gitache submodule as-is.
  set(FETCHCONTENT_SOURCE_DIR_GITACHE "${CMAKE_CURRENT_LIST_DIR}/gitache" CACHE INTERNAL "" FORCE)
endif ()

FetchContent_Declare(
  gitache
  GIT_REPOSITORY "https://github.com/CarloWood/gitache.git"
  GIT_TAG        "master"  # Latest SHA1 of branch "stable".
)

# To enable DEBUG messages, pass -DCMAKE_MESSAGE_LOG_LEVEL=DEBUG to cmake on the command line.
message(DEBUG "DEBUG: cwm4/cmake/StableGitache: calling `FetchContent_MakeAvailable(gitache)`")
FetchContent_MakeAvailable(gitache)
message(DEBUG "DEBUG: cwm4/cmake/StableGitache: returned from `FetchContent_MakeAvailable(gitache)`")
# End of gitache configuration.

# This project uses aicxx modules.
include(cwm4/cmake/AICxxProject)

#Exit if someone tries to contaminates the source directory with an in-source build.
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Please do out-of-source builds.\nCleanup: \"rm -rf CMake CMakeCache.txt CMakeFiles/\"")
endif()

include(AICxxSubmodules)

add_subdirectory(src)
