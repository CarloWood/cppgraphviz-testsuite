#pragma once

#include "Node.hpp"

namespace cppgraphviz {

class LabelNode : public Node
{
 public:
  using Node::Node;

  void initialize() override
  {
    // Add the attributes of this LabelNode.
    node_attributes(node_tracker_->node_ptr().attribute_list());
  }

  virtual void node_attributes(dot::AttributeList& list)
  {
    // Derive from LabelNode and override node_attributes to add a label, shape etc.
    list.add({"label", "<unknown LabelNode>"});
  }
};

} // namespace cppgraphviz
