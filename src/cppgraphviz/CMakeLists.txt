add_library(libcppgraphviz
  Graph.cpp
  Graph.hpp
  GraphTracker.cpp
  GraphTracker.hpp
  IndexedContainerSet.hpp
  Item.cpp
  Item.hpp
  LabelNode.hpp
  MemoryArea.hpp
  MemoryAreaToGraphLinker.cpp
  MemoryAreaToGraphLinker.hpp
  Node.hpp
  NodeTracker.cpp
  NodeTracker.hpp
  debug_ostream_operators.cpp
  debug_ostream_operators.hpp
)

target_link_libraries(libcppgraphviz
 PRIVATE
  CppGraphviz::dot ${AICXX_OBJECTS_LIST}
)

add_subdirectory(dot)
