#include "sys.h"
#include "NodeTracker.hpp"
#include "Node.hpp"

namespace cppgraphviz {

void NodeTracker::set_graph_tracker(utils::Badge<Graph>, std::shared_ptr<GraphTracker> graph_tracker)
{
  node_->set_graph_tracker(std::move(graph_tracker));
}

} // namespace cppgraphviz
