#pragma once

#include "dot/Graph.hpp"
#include <string_view>

namespace cppgraphviz {

class Node
{
 private:
  dot::Node node_;

 public:
  void initialize()
  {
    node_attributes(node_.attribute_list());
  }

  void add_to_graph(dot::GraphData& graph_data) const
  {
    graph_data.add_node(node_);
  }

  dot::Node const& node() const { return node_; }

  virtual void node_attributes(dot::AttributeList& list)
  {
    list.add({"label", "<unknown label>"});
  }
};

} // namespace cppgraphviz
