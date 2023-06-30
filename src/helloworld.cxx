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
  dot::DigraphPtr::unlocked_type::wat g0_w(g0.item());
  g0_w->add_attribute({"what", "g0"});

  dot::NodePtr n;
  {
    dot::NodePtr::unlocked_type::wat n_w(n.item());
    n_w->add_attribute({"what", "n"});

    //g0_w->add(n, n_w);

    n_w->add_attribute({"style", "filled"});
    n_w->add_attribute({"fillcolor", "green"});
    n_w->add_attribute({"shape", "hexagon"});
    n_w->add_attribute({"label", "n"});
  }
  g0_w->add(n);

  dot::NodePtr n2;
  {
    dot::NodePtr::unlocked_type::wat n2_w(n2.item());
    n2_w->add_attribute({"what", "n2"});
    n2_w->add_attribute({"label", "n2"});
  }
  g0_w->add(n2);

  dot::EdgePtr e1(n, n);
  {
    dot::EdgePtr::unlocked_type::wat e1_w(e1.item());
    e1_w->add_attribute({"what", "e1"});
    e1_w->add_attribute({"label", "e1"});
  }
  g0_w->add(e1);

  dot::EdgePtr e2(n2, n);
  {
    dot::EdgePtr::unlocked_type::wat e2_w(e2.item());
    e2_w->add_attribute({"what", "e2"});
    e2_w->add_attribute({"label", "e2"});
  }
  g0_w->add(e2);

  dot::NodePtr n10;
  {
    dot::NodePtr::unlocked_type::wat n10_w(n10.item());
    n10_w->add_attribute({"what", "n10"});
    n10_w->add_attribute({"label", "n10"});
  }

  dot::NodePtr n11;
  {
    dot::NodePtr::unlocked_type::wat n11_w(n11.item());
    n11_w->add_attribute({"what", "n11"});
    n11_w->add_attribute({"label", "n11"});
    n11_w->add_attribute({"color", "purple"});
  }

  dot::EdgePtr e3(n10, n11);
  {
    dot::EdgePtr::unlocked_type::wat e3_w(e3.item());
    e3_w->add_attribute({"what", "e3"});
    e3_w->add_attribute({"label", "e3"});
  }

  dot::GraphPtr g1;
  {
    dot::GraphPtr::unlocked_type::wat g1_w(g1.item());
    g1_w->add_attribute({"what", "g1"});
    g1_w->add_attribute({"label", "g1"});

    g1_w->add(n10);
    g1_w->add(n11);
    g1_w->add(e3);

    g0_w->add(g1, g1_w);

    dot::EdgePtr e4;
    dot::EdgePtr::unlocked_type::wat e4_w(e4.item());
    e4_w->add_attribute({"what", "e4"});
    g0_w->add(e4, e4_w);
    e4_w->set_nodes(n2, n10);

    // node [style="filled", fillcolor="white"]
    g0_w->add_node_attribute({"style", "filled"});
    g0_w->add_node_attribute({"fillcolor", "white"});
    // graph [label="g1", style="rounded", fillcolor="white", bgcolor="lightgrey"]
    g1_w->add_attribute({"style", "rounded"});
    g1_w->add_attribute({"fillcolor", "gray"});
    g1_w->add_attribute({"bgcolor", "lightgrey"});
  }

#if 1
  dot::EdgePtr e5;
  {
    dot::EdgePtr::unlocked_type::wat e5_w(e5.item());
    e5_w->add_attribute({"what", "e5"});
    g0_w->add(e5, e5_w);
  }

  utils::Vector<A, AIndex> v1 = { 3, 4, 5 };
  std::array<char const*, 3> label = { "hello", "world", "foobar" };
  for (auto i = v1.ibegin(); i != v1.iend(); ++i)
  {
    dot::NodePtr::unlocked_type::wat v1_i_w(v1[i].item());
    v1_i_w->add_attribute({"what", label[i.get_value()]});
    v1_i_w->add_attribute({"label", label[i.get_value()]});
  }
  AIndex last = v1.iend() - 1;
  {
    dot::NodePtr::unlocked_type::wat v1_last_w(v1[last].item());
    v1_last_w->add_attribute({"bgcolor", "yellow"});
  }
  AIndex second = last - 1;

  dot::TableNodePtr tableA;
  {
    dot::TableNodePtr::unlocked_type::wat tableA_w(tableA.item());
    tableA_w->add_attribute({"what", "tableA"});
    tableA_w->add_attribute({"bgcolor", "lightblue"});
    tableA_w->add_attribute({"color", "purple"});
  //  tableA_w->add_attribute({"fontname", "Arial"});
  //  tableA_w->add_attribute({"fontsize", "18"});
    tableA_w->add_attribute({"fontcolor", "blue"});
    dot::NodePtr::unlocked_type::wat v1_second_w(v1[second].item());
    v1_second_w->add_attribute({"fontname", "Times New Roman"});
    v1_second_w->add_attribute({"fontsize", "12"});
    v1_second_w->add_attribute({"fontcolor", "red"});
    tableA_w->link_container(v1);
  }

  g0_w->add(tableA);
  {
    dot::EdgePtr::unlocked_type::wat e5_w(e5.item());
    e5_w->set_nodes(n11, tableA[1]);
  }

  utils::Vector<B, AIndex> v2 = { {6}, {7}, {8} };
  std::array<char const*, 3> label2 = { "first", "second", "third" };
  dot::TableNodePtr tableB;
  {
    dot::TableNodePtr::unlocked_type::wat tableB_w(tableB.item());
    tableB_w->add_attribute({"what", "tableB"});
    tableB_w->link_container(v2);
  }
  for (auto i = v2.ibegin(); i != v2.iend(); ++i)
  {
    dot::NodePtr::unlocked_type::wat v2_i_w(v2[i].item());
    v2_i_w->add_attribute({"what", label2[i.get_value()]});
    v2_i_w->add_attribute({"label", label2[i.get_value()]});
  }

  g0_w->add(tableB);
#endif
  g0_w->write_dot(std::cout);
}
