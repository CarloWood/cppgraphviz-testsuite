#include "sys.h"
#include "Graph.hpp"
#include "Node.hpp"

namespace cppgraphviz {

void Graph::add_node(std::weak_ptr<NodeTracker> weak_node_tracker)
{
  std::shared_ptr<NodeTracker> node_tracker = weak_node_tracker.lock();
  if (node_tracker)
  {
    graph_tracker_->graph_ptr()->add(node_tracker->node_ptr());
    node_trackers_.push_back(std::move(weak_node_tracker));
    node_tracker->set_graph_tracker({}, graph_tracker_);
  }
}

void Graph::write_dot(std::ostream& os) const
{
  for (std::weak_ptr<NodeTracker> const& weak_node_tracker : node_trackers_)
  {
    std::shared_ptr<NodeTracker> node_tracker = weak_node_tracker.lock();
    if (node_tracker)
    {
      Node& node = *node_tracker;
      node.initialize();
    }
  }
  graph_tracker_->graph_ptr()->write_dot(os);
}

} // namespace cppgraphviz
