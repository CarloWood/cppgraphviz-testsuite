#include "sys.h"
#include "MemoryAreaToGraphLinker.hpp"
#include "Node.hpp"
#include <exception>

namespace cppgraphviz {

std::shared_ptr<GraphTracker> MemoryAreaToGraphLinker::get_graph_tracker(std::weak_ptr<GraphTracker> weak_root_graph, Node* object)
{
  std::shared_ptr<GraphTracker> root_graph = weak_root_graph.lock();
  if (!root_graph)
    throw std::runtime_error("Adding a node while the root graph is already deleted.");
  MemoryArea node_area(reinterpret_cast<char const*>(object), sizeof(Node));   // The size of the actual object is probably larger
                                                // (Node is just a base class), but this will have to do.
  if (node_area.lays_within(memory_area_))
    return current_graph_;

  // It should not be possible that a Node is being constructed that only partially falls within the current memory area.
  ASSERT(!node_area.overlaps_with(memory_area_));

  return std::move(root_graph);
}

void MemoryAreaToGraphLinker::start_new_subgraph_for(MemoryArea memory_area, std::weak_ptr<GraphTracker> subgraph)
{
}

void MemoryAreaToGraphLinker::end_subgraph()
{
}

} // namespace cppgraphviz
