#include "sys.h"
#include "NodeTracker.hpp"
#include "Node.hpp"

namespace cppgraphviz {

void NodeTracker::set_parent_graph_tracker(utils::Badge<Graph>, std::shared_ptr<GraphTracker> parent_graph_tracker)
{
  node_->set_parent_graph_tracker(std::move(parent_graph_tracker));
}

} // namespace cppgraphviz
