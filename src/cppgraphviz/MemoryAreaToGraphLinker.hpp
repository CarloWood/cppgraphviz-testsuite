#pragma once

#include "MemoryArea.hpp"
#include "GraphTracker.hpp"

namespace cppgraphviz {

class Node;

class MemoryAreaToGraphLinker
{
 private:
  MemoryArea memory_area_;
  std::shared_ptr<GraphTracker> current_graph_;

 public:
  MemoryAreaToGraphLinker() : memory_area_(nullptr, 0) { }

  std::shared_ptr<GraphTracker> get_graph_tracker(std::weak_ptr<GraphTracker> root_graph, Node* object);

  void start_new_subgraph_for(MemoryArea memory_area, std::weak_ptr<GraphTracker> subgraph);
  void end_subgraph();
};

} // namespace cppgraphviz
