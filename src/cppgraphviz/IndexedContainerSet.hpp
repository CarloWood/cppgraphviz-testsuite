#pragma once

#include "dot/Graph.hpp"

namespace cppgraphviz {

// A cluster graph containing one or more indexed containers that use Index as index type.
template<typename Index>
class IndexedContainerSet
{
 private:
  dot::Graph outer_subgraph_;   // This subgraph wraps the inner subgraph.
  dot::Graph inner_subgraph_;   // This subgraph contains the dot::TableNode's that represent the indexed containers.

 private:
  void initialize()
  {
    outer_subgraph_.add_attribute({"cluster", "true"});
    outer_subgraph_.add_attribute({"style", "rounded"});
    outer_subgraph_.add_attribute({"color", "lightblue"});
    outer_subgraph_.add_graph(inner_subgraph_);
    inner_subgraph_.add_attribute({"cluster", "false"});
    dot::RankDir rankdir = outer_subgraph_.get_rankdir();
    if (rankdir == dot::TB || rankdir == dot::BT)
      inner_subgraph_.add_attribute({"rank", "same"});
  }

 public:
  IndexedContainerSet() { initialize(); }
  IndexedContainerSet(std::string const& label) { initialize(); set_label(label); }

  void set_label(std::string const& label)
  {
    outer_subgraph_.add_attribute({"label", label});
  }

  void add_container(dot::TableNode const& container)
  {
    inner_subgraph_.add_table_node(container);
  }

  void add_container(dot::TableNode&& container)
  {
    inner_subgraph_.add_table_node(std::move(container));
  }

  void add_to_graph(dot::GraphData& graph_data) const;
};

template<typename Index>
void IndexedContainerSet<Index>::add_to_graph(dot::GraphData& graph_data) const
{
  graph_data.add_graph(outer_subgraph_);
}

} // namespace cppgraphviz
