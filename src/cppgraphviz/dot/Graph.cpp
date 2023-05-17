#include "sys.h"
#include "Graph.hpp"
#include <iostream>
#include "debug.h"

namespace cppgraphviz::dot {

void GraphGraph::add_graph_item(GraphItem const* graph_item)
{
  auto ibp = items_.try_emplace(graph_item->dot_id(), graph_item);
  // Do not add the same graph item twice.
  ASSERT(ibp.second);
  if (graph_item->is_graph())
  {
    auto& subgraph = static_cast<ConstGraphItemPtrTemplate<GraphGraph>&>(ibp.first->second);
    // The subgraph must be of the same type as this graph.
    subgraph.item().set_digraph(digraph_);
    subgraph.item().set_rankdir(rankdir_);
  }
}

void GraphGraph::remove_graph_item(GraphItem const* graph_item)
{
  bool erased = items_.erase(graph_item->dot_id());
  // That's unexpected... we shouldn't be calling remove_graph_item unless it is there.
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
    for (auto& graph_pair : items_)
    {
      ConstGraphItemPtr const& graph_item_ptr = graph_pair.second;
      if (!graph_item_ptr.item().is_graph())
        continue;
      ConstGraphItemPtrTemplate<GraphGraph> const& graph = static_cast<ConstGraphItemPtrTemplate<GraphGraph> const&>(graph_item_ptr);
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
    for (auto& graph_pair : items_)
    {
      ConstGraphItemPtr const& graph_item_ptr = graph_pair.second;
      if (!graph_item_ptr.item().is_graph())
        continue;
      ConstGraphItemPtrTemplate<GraphGraph> const& graph = static_cast<ConstGraphItemPtrTemplate<GraphGraph> const&>(graph_item_ptr);
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

  std::map<item_type_type, std::string> output;

  // Write all items to the output map, sorting them by item_type.
  for (auto const& item_pair : items_)
  {
    std::ostringstream oss;
    if (digraph_)
      oss << digraph;
    GraphItem const& graph_item = item_pair.second.item();
    graph_item.write_dot_to(oss, indentation);
    auto ibp = output.try_emplace(graph_item.item_type());
    ibp.first->second.append(oss.str());
  }

  // Write output to os.
  for (auto item_type_string_pair : output)
    os << item_type_string_pair.second;
}

void GraphGraph::write_dot_to(std::ostream& os, std::string& indentation) const
{
  os << indentation << "subgraph " << dot_id() << " {\n";
  write_body_to(os, indentation);
  os << indentation << "}\n";
}

utils::iomanip::Index DigraphIomanip::s_index;
DigraphIomanip digraph;

} // namespace cppgraphviz::dot
