#pragma once

#include "GraphItem.hpp"
#include <map>

namespace cppgraphviz {

// A graph node currently only has an id and an attribute list,
// both of which are provided by GraphItemData.
class NodeData : public GraphItemData
{
};

// A Node only has a default constructor at the moment;
// therefore it can be a simple `using` alias declaration.
using Node = GraphItem<NodeData>;

} // namespace cppgraphviz
