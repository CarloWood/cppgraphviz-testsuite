#pragma once

#include "utils/ObjectTracker.h"
#include "utils/Badge.h"
#include <memory>

namespace cppgraphviz {

class Node;
class Graph;
class GraphTracker;

// Note that Tracked should be Node or Graph, both of which are derived from Item.
template<typename Tracked>
class ItemTracker : public utils::ObjectTracker<Tracked>
{
 public:
  ItemTracker(Tracked* tracked) : utils::ObjectTracker<Tracked>(tracked) { }

  void set_parent_graph_tracker(utils::Badge<Graph, Node>, std::weak_ptr<GraphTracker> parent_graph_tracker)
  {
    this->tracked_object_ptr_->set_parent_graph_tracker(std::move(parent_graph_tracker));
  }
};

} // namespace cppgraphviz
