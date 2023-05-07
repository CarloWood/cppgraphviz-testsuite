#pragma once

#include "Attribute.hpp"
#include <iosfwd>
#include <set>

namespace cppgraphviz {

class AttributeList
{
 private:
  std::set<Attribute> attributes_;

 public:
  void add(Attribute const& attribute);

  operator bool() const { return !attributes_.empty(); }

  void print_on(std::ostream& os) const;
};

inline std::ostream& operator<<(std::ostream& os, AttributeList const& attribute_list)
{
  attribute_list.print_on(os);
  return os;
}

} // namespace cppgraphviz
