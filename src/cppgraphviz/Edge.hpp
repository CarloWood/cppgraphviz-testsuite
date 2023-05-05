#pragma once

#include "GraphItem.hpp"

namespace cppgraphviz {

class EdgeData : public GraphItemData
{
 private:
  // The nodes of this edge.
  ID_type from_;
  ID_type to_;

 public:
  void set_nodes(ID_type from, ID_type to)
  {
    from_ = from;
    to_ = to;
  }

  ID_type from_id() const { return from_; }
  ID_type to_id() const { return to_; }
};

class Edge : public GraphItem<EdgeData>
{
 public:
  Edge(Node const& from, Node const& to)
  {
    data({}).set_nodes(from.data({}).id(), to.data({}).id());
  }
};

} // namespace cppgraphviz
