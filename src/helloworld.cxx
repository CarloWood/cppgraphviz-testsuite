#include "sys.h"
#include "cppgraphviz/dot/Graph.h"
#include "cppgraphviz/dot/Node.h"
#include "cppgraphviz/dot/Edge.h"
#include "cppgraphviz/dot/TableNode.h"
#include "debug.h"
#include <utils/Vector.h>
#include <iostream>

namespace dot = cppgraphviz::dot;

struct A : dot::NodePtr
{
  int m_;

  A(int m) : m_(m) { }
};

struct B : dot::NodePtr
{
  int m_;

  B(int m) : m_(m) { }
};

using AIndex = utils::VectorIndex<A>;

int main()
{
  Debug(NAMESPACE_DEBUG::init());
  Debug(libcw_do.set_ostream(&std::cerr));

  dot::DigraphPtr g0;
  g0->add_attribute({"what", "g0"});

  dot::NodePtr n;
  n->add_attribute({"what", "n"});

  g0->add(n);

  n->add_attribute({"style", "filled"});
  n->add_attribute({"fillcolor", "green"});
  n->add_attribute({"shape", "hexagon"});

  dot::NodePtr n2;
  n2->add_attribute({"what", "n2"});

  n2->add_attribute({"label", "n2"});

  dot::EdgePtr e1(n, n);
  e1->add_attribute({"what", "e1"});

  dot::EdgePtr e2(n2, n);
  e2->add_attribute({"what", "e2"});

  g0->add(e1);
  g0->add(e2);

  e1->add_attribute({"label", "e1"});
  e2->add_attribute({"label", "e2"});
  n->add_attribute({"label", "n"});

  g0->add(n2);

  dot::NodePtr n10;
  n10->add_attribute({"what", "n10"});

  n10->add_attribute({"label", "n10"});

  dot::NodePtr n11;
  n11->add_attribute({"what", "n11"});

  n11->add_attribute({"label", "n11"});
  n11->add_attribute({"color", "purple"});

  dot::EdgePtr e3(n10, n11);
  e3->add_attribute({"what", "e3"});

  e3->add_attribute({"label", "e3"});

  dot::GraphPtr g1;
  g1->add_attribute({"what", "g1"});

  g1->add_attribute({"label", "g1"});
  g1->add(n10);
  g1->add(n11);
  g1->add(e3);

  g0->add(g1);
  dot::EdgePtr e4;
  e4->add_attribute({"what", "e4"});
  g0->add(e4);
  e4->set_nodes(n2, n10);

  // node [style="filled", fillcolor="white"]
  g0->add_node_attribute({"style", "filled"});
  g0->add_node_attribute({"fillcolor", "white"});
  // graph [label="g1", style="rounded", fillcolor="white", bgcolor="lightgrey"]
  g1->add_attribute({"style", "rounded"});
  g1->add_attribute({"fillcolor", "gray"});
  g1->add_attribute({"bgcolor", "lightgrey"});

  dot::EdgePtr e5;
  e5->add_attribute({"what", "e5"});
  g0->add(e5);
  utils::Vector<A, AIndex> v1 = { {3}, {4}, {5} };
  std::array<char const*, 3> label = { "hello", "world", "foobar" };
  for (auto i = v1.ibegin(); i != v1.iend(); ++i)
  {
    v1[i]->add_attribute({"what", label[i.get_value()]});
    v1[i]->add_attribute({"label", label[i.get_value()]});
  }
  AIndex last = v1.iend() - 1;
  v1[last]->add_attribute({"bgcolor", "yellow"});
  AIndex second = last - 1;

  dot::TableNodePtr tableA;
  tableA->add_attribute({"what", "tableA"});
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
  dot::TableNodePtr tableB;
  tableB->add_attribute({"what", "tableB"});
  tableB->link_container(v2);
  for (auto i = v2.ibegin(); i != v2.iend(); ++i)
  {
    v2[i]->add_attribute({"what", label2[i.get_value()]});
    v2[i]->add_attribute({"label", label2[i.get_value()]});
  }
  g0->add(tableB);

  g0->write_dot(std::cout);
}
