#pragma once

#include "DotID.hpp"
#include "Node.hpp"
#include "Edge.hpp"
#include "TableNode.hpp"
#include <string>
#include <map>
#include <iosfwd>

namespace cppgraphviz::dot {

enum RankDir {
  TB,
  LR,
  BT,
  RL
};

class Graph;

// GraphData is derived from GraphItemData for its id and (optional) attribute list.
// A graph can be directional or not, and/or strict (only allowing one edge between
// nodes (two for a digraph)).
//
// A graph contains a list of nodes, edges and (optionally) other (sub)graphs.
class GraphData : public GraphItemData
{
 private:
  // Configuration.
  bool digraph_ = false;
  bool strict_ = false;
  bool concentrate_ = false;
  RankDir rankdir_ = TB;

  // Default node and edge attributes. The default (sub)graph attributes are stored in GraphItemData.
  AttributeList node_attribute_list_;
  AttributeList edge_attribute_list_;

  // The list of all (sub)graphs of this graph, by ID.
  std::map<DotID_type, GraphItem<GraphData>> graphs_;
  // The list of all nodes of this graph, by ID.
  std::map<DotID_type, GraphItem<NodeData>> nodes_;
  // The list of all edges of this graph, by ID.
  std::map<DotID_type, GraphItem<EdgeData>> edges_;
  // The list of all "table nodes", by ID;
  std::map<DotID_type, GraphItem<TableNodeData>> table_nodes_;

 private:
  void write_body_to(std::ostream& os, std::string indentation = {}) const;

 public:
  //---------------------------------------------------------------------------
  void set_digraph(bool digraph);
  void set_strict(bool strict) { strict_ = strict; }
  void set_concentrate(bool concentrate) { concentrate_ = concentrate; }
  void set_rankdir(RankDir rankdir);

  // Write graph to os in dot format.
  void write_dot(std::ostream& os) const;

  // Accessors.
  bool is_digraph() const { return digraph_; }
  bool is_strict() const { return strict_; }
  bool is_concentrate() const { return concentrate_; }
  RankDir get_rankdir() const { return rankdir_; }

  //---------------------------------------------------------------------------
  void add_graph(Graph const& graph);
  void add_node(Node const& node);
  void add_edge(Edge const& edge);
  void add_table_node(TableNode const& table_node);

  template<typename T>
  void add(T const& obj)
  {
    obj.add_to_graph(*this);
  }

  void add_node_attribute(Attribute&& attribute);
  void add_edge_attribute(Attribute&& attribute);
};

template<typename T>
concept ConceptHasAddToGraph = requires(T obj)
{
  obj.add_to_graph(std::declval<GraphData&>());
};

class Graph : public GraphItem<GraphData>
{
 public:
  Graph(bool strict = false)
  {
    data().set_strict(strict);
  }

 protected:
  Graph(bool digraph, bool strict = false)
  {
    data().set_digraph(digraph);
    data().set_strict(strict);
  }

 public:
  //---------------------------------------------------------------------------
  void set_digraph(bool digraph = true) { data().set_digraph(digraph); }
  void set_strict(bool strict = true) { data().set_strict(strict); }
  void set_concentrate(bool concentrate = true) { data().set_concentrate(concentrate); }
  void set_rankdir(RankDir rankdir) { data().set_rankdir(rankdir); }

  // Write graph to os in dot format.
  void write_dot(std::ostream& os) const
  {
    data().write_dot(os);
  }

  // Accessors.
  bool is_digraph() const { return data().is_digraph(); }
  bool is_strict() const { return data().is_strict(); }
  bool is_concentrate() const { return data().is_concentrate(); }
  RankDir get_rankdir() const { return data().get_rankdir(); }

  //---------------------------------------------------------------------------
  void add_graph(Graph const& graph) { data().add_graph(graph); }
  void add_node(Node const& node) { data().add_node(node); }
  void add_edge(Edge const& edge) { data().add_edge(edge); }
  void add_table_node(TableNode const& table_node) { data().add_table_node(table_node); }

  // Add an arbitrary object to the graph that knows how to add itself.
  // This is just a more intuitive interface; now one can use `graph.add(obj)` instead of `obj.add_to_graph(graph_data)`.
  template<ConceptHasAddToGraph T>
  void add(T const& obj)
  {
    data().add(obj);
  }

  void add_node_attribute(Attribute&& attribute) { data().add_node_attribute(std::move(attribute)); }
  void add_edge_attribute(Attribute&& attribute) { data().add_edge_attribute(std::move(attribute)); }
};

static_assert(sizeof(Graph) == sizeof(GraphItem<GraphData>), "Graph may not have any member variables of its own!");

class Digraph : public Graph {
 public:
  Digraph(bool strict = false) : Graph(true, strict) { }
};

} // namespace cppgraphviz::dot
