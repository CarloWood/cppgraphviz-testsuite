#pragma once

#include "MemoryRegion.hpp"
#include "GraphTracker.hpp"

namespace cppgraphviz {
using utils::has_print_on::operator<<;

class Item;
class MemoryRegionToGraphLinker;

class MemoryRegionToGraph
{
 private:
  MemoryRegion memory_region_;
  std::shared_ptr<GraphTracker> current_graph_;

 private:
  friend class MemoryRegionToGraphLinker;
  // Use to construct a key for MemoryRegionToGraphLinker::memory_region_to_graph_map_.
  MemoryRegionToGraph(MemoryRegion const& memory_region) : memory_region_(memory_region) { }
  friend bool operator<(MemoryRegionToGraph const& lhs, MemoryRegionToGraph const& rhs) { return lhs.memory_region_ < rhs.memory_region_; }

 public:
  MemoryRegionToGraph(MemoryRegion const& memory_region, std::shared_ptr<GraphTracker> current_graph) :
    memory_region_(memory_region), current_graph_(std::move(current_graph)) { }

  MemoryRegionToGraph(MemoryRegionToGraph&& other) : memory_region_(other.memory_region_), current_graph_(std::move(other.current_graph_)) { }

  // Return current_graph_ if [object, object + size> falls inside memory_region_, otherwise return default_graph.
  std::shared_ptr<GraphTracker> const& get_graph_tracker(
      MemoryRegion const& memory_region_key, std::shared_ptr<GraphTracker> const& default_graph) const;

  // Accessor.
  std::shared_ptr<GraphTracker> current_graph() const { return current_graph_; }

#ifdef CWDEBUG
  void print_on(std::ostream& os) const;
#endif
};

// MemoryRegion's can be inside one another, but they can not
// partially overlap.
//
// ---
//  ^
//  |     - MemoryRegion 1
//  v
// ---
//
// ---
//  ^
//  |     - MemoryRegion 2
//  |
//  | ---
//  |  ^
//  |  |  - MemoryRegion 3
//  |  v
//  | ---
//  |
//  v
// ---
//

class MemoryRegionToGraphLinker
{
 private:
  std::map<MemoryRegionToGraph, MemoryRegionToGraphLinker> memory_region_to_graph_map_;

 private:
  // Returns true if successful.
  bool erase_memory_region_to_graph(MemoryRegion const& memory_region);

 public:
  std::shared_ptr<GraphTracker> get_graph_tracker(std::shared_ptr<GraphTracker> const& default_graph, MemoryRegion const& item_area) const;
  std::shared_ptr<GraphTracker> get_graph_tracker(MemoryRegion const& item_area) const;
  std::shared_ptr<GraphTracker> get_graph_tracker(std::weak_ptr<GraphTracker> weak_root_graph, Item* object) const;
  std::shared_ptr<GraphTracker> get_graph_tracker(Item* object) const;

  void start_new_subgraph_for(MemoryRegion memory_region, std::shared_ptr<GraphTracker> const& subgraph);
  void end_subgraph(MemoryRegion item_area);

  bool empty() const { return memory_region_to_graph_map_.empty(); }

#ifdef CWDEBUG
  void print_on_with_indentation(std::ostream& os, std::string indentation) const;
  void print_on(std::ostream& os) const { print_on_with_indentation(os, {}); }
#endif
};

} // namespace cppgraphviz
