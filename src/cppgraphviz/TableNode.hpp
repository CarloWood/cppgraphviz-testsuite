#pragma once

#include "dot/TableNode.hpp"

namespace cppgraphviz {

class TableNodeData : public dot::TableGraphNode
{
 public:
  void initialize()
  {
    for_all_elements(
        [](dot::GraphNode& graph_node){
          //FIXME
          //static_cast<GraphNode&>(graph_node).initialize();
        });
  }
};

class TableNode : public dot::GraphItemPtrTemplate<TableNodeData>
{
 public:
  void initialize()
  {
    item().initialize();
  }
};

} // namespace cppgraphviz
