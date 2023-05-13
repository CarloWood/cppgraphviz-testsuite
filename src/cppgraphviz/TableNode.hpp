#pragma once

#include "dot/TableNode.hpp"

namespace cppgraphviz {

class TableNodeData : public dot::TableNodeData
{
 public:
  void initialize()
  {
    for_all_elements(
        [](dot::NodeData& node_data){
          //FIXME
          //static_cast<NodeData&>(node_data).initialize();
        });
  }
};

class TableNode : public dot::TableNodeTemplate<TableNodeData>
{
 public:
  void initialize()
  {
    data().initialize();
  }
};

} // namespace cppgraphviz
