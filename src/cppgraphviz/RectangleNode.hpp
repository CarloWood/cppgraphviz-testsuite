#pragma once

#include "LabelNode.hpp"
#include <string>

namespace cppgraphviz {

struct RectangleNode : LabelNode
{
  std::string label_;

  using LabelNode::LabelNode;

  void node_attributes(dot::AttributeList& list) override
  {
    list += {"shape", "rectangle"};
    if (!label_.empty())
      list += {"label", label_};
    LabelNode::node_attributes(list);
  }

  void set_label(std::string const& label)
  {
    label_ = label;
  }
};

} // namespace cppgraphviz
