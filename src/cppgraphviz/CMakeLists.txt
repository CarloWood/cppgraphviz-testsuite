add_library(libcppgraphviz
  Node.cpp
  Node.hpp
  Graph.cpp
  Graph.hpp
  Class.hpp
  IndexedContainerSet.hpp
  LabelNode.hpp
  NodeTracker.hpp
  TableNode.hpp
)

target_link_libraries(libcppgraphviz
 PRIVATE
  CppGraphviz::dot ${AICXX_OBJECTS_LIST}
)

add_subdirectory(dot)
