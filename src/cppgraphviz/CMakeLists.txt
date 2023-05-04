add_library(libcppgraphviz
  Graph.cxx
  Graph.hpp
)

# Required include search-paths.
get_target_property(CWDS_INTERFACE_INCLUDE_DIRECTORIES AICxx::cwds INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(libcppgraphviz
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CWDS_INTERFACE_INCLUDE_DIRECTORIES}" # For sys.h and debug.h.
)
