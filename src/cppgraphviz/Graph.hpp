#pragma once

#include "ID.hpp"
#include "Node.hpp"
#include "Edge.hpp"
#include <map>
#include <iosfwd>

namespace cppgraphviz {

class Graph
{
 private:
  // Configuration.
  bool digraph_ = false;
  bool strict_;
  // The unique dot ID of this graph.
  utils::UniqueID<ID_type> dot_id_ = s_unique_id_context.get_id();

  // The list of all nodes of this graph, by ID.
  std::map<ID_type, Node> nodes_;
  // The list of all edges of this graph, by ID.
  std::map<ID_type, Edge> edges_;

 public:
  Graph(bool strict = false) : strict_(strict) { }

 protected:
  Graph(bool digraph, bool strict = false) : digraph_(digraph), strict_(strict) { }

 public:
  //---------------------------------------------------------------------------
  void set_digraph(bool digraph = true) { digraph_ = digraph; }
  void set_strict(bool strict = true) { strict_ = strict; }

  // Write graph to os in dot format.
  void write_dot(std::ostream& os) const;

  // Accessors.
  bool is_digraph() const { return digraph_; }
  bool is_strict() const { return strict_; }
  ID_type id() const { return dot_id_; }

  //---------------------------------------------------------------------------
  void add_node(Node const& node);
  void add_edge(Edge const& edge);
};

class Digraph : public Graph {
 public:
  Digraph(bool strict = false) : Graph(true, strict) { }
};

} // namespace cppgraphviz
