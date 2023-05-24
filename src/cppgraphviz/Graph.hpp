#pragma once

#include "dot/Graph.hpp"
#include "GraphTracker.hpp"
#include "NodeTracker.hpp"
#include "Item.hpp"
#include <vector>
#include <memory>
#ifdef CWDEBUG
#include "debug_ostream_operators.hpp"
#endif

namespace cppgraphviz {

template<typename T>
class Class;

class Graph : public Item
{
 private:
  std::shared_ptr<GraphTracker> graph_tracker_;                 // Tracker of this Graph.
  std::vector<std::weak_ptr<NodeTracker>> node_trackers_;       // The nodes that are added to this graph.
  std::vector<std::weak_ptr<GraphTracker>> graph_trackers_;     // The subgraphs that are added to this graph.

  // Called from the public constructor below in order to keep the shared_ptr returned by create alive long enough.
  enum PE { pe };
  Graph(PE, std::weak_ptr<GraphTracker> const& root_graph, char const* what) :
    Item({}, root_graph), graph_tracker_(root_graph.lock())
  {
    DoutEntering(dc::notice, "Graph(\"" << what << "\") [" << this << "]");
    graph_tracker_->set_what(what);
  }

 public:
  // Node
  // Create a new Graph/GraphTracker pair. This is a root graph.
  Graph(char const* what) : Graph(pe, GraphTracker::create(this), what) { }

  // Create a new Graph/GraphTracker pair. This is a subgraph.
  Graph(std::weak_ptr<GraphTracker> const& root_graph, char const* what) :
    Item(root_graph, this), graph_tracker_(GraphTracker::create(this))
  {
    DoutEntering(dc::notice, "Graph(root_graph, \"" << what << "\") [" << this << "]");
    graph_tracker_->set_what(what);
    get_parent_graph().add_graph(graph_tracker_);
  }

  // Move a Graph, updating its GraphTracker.
  Graph(Graph&& other, char const* what) :
    Item(std::move(other)),
    graph_tracker_(std::move(other.graph_tracker_)),
    node_trackers_(std::move(other.node_trackers_)),
    graph_trackers_(std::move(other.graph_trackers_))
  {
    DoutEntering(dc::notice, "Graph(Graph&& " << &other << ", \"" << what << "\") [" << this << "]");
    graph_tracker_->set_what(what);
    graph_tracker_->set_item({}, this);
  }

  // Copying a Graph is not allowed.
  Graph(Graph const& other) = delete;

 private:
  template<typename T>
  friend class Class;
  Graph(Graph const& other, char const* what) :
    Item(other.root_graph_tracker(), this),
    graph_tracker_(GraphTracker::create(this)),
    node_trackers_{},
    graph_trackers_{}
  {
    DoutEntering(dc::notice, "Graph(Graph const& " << &other << ", \"" << what << "\") [" << this << "]");
    graph_tracker_->set_what(what);
    graph_tracker_->set_item({}, this);
    get_parent_graph().add_graph(graph_tracker_);
  }

 public:
  ~Graph()
  {
    DoutEntering(dc::notice, "~Graph() [" << this << "]");
    // The root graph and moved graphs don't have a parent.
    std::shared_ptr<GraphTracker> parent_graph_tracker = parent_graph_tracker_.lock();
    if (parent_graph_tracker)
      parent_graph_tracker->get_graph().remove_graph(std::move(graph_tracker_));

    // Make a copy of the child items and then remove them from this graph for proper bookkeeping.
    auto node_trackers = std::move(node_trackers_);
    auto graph_trackers = std::move(graph_trackers_);
    for (auto& weak_node_tracker : node_trackers)
      remove_node(weak_node_tracker.lock());
    for (auto& weak_graph_tracker : graph_trackers)
      remove_graph(weak_graph_tracker.lock());
  }

  // This must be passed to the constructor of every Node.
  operator std::weak_ptr<GraphTracker>() const { return graph_tracker_; }

  // Accessor.
  std::shared_ptr<GraphTracker> const& graph_tracker() const { return graph_tracker_; }

  void add_node(std::weak_ptr<NodeTracker> node_tracker);
  void remove_node(std::shared_ptr<NodeTracker>&& node_tracker);
  void add_graph(std::weak_ptr<GraphTracker> graph_tracker);
  void remove_graph(std::shared_ptr<GraphTracker>&& graph_tracker);
  void write_dot(std::ostream& os) const;

  void initialize() override
  {
    // Add the attributes of this Node.
    item_attributes(graph_tracker_->graph_ptr().attribute_list());
    call_initialize_on_items();
  }

 private:
  void call_initialize_on_items() const;
};

} // namespace cppgraphviz
