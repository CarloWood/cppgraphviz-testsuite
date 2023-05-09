#pragma once

#include "DotID.hpp"
#include "Node.hpp"
#include <vector>
#include <string>
#include <functional>

namespace cppgraphviz::dot {

class TableElement
{
 private:
  Node node_;

 public:
  TableElement(Node const& node) : node_(node) { }

  std::string label() const
  {
    return node_.data().attribute_list().get("label", "<no label>");
  }

  AttributeList const& attribute_list() const
  {
    return node_.data().attribute_list();
  }

  Node const& node() const
  {
    return node_;
  }
};

} // namespace cppgraphviz::dot