#include "sys.h"
#include "debug.h"
#include "cppgraphviz/dot/Graph.hpp"
#include "cppgraphviz/dot/Node.hpp"
#include "cppgraphviz/dot/Edge.hpp"
#include "cppgraphviz/dot/TableNode.hpp"
#include <utils/Vector.h>
#include <iostream>

struct A : cppgraphviz::Node
{
  int m_;

  A(int m) : m_(m) { }
};

struct B : cppgraphviz::Node
{
  int m_;

  B(int m) : m_(m) { }
};

using AIndex = utils::VectorIndex<A>;

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

  cppgraphviz::Node n10;
  n10.add_attribute({"label", "n10"});

  cppgraphviz::Node n11;
  n11.add_attribute({"label", "n11"});
  n11.add_attribute({"color", "purple"});

  cppgraphviz::Edge e3(n10, n11);
  e3.add_attribute({"label", "e3"});

  cppgraphviz::Graph g1;
  g1.add_attribute({"label", "g1"});
  g1.add_node(n10);
  g1.add_node(n11);
  g1.add_edge(e3);

  g0.add_graph(g1);
  cppgraphviz::Edge e4;
  g0.add_edge(e4);
  e4.set_nodes(n2, n10);

  // node [style="filled", fillcolor="white"]
  g0.add_node_attribute({"style", "filled"});
  g0.add_node_attribute({"fillcolor", "white"});
  // graph [label="g1", style="rounded", fillcolor="white", bgcolor="lightgrey"]
  g1.add_attribute({"style", "rounded"});
  g1.add_attribute({"fillcolor", "gray"});
  g1.add_attribute({"bgcolor", "lightgrey"});

  cppgraphviz::Edge e5;
  g0.add_edge(e5);
  utils::Vector<A, AIndex> v1 = { {3}, {4}, {5} };
  std::array<char const*, 3> label = { "hello", "world", "foobar" };
  for (auto i = v1.ibegin(); i != v1.iend(); ++i)
    v1[i].add_attribute({"label", label[i.get_value()]});
  AIndex last = v1.iend() - 1;
  v1[last].add_attribute({"bgcolor", "yellow"});
  AIndex second = last - 1;

  cppgraphviz::TableNode tableA;
  tableA.add_attribute({"bgcolor", "lightblue"});
  tableA.add_attribute({"color", "purple"});
//  tableA.add_attribute({"fontname", "Arial"});
//  tableA.add_attribute({"fontsize", "18"});
  tableA.add_attribute({"fontcolor", "blue"});
  v1[second].add_attribute({"fontname", "Times New Roman"});
  v1[second].add_attribute({"fontsize", "12"});
  v1[second].add_attribute({"fontcolor", "red"});
  tableA.add_elements(v1);
  g0.add_table_node(tableA);
  e5.set_nodes(n11, tableA[1]);

  utils::Vector<B, AIndex> v2 = { {6}, {7}, {8} };
  std::array<char const*, 3> label2 = { "first", "second", "third" };
  cppgraphviz::TableNode tableB;
  tableB.add_elements(v2);
  for (auto i = v2.ibegin(); i != v2.iend(); ++i)
    v2[i].add_attribute({"label", label2[i.get_value()]});
  g0.add_table_node(tableB);

  g0.write_dot(std::cout);
}
