#pragma once

#include "NodeTracker.hpp"
#include "Item.hpp"
#include "Graph.hpp"
#include <boost/intrusive_ptr.hpp>

namespace cppgraphviz {

template<typename T>
class Class;

class Node : public Item
{
 protected:
  std::shared_ptr<NodeTracker> node_tracker_;           // Tracker of this Node.

 public:
  // Create a new Node/NodeTracker pair. This Node is not associated with a graph yet.
  Node(std::weak_ptr<GraphTracker> root_graph, char const* what) :
    node_tracker_(NodeTracker::create(this)), Item(std::move(root_graph), this)
  {
    node_tracker_->set_what(what);
    get_parent_graph().add_node(node_tracker_);
  }

  // Move a Node, updating its NodeTracker.
  Node(Node&& node, char const* what) :
    node_tracker_(std::move(node.node_tracker_)), Item(std::move(node))
  {
    node_tracker_->set_what(what);
    node_tracker_->set_node({}, this);
  }

  // Copy a Node, creating a new NodeTracker as well.
  Node(Node const& other, char const* what) : node_tracker_(NodeTracker::create(this)), Item(other)
  {
    node_tracker_->set_what(what);
    get_parent_graph().add_node(node_tracker_);
  }

  operator std::weak_ptr<NodeTracker>() const { return node_tracker_; }

  void initialize()
  {
    // Add the attributes of this Node.
    item_attributes(node_tracker_->node_ptr().attribute_list());
  }
};

} // namespace cppgraphviz
