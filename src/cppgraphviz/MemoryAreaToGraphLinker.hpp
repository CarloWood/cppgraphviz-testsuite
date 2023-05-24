#pragma once

#include "MemoryArea.hpp"
#include "GraphTracker.hpp"

namespace cppgraphviz {
using utils::has_print_on::operator<<;

class Item;
class MemoryAreaToGraphLinker;

class MemoryAreaToGraph
{
 private:
  MemoryArea memory_area_;
  std::shared_ptr<GraphTracker> current_graph_;

 private:
  friend class MemoryAreaToGraphLinker;
  // Use to construct a key for MemoryAreaToGraphLinker::memory_area_to_graph_map_.
  MemoryAreaToGraph(MemoryArea const& memory_area) : memory_area_(memory_area) { }
  friend bool operator<(MemoryAreaToGraph const& lhs, MemoryAreaToGraph const& rhs) { return lhs.memory_area_ < rhs.memory_area_; }

 public:
  MemoryAreaToGraph(MemoryArea const& memory_area, std::shared_ptr<GraphTracker> current_graph) :
    memory_area_(memory_area), current_graph_(std::move(current_graph)) { }

  MemoryAreaToGraph(MemoryAreaToGraph&& other) : memory_area_(other.memory_area_), current_graph_(std::move(other.current_graph_)) { }

  // Return current_graph_ if [object, object + size> falls inside memory_area_, otherwise return default_graph.
  std::shared_ptr<GraphTracker> const& get_graph_tracker(
      MemoryArea const& memory_area_key, std::shared_ptr<GraphTracker> const& default_graph) const;

  // Accessor.
  std::shared_ptr<GraphTracker> current_graph() const { return current_graph_; }

#ifdef CWDEBUG
  void print_on(std::ostream& os) const;
#endif
};

// MemoryArea's can be inside one another, but they can not
// partially overlap.
//
// ---
//  ^
//  |     - MemoryArea 1
//  v
// ---
//
// ---
//  ^
//  |     - MemoryArea 2
//  |
//  | ---
//  |  ^
//  |  |  - MemoryArea 3
//  |  v
//  | ---
//  |
//  v
// ---
//

class MemoryAreaToGraphLinker
{
 private:
  std::map<MemoryAreaToGraph, MemoryAreaToGraphLinker> memory_area_to_graph_map_;

 private:
  // Returns true if successful.
  bool erase_memory_area_to_graph(MemoryArea const& memory_area);

 public:
  std::shared_ptr<GraphTracker> get_graph_tracker(std::shared_ptr<GraphTracker> const& default_graph, MemoryArea const& node_area) const;
  std::shared_ptr<GraphTracker> get_graph_tracker(std::weak_ptr<GraphTracker> weak_root_graph, Item* object) const;

  void start_new_subgraph_for(MemoryArea memory_area, std::shared_ptr<GraphTracker> const& subgraph);
  void end_subgraph(MemoryArea node_area);

  bool empty() const { return memory_area_to_graph_map_.empty(); }

#ifdef CWDEBUG
  void print_on_with_indentation(std::ostream& os, std::string indentation) const;
  void print_on(std::ostream& os) const { print_on_with_indentation(os, {}); }
#endif
};

} // namespace cppgraphviz
