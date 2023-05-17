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
  item_type_type item_type() const override { return item_type_node; }
  void write_dot_to(std::ostream& os, std::string& indentation) const override;
};

template<typename T>
concept ConceptIsGraphNode = std::is_base_of_v<GraphNode, T>;

// A Node is just a pointer to GraphNode.
using Node = GraphItemPtrTemplate<GraphNode>;

} // namespace cppgraphviz::dot
