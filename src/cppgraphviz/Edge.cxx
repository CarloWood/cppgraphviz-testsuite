#include "sys.h"
#include "Edge.hpp"
#include "Node.hpp"

namespace cppgraphviz {

void EdgeData::set_nodes(Node const& from, Node const& to)
{
  from_ = from.data({}).id();
  to_ = to.data({}).id();
}

} // namespace cppgraphviz
