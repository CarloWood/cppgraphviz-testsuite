#pragma once

#include "GraphItemID.hpp"
#include <utils/AIRefCount.h>

namespace cppgraphviz::dot {

class GraphData;

// Base class of GraphData, EdgeData and NodeData.
//
// This class provides a unique id for each graph (item) and a general attribute list.
class GraphItem : public AIRefCount, public GraphItemID
{
 public:
  using graph_data_type = GraphData;

  GraphItem() : GraphItemID(s_unique_id_context.get_id()) { }
  GraphItem(GraphItem const& other) = delete;
  GraphItem(GraphItem&& other) = delete;
};

} // namespace cppgraphviz::dot
