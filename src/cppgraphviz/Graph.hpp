#pragma once

#include "Node.hpp"
#include "dot/Graph.hpp"
#include <vector>

// Define this to 1 to print debug output about constructing and destructing GraphData objects.
#define ENABLE_GRAPHDATA_TRACKING 0

//-------------------------
#ifndef CWDEBUG
#define TRACK_GRAPHDATA 0
#else
#define TRACK_GRAPHDATA ENABLE_GRAPHDATA_TRACKING
#endif
#if TRACK_GRAPHDATA
#include "cwds/tracked.h"
#endif

namespace cppgraphviz {

#if TRACK_GRAPHDATA
extern char const* name_GraphData;
#endif

class GraphData : public dot::GraphGraph
#if TRACK_GRAPHDATA
                  , public tracked::Tracked<&name_GraphData>
#endif
{
#if TRACK_GRAPHDATA
  using tracked::Tracked<&name_GraphData>::Tracked;

 public:
  GraphData() = default;

  GraphData(GraphData& orig) = delete;
  GraphData(GraphData const& orig) = delete;
  GraphData(GraphData&& orig) = delete;
  GraphData(GraphData const&& orig) = delete;
#endif // TRACK_GRAPHDATA

 public:
  using graph_graph_type = GraphData;

 private:
  // A Node has a std::shared_ptr<NodeTracker> (tracker_).
  // A NodeTracker has a Node* (node_) that points back to the Node.
  //
  // A Node also has a boost::intrusive_ptr<GraphData> (graph_data_),
  // and the GraphData has a std::weak_ptr<NodeTracker> (element of node_trackers_)
  // that points back to the NodeTracker that that Node points to.
  //
  //        --graph_data_------------------------------> GraphData
  //   Node --tracker_-> NodeTracker <--node_trackers_--
  //        <---node_---
  //
  // The relationship between a Node and a NodeTracker is 1-on-1,
  // and the Node manages the NodeTracker. Therefore we can also
  // see the 'Node / NodeTracker' pair as a single "object".
  // This is why a Node automatically converts into a NodeTracker
  // and visa versa, and we use the name 'node' for both.
  // (a variable of type std::shared_ptr<NodeTracker> is still called
  // a node_tracker though).
  std::vector<std::weak_ptr<NodeTracker>> node_trackers_;

 public:
  void track_node(Node* node)
  {
    node_trackers_.emplace_back(node->get_tracker());
  }

  void add_nodes_from(boost::intrusive_ptr<GraphData> const& current_graph)
  {
    ASSERT(current_graph.get() != this);
    // Run over all (tracked) nodes in the "current graph".
    for (std::weak_ptr<NodeTracker> node_weak_ptr : current_graph->node_trackers_)
    {
      std::shared_ptr<NodeTracker> node_tracker = node_weak_ptr.lock();
      if (node_tracker)
      {
        //node_trackers_.emplace_back(node_tracker);
        Node& node = *node_tracker;
        node.set_graph_data(this);
        node.add_to_graph_impl(this);
      }
    }
  }
};

using Graph = dot::GraphItemPtrTemplate<GraphData>;

} // namespace cppgraphviz
