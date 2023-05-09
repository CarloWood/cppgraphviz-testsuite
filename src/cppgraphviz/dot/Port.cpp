#include "sys.h"
#include "Port.hpp"
#include "Node.hpp"
#include <iostream>

namespace cppgraphviz {

Port::Port(Node const& node) : id_(node.data().dot_id())
{
}

void Port::write_to(std::ostream& os) const
{
  os << id_;
  if (port_.has_value())
    os << ':' << port_.value();
}

} // namespace cppgraphviz
