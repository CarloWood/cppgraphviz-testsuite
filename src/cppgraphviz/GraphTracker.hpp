#pragma once

#include "dot/Graph.hpp"
#include <utils/Badge.h>
#include <memory>

namespace cppgraphviz {

class Graph;

class GraphTracker
{
 private:
  Graph* graph_;                // The graph that is being tracked.
  dot::GraphPtr graph_ptr_;     // Unique pointer to the corresponding dot::GraphItem.

 public:
  // Private constructor, called by create.
  GraphTracker(utils::Badge<GraphTracker>, Graph* graph) : graph_(graph) { }

  static std::shared_ptr<GraphTracker> create(Graph* graph)
  {
    return std::make_shared<GraphTracker>(utils::Badge<GraphTracker>{}, graph);
  }

  void set_graph(utils::Badge<Graph>, Graph* graph)
  {
    graph_ = graph;
  }

  void set_what(char const* what)
  {
    graph_ptr_->attribute_list().remove("what");
    graph_ptr_->attribute_list().add({"what", what});
  }

  // Accessors.
  operator Graph const&() const { return *graph_; }
  operator Graph&() { return *graph_; }

  dot::GraphPtr const& graph_ptr() const { return graph_ptr_; }
  dot::GraphPtr& graph_ptr() { return graph_ptr_; }
};

} // namespace cppgraphviz
