#include "sys.h"
#include "Graph.hpp"
#include <iostream>
#include "debug.h"

namespace cppgraphviz {

void Graph::add_node(Node const& node)
{
  NodeData const& node_data = node.data({});
  auto ibp = nodes_.try_emplace(node_data.id(), node);
  // Do not add the same node twice.
  ASSERT(ibp.second);
}

void Graph::add_edge(Edge const& edge)
{
  EdgeData const& edge_data = edge.data({});
  auto ibp = edges_.try_emplace(edge_data.id(), edge);
  // Do not add the same edge twice.
  ASSERT(ibp.second);
}

void Graph::write_dot(std::ostream& os) const
{
  // [ strict ] (graph | digraph) [ ID ] '{' stmt_list '}'
  if (strict_)
    os << "strict ";
  if (digraph_)
    os << "di";
  os << "graph " << dot_id_ << " {\n";

  // stmt_list	:	[ stmt [ ';' ] stmt_list ]
  // stmt	:	node_stmt
  //            |       edge_stmt
  //            |       attr_stmt
  //            |       ID '=' ID
  //            |       subgraph

  // Write all node_stmt's first.
  for (auto const& node_pair : nodes_)
  {
    Node const& node = node_pair.second;
    NodeData const& node_data = node.data({});
    // node_stmt	:	node_id [ attr_list ]
    os << "  " << node_data.id() << " [" << node_data.attribute_list() << "]\n";
  }

  // Write all edge_stmt's.
  for (auto const& edge_pair : edges_)
  {
    Edge const& edge = edge_pair.second;
    EdgeData const& edge_data = edge.data({});
    // edge_stmt	:	(node_id | subgraph) edgeRHS [ attr_list ]
    // edgeRHS	:	edgeop (node_id | subgraph) [ edgeRHS ]
    os << "  " << edge_data.from_id() << (digraph_ ? " -> " : " -- ") << edge_data.to_id() << " [" << edge_data.attribute_list() << "]\n";
  }

  // Close the [di]graph.
  os << '}' << std::endl;
}

} // namespace cppgraphviz
