#pragma once

#include "dot/Node.hpp"
#include <utils/Badge.h>
#include <memory>

namespace cppgraphviz {

class Graph;
class GraphPtr;
class Node;
class GraphTracker;

// A Node has a std::shared_ptr<NodeTracker> (tracker_).
// A NodeTracker has a Node* (node_) that points back to the Node.
//
// A Node also has a GraphPtr (graph_ptr_),
// and the Graph has a std::weak_ptr<NodeTracker> (element of node_trackers_)
// that points back to the NodeTracker that that Node points to.
//
//        --graph_ptr_ ------------------------------> Graph
//   Node --tracker_-> NodeTracker <--node_trackers_--
//        <---node_---
//
// The relationship between a Node and a NodeTracker is 1-on-1,
// and the Node manages the NodeTracker. Therefore we can also
// see the 'Node / NodeTracker' pair as a single "object".
// This is why a NodeTracker automatically converts to a Node,
// and we use the name 'node' for both (a variable of type
// std::shared_ptr<NodeTracker> is still called a node_tracker
// though).

class NodeTracker
{
 private:
  Node* node_;                  // The node that is being tracked.
  dot::NodePtr node_ptr_;       // Unique pointer to the corresponding dot::NodeItem.

 public:
  // Private constructor, called by create.
  NodeTracker(utils::Badge<NodeTracker>, Node* node) : node_(node) { }

  static std::shared_ptr<NodeTracker> create(Node* node)
  {
    return std::make_shared<NodeTracker>(utils::Badge<NodeTracker>{}, node);
  }

  void set_node(utils::Badge<Node>, Node* node)
  {
    node_ = node;
  }

  void set_what(std::string_view what)
  {
    node_ptr_->attribute_list().remove("what");
    node_ptr_->attribute_list().add({"what", what});
  }

  void set_parent_graph_tracker(utils::Badge<Graph>, std::shared_ptr<GraphTracker> parent_graph_tracker);

  // Accessors.
  operator Node const&() const { return *node_; }
  operator Node&() { return *node_; }

  dot::NodePtr const& node_ptr() const { return node_ptr_; }
  dot::NodePtr& node_ptr() { return node_ptr_; }
};

} // namespace cppgraphviz
