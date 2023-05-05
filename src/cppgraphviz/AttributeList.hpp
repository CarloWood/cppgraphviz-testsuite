#pragma once

#include "Attribute.hpp"
#include <iosfwd>
#include <vector>

namespace cppgraphviz {

class AttributeList
{
 private:
  std::vector<Attribute> attributes_;

 public:
  void add(Attribute const& attribute);

  void print_on(std::ostream& os) const;
};

inline std::ostream& operator<<(std::ostream& os, AttributeList const& attribute_list)
{
  attribute_list.print_on(os);
  return os;
}

} // namespace cppgraphviz
