#pragma once

#include "Node.hpp"
#include "dot/Graph.hpp"
#include <type_traits>
#include <concepts>

namespace cppgraphviz {

template<typename T>
concept ConceptIsNode = std::is_base_of_v<Node, std::remove_reference_t<std::remove_pointer_t<T>>>;

class Class
{
 private:
  dot::Graph subgraph_;

 public:
  void initialize()
  {
    node_attributes(subgraph_.attribute_list());
  }

  void add_to_graph(dot::GraphData& graph_data) const
  {
    graph_data.add_graph(subgraph_);
  }

  virtual void node_attributes(dot::AttributeList& list)
  {
    std::string prev_style;
    if (list.has_key("style"))
    {
      prev_style = std::string{list.get_value("style")} + ",";
      list.remove("style");
    }
    list += {{"cluster", "true"}, {"style", prev_style + "rounded"}, {"label", "<Class>"}};
  }

  template<ConceptIsNode Node>
  void add_node_member(Node& node)
  {
    node.initialize();
    subgraph_.add_node(node.node());
  }

  template<ConceptIsNode... Nodes>
  void add_node_members(Nodes&&... nodes)
  {
    (add_node_member(std::forward<Nodes>(nodes)), ...);
  }

#if -0
  template<ConceptIsTableNode TableNode>
  void add_table_node_member(TableNode& table_node)
  {
    table_node.initialize();
    subgraph_.add_table_node(table_node.table_node());
  }

  template<ConceptIsTableNode... TableNodes>
  add_table_node_members(TableNodes&&... table_nodes)
  {
    (add_table_node_member(std::forward<TableNodes>(table_nodes)), ...);
  }
#endif
};

} // namespace cppgraphviz
