#include "sys.h"
#include "GraphTracker.hpp"
#include "Graph.hpp"

namespace cppgraphviz {

void GraphTracker::set_parent_graph_tracker(utils::Badge<Graph>, std::shared_ptr<GraphTracker> parent_graph_tracker)
{
  graph_->set_parent_graph_tracker(std::move(parent_graph_tracker));
}

} // namespace cppgraphviz
