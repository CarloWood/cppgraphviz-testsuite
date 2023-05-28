#include "sys.h"
#include "MemoryAreaToGraphLinker.hpp"
#include "Node.hpp"
#include <exception>
#ifdef CWDEBUG
#include "utils/has_print_on.h"
#endif

namespace cppgraphviz {

std::shared_ptr<GraphTracker> const& MemoryAreaToGraph::get_graph_tracker(
    MemoryArea const& memory_area_key, std::shared_ptr<GraphTracker> const& default_graph) const
{
  DoutEntering(dc::notice, "MemoryAreaToGraph::get_graph_tracker(" << memory_area_key << ", " << default_graph << ")");

  if (memory_area_key.lays_within(memory_area_))
  {
    Dout(dc::notice, memory_area_key << " lays within " << memory_area_ << ": returning " << current_graph_);
    return current_graph_;
  }

  // It should not be possible that a Node is being constructed that only partially falls within the current memory area.
  ASSERT(!memory_area_key.overlaps_with(memory_area_));

  Dout(dc::notice, memory_area_key << " does not lay within " << memory_area_ << ": returning " << default_graph);
  return default_graph;
}

#ifdef CWDEBUG
void MemoryAreaToGraph::print_on(std::ostream& os) const
{
  os << memory_area_ << " : " << current_graph_;
}
#endif

std::shared_ptr<GraphTracker> MemoryAreaToGraphLinker::get_graph_tracker(
    std::shared_ptr<GraphTracker> const& default_graph, MemoryArea const& node_area) const
{
  DoutEntering(dc::notice, "MemoryAreaToGraphLinker::get_graph_tracker(" << default_graph << ", " << node_area << ")");

  auto iter = memory_area_to_graph_map_.find(node_area);

  if (iter == memory_area_to_graph_map_.end())
  {
    Dout(dc::notice, "not found; returning: " << default_graph);
    return std::move(default_graph);
  }

  return iter->second.get_graph_tracker(iter->first.current_graph(), node_area);
}

std::shared_ptr<GraphTracker> MemoryAreaToGraphLinker::get_graph_tracker(
    MemoryArea const& node_area) const
{
  DoutEntering(dc::notice, "MemoryAreaToGraphLinker::get_graph_tracker(" << node_area << ")");

  auto iter = memory_area_to_graph_map_.find(node_area);

  // This can happen for example when creating a temporary in the constructor of a class;
  // we just don't add those to any graph at all until they are moved (or copied) into
  // a memory region that belongs to a managed Class.
  if (iter == memory_area_to_graph_map_.end())
    return {};

  return iter->second.get_graph_tracker(iter->first.current_graph(), node_area);
}

std::shared_ptr<GraphTracker> MemoryAreaToGraphLinker::get_graph_tracker(
    std::weak_ptr<GraphTracker> weak_root_graph, Item* object) const
{
  DoutEntering(dc::notice, "MemoryAreaToGraphLinker::get_graph_tracker(" << weak_root_graph << ", " << (void*)object << ")");

  std::shared_ptr<GraphTracker> root_graph = weak_root_graph.lock();
  if (!root_graph)
    throw std::runtime_error("Adding a node while the root graph is already deleted.");

  // The size of the actual object is probably larger (Item is just a base class), but this will have to do.
  MemoryArea node_area(reinterpret_cast<char const*>(object), sizeof(Item));

  return get_graph_tracker(root_graph, node_area);
}

std::shared_ptr<GraphTracker> MemoryAreaToGraphLinker::get_graph_tracker(
    Item* object) const
{
  DoutEntering(dc::notice, "MemoryAreaToGraphLinker::get_graph_tracker(" << (void*)object << ")");

  // The size of the actual object is probably larger (Item is just a base class), but this will have to do.
  MemoryArea node_area(reinterpret_cast<char const*>(object), sizeof(Item));

  return get_graph_tracker(node_area);
}

void MemoryAreaToGraphLinker::start_new_subgraph_for(MemoryArea memory_area, std::shared_ptr<GraphTracker> const& subgraph)
{
  DoutEntering(dc::notice, "start_new_subgraph_for(" << memory_area << ", " << subgraph << ")");
  Dout(dc::notice, "before:\n" << *this);

  // MemoryAreaToGraph, MemoryAreaToGraphLinker
  MemoryAreaToGraph memory_area_to_graph(memory_area, subgraph);
  auto ibp = memory_area_to_graph_map_.try_emplace(std::move(memory_area_to_graph));
  while (!ibp.second)
    ibp = ibp.first->second.memory_area_to_graph_map_.try_emplace(std::move(memory_area_to_graph));

  Dout(dc::notice, "after:\n" << *this);
}

bool MemoryAreaToGraphLinker::erase_memory_area_to_graph(MemoryArea const& node_area)
{
  DoutEntering(dc::notice, "erase_memory_area_to_graph(" << node_area << ")");

  auto iter = memory_area_to_graph_map_.find(node_area);
  if (iter == memory_area_to_graph_map_.end())
    return false;

  Dout(dc::notice, "before:\n" << *this);

  if (!iter->second.erase_memory_area_to_graph(node_area))
    memory_area_to_graph_map_.erase(iter);

  Dout(dc::notice, "after:\n" << *this);

  return true;
}

void MemoryAreaToGraphLinker::end_subgraph(MemoryArea node_area)
{
  DoutEntering(dc::notice, "end_subgraph(" << node_area << ")");

  bool success = erase_memory_area_to_graph(node_area);
  // Paranoia check.
  ASSERT(success);
}

#ifdef CWDEBUG
void MemoryAreaToGraphLinker::print_on_with_indentation(std::ostream& os, std::string indentation) const
{
  for (auto& key_value_pair : memory_area_to_graph_map_)
  {
    MemoryAreaToGraph const& memory_area_to_graph = key_value_pair.first;
    MemoryAreaToGraphLinker const& memory_area_to_graph_linker = key_value_pair.second;
    os << indentation << memory_area_to_graph << '\n';
    if (!memory_area_to_graph_linker.empty())
      memory_area_to_graph_linker.print_on_with_indentation(os, indentation + "    ");
  }
}
#endif

} // namespace cppgraphviz
