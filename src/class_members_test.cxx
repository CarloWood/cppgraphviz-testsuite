#include "sys.h"
#include "cppgraphviz/LabelNode.hpp"
#include "cppgraphviz/Class.hpp"
#include "cppgraphviz/IndexedContainerSet.hpp"
#include "cppgraphviz/dot/Graph.hpp"
#include "utils/Array.h"
#include "debug.h"

using namespace cppgraphviz;

struct RectangleNode : LabelNode
{
  std::string label_;

  void node_attributes(dot::AttributeList& list) override
  {
    list += {{"shape", "rectangle"}, {"label", label_}};
  }

  void set_label(std::string const& label)
  {
    label_ = label;
  }
};

struct A : RectangleNode
{
  int m_;

  A(int m) : m_(m) { }

  void node_attributes(dot::AttributeList& list) override
  {
    if (label_.empty())
      label_ = "A:" + std::to_string(m_);
    RectangleNode::node_attributes(list);
  }
};

struct B : RectangleNode
{
  int m_;

  B(int m) : m_(m) { }

  void node_attributes(dot::AttributeList& list) override
  {
    if (label_.empty())
      label_ = "B:" + std::to_string(m_);
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
struct ACategory;
using AIndex = utils::ArrayIndex<ACategory>;

struct D : Class
{
  utils::Array<A, 3, AIndex> as_ = { 1, 2, 3 };
  B b_{1000};

  TableNode as_table_node_;
  IndexedContainerSet<AIndex>& as_container_set_;

  D(IndexedContainerSet<AIndex>& as_container_set) : as_container_set_(as_container_set)
  {
    // as_ --> TableNode --> IndexContainerSet --> Graph
    as_table_node_->link_container(as_);
    as_container_set_.add_container(as_table_node_);
    Class::add_table_node_member(as_table_node_);
    // Also add normal member b_.
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
  Debug(libcw_do.set_ostream(&std::cerr));
#if TRACK_GRAPHDATA
  Debug(if (!dc::tracked.is_on()) dc::tracked.on());
#endif

#if -0
  IndexedContainerSet<AIndex> container_set("AIndex");

  utils::Array<A, 3, AIndex> as = { 20, 21, 22 };
  for (auto& a : as) a.initialize();
#endif

  Dout(dc::notice, "Constructing b4");
  B b(20);

  Dout(dc::notice, "Constructing c");
  C c(13, 42);

#if -0
  D d(container_set);
#endif

  Graph g0;

  Dout(dc::notice, "Constructing b2 from b");
  B b2(b);
  b2.set_label("b2");
  B b2m(std::move(b2));

  C c2(c);

  {
    Dout(dc::notice, "Constructing b3");
    B b3(b);
    b3.set_label("b3");
    //b3.initialize();
//    Dout(dc::notice, "Adding b3 to g0");
//    g0.add(b3);
    Dout(dc::notice, "Destructing b3");
  }
  g0->add(b);

  {
    Dout(dc::notice, "Constructing b4");
    B b4(b);
    b4.set_label("b4");
    //b4.initialize();
    Dout(dc::notice, "Destructing b4");
  }

//  for (auto& a : as) g0.add(a);
//  g0.add(d);
//  g0.add(container_set);
  Dout(dc::notice, "Adding c to g0");
  g0->add(c);

  Dout(dc::notice, "Calling initialize");
  b.initialize();       // Calls Node::initialize()
  c.initialize();       // Calls Class::initialize()
  b2m.initialize();      // Calls Node::initialize()
//  d.initialize();

  Dout(dc::notice, "Calling write_dot");
  g0->write_dot(std::cout);

  Dout(dc::notice, "Leaving main.");
}
