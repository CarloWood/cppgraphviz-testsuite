#include "sys.h"
#include "MemoryAreaToGraphLinker.hpp"
#include "Node.hpp"
#include <exception>

namespace cppgraphviz {

std::shared_ptr<GraphTracker> MemoryAreaToGraph::get_graph_tracker(
    MemoryArea const& memory_area_key, std::shared_ptr<GraphTracker> const& default_graph) const
{
  if (memory_area_key.lays_within(memory_area_))
    return current_graph_;

  // It should not be possible that a Node is being constructed that only partially falls within the current memory area.
  ASSERT(!memory_area_key.overlaps_with(memory_area_));

  return default_graph;
}

std::shared_ptr<GraphTracker> MemoryAreaToGraphLinker::get_graph_tracker(std::weak_ptr<GraphTracker> weak_root_graph, Item* object) const
{
  std::shared_ptr<GraphTracker> root_graph = weak_root_graph.lock();
  if (!root_graph)
    throw std::runtime_error("Adding a node while the root graph is already deleted.");

  // The size of the actual object is probably larger (Item is just a base class), but this will have to do.
  MemoryArea node_area(reinterpret_cast<char const*>(object), sizeof(Item));

  return get_graph_tracker(std::move(root_graph), node_area);
}

std::shared_ptr<GraphTracker> MemoryAreaToGraphLinker::get_graph_tracker(Item* object) const
{
  // The size of the actual object is probably larger (Item is just a base class), but this will have to do.
  MemoryArea node_area(reinterpret_cast<char const*>(object), sizeof(Item));

  return get_graph_tracker(std::shared_ptr<GraphTracker>{}, node_area);
}

std::shared_ptr<GraphTracker> MemoryAreaToGraphLinker::get_graph_tracker(
    std::shared_ptr<GraphTracker>&& default_graph, MemoryArea const& node_area) const
{
  auto iter = memory_area_to_graph_map_.find(node_area);

  if (iter == memory_area_to_graph_map_.end())
    return std::move(default_graph);

  return iter->second.get_graph_tracker(iter->first.current_graph(), node_area);
}

void MemoryAreaToGraphLinker::start_new_subgraph_for(MemoryArea memory_area, std::shared_ptr<GraphTracker> const& subgraph)
{
  DoutEntering(dc::notice, "start_new_subgraph_for(" << memory_area << ", \"" << subgraph << "\")");

  // MemoryAreaToGraph, MemoryAreaToGraphLinker
  MemoryAreaToGraph memory_area_to_graph(memory_area, subgraph);
  auto ibp = memory_area_to_graph_map_.try_emplace(std::move(memory_area_to_graph));
  while (!ibp.second)
    ibp = ibp.first->second.memory_area_to_graph_map_.try_emplace(std::move(memory_area_to_graph));
}

bool MemoryAreaToGraphLinker::erase_memory_area_to_graph(MemoryArea const& node_area)
{
  auto iter = memory_area_to_graph_map_.find(node_area);
  if (iter == memory_area_to_graph_map_.end())
    return false;

  if (!iter->second.erase_memory_area_to_graph(node_area))
    memory_area_to_graph_map_.erase(iter);

  return true;
}

void MemoryAreaToGraphLinker::end_subgraph(MemoryArea node_area)
{
  DoutEntering(dc::notice, "end_subgraph(" << node_area << ")");

  bool success = erase_memory_area_to_graph(node_area);
  // Paranoia check.
  ASSERT(success);
}

} // namespace cppgraphviz
