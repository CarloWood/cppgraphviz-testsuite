#include "sys.h"
#include "MemoryRegionToOwnerLinker.hpp"
#include "Node.hpp"
#include <exception>
#ifdef CWDEBUG
#include "utils/has_print_on.h"
#endif

namespace cppgraphviz {

std::shared_ptr<GraphTracker> const& MemoryRegionToOwner::get_graph_tracker(
    MemoryRegion const& memory_region_key, std::shared_ptr<GraphTracker> const& default_graph) const
{
  DoutEntering(dc::notice, "MemoryRegionToOwner::get_graph_tracker(" << memory_region_key << ", " << default_graph << ")");

  if (memory_region_key.lays_within(memory_region_))
  {
    Dout(dc::notice, memory_region_key << " lays within " << memory_region_ << ": returning " << current_graph_);
    return current_graph_;
  }

  // It should not be possible that a Node is being constructed that only partially falls within the current memory area.
  ASSERT(!memory_region_key.overlaps_with(memory_region_));

  Dout(dc::notice, memory_region_key << " does not lay within " << memory_region_ << ": returning " << default_graph);
  return default_graph;
}

#ifdef CWDEBUG
void MemoryRegionToOwner::print_on(std::ostream& os) const
{
  os << memory_region_ << " : " << current_graph_;
}
#endif

std::shared_ptr<GraphTracker> MemoryRegionToOwnerLinker::get_graph_tracker(
    std::shared_ptr<GraphTracker> const& default_graph, MemoryRegion const& item_area) const
{
  DoutEntering(dc::notice, "MemoryRegionToOwnerLinker::get_graph_tracker(" << default_graph << ", " << item_area << ")");

  auto iter = memory_region_to_graph_map_.find(item_area);

  if (iter == memory_region_to_graph_map_.end())
  {
    Dout(dc::notice, "not found; returning: " << default_graph);
    return std::move(default_graph);
  }

  return iter->second.get_graph_tracker(iter->first.current_graph(), item_area);
}

std::shared_ptr<GraphTracker> MemoryRegionToOwnerLinker::get_graph_tracker(
    MemoryRegion const& item_area) const
{
  DoutEntering(dc::notice, "MemoryRegionToOwnerLinker::get_graph_tracker(" << item_area << ")");

  auto iter = memory_region_to_graph_map_.find(item_area);

  // This can happen for example when creating a temporary in the constructor of a class;
  // we just don't add those to any graph at all until they are moved (or copied) into
  // a memory region that belongs to a managed Class.
  if (iter == memory_region_to_graph_map_.end())
    return {};

  return iter->second.get_graph_tracker(iter->first.current_graph(), item_area);
}

std::shared_ptr<GraphTracker> MemoryRegionToOwnerLinker::get_graph_tracker(
    std::weak_ptr<GraphTracker> weak_root_graph, void const* object) const
{
  DoutEntering(dc::notice, "MemoryRegionToOwnerLinker::get_graph_tracker(" << weak_root_graph << ", " << object << ")");

  std::shared_ptr<GraphTracker> root_graph = weak_root_graph.lock();
  if (!root_graph)
    throw std::runtime_error("Adding a node while the root graph is already deleted.");

  // The size of the actual object is probably larger (Item is just a base class), but this will have to do.
  MemoryRegion item_area(static_cast<char const*>(object), sizeof(ItemTemplate<GraphTracker>));

  return get_graph_tracker(root_graph, item_area);
}

std::shared_ptr<GraphTracker> MemoryRegionToOwnerLinker::get_graph_tracker(
    void const* object) const
{
  DoutEntering(dc::notice, "MemoryRegionToOwnerLinker::get_graph_tracker(" << object << ")");

  // The size of the actual object is probably larger (Item is just a base class), but this will have to do.
  MemoryRegion item_area(reinterpret_cast<char const*>(object), sizeof(ItemTemplate<GraphTracker>));

  return get_graph_tracker(item_area);
}

void MemoryRegionToOwnerLinker::start_new_subgraph_for(MemoryRegion memory_region, std::shared_ptr<GraphTracker> const& subgraph)
{
  DoutEntering(dc::notice, "start_new_subgraph_for(" << memory_region << ", " << subgraph << ")");
  Dout(dc::notice, "before:\n" << *this);

  // MemoryRegionToOwner, MemoryRegionToOwnerLinker
  MemoryRegionToOwner memory_region_to_graph(memory_region, subgraph);
  auto ibp = memory_region_to_graph_map_.try_emplace(std::move(memory_region_to_graph));
  while (!ibp.second)
    ibp = ibp.first->second.memory_region_to_graph_map_.try_emplace(std::move(memory_region_to_graph));

  Dout(dc::notice, "after:\n" << *this);
}

bool MemoryRegionToOwnerLinker::erase_memory_region_to_graph(MemoryRegion const& item_area)
{
  DoutEntering(dc::notice, "erase_memory_region_to_graph(" << item_area << ")");

  auto iter = memory_region_to_graph_map_.find(item_area);
  if (iter == memory_region_to_graph_map_.end())
    return false;

  Dout(dc::notice, "before:\n" << *this);

  if (!iter->second.erase_memory_region_to_graph(item_area))
    memory_region_to_graph_map_.erase(iter);

  Dout(dc::notice, "after:\n" << *this);

  return true;
}

void MemoryRegionToOwnerLinker::end_subgraph(MemoryRegion item_area)
{
  DoutEntering(dc::notice, "end_subgraph(" << item_area << ")");

  bool success = erase_memory_region_to_graph(item_area);
  // Paranoia check.
  ASSERT(success);
}

#ifdef CWDEBUG
void MemoryRegionToOwnerLinker::print_on_with_indentation(std::ostream& os, std::string indentation) const
{
  for (auto& key_value_pair : memory_region_to_graph_map_)
  {
    MemoryRegionToOwner const& memory_region_to_graph = key_value_pair.first;
    MemoryRegionToOwnerLinker const& memory_region_to_graph_linker = key_value_pair.second;
    os << indentation << memory_region_to_graph << '\n';
    if (!memory_region_to_graph_linker.empty())
      memory_region_to_graph_linker.print_on_with_indentation(os, indentation + "    ");
  }
}
#endif

} // namespace cppgraphviz
