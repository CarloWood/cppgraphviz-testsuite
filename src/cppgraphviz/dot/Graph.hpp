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

template<typename T>
concept ConceptIsGraphGraph = std::is_base_of_v<GraphGraph, T>;

template<ConceptIsGraphGraph GG>
class GraphTemplate;
class GraphGraph;
using Graph = GraphTemplate<GraphGraph>;

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
  virtual void set_digraph(bool digraph) const;
  virtual void set_rankdir(RankDir rankdir) const;
  void set_strict(bool strict) { strict_ = strict; }
  void set_concentrate(bool concentrate) { concentrate_ = concentrate; }

  // Write graph to os in dot format.
  void write_dot(std::ostream& os) const;

  // Accessors.
  bool is_digraph() const { return digraph_; }
  bool is_strict() const { return strict_; }
  bool is_concentrate() const { return concentrate_; }
  RankDir get_rankdir() const { return rankdir_; }

  //---------------------------------------------------------------------------
  void add_graph(GraphGraph const* graph_graph);
  void add_node(GraphNode const* graph_node);
  void add_edge(GraphEdge const* graph_edge);
  void add_table_graph_node(TableGraphNode const* table_graph_node);

  void remove_graph(GraphGraph const* graph_graph);
  void remove_node(GraphNode const* graph_node);
  void remove_edge(GraphEdge const* graph_edge);
  void remove_table_graph_node(TableGraphNode const* table_graph_node);

  template<ConceptIsGraphGraph GG>
  void add_graph(GraphItemPtrTemplate<GG> const& graph)
  {
    add_graph(&graph.item());
  }

  template<ConceptIsGraphNode GN>
  void add_node(GraphItemPtrTemplate<GN> const& node)
  {
    add_node(&node.item());
  }

  template<ConceptIsGraphEdge GE>
  void add_edge(GraphItemPtrTemplate<GE> const& edge)
  {
    add_edge(&edge.item());
  }

  template<ConceptIsTableGraphNode TGN>
  void add_table_graph_node(GraphItemPtrTemplate<TGN> const& table_graph_node)
  {
    add_table_graph_node(&table_graph_node.item());
  }

  template<ConceptIsGraphGraph GG>
  void remove_graph(GraphItemPtrTemplate<GG> const& graph)
  {
    remove_graph(&graph.item());
  }

  template<ConceptIsGraphNode GN>
  void remove_node(GraphItemPtrTemplate<GN> const& node)
  {
    remove_node(&node.item());
  }

  template<ConceptIsGraphEdge GE>
  void remove_edge(GraphItemPtrTemplate<GE> const& edge)
  {
    remove_edge(&edge.item());
  }

  template<ConceptIsTableGraphNode TGN>
  void remove_table_graph_node(GraphItemPtrTemplate<TGN> const& table_graph_node)
  {
    remove_table_graph_node(&table_graph_node.item());
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

template<ConceptIsGraphGraph GG>
class GraphTemplate : public GraphItemPtrTemplate<GG>
{
 public:
  GraphTemplate(bool strict = false)
  {
    this->item().set_strict(strict);
  }

 protected:
  GraphTemplate(bool digraph, bool strict = false)
  {
    this->item().set_digraph(digraph);
    this->item().set_strict(strict);
  }

 public:
  //---------------------------------------------------------------------------
  void set_digraph(bool digraph = true) { this->item().set_digraph(digraph); }
  void set_rankdir(RankDir rankdir) { this->item().set_rankdir(rankdir); }
  void set_strict(bool strict = true) { this->item().set_strict(strict); }
  void set_concentrate(bool concentrate = true) { this->item().set_concentrate(concentrate); }

  // Write graph to os in dot format.
  void write_dot(std::ostream& os) const
  {
    this->item().write_dot(os);
  }

  // Accessors.
  bool is_digraph() const { return this->item().is_digraph(); }
  bool is_strict() const { return this->item().is_strict(); }
  bool is_concentrate() const { return this->item().is_concentrate(); }
  RankDir get_rankdir() const { return this->item().get_rankdir(); }

  //---------------------------------------------------------------------------
  void add_graph(GraphTemplate<GraphGraph> const& graph) { this->item().add_graph(graph); }
  void add_node(Node const& node) { this->item().add_node(node); }
  void add_edge(Edge const& edge) { this->item().add_edge(edge); }

  template<ConceptIsTableGraphNode TGN>
  void add_table_graph_node(TableNodeTemplate<TGN> const& table_graph_node)
  {
    this->item().add_table_graph_node(table_graph_node);
  }

  // Add an arbitrary object to the graph that knows how to add itself.
  // This is just a more intuitive interface; now one can use `graph.add(obj)` instead of `obj.add_to_graph(graph_graph)`.
  template<ConceptHasAddToGraph T>
  void add(T& obj)
  {
    this->item().add(obj);
  }

  void add_node_attribute(Attribute&& attribute) { this->item().add_node_attribute(std::move(attribute)); }
  void add_edge_attribute(Attribute&& attribute) { this->item().add_edge_attribute(std::move(attribute)); }
};

static_assert(sizeof(Graph) == sizeof(GraphItemPtrTemplate<GraphGraph>), "Graph may not have any member variables of its own!");

class Digraph : public Graph {
 public:
  Digraph(bool strict = false) : Graph(true, strict) { }
};

} // namespace cppgraphviz::dot
