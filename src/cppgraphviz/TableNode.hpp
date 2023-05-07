#pragma once

#include "GraphItemID.hpp"
#include <utils/UniqueID.h>
#include <functional>
#include <vector>

namespace cppgraphviz {

using TableNodeData = GraphItemID;

class TableNode : GraphItemID
{
 private:
  // List of all nodes in the table.
  std::vector<TableNodeData> nodes_;

 public:
  TableNode(utils::UniqueID<ID_type> dot_id, std::function<Node const&(size_t)> at, size_t size) : GraphItemID(dot_id)
  {
    for (size_t i = 0; i < size; ++i)
      nodes_.emplace_back(at(i).data({}));
  }

  template<typename Table>
  TableNode(utils::UniqueID<ID_type> dot_id, Table const& table) :
    TableNode(dot_id, [&](size_t i){ return table[typename Table::index_type{i}]; }, table.size())
  {
  }
};

} // namespace cppgraphviz
