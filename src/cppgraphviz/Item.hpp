#pragma once

#include "MemoryRegionToGraphLinker.hpp"
#include "utils/Badge.h"
#ifdef CWDEBUG
#include "debug_ostream_operators.hpp"
#endif

namespace cppgraphviz {

class Item
{
 protected:
  std::weak_ptr<GraphTracker> root_graph_tracker_;      // The root graph of this Item.
  std::weak_ptr<GraphTracker> parent_graph_tracker_;    // The graph that this Item was added to.

  template<typename T> friend class Class;
  static thread_local MemoryRegionToGraphLinker current_graph_linker_;

 private:
  void extract_root_graph();

 public:
  // This is used by Graph when it is the root graph.
  Item(std::weak_ptr<GraphTracker> root_graph_tracker) : root_graph_tracker_(std::move(root_graph_tracker)), parent_graph_tracker_{} { }

  // This is used by Node that is a member of a class.
  Item(Item* object) : parent_graph_tracker_(current_graph_linker_.get_graph_tracker(object))
  {
    extract_root_graph();
  }

  // This is used by Node when it must be added to root_graph.
  Item(std::weak_ptr<GraphTracker> const& root_graph_tracker, Item* object) :
    root_graph_tracker_(root_graph_tracker),
    parent_graph_tracker_(
        !root_graph_tracker.expired() ? current_graph_linker_.get_graph_tracker(root_graph_tracker, object)
                                      : current_graph_linker_.get_graph_tracker(object))
  {
    if (root_graph_tracker_.use_count() == 0 && parent_graph_tracker_.use_count() > 0)
    {
      // The root graph is still unknown, but now we just initialized the member of a class.
      extract_root_graph();
    }
  }

  Item(Item&& other) :
    root_graph_tracker_(std::move(other.root_graph_tracker_)), parent_graph_tracker_(std::move(other.parent_graph_tracker_)) { }

  // Accessors.

  std::shared_ptr<GraphTracker const> parent_graph_tracker() const
  {
    return parent_graph_tracker_.lock();
  }

  std::shared_ptr<GraphTracker> parent_graph_tracker()
  {
    return parent_graph_tracker_.lock();
  }

  Graph& get_parent_graph()
  {
    std::shared_ptr<GraphTracker> parent_graph_tracker = parent_graph_tracker_.lock();
    // Don't call get_parent_graph if this Item doesn't have one.
    ASSERT(parent_graph_tracker);
    return *parent_graph_tracker;
  }

  Graph const& get_parent_graph() const
  {
    std::shared_ptr<GraphTracker const> parent_graph_tracker = parent_graph_tracker_.lock();
    // Don't call get_parent_graph if this Item doesn't have one.
    ASSERT(parent_graph_tracker);
    return *parent_graph_tracker;
  }

  std::weak_ptr<GraphTracker> const& root_graph_tracker() const { return root_graph_tracker_; }

  virtual void initialize() = 0;

 private:
  template<typename Tracked>
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

template<typename Tracker>
class ItemTemplate : public utils::TrackedObject<Tracker>, public Item
{
 public:
  // This is used by Graph when it is the root graph.
  ItemTemplate(utils::Badge<Graph>) : Item(this->tracker_) { }

  // This is used by Node that is a member of a class.
  ItemTemplate(Item* object) : Item(object) { }

  // This is used by Node when it must be added to root_graph.
  ItemTemplate(std::weak_ptr<GraphTracker> const& root_graph_tracker, Item* object) : Item(root_graph_tracker, object) { }

  ItemTemplate(ItemTemplate&& other) : utils::TrackedObject<Tracker>(std::move(other)), Item(std::move(other)) { }
};

} // namespace cppgraphviz
