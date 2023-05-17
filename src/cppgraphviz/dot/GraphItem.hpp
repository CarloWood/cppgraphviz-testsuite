#pragma once

#include "GraphItemID.hpp"
#include "item_types.hpp"
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

  bool is_graph() const { return dot::is_graph(item_type()); }

  virtual item_type_type item_type() const = 0;
  virtual void write_dot_to(std::ostream& os, std::string& indentation) const = 0;
};

} // namespace cppgraphviz::dot
