#include "sys.h"
#include "Graph.hpp"
#include <iostream>
#include "debug.h"

namespace cppgraphviz {

void GraphData::add_graph(Graph const& graph)
{
  GraphData const& graph_data = graph.data({});
  auto ibp = graphs_.try_emplace(graph_data.id(), graph);
  // Do not add the same graph twice.
  ASSERT(ibp.second);
  GraphItem<GraphData>& subgraph = ibp.first->second;
  // The subgraph must be of the same type as this graph.
  subgraph.data({}).set_digraph(digraph_);
}

void GraphData::add_node(Node const& node)
{
  NodeData const& node_data = node.data({});
  auto ibp = nodes_.try_emplace(node_data.id(), node);
  // Do not add the same node twice.
  ASSERT(ibp.second);
}

void GraphData::add_edge(Edge const& edge)
{
  EdgeData const& edge_data = edge.data({});
  auto ibp = edges_.try_emplace(edge_data.id(), edge);
  // Do not add the same edge twice.
  ASSERT(ibp.second);
}

void GraphData::add_node_attribute(Attribute const& attribute)
{
  node_attribute_list_.add(attribute);
}

void GraphData::add_edge_attribute(Attribute const& attribute)
{
  edge_attribute_list_.add(attribute);
}

void GraphData::set_digraph(bool digraph)
{
  if (digraph_ != digraph)
  {
    digraph_ = digraph;
    // Recursively change all subgraphs.
    for (auto& graph_pair : graphs_)
    {
      GraphItem<GraphData>& graph = graph_pair.second;
      GraphData& graph_data = graph.data({});
      graph_data.set_digraph(digraph);
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
  os << "graph " << id() << " {\n";

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
    GraphItem<NodeData> const& node = node_pair.second;
    NodeData const& node_data = node.data({});
    // node_stmt	:	node_id [ attr_list ]
    os << indentation << node_data.id() << " [" << node_data.attribute_list() << "]\n";
  }

  // Write all subgraph's.
  for (auto const& graph_pair : graphs_)
  {
    GraphItem<GraphData> const& graph = graph_pair.second;
    GraphData const& graph_data = graph.data({});
    os << indentation << "subgraph cluster_" << graph_data.id() << " {\n";
    graph_data.write_body_to(os, indentation);
    os << indentation << "}\n";
  }

  // Write all edge_stmt's.
  for (auto const& edge_pair : edges_)
  {
    GraphItem<EdgeData> const& edge = edge_pair.second;
    EdgeData const& edge_data = edge.data({});
    // edge_stmt	:	(node_id | subgraph) edgeRHS [ attr_list ]
    // edgeRHS	:	edgeop (node_id | subgraph) [ edgeRHS ]
    os << indentation << edge_data.from_id() << (digraph_ ? " -> " : " -- ") << edge_data.to_id() << " [" << edge_data.attribute_list() << "]\n";
  }
}

} // namespace cppgraphviz
