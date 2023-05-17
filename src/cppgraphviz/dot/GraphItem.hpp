#pragma once

#include "GraphItemID.hpp"
#include <utils/AIRefCount.h>

namespace cppgraphviz::dot {

class GraphGraph;

// Base class of GraphGraph, GraphEdge and GraphNode.
//
// This class provides a unique id for each graph (item) and a general attribute list.
class GraphItem : public AIRefCount, public GraphItemID
{
 public:
  using graph_graph_type = GraphGraph;

  GraphItem() : GraphItemID(s_unique_id_context.get_id()) { }
  GraphItem(GraphItem const& other) = delete;
  GraphItem(GraphItem&& other) = delete;
};

} // namespace cppgraphviz::dot
