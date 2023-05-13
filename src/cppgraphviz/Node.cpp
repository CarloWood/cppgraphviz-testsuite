#include "sys.h"
#include "Node.hpp"
#include "Graph.hpp"

namespace cppgraphviz {

Node::Node() : tracker_(std::make_shared<NodeTracker>(this)), graph_data_(new GraphData)
{
  graph_data_->track_node(this);
}

Node::Node(Node const& other) : tracker_(std::make_shared<NodeTracker>(this)) COMMA_CWDEBUG_ONLY(copied_(true))
{
  set_graph_data(other.graph_data_);
}

Node::Node(Node&& other) noexcept : tracker_(std::move(other.tracker_)), graph_data_(std::move(other.graph_data_))
{
  tracker_->set_node(this);
}

void Node::set_graph_data(boost::intrusive_ptr<GraphData> graph_data)
{
//  graph_data_->remove_node(this);
  graph_data_ = std::move(graph_data);
  ASSERT(graph_data_);
  graph_data_->track_node(this);
}

void Node::add_to_graph(GraphData& graph_data)
{
  // The current graph might be a temporary one, so we have to keep it alive until we're done with it.
  boost::intrusive_ptr<GraphData> current_graph = get_graph_data();
  // Don't add a Node twice to the same GraphData.
  ASSERT(current_graph.get() != &graph_data);
  graph_data.add_nodes_from(current_graph);
}

void Node::copied()
{
#ifdef CWDEBUG
  // Only call this from the copy constructor of LabelNode etc.
  ASSERT(copied_);
  copied_ = false;
#endif
  add_to_graph_impl(graph_data_.get());
}

void Node::destructed()
{
  if (graph_data_)
    remove_from_graph_impl(graph_data_.get());
#ifdef CWDEBUG
  // Only call this from the destructor of LabelNode etc.
  ASSERT(!destructed_);
  destructed_ = true;
#endif
}

} // namespace cppgraphviz
