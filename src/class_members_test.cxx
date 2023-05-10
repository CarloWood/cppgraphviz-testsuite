#include "sys.h"
#include "cppgraphviz/Node.hpp"
#include "cppgraphviz/Class.hpp"
#include "cppgraphviz/dot/Graph.hpp"
#include "utils/Array.h"
#include "debug.h"

using namespace cppgraphviz;

struct RectangleNode : Node
{
  void node_attributes(dot::AttributeList& list) override
  {
    list += {"shape", "rectangle"};
  }
};

struct A : RectangleNode
{
  int m_;

  A(int m) : m_(m) { }

  void node_attributes(dot::AttributeList& list) override
  {
    list += {"label", "A:" + std::to_string(m_)};
    RectangleNode::node_attributes(list);
  }
};

struct B : RectangleNode
{
  int m_;

  B(int m) : m_(m) { }

  void node_attributes(dot::AttributeList& list) override
  {
    list += {{"label", "B:" + std::to_string(m_)}};
    RectangleNode::node_attributes(list);
  }
};

struct C : Class
{
  A a_;
  B b_;

  C(int a, int b) : a_(a), b_(b)
  {
    Class::add_node_members(a_, b_);
  }

 private:
  void node_attributes(dot::AttributeList& list) override
  {
    list += {
      {"label", "C"},
      {"shape", "folder"},
      {"color", "blue"},
//      {"style", "filled"},
//      {"fillcolor", "yellow"},
    };
    Class::node_attributes(list);
  }
};

#if -0
using AIndex = utils::ArrayIndex<A>;

struct D : Class
{
  utils::Array<A, 3, AIndex> as_ = { 1, 2, 3 };
  B b_{1000};

  D()
  {
    Class::add_table_node_members(as_);
    Class::add_node_member(b_);
  }

 private:
  void node_attributes(dot::AttributeList& list) override
  {
    list += {
      {"label", "D"},
      {"color", "red"},
    };
    Class::node_attributes(list);
  }
};
#endif

int main()
{
  Debug(NAMESPACE_DEBUG::init());

  std::array<A, 3> as = { 20, 21, 22 };
  for (auto& a : as) a.initialize();

  B b(20);
  b.initialize();

  C c(13, 42);
  c.initialize();

  dot::Graph g0;
  for (auto& a : as) g0.add(a);
  g0.add(b);
  g0.add(c);

  g0.write_dot(std::cout);
}
