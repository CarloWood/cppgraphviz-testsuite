#pragma once

#include "dot/Graph.hpp"
#include <utils/Badge.h>
#include <memory>

namespace cppgraphviz {

class Graph;

// GraphTracker objects can only be created by calling the static GraphTracker::create,
// which uses std::make_shared<GraphTracker> to create it.
//
// GraphTracker is derived from std::enable_shared_from_this<GraphTracker> so that we
// can use graph_tracker_ = parent_graph_->shared_from_this(); where parent_graph_
// is a plain GraphTracker* (that we got by calling `get()` on a shared_ptr.
//
// Like NodeTracker, GraphTracker contains a pointer to both, the corresponding Graph
// object in a 1-on-1 relationship with its GraphTracker, and a pointer to the
// dot::GraphItem that represents the graphical (sub)graph in dot.
//
// In turn Graph has a std::shared_ptr<GraphTracker> graph_tracker_; pointing to
// its GraphTracker object, and whenever the Graph is moved it adjusts the Graph*
// in its tracker by calling set_graph.
//
class GraphTracker : std::enable_shared_from_this<GraphTracker>
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
