#pragma once

#include "NodeTracker.hpp"
#include "GraphTracker.hpp"
#include "Graph.hpp"
#include "MemoryAreaToGraphLinker.hpp"
#include <boost/intrusive_ptr.hpp>

namespace cppgraphviz {

template<typename T>
class Class;
class LastClassMember;

class Node
{
 protected:
  std::shared_ptr<NodeTracker> node_tracker_;
  std::shared_ptr<GraphTracker> graph_tracker_;

  template<typename T> friend class Class;
  friend class LastClassMember;
  static thread_local MemoryAreaToGraphLinker current_graph_linker_;

 public:
  // Create a new Node/NodeTracker pair. This Node is not associated with a graph yet.
  Node(std::weak_ptr<GraphTracker> root_graph, char const* what) :
    node_tracker_(NodeTracker::create(this)), graph_tracker_(current_graph_linker_.get_graph_tracker(std::move(root_graph), this))
  {
    node_tracker_->set_what(what);
    get_graph().add_node(node_tracker_);
  }

  // Move a Node, updating its NodeTracker.
  Node(Node&& node, char const* what) : node_tracker_(std::move(node.node_tracker_)), graph_tracker_(std::move(node.graph_tracker_))
  {
    node_tracker_->set_what(what);
    node_tracker_->set_node({}, this);
  }

  // Copy a Node, creating a new NodeTracker as well.
  Node(Node const& other, char const* what) : node_tracker_(NodeTracker::create(this)), graph_tracker_(other.graph_tracker_)
  {
    node_tracker_->set_what(what);
    get_graph().add_node(node_tracker_);
  }

  // Accessors.
  Graph& get_graph() { return *graph_tracker_; }
  Graph const& get_graph() const { return *graph_tracker_; }

  operator std::weak_ptr<NodeTracker>() const { return node_tracker_; }

  void initialize()
  {
    // Add the attributes of this Node.
    node_attributes(node_tracker_->node_ptr().attribute_list());
  }

 protected:
  virtual void node_attributes(dot::AttributeList& list) { }

 private:
  friend class NodeTracker;
  void set_graph_tracker(std::shared_ptr<GraphTracker>&& graph_tracker)
  {
    graph_tracker_ = std::move(graph_tracker);
  }
};

} // namespace cppgraphviz
