#pragma once

#include "GraphItem.hpp"
#include "GraphItemPtr.hpp"
#include <concepts>
#include <type_traits>

namespace cppgraphviz::dot {

// A graph node currently only has an id and an attribute list,
// both of which are provided by GraphItem.
class GraphNode : public GraphItem
{
};

template<typename T>
concept ConceptIsGraphNode = std::is_base_of_v<GraphNode, T>;

// A Node is just a pointer to GraphNode.
class Node : public GraphItemPtrTemplate<GraphNode>
{
};

} // namespace cppgraphviz::dot
