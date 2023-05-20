#pragma once

#include "Node.hpp"

namespace cppgraphviz {

class LabelNode : public Node
{
 public:
  using Node::Node;

  void node_attributes(dot::AttributeList& list) override
  {
    // Derive from LabelNode and override node_attributes to add a label, shape etc.
    list.add({"label", "<unknown LabelNode>"});
    Node::node_attributes(list);
  }
};

} // namespace cppgraphviz
