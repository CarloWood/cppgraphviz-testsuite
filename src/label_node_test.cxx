#include "sys.h"
#include "cppgraphviz/Graph.hpp"
#include "cppgraphviz/RectangleNode.hpp"

using namespace cppgraphviz;

int main()
{
  Debug(NAMESPACE_DEBUG::init());
  Debug(libcw_do.set_ostream(&std::cerr));

  Graph g0("g0");

  RectangleNode n0(g0, "n0");

  Graph g1(std::move(g0), "g1");

  RectangleNode n1(n0, "n1");

  n0.set_label("n0");
  n1.set_label("n1");

  g1.write_dot(std::cout);
}
