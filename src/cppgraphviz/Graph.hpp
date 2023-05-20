#pragma once

#include "dot/Graph.hpp"
#include "GraphTracker.hpp"
#include "NodeTracker.hpp"
#include <vector>
#include <memory>

namespace cppgraphviz {

class Graph
{
 private:
  std::shared_ptr<GraphTracker> graph_tracker_;
  std::vector<std::weak_ptr<NodeTracker>> node_trackers_;

 public:
  // Create a new Graph/GraphTracker pair. This is a root graph.
  Graph(char const* what) : graph_tracker_(GraphTracker::create(this))
  {
    graph_tracker_->set_what(what);
  }

  // Create a new Graph/GraphTracker pair. This is a subgraph.
  Graph(std::weak_ptr<GraphTracker> root_graph, char const* what) : graph_tracker_(GraphTracker::create(this))
  {
    graph_tracker_->set_what(what);
    //FIXME: use root_graph -- add this subgraph to a graph.
  }

  // Move a Graph, updating its GraphTracker.
  Graph(Graph&& graph, char const* what) : graph_tracker_(std::move(graph.graph_tracker_)), node_trackers_(std::move(graph.node_trackers_))
  {
    graph_tracker_->set_what(what);
    graph_tracker_->set_graph({}, this);
  }

  // Copying a Graph is not allowed.
  Graph(Graph const& other) = delete;

  // This must be passed to the constructor of every Node.
  operator std::weak_ptr<GraphTracker>() const { return graph_tracker_; }

  void add_node(std::weak_ptr<NodeTracker> node_tracker);
  void write_dot(std::ostream& os) const;
};

} // namespace cppgraphviz
