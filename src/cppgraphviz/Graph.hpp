#pragma once

#include "ID.hpp"
#include "Node.hpp"
#include "Edge.hpp"
#include "TableNode.hpp"
#include <string>
#include <map>
#include <iosfwd>

namespace cppgraphviz {

class Graph;

template <typename T>
concept ConceptTable = requires(T t)
{
  { t.ibegin() } -> std::same_as<typename T::index_type>;
  { t.iend() } -> std::same_as<typename T::index_type>;
  { std::declval<T>()[std::declval<typename T::index_type>()] } -> std::same_as<typename T::reference>;
  { std::declval<T const>()[std::declval<typename T::index_type>()] } -> std::same_as<typename T::const_reference>;
};

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

  // Default node and edge attributes. The default (sub)graph attributes are stored in GraphItemData.
  AttributeList node_attribute_list_;
  AttributeList edge_attribute_list_;

  // The list of all (sub)graphs of this graph, by ID.
  std::map<ID_type, GraphItem<GraphData>> graphs_;
  // The list of all nodes of this graph, by ID.
  std::map<ID_type, GraphItem<NodeData>> nodes_;
  // The list of all edges of this graph, by ID.
  std::map<ID_type, GraphItem<EdgeData>> edges_;
  // The list of all "table nodes", by ID;
  std::map<ID_type, TableNode> table_nodes_;

 private:
  void write_body_to(std::ostream& os, std::string indentation = {}) const;

 public:
  //---------------------------------------------------------------------------
  void set_digraph(bool digraph);
  void set_strict(bool strict) { strict_ = strict; }

  // Write graph to os in dot format.
  void write_dot(std::ostream& os) const;

  // Accessors.
  bool is_digraph() const { return digraph_; }
  bool is_strict() const { return strict_; }

  //---------------------------------------------------------------------------
  void add_graph(Graph const& graph);
  void add_node(Node const& node);
  void add_edge(Edge const& edge);

  template<ConceptTable Table>
  void add_table(Table const& table);

  void add_node_attribute(Attribute const& attribute);
  void add_edge_attribute(Attribute const& attribute);
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

  // Write graph to os in dot format.
  void write_dot(std::ostream& os) const { data().write_dot(os); }

  // Accessors.
  bool is_digraph() const { return data().is_digraph(); }
  bool is_strict() const { return data().is_strict(); }

  //---------------------------------------------------------------------------
  void add_graph(Graph const& graph) { data().add_graph(graph); }
  void add_node(Node const& node) { data().add_node(node); }
  void add_edge(Edge const& edge) { data().add_edge(edge); }

  template<ConceptTable Table>
  void add_table(Table const& table)
  {
    data().add_table(table);
  }

  void add_node_attribute(Attribute const& attribute) { data().add_node_attribute(attribute); }
  void add_edge_attribute(Attribute const& attribute) { data().add_edge_attribute(attribute); }
};

static_assert(sizeof(Graph) == sizeof(GraphItem<GraphData>), "Graph may not have any member variables of its own!");

class Digraph : public Graph {
 public:
  Digraph(bool strict = false) : Graph(true, strict) { }
};

} // namespace cppgraphviz

namespace cppgraphviz {

template<ConceptTable Table>
void GraphData::add_table(Table const& table)
{
  utils::UniqueID<ID_type> dot_id = s_unique_id_context.get_id();
  table_nodes_.try_emplace(dot_id, dot_id, table);
}

} // namespace cppgraphviz
