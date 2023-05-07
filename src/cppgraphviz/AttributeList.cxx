#include "sys.h"
#include "AttributeList.hpp"
#include <iostream>

namespace cppgraphviz {

void AttributeList::add(Attribute const& attribute)
{
  attributes_.insert(attribute);
}

void AttributeList::print_on(std::ostream& os) const
{
  char const* prefix = "";
  for (Attribute const& attribute : attributes_)
  {
    os << prefix << attribute;
    prefix = ", ";
  }
}

} // namespace cppgraphviz
