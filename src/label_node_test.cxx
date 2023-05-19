#include "sys.h"
#include "cppgraphviz/Graph.hpp"
#include "cppgraphviz/LabelNode.hpp"

using namespace cppgraphviz;

int main()
{
  Debug(NAMESPACE_DEBUG::init());
  Debug(libcw_do.set_ostream(&std::cerr));

  Graph g0("g0");

  LabelNode n0("n0");
  g0.add_node(n0);

  LabelNode n1(n0, "n1");

  g0.write_dot(std::cout);
}
