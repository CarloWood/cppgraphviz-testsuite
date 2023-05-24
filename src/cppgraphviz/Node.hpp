#pragma once

#include "NodeTracker.hpp"
#include "Item.hpp"
#include "Graph.hpp"
#include <boost/intrusive_ptr.hpp>
#ifdef CWDEBUG
#include "debug_ostream_operators.hpp"
#endif

namespace cppgraphviz {

template<typename T>
class Class;

class Node : public Item
{
 protected:
  std::shared_ptr<NodeTracker> node_tracker_;           // Tracker of this Node.

 public:
  // Create a new Node/NodeTracker pair.
  Node(std::weak_ptr<GraphTracker> const& root_graph, char const* what) :
    Item(root_graph, this), node_tracker_(NodeTracker::create(this))
  {
    DoutEntering(dc::notice, "Node(root_graph, \"" << what << "\") [" << this << "]");
    node_tracker_->set_what(what);
    get_parent_graph().add_node(node_tracker_);
  }

  // Move a Node, updating its NodeTracker.
  Node(Node&& node, char const* what) :
    Item(std::move(node)), node_tracker_(std::move(node.node_tracker_))
  {
    DoutEntering(dc::notice, "Node(Node&& " << &node << ", \"" << what << "\") [" << this << "]");
    node_tracker_->set_what(what);
    node_tracker_->set_item({}, this);
  }

  Node(Node&& node) :
    Item(std::move(node)), node_tracker_(std::move(node.node_tracker_))
  {
    DoutEntering(dc::notice, "default Node(Node&& " << &node << ") [" << this << "]");
    node_tracker_->set_item({}, this);
  }

  // Copy a Node, creating a new NodeTracker as well.
  Node(Node const& other, char const* what) :
    Item(other.root_graph_tracker(), this), node_tracker_(NodeTracker::create(this))
  {
    DoutEntering(dc::notice, "Node(Node const& " << &other << ", \"" << what << "\") [" << this << "]");
    node_tracker_->set_what(what);
    get_parent_graph().add_node(node_tracker_);
  }

  Node(Node const& other) :
    Item(other.root_graph_tracker(), this), node_tracker_(NodeTracker::create(this))
  {
    DoutEntering(dc::notice, "default Node(Node const& " << &other << ") [" << this << "]");
    std::string_view what = other.node_tracker_->node_ptr()->attribute_list().get_value("what");
    node_tracker_->set_what(what);
    get_parent_graph().add_node(node_tracker_);
  }

  ~Node()
  {
    DoutEntering(dc::notice, "~Node() [" << this << "]");

    // If Node was moved then node_tracker_ (and parent_graph_tracker_) will be null.
    std::shared_ptr<GraphTracker> parent_graph_tracker = parent_graph_tracker_.lock();
    if (parent_graph_tracker && node_tracker_)
      parent_graph_tracker->get_graph().remove_node(std::move(node_tracker_));
  }

  operator std::weak_ptr<NodeTracker>() const { return node_tracker_; }

  void initialize() override
  {
    // Add the attributes of this Node.
    item_attributes(node_tracker_->node_ptr().attribute_list());
  }
};

} // namespace cppgraphviz
