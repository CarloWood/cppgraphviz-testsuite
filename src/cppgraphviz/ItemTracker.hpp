#pragma once

#include "utils/Badge.h"
#include <memory>

namespace cppgraphviz {

class Item;
class Node;
class Graph;
class GraphTracker;

class ItemTracker
{
 protected:
  Item* item_;

 public:
  ItemTracker(Item* item) : item_(item) { }

  void set_item(utils::Badge<Graph, Node>, Item* item)
  {
    item_ = item;
  }

  void set_parent_graph_tracker(utils::Badge<Graph, Node>, std::shared_ptr<GraphTracker> parent_graph_tracker);

  // Automatical conversation to an Item reference.
  operator Item const&() const { return *item_; }
  operator Item&() { return *item_; }
};

} // namespace cppgraphviz
