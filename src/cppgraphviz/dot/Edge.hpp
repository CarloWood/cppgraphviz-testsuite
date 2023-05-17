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
};

template<typename T>
concept ConceptIsGraphEdge = std::is_base_of_v<GraphEdge, T>;

// This class may not have any additional members.
class Edge : public GraphItemPtrTemplate<GraphEdge>
{
 public:
  Edge() = default;
  Edge(Port const& from, Port const& to) { set_nodes(from, to); }

  // Set the nodes that this edges is connected to.
  void set_nodes(Port const& from, Port const& to) { item().set_nodes(from, to); }

  // Accessors.
  Port from_port() const { return item().from_port(); }
  Port to_port() const { return item().to_port(); }
};

static_assert(sizeof(Edge) == sizeof(GraphItemPtr), "Edge may not have any additional members!");

} // namespace cppgraphviz::dot
