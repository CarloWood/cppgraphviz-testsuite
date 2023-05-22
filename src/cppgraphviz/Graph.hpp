#pragma once

#include "dot/Graph.hpp"
#include "GraphTracker.hpp"
#include "NodeTracker.hpp"
#include "Item.hpp"
#include <vector>
#include <memory>

namespace cppgraphviz {

class Graph : public Item
{
 private:
  std::shared_ptr<GraphTracker> graph_tracker_;                 // Tracker of this Graph.
  std::vector<std::weak_ptr<NodeTracker>> node_trackers_;       // The nodes that are added to this graph.
  std::vector<std::weak_ptr<GraphTracker>> graph_trackers_;     // The subgraphs that are added to this graph.

 public:
  // Node
  // Create a new Graph/GraphTracker pair. This is a root graph.
  Graph(char const* what) :
    graph_tracker_(GraphTracker::create(this)), Item(this)
  {
    graph_tracker_->set_what(what);
  }

  // Create a new Graph/GraphTracker pair. This is a subgraph.
  Graph(std::weak_ptr<GraphTracker> root_graph, char const* what) :
    graph_tracker_(GraphTracker::create(this)), Item(std::move(root_graph), this)
  {
    graph_tracker_->set_what(what);
    get_parent_graph().add_graph(graph_tracker_);
  }

  // Move a Graph, updating its GraphTracker.
  Graph(Graph&& graph, char const* what) :
    Item(std::move(graph)),
    graph_tracker_(std::move(graph.graph_tracker_)),
    node_trackers_(std::move(graph.node_trackers_)), graph_trackers_(std::move(graph.graph_trackers_))
  {
    graph_tracker_->set_what(what);
    graph_tracker_->set_graph({}, this);
  }

  // Copying a Graph is not allowed.
  Graph(Graph const& other) = delete;

  // This must be passed to the constructor of every Node.
  operator std::weak_ptr<GraphTracker>() const { return graph_tracker_; }

  // Accessor.
  std::shared_ptr<GraphTracker> const& graph_tracker() const { return graph_tracker_; }

  void add_node(std::weak_ptr<NodeTracker> node_tracker);
  void add_graph(std::weak_ptr<GraphTracker> graph_tracker);
  void write_dot(std::ostream& os) const;

  void initialize()
  {
    // Add the attributes of this Node.
    item_attributes(graph_tracker_->graph_ptr().attribute_list());
    call_initialize_on_items();
  }

 private:
  void call_initialize_on_items() const;
};

} // namespace cppgraphviz
