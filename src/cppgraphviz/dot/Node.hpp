#pragma once

#include "Item.hpp"
#include "ItemPtr.hpp"
#include <concepts>
#include <type_traits>

namespace cppgraphviz::dot {

// A graph node currently only has an id and an attribute list,
// both of which are provided by Item.
class GraphNode : public Item
{
  item_type_type item_type() const override { return item_type_node; }
  void write_dot_to(std::ostream& os, std::string& indentation) const override;
};

template<typename T>
concept ConceptIsGraphNode = std::is_base_of_v<GraphNode, T>;

// A Node is just a pointer to GraphNode.
using Node = ItemPtrTemplate<GraphNode>;

} // namespace cppgraphviz::dot
