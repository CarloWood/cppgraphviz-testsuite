#include "sys.h"
#include "debug.h"
#include "cppgraphviz/Graph.hpp"
#include <iostream>

int main()
{
  Debug(NAMESPACE_DEBUG::init());

  cppgraphviz::Digraph g0;
  cppgraphviz::Graph g1;
  g1.set_strict();

  g0.write_dot(std::cout);
  g1.write_dot(std::cout);
}
