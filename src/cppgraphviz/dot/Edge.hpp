#pragma once

#include "GraphItem.hpp"
#include "Port.hpp"
#include <concepts>
#include <type_traits>

namespace cppgraphviz::dot {

class GraphEdge : public GraphItem
{
 private:
  // The nodes of this edge.
  Port from_;
  Port to_;

 public:
  void set_nodes(Port const& from, Port const& to);

  Port const& from_port() const { return from_; }
  Port const& to_port() const { return to_; }

  item_type_type item_type() const override { return item_type_edge; }
  void write_dot_to(std::ostream& os, std::string& indentation) const override;
};

template<typename T>
concept ConceptIsGraphEdge = std::is_base_of_v<GraphEdge, T>;

// This class may not have any additional members.
struct Edge : public GraphItemPtrTemplate<GraphEdge>
{
  Edge() = default;
  Edge(Port const& from, Port const& to) { item().set_nodes(from, to); }
};

static_assert(sizeof(Edge) == sizeof(GraphItemPtr), "Edge may not have any additional members!");

} // namespace cppgraphviz::dot
