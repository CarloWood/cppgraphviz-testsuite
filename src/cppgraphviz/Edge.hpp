#pragma once

#include "GraphItem.hpp"

namespace cppgraphviz {

class NodeData;
using Node = GraphItem<NodeData>;

class EdgeData : public GraphItemData
{
 private:
  // The nodes of this edge.
  ID_type from_;
  ID_type to_;

 public:
  void set_nodes(Node const& from, Node const& to);

  ID_type from_id() const { return from_; }
  ID_type to_id() const { return to_; }
};

// This class may not have any additional members.
class Edge : public GraphItem<EdgeData>
{
 public:
  Edge() = default;
  Edge(Node const& from, Node const& to)
  {
    set_nodes(from, to);
  }

 public:
  void set_nodes(Node const& from, Node const& to) { data().set_nodes(from, to); }
  ID_type from_id() const { return data().from_id(); }
  ID_type to_id() const { return data().to_id(); }
};

static_assert(sizeof(Edge) == sizeof(GraphItem<EdgeData>), "Edge may not have any additional members!");

} // namespace cppgraphviz
