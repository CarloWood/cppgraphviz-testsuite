add_library(libcppgraphviz
  Graph.cpp
  Graph.hpp
  GraphTracker.hpp
  IndexedContainerSet.hpp
  LabelNode.hpp
  MemoryArea.hpp
  MemoryAreaToGraphLinker.cpp
  MemoryAreaToGraphLinker.hpp
  Node.cpp
  Node.hpp
  NodeTracker.cpp
  NodeTracker.hpp
)

target_link_libraries(libcppgraphviz
 PRIVATE
  CppGraphviz::dot ${AICXX_OBJECTS_LIST}
)

add_subdirectory(dot)
