#include "sys.h"
#include "Graph.hpp"
#include <iostream>
#include "debug.h"

namespace cppgraphviz::dot {

void GraphGraph::add_graph_graph(GraphGraph const* graph_graph)
{
  auto ibp = graphs_.try_emplace(graph_graph->dot_id(), graph_graph);
  // Do not add the same graph twice.
  ASSERT(ibp.second);
  ConstGraphItemPtrTemplate<GraphGraph>& subgraph = ibp.first->second;
  // The subgraph must be of the same type as this graph.
  subgraph.item().set_digraph(digraph_);
  subgraph.item().set_rankdir(rankdir_);
}

void GraphGraph::remove_graph_graph(GraphGraph const* graph_graph)
{
  bool erased = graphs_.erase(graph_graph->dot_id());
  // That's unexpected... we shouldn't be calling remove_graph unless it is there.
  ASSERT(erased);
}

void GraphGraph::add_graph_node(GraphNode const* graph_node)
{
  DoutEntering(dc::notice, "GraphGraph::add_graph_node(" << graph_node << ") [" << this << "]");
  auto ibp = nodes_.try_emplace(graph_node->dot_id(), graph_node);
  // Do not add the same node twice.
  ASSERT(ibp.second);
}

void GraphGraph::remove_graph_node(GraphNode const* graph_node)
{
  DoutEntering(dc::notice, "GraphGraph::remove_graph_node(" << graph_node << ") [" << this << "]");
  bool erased = nodes_.erase(graph_node->dot_id());
  // That's unexpected... we shouldn't be calling remove_node unless it is there.
  ASSERT(erased);
}

void GraphGraph::add_graph_edge(GraphEdge const* graph_edge)
{
  auto ibp = edges_.try_emplace(graph_edge->dot_id(), graph_edge);
  // Do not add the same edge twice.
  ASSERT(ibp.second);
}

void GraphGraph::remove_graph_edge(GraphEdge const* graph_edge)
{
  bool erased = edges_.erase(graph_edge->dot_id());
  // That's unexpected... we shouldn't be calling remove_edge unless it is there.
  ASSERT(erased);
}

void GraphGraph::add_table_graph_node(TableGraphNode const* table_graph_node)
{
  auto ibp = table_nodes_.try_emplace(table_graph_node->dot_id(), table_graph_node);
  // Do not add the same table_graph_node twice.
  ASSERT(ibp.second);
}

void GraphGraph::remove_table_graph_node(TableGraphNode const* table_graph_node)
{
  bool erased = table_nodes_.erase(table_graph_node->dot_id());
  // That's unexpected... we shouldn't be calling remove_table_graph_node unless it is there.
  ASSERT(erased);
}

void GraphGraph::add_node_attribute(Attribute&& attribute)
{
  node_attribute_list_.add(std::move(attribute));
}

void GraphGraph::add_edge_attribute(Attribute&& attribute)
{
  edge_attribute_list_.add(std::move(attribute));
}

void GraphGraph::set_digraph(bool digraph) const
{
  if (digraph_ != digraph)
  {
    digraph_ = digraph;
    // Recursively change all subgraphs.
    for (auto& graph_pair : graphs_)
    {
      ConstGraphItemPtrTemplate<GraphGraph> const& graph = graph_pair.second;
      GraphGraph const& graph_graph = graph.item();
      graph_graph.set_digraph(digraph);
    }
  }
}

void GraphGraph::set_rankdir(RankDir rankdir) const
{
  if (rankdir_ != rankdir)
  {
    rankdir_ = rankdir;
    // Recursively change all subgraphs.
    for (auto& graph_pair : graphs_)
    {
      ConstGraphItemPtrTemplate<GraphGraph> const& graph = graph_pair.second;
      GraphGraph const& graph_graph = graph.item();
      graph_graph.set_rankdir(rankdir);
    }
  }
}

void GraphGraph::write_dot(std::ostream& os) const
{
  // [ strict ] (graph | digraph) [ ID ] '{' stmt_list '}'
  if (strict_)
    os << "strict ";
  if (digraph_)
    os << "di";
  os << "graph " << dot_id() << " {\n";
  if (rankdir_ != TB)
  {
    os << "  rankdir=";
    if (rankdir_ == LR)
      os << "LR";
    else if (rankdir_ == BT)
      os << "BT";
    else
      os << "RL";
    os << '\n';
  }
  os << "  compound=true\n";
  if (concentrate_)
    os << "  concentrate=true\n";

  write_body_to(os);

  // Close the [di]graph.
  os << '}' << std::endl;
}

void GraphGraph::write_body_to(std::ostream& os, std::string indentation) const
{
  // stmt_list	:	[ stmt [ ';' ] stmt_list ]
  // stmt	:	node_stmt
  //            |       edge_stmt
  //            |       attr_stmt
  //            |       ID '=' ID
  //            |       subgraph

  indentation += "  ";

  // Write default attributes.
  if (attribute_list())
    os << indentation << "graph [" << attribute_list() << "]\n";
  if (node_attribute_list_)
    os << indentation << "node [" << node_attribute_list_ << "]\n";
  if (edge_attribute_list_)
    os << indentation << "edge [" << edge_attribute_list_ << "]\n";

  // Write all node_stmt's first.
  for (auto const& node_pair : nodes_)
  {
    ConstGraphItemPtrTemplate<GraphNode> const& node = node_pair.second;
    GraphNode const& graph_node = node.item();
    // node_stmt	:	node_id [ attr_list ]
    os << indentation << graph_node.dot_id() << " [" << graph_node.attribute_list() << "]\n";
  }

  // Write all tables.
  for (auto const& table_node_pair : table_nodes_)
  {
    ConstGraphItemPtrTemplate<TableGraphNode> const& table_node = table_node_pair.second;
    TableGraphNode const& table_graph_node = table_node.item();
    table_graph_node.write_html_to(os, indentation);
  }

  // Write all subgraph's.
  for (auto const& graph_pair : graphs_)
  {
    ConstGraphItemPtrTemplate<GraphGraph> const& graph = graph_pair.second;
    GraphGraph const& graph_graph = graph.item();
    os << indentation << "subgraph " << graph_graph.dot_id() << " {\n";
    graph_graph.write_body_to(os, indentation);
    os << indentation << "}\n";
  }

  // Write all edge_stmt's.
  for (auto const& edge_pair : edges_)
  {
    ConstGraphItemPtrTemplate<GraphEdge> const& edge = edge_pair.second;
    GraphEdge const& graph_edge = edge.item();
    // edge_stmt	:	(node_id | subgraph) edgeRHS [ attr_list ]
    // edgeRHS	:	edgeop (node_id | subgraph) [ edgeRHS ]
    os << indentation << graph_edge.from_port() << (digraph_ ? " -> " : " -- ") << graph_edge.to_port() <<
      " [" << graph_edge.attribute_list() << "]\n";
  }
}

} // namespace cppgraphviz::dot
