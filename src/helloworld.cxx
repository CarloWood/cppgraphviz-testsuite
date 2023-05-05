#include "sys.h"
#include "debug.h"
#include "cppgraphviz/Graph.hpp"
#include "cppgraphviz/Node.hpp"
#include "cppgraphviz/Edge.hpp"
#include <iostream>

int main()
{
  Debug(NAMESPACE_DEBUG::init());

  cppgraphviz::Digraph g0;
  cppgraphviz::Node n;

  g0.add_node(n);

  n.add_attribute({"style", "filled"});
  n.add_attribute({"fillcolor", "green"});
  n.add_attribute({"shape", "hexagon"});

  cppgraphviz::Node n2;
  n2.add_attribute({"label", "n2"});
  cppgraphviz::Edge e1(n, n);
  cppgraphviz::Edge e2(n2, n);

  g0.add_edge(e1);
  g0.add_edge(e2);

  e1.add_attribute({"label", "e1"});
  e2.add_attribute({"label", "e2"});
  n.add_attribute({"label", "n"});

  g0.add_node(n2);

  g0.write_dot(std::cout);
}
