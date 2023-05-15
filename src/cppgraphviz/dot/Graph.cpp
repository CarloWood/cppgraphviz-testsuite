#include "sys.h"
#include "Graph.hpp"
#include <iostream>
#include "debug.h"

namespace cppgraphviz::dot {

void GraphData::add_graph(GraphData const* graph_data)
{
  auto ibp = graphs_.try_emplace(graph_data->dot_id(), graph_data);
  // Do not add the same graph twice.
  ASSERT(ibp.second);
  GraphItemPtr<GraphData const>& subgraph = ibp.first->second;
  // The subgraph must be of the same type as this graph.
  subgraph.data({}).set_digraph(digraph_);
  subgraph.data({}).set_rankdir(rankdir_);
}

void GraphData::add_node(NodeData const* node_data)
{
  DoutEntering(dc::notice, "GraphData::add_node(" << node_data << ") [" << this << "]");
  auto ibp = nodes_.try_emplace(node_data->dot_id(), node_data);
  // Do not add the same node twice.
  ASSERT(ibp.second);
}

void GraphData::remove_graph(GraphData const* graph_data)
{
  bool erased = graphs_.erase(graph_data->dot_id());
  // That's unexpected... we shouldn't be calling remove_graph unless it is there.
  ASSERT(erased);
}

void GraphData::remove_node(NodeData const* node_data)
{
  DoutEntering(dc::notice, "GraphData::remove_node(" << node_data << ") [" << this << "]");
  bool erased = nodes_.erase(node_data->dot_id());
  // That's unexpected... we shouldn't be calling remove_node unless it is there.
  ASSERT(erased);
}

void GraphData::remove_edge(EdgeData const* edge_data)
{
  bool erased = edges_.erase(edge_data->dot_id());
  // That's unexpected... we shouldn't be calling remove_edge unless it is there.
  ASSERT(erased);
}

void GraphData::remove_table_node(TableNodeData const* table_node_data)
{
  bool erased = table_nodes_.erase(table_node_data->dot_id());
  // That's unexpected... we shouldn't be calling remove_table_node unless it is there.
  ASSERT(erased);
}

void GraphData::add_edge(EdgeData const* edge_data)
{
  auto ibp = edges_.try_emplace(edge_data->dot_id(), edge_data);
  // Do not add the same edge twice.
  ASSERT(ibp.second);
}

void GraphData::add_table_node(TableNodeData const* table_node_data)
{
  auto ibp = table_nodes_.try_emplace(table_node_data->dot_id(), table_node_data);
  // Do not add the same table_node twice.
  ASSERT(ibp.second);
}

void GraphData::add_node_attribute(Attribute&& attribute)
{
  node_attribute_list_.add(std::move(attribute));
}

void GraphData::add_edge_attribute(Attribute&& attribute)
{
  edge_attribute_list_.add(std::move(attribute));
}

void GraphData::set_digraph(bool digraph) const
{
  if (digraph_ != digraph)
  {
    digraph_ = digraph;
    // Recursively change all subgraphs.
    for (auto& graph_pair : graphs_)
    {
      GraphItemPtr<GraphData const> const& graph = graph_pair.second;
      GraphData const& graph_data = graph.data({});
      graph_data.set_digraph(digraph);
    }
  }
}

void GraphData::set_rankdir(RankDir rankdir) const
{
  if (rankdir_ != rankdir)
  {
    rankdir_ = rankdir;
    // Recursively change all subgraphs.
    for (auto& graph_pair : graphs_)
    {
      GraphItemPtr<GraphData const> const& graph = graph_pair.second;
      GraphData const& graph_data = graph.data({});
      graph_data.set_rankdir(rankdir);
    }
  }
}

void GraphData::write_dot(std::ostream& os) const
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

void GraphData::write_body_to(std::ostream& os, std::string indentation) const
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
    GraphItemPtr<NodeData const> const& node = node_pair.second;
    NodeData const& node_data = node.data({});
    // node_stmt	:	node_id [ attr_list ]
    os << indentation << node_data.dot_id() << " [" << node_data.attribute_list() << "]\n";
  }

  // Write all tables.
  for (auto const& table_node_pair : table_nodes_)
  {
    GraphItemPtr<TableNodeData const> const& table_node = table_node_pair.second;
    TableNodeData const& table_node_data = table_node.data({});
    table_node_data.write_html_to(os, indentation);
  }

  // Write all subgraph's.
  for (auto const& graph_pair : graphs_)
  {
    GraphItemPtr<GraphData const> const& graph = graph_pair.second;
    GraphData const& graph_data = graph.data({});
    os << indentation << "subgraph " << graph_data.dot_id() << " {\n";
    graph_data.write_body_to(os, indentation);
    os << indentation << "}\n";
  }

  // Write all edge_stmt's.
  for (auto const& edge_pair : edges_)
  {
    GraphItemPtr<EdgeData const> const& edge = edge_pair.second;
    EdgeData const& edge_data = edge.data({});
    // edge_stmt	:	(node_id | subgraph) edgeRHS [ attr_list ]
    // edgeRHS	:	edgeop (node_id | subgraph) [ edgeRHS ]
    os << indentation << edge_data.from_port() << (digraph_ ? " -> " : " -- ") << edge_data.to_port() <<
      " [" << edge_data.attribute_list() << "]\n";
  }
}

} // namespace cppgraphviz::dot
