#pragma once

#include "GraphItem.hpp"

namespace cppgraphviz {

class NodeData : public GraphItemData
{
};

using Node = GraphItem<NodeData>;

} // namespace cppgraphviz
