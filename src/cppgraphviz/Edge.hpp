#pragma once

#include "GraphItem.hpp"
#include "Port.hpp"

namespace cppgraphviz {

class EdgeData : public GraphItemData
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

// This class may not have any additional members.
class Edge : public GraphItem<EdgeData>
{
 public:
  Edge() = default;
  Edge(Port const& from, Port const& to)
  {
    set_nodes(from, to);
  }

 public:
  void set_nodes(Port const& from, Port const& to) { data().set_nodes(from, to); }
  Port from_port() const { return data().from_port(); }
  Port to_port() const { return data().to_port(); }
};

static_assert(sizeof(Edge) == sizeof(GraphItem<EdgeData>), "Edge may not have any additional members!");

} // namespace cppgraphviz
