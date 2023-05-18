#pragma once

#include "Graph.hpp"
#include "Node.hpp"
#include <string_view>

namespace cppgraphviz {

class LabelNodeData : public dot::NodeItem
{
 public:
  using graph_item_type = GraphData;
};

class LabelNode : public Node, public dot::ItemPtrTemplate<LabelNodeData>
{
 public:
  LabelNode() = default;
  LabelNode(LabelNode&& label_node) = default;
  LabelNode(LabelNode const& label_node) : Node(label_node), dot::ItemPtrTemplate<LabelNodeData>() { copied(); }
  ~LabelNode() { destructed(); }

  void initialize() override
  {
    // Add the attributes of this LabelNode.
    node_attributes(attribute_list());
  }

  virtual void node_attributes(dot::AttributeList& list)
  {
    // Derive from LabelNode and override node_attributes to add a label, shape etc.
    list.add({"label", "<unknown LabelNode>"});
  }

 private:
  void add_to_graph_impl(GraphData* graph_data) override
  {
    graph_data->add(*this);
  }

  void remove_from_graph_impl(GraphData* graph_data) override
  {
    graph_data->remove_item(*this);
  }
};

} // namespace cppgraphviz
