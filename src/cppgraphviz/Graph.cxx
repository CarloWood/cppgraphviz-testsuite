#include "sys.h"
#include "Graph.hpp"
#include <iostream>

namespace cppgraphviz {

//static
utils::UniqueIDContext<Graph::ID_type> Graph::s_unique_id_context;

void Graph::write_dot(std::ostream& os) const
{
  if (strict_)
    os << "strict ";
  if (digraph_)
    os << "di";
  os << "graph " << dot_id_ << " {\n";

  os << '}' << std::endl;
}

} // namespace cppgraphviz
