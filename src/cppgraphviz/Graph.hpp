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
  // Create a new Graph/GraphTracker pair. This Graph is not associated with a graph yet.
  Graph(char const* what) : graph_tracker_(GraphTracker::create(this))
  {
    graph_tracker_->set_what(what);
  }

  // Move a Graph, updating its GraphTracker.
  Graph(Graph&& graph, char const* what) : graph_tracker_(std::move(graph.graph_tracker_)), node_trackers_(std::move(graph.node_trackers_))
  {
    graph_tracker_->set_what(what);
    graph_tracker_->set_graph({}, this);
  }

  // Copying a Graph is not allowed.
  Graph(Graph const& other) = delete;

  void add_node(std::weak_ptr<NodeTracker> node_tracker);
  void write_dot(std::ostream& os) const;
};

} // namespace cppgraphviz
