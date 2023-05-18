#pragma once

#include "dot/TableNode.hpp"

namespace cppgraphviz {

class TableNodeData : public dot::TableNodeItem
{
 public:
  void initialize()
  {
    for_all_elements(
        [](dot::NodeItem& node_item){
          //FIXME
          //static_cast<GraphNode&>(node_item).initialize();
        });
  }
};

class TableNode : public dot::ItemPtrTemplate<TableNodeData>
{
 public:
  void initialize()
  {
    item().initialize();
  }
};

} // namespace cppgraphviz
