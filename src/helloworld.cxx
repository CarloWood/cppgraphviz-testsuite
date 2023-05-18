#include "sys.h"
#include "cppgraphviz/dot/Graph.hpp"
#include "cppgraphviz/dot/Node.hpp"
#include "cppgraphviz/dot/Edge.hpp"
#include "cppgraphviz/dot/TableNode.hpp"
#include "debug.h"
#include <utils/Vector.h>
#include <iostream>

namespace dot = cppgraphviz::dot;

struct A : dot::Node
{
  int m_;

  A(int m) : m_(m) { }
};

struct B : dot::Node
{
  int m_;

  B(int m) : m_(m) { }
};

using AIndex = utils::VectorIndex<A>;

int main()
{
  Debug(NAMESPACE_DEBUG::init());
  Debug(libcw_do.set_ostream(&std::cerr));

  dot::Digraph g0;
  dot::Node n;

  g0->add(n);

  n->add_attribute({"style", "filled"});
  n->add_attribute({"fillcolor", "green"});
  n->add_attribute({"shape", "hexagon"});

  dot::Node n2;
  n2->add_attribute({"label", "n2"});
  dot::Edge e1(n, n);
  dot::Edge e2(n2, n);

  g0->add(e1);
  g0->add(e2);

  e1->add_attribute({"label", "e1"});
  e2->add_attribute({"label", "e2"});
  n->add_attribute({"label", "n"});

  g0->add(n2);

  dot::Node n10;
  n10->add_attribute({"label", "n10"});

  dot::Node n11;
  n11->add_attribute({"label", "n11"});
  n11->add_attribute({"color", "purple"});

  dot::Edge e3(n10, n11);
  e3->add_attribute({"label", "e3"});

  dot::Graph g1;
  g1->add_attribute({"label", "g1"});
  g1->add(n10);
  g1->add(n11);
  g1->add(e3);

  g0->add(g1);
  dot::Edge e4;
  g0->add(e4);
  e4->set_nodes(n2, n10);

  // node [style="filled", fillcolor="white"]
  g0->add_node_attribute({"style", "filled"});
  g0->add_node_attribute({"fillcolor", "white"});
  // graph [label="g1", style="rounded", fillcolor="white", bgcolor="lightgrey"]
  g1->add_attribute({"style", "rounded"});
  g1->add_attribute({"fillcolor", "gray"});
  g1->add_attribute({"bgcolor", "lightgrey"});

  dot::Edge e5;
  g0->add(e5);
  utils::Vector<A, AIndex> v1 = { {3}, {4}, {5} };
  std::array<char const*, 3> label = { "hello", "world", "foobar" };
  for (auto i = v1.ibegin(); i != v1.iend(); ++i)
    v1[i]->add_attribute({"label", label[i.get_value()]});
  AIndex last = v1.iend() - 1;
  v1[last]->add_attribute({"bgcolor", "yellow"});
  AIndex second = last - 1;

  dot::TableNode tableA;
  tableA->add_attribute({"bgcolor", "lightblue"});
  tableA->add_attribute({"color", "purple"});
//  tableA->add_attribute({"fontname", "Arial"});
//  tableA->add_attribute({"fontsize", "18"});
  tableA->add_attribute({"fontcolor", "blue"});
  v1[second]->add_attribute({"fontname", "Times New Roman"});
  v1[second]->add_attribute({"fontsize", "12"});
  v1[second]->add_attribute({"fontcolor", "red"});
  tableA->link_container(v1);
  g0->add(tableA);
  e5->set_nodes(n11, tableA[1]);

  utils::Vector<B, AIndex> v2 = { {6}, {7}, {8} };
  std::array<char const*, 3> label2 = { "first", "second", "third" };
  dot::TableNode tableB;
  tableB->link_container(v2);
  for (auto i = v2.ibegin(); i != v2.iend(); ++i)
    v2[i]->add_attribute({"label", label2[i.get_value()]});
  g0->add(tableB);

  g0->write_dot(std::cout);
}
