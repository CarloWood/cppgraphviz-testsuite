#include "sys.h"
#include "Edge.hpp"
#include "Node.hpp"

namespace cppgraphviz {

void EdgeData::set_nodes(Port const& from, Port const& to)
{
  from_ = from;
  to_ = to;
}

} // namespace cppgraphviz
