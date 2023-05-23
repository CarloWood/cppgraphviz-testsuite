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
    node_tracker->set_parent_graph_tracker({}, graph_tracker_);
  }
}

void Graph::remove_node(std::shared_ptr<NodeTracker>&& node_tracker)
{
  node_tracker->set_parent_graph_tracker({}, {});
  // Erase node_tracker and any expired elements from node_trackers_.
  std::erase_if(node_trackers_,
      [&node_tracker](std::weak_ptr<NodeTracker> const& wp){
        auto sp = wp.lock();
        return !sp || sp == node_tracker;
      });
  graph_tracker_->graph_ptr()->remove(node_tracker->node_ptr());
}

void Graph::add_graph(std::weak_ptr<GraphTracker> weak_graph_tracker)
{
  std::shared_ptr<GraphTracker> graph_tracker = weak_graph_tracker.lock();
  if (graph_tracker)
  {
    graph_tracker_->graph_ptr()->add(graph_tracker->graph_ptr());
    graph_trackers_.push_back(std::move(weak_graph_tracker));
    graph_tracker->set_parent_graph_tracker({}, graph_tracker_);
  }
}

void Graph::remove_graph(std::shared_ptr<GraphTracker>&& graph_tracker)
{
  graph_tracker->set_parent_graph_tracker({}, {});
  // Erase graph_tracker and any expired elements from graph_trackers_.
  std::erase_if(graph_trackers_,
      [&graph_tracker](std::weak_ptr<GraphTracker> const& wp){
        auto sp = wp.lock();
        return !sp || sp == graph_tracker;
      });
  graph_tracker_->graph_ptr()->remove(graph_tracker->graph_ptr());
}

void Graph::call_initialize_on_items() const
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
  for (std::weak_ptr<GraphTracker> const& weak_graph_tracker : graph_trackers_)
  {
    std::shared_ptr<GraphTracker> graph_tracker = weak_graph_tracker.lock();
    if (graph_tracker)
    {
      Graph& graph = *graph_tracker;
      graph.initialize();
    }
  }
}

void Graph::write_dot(std::ostream& os) const
{
  call_initialize_on_items();
  graph_tracker_->graph_ptr()->write_dot(os);
}

} // namespace cppgraphviz
