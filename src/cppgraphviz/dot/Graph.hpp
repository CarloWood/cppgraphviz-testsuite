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

class GraphGraph;

template<typename T>
concept ConceptIsGraphGraph = std::is_base_of_v<GraphGraph, T>;

// GraphGraph is derived from GraphItem for its id and (optional) attribute list.
// A graph can be directional or not, and/or strict (only allowing one edge between
// nodes (two for a digraph)).
//
// A graph contains a list of nodes, edges and (optionally) other (sub)graphs.
class GraphGraph : public GraphItem
{
 private:
  // Configuration.
  mutable bool digraph_ = false;
  mutable RankDir rankdir_ = TB;
  bool strict_ = false;
  bool concentrate_ = false;

  // Default node and edge attributes. The default (sub)graph attributes are stored in GraphItem.
  AttributeList node_attribute_list_;
  AttributeList edge_attribute_list_;

  // The list of all (sub)graphs of this graph, by ID.
  std::map<DotID_type, ConstGraphItemPtrTemplate<GraphGraph>> graphs_;
  // The list of all nodes of this graph, by ID.
  std::map<DotID_type, ConstGraphItemPtrTemplate<GraphNode>> nodes_;
  // The list of all edges of this graph, by ID.
  std::map<DotID_type, ConstGraphItemPtrTemplate<GraphEdge>> edges_;
  // The list of all "table nodes", by ID;
  std::map<DotID_type, ConstGraphItemPtrTemplate<TableGraphNode>> table_nodes_;

 private:
  void write_body_to(std::ostream& os, std::string indentation = {}) const;

 public:
  //---------------------------------------------------------------------------
  virtual void set_digraph(bool digraph = true) const;
  virtual void set_rankdir(RankDir rankdir) const;
  void set_strict(bool strict = true) { strict_ = strict; }
  void set_concentrate(bool concentrate) { concentrate_ = concentrate; }

  // Write graph to os in dot format.
  void write_dot(std::ostream& os) const;

  // Accessors.
  bool is_digraph() const { return digraph_; }
  bool is_strict() const { return strict_; }
  bool is_concentrate() const { return concentrate_; }
  RankDir get_rankdir() const { return rankdir_; }

  //---------------------------------------------------------------------------
  void add_graph_graph(GraphGraph const* graph_graph);
  void add_graph_node(GraphNode const* graph_node);
  void add_graph_edge(GraphEdge const* graph_edge);
  void add_table_graph_node(TableGraphNode const* table_graph_node);

  void remove_graph_graph(GraphGraph const* graph_graph);
  void remove_graph_node(GraphNode const* graph_node);
  void remove_graph_edge(GraphEdge const* graph_edge);
  void remove_table_graph_node(TableGraphNode const* table_graph_node);

  template<ConceptIsGraphGraph GG>
  void add_graph(GraphItemPtrTemplate<GG> const& graph)
  {
    add_graph_graph(&graph.item());
  }

  template<ConceptIsGraphNode GN>
  void add_node(GraphItemPtrTemplate<GN> const& node)
  {
    add_graph_node(&node.item());
  }

  template<ConceptIsGraphEdge GE>
  void add_edge(GraphItemPtrTemplate<GE> const& edge)
  {
    add_graph_edge(&edge.item());
  }

  template<ConceptIsTableGraphNode TGN>
  void add_table_node(GraphItemPtrTemplate<TGN> const& table_node)
  {
    add_table_graph_node(&table_node.item());
  }

  template<ConceptIsGraphGraph GG>
  void remove_graph(GraphItemPtrTemplate<GG> const& graph)
  {
    remove_graph_graph(&graph.item());
  }

  template<ConceptIsGraphNode GN>
  void remove_node(GraphItemPtrTemplate<GN> const& node)
  {
    remove_graph_node(&node.item());
  }

  template<ConceptIsGraphEdge GE>
  void remove_edge(GraphItemPtrTemplate<GE> const& edge)
  {
    remove_graph_edge(&edge.item());
  }

  template<ConceptIsTableGraphNode TGN>
  void remove_table_node(GraphItemPtrTemplate<TGN> const& table_node)
  {
    remove_table_graph_node(&table_node.item());
  }

  template<typename T>
  void add(T& obj)
  {
    obj.add_to_graph(*static_cast<typename T::item_type::graph_graph_type*>(this));
  }

  void add_node_attribute(Attribute&& attribute);
  void add_edge_attribute(Attribute&& attribute);
};

template<typename T>
concept ConceptHasAddToGraph = requires(T obj)
{
  obj.add_to_graph(std::declval<typename T::item_type::graph_graph_type&>());
};

class Graph : public GraphItemPtrTemplate<GraphGraph>
{
 public:
  Graph(bool strict = false)
  {
    item().set_strict(strict);
  }

 protected:
  Graph(bool digraph, bool strict = false)
  {
    item().set_digraph(digraph);
    item().set_strict(strict);
  }
};

static_assert(sizeof(Graph) == sizeof(GraphItemPtr), "Graph may not have any member variables of its own!");

struct Digraph : public Graph
{
  Digraph(bool strict = false) : Graph(true, strict) { }
};

} // namespace cppgraphviz::dot
