#pragma once

#include "LabelNode.hpp"
#include "TableNode.hpp"
#include "Graph.hpp"
#include <type_traits>
#include <concepts>

namespace cppgraphviz {

template<typename T>
concept ConceptIsLabelNode = std::is_base_of_v<LabelNode, std::remove_reference_t<std::remove_pointer_t<T>>>;

template<typename T>
concept ConceptIsTableNode = std::is_base_of_v<TableNode, std::remove_reference_t<std::remove_pointer_t<T>>>;

class Class : public Node
{
 public:
  using data_type = GraphData;

 private:
  Graph subgraph_;

 private:
  void add_to_graph_impl(GraphData* graph_data) override
  {
    graph_data->add_graph(subgraph_);
  }

  void remove_from_graph_impl(GraphData* graph_data) override
  {
    graph_data->remove_graph(subgraph_);
  }

 public:
  Class() = default;
  Class(Class&& other) = default;
  Class(Class const& other) : Node(other) { copied(); }
  ~Class() { destructed(); }

  void initialize() override
  {
    // Add the attributes of subgraph_.
    node_attributes(subgraph_.attribute_list());
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

  template<ConceptIsLabelNode LN>
  void add_node_member(LN& label_node)
  {
    label_node.initialize();
    subgraph_.add(label_node);
  }

  template<ConceptIsLabelNode... LNs>
  void add_node_members(LNs&&... label_nodes)
  {
    (add_node_member(std::forward<LNs>(label_nodes)), ...);
  }

  template<ConceptIsTableNode TableNode>
  void add_table_node_member(TableNode& table_node)
  {
    table_node.initialize();
    subgraph_.add_table_node(table_node);
  }

  template<ConceptIsTableNode... TableNodes>
  void add_table_node_members(TableNodes&&... table_nodes)
  {
    (add_table_node_member(std::forward<TableNodes>(table_nodes)), ...);
  }
};

} // namespace cppgraphviz
