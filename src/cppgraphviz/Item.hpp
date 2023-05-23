#pragma once

#include "MemoryAreaToGraphLinker.hpp"

namespace cppgraphviz {

class NodeTracker;

class Item
{
 protected:
  std::shared_ptr<GraphTracker> parent_graph_tracker_;  // The graph that this Node was added to.

  template<typename T> friend class Class;
  static thread_local MemoryAreaToGraphLinker current_graph_linker_;

 public:
  Item(Item* object) :
    parent_graph_tracker_(current_graph_linker_.get_graph_tracker(object)) { }

  Item(std::weak_ptr<GraphTracker>&& root_graph, Item* object) :
    parent_graph_tracker_(current_graph_linker_.get_graph_tracker(std::move(root_graph), object)) { }

  Item(Item&& other) :
    parent_graph_tracker_(std::move(other.parent_graph_tracker_)) { }

  Item(Item const& other) :
    parent_graph_tracker_(other.parent_graph_tracker_) { }

  // Accessors.
  Graph& get_parent_graph() { return parent_graph_tracker_->get_graph(); }
  Graph const& get_parent_graph() const { return parent_graph_tracker_->get_graph(); }

  bool has_parent() const { return static_cast<bool>(parent_graph_tracker_); }

  virtual void initialize() = 0;

 private:
  friend class ItemTracker;
  void set_parent_graph_tracker(std::shared_ptr<GraphTracker>&& parent_graph_tracker)
  {
    parent_graph_tracker_ = std::move(parent_graph_tracker);
  }

 protected:
  virtual void item_attributes(dot::AttributeList& list)
  {
    //FIXME: remove this.
    ASSERT(false);
  }
};

} // namespace cppgraphviz
