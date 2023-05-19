add_library(libcppgraphviz
  NodeTracker.cpp
  NodeTracker.hpp
  Graph.cpp
  Graph.hpp
  IndexedContainerSet.hpp
  Node.hpp
)

target_link_libraries(libcppgraphviz
 PRIVATE
  CppGraphviz::dot ${AICXX_OBJECTS_LIST}
)

add_subdirectory(dot)
