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
concept ConceptIsGraphData = std::is_base_of_v<GraphData, T>;

template<ConceptIsGraphData GD>
class GraphTemplate;
class GraphData;
using Graph = GraphTemplate<GraphData>;

// GraphData is derived from GraphItem for its id and (optional) attribute list.
// A graph can be directional or not, and/or strict (only allowing one edge between
// nodes (two for a digraph)).
//
// A graph contains a list of nodes, edges and (optionally) other (sub)graphs.
class GraphData : public GraphItem
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
  std::map<DotID_type, GraphItemPtr<GraphData const>> graphs_;
  // The list of all nodes of this graph, by ID.
  std::map<DotID_type, GraphItemPtr<NodeData const>> nodes_;
  // The list of all edges of this graph, by ID.
  std::map<DotID_type, GraphItemPtr<EdgeData const>> edges_;
  // The list of all "table nodes", by ID;
  std::map<DotID_type, GraphItemPtr<TableNodeData const>> table_nodes_;

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
  void add_graph(GraphData const* graph_data);
  void add_node(NodeData const* node_data);
  void add_edge(EdgeData const* edge_data);
  void add_table_node(TableNodeData const* table_node_data);

  void remove_graph(GraphData const* graph_data);
  void remove_node(NodeData const* node_data);
  void remove_edge(EdgeData const* edge_data);
  void remove_table_node(TableNodeData const* table_node_data);

  template<ConceptIsGraphData GD>
  void add_graph(GraphItemPtr<GD> const& graph)
  {
    add_graph(&graph.data({}));
  }

  template<ConceptIsNodeData ND>
  void add_node(GraphItemPtr<ND> const& node)
  {
    add_node(&node.data({}));
  }

  template<ConceptIsEdgeData ED>
  void add_edge(GraphItemPtr<ED> const& edge)
  {
    add_edge(&edge.data({}));
  }

  template<ConceptIsTableNodeData TND>
  void add_table_node(GraphItemPtr<TND> const& table_node)
  {
    add_table_node(&table_node.data({}));
  }

  template<ConceptIsGraphData GD>
  void remove_graph(GraphItemPtr<GD> const& graph)
  {
    remove_graph(&graph.data({}));
  }

  template<ConceptIsNodeData ND>
  void remove_node(GraphItemPtr<ND> const& node)
  {
    remove_node(&node.data({}));
  }

  template<ConceptIsEdgeData ED>
  void remove_edge(GraphItemPtr<ED> const& edge)
  {
    remove_edge(&edge.data({}));
  }

  template<ConceptIsTableNodeData TND>
  void remove_table_node(GraphItemPtr<TND> const& table_node)
  {
    remove_table_node(&table_node.data({}));
  }

  template<typename T>
  void add(T& obj)
  {
    obj.add_to_graph(*static_cast<typename T::data_type::graph_data_type*>(this));
  }

  void add_node_attribute(Attribute&& attribute);
  void add_edge_attribute(Attribute&& attribute);
};

template<typename T>
concept ConceptHasAddToGraph = requires(T obj)
{
  obj.add_to_graph(std::declval<typename T::data_type::graph_data_type&>());
};

template<ConceptIsGraphData GD>
class GraphTemplate : public GraphItemPtr<GD>
{
 public:
  GraphTemplate(bool strict = false)
  {
    this->data().set_strict(strict);
  }

 protected:
  GraphTemplate(bool digraph, bool strict = false)
  {
    this->data().set_digraph(digraph);
    this->data().set_strict(strict);
  }

 public:
  //---------------------------------------------------------------------------
  void set_digraph(bool digraph = true) { this->data().set_digraph(digraph); }
  void set_rankdir(RankDir rankdir) { this->data().set_rankdir(rankdir); }
  void set_strict(bool strict = true) { this->data().set_strict(strict); }
  void set_concentrate(bool concentrate = true) { this->data().set_concentrate(concentrate); }

  // Write graph to os in dot format.
  void write_dot(std::ostream& os) const
  {
    this->data().write_dot(os);
  }

  // Accessors.
  bool is_digraph() const { return this->data().is_digraph(); }
  bool is_strict() const { return this->data().is_strict(); }
  bool is_concentrate() const { return this->data().is_concentrate(); }
  RankDir get_rankdir() const { return this->data().get_rankdir(); }

  //---------------------------------------------------------------------------
  void add_graph(GraphTemplate<GraphData> const& graph) { this->data().add_graph(graph); }
  void add_node(Node const& node) { this->data().add_node(node); }
  void add_edge(Edge const& edge) { this->data().add_edge(edge); }

  template<ConceptIsTableNodeData TND>
  void add_table_node(TableNodeTemplate<TND> const& table_node)
  {
    this->data().add_table_node(table_node);
  }

  // Add an arbitrary object to the graph that knows how to add itself.
  // This is just a more intuitive interface; now one can use `graph.add(obj)` instead of `obj.add_to_graph(graph_data)`.
  template<ConceptHasAddToGraph T>
  void add(T& obj)
  {
    this->data().add(obj);
  }

  void add_node_attribute(Attribute&& attribute) { this->data().add_node_attribute(std::move(attribute)); }
  void add_edge_attribute(Attribute&& attribute) { this->data().add_edge_attribute(std::move(attribute)); }
};

static_assert(sizeof(Graph) == sizeof(GraphItemPtr<GraphData>), "Graph may not have any member variables of its own!");

class Digraph : public Graph {
 public:
  Digraph(bool strict = false) : Graph(true, strict) { }
};

} // namespace cppgraphviz::dot
