#pragma once

#include "MemoryAreaToGraphLinker.hpp"
#include "utils/Badge.h"
#ifdef CWDEBUG
#include "debug_ostream_operators.hpp"
#endif

namespace cppgraphviz {

class NodeTracker;

class Item
{
 protected:
  std::weak_ptr<GraphTracker> root_graph_tracker_;      // The root graph of this Item.
  std::weak_ptr<GraphTracker> parent_graph_tracker_;    // The graph that this Item was added to.

  template<typename T> friend class Class;
  static thread_local MemoryAreaToGraphLinker current_graph_linker_;

 public:
  // This is used by Graph when it is the root graph.
  Item(utils::Badge<Graph>, std::weak_ptr<GraphTracker> const& root_graph_tracker) :
    root_graph_tracker_(root_graph_tracker), parent_graph_tracker_{} { }

  // This is used by Node when it must be added to root_graph.
  Item(std::weak_ptr<GraphTracker> const& root_graph_tracker, Item* object) :
    root_graph_tracker_(root_graph_tracker),
    parent_graph_tracker_(current_graph_linker_.get_graph_tracker(root_graph_tracker, object)) { }

  Item(Item&& other) :
    root_graph_tracker_(std::move(other.root_graph_tracker_)), parent_graph_tracker_(std::move(other.parent_graph_tracker_)) { }

  // Accessors.

  Graph& get_parent_graph()
  {
    std::shared_ptr<GraphTracker> parent_graph_tracker = parent_graph_tracker_.lock();
    // Don't call get_parent_graph if this Item doesn't have one.
    ASSERT(parent_graph_tracker);
    return parent_graph_tracker->get_graph();
  }

  Graph const& get_parent_graph() const
  {
    std::shared_ptr<GraphTracker const> parent_graph_tracker = parent_graph_tracker_.lock();
    // Don't call get_parent_graph if this Item doesn't have one.
    ASSERT(parent_graph_tracker);
    return parent_graph_tracker->get_graph();
  }

  std::weak_ptr<GraphTracker> const& root_graph_tracker() const { return root_graph_tracker_; }

  virtual void initialize() = 0;

 private:
  friend class ItemTracker;
  void set_parent_graph_tracker(std::weak_ptr<GraphTracker>&& parent_graph_tracker)
  {
    DoutEntering(dc::notice, "set_parent_graph_tracker(" << parent_graph_tracker << ") [" << this << "]");
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
