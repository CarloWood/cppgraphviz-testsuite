add_library(libcppgraphviz
  ID.cxx
  ID.hpp
  Graph.cxx
  Graph.hpp
  AttributeList.cxx
  AttributeList.hpp
  Node.hpp
  Edge.hpp
  GraphItem.hpp
  Attribute.hpp
)

# Required include search-paths.
get_target_property(CWDS_INTERFACE_INCLUDE_DIRECTORIES AICxx::cwds INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(libcppgraphviz
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CWDS_INTERFACE_INCLUDE_DIRECTORIES}" # For sys.h and debug.h.
)
