#include "sys.h"
#include "GraphTracker.hpp"
#include "Graph.hpp"

namespace cppgraphviz {

GraphTracker::GraphTracker(utils::Badge<GraphTracker>, Graph* graph) : ItemTracker(graph)
{
}

Graph const& GraphTracker::get_graph() const
{
  return static_cast<Graph const&>(*item_);
}

Graph& GraphTracker::get_graph()
{
  return static_cast<Graph&>(*item_);
}

} // namespace cppgraphviz
