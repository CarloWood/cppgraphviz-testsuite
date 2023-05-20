#include "sys.h"
#include "cppgraphviz/RectangleNode.hpp"
#include "cppgraphviz/Class.hpp"
#include "cppgraphviz/IndexedContainerSet.hpp"
#include "cppgraphviz/dot/Graph.hpp"
#include "utils/Array.h"
#include "debug.h"

using namespace cppgraphviz;

struct A : RectangleNode
{
  int m_;

  A(int m, std::weak_ptr<GraphTracker> root_graph, char const* what) : RectangleNode(root_graph, what), m_(m) { }

  A(A const& other, char const* what) : RectangleNode(other, what), m_(other.m_) { }
  A(A&& other, char const* what) : RectangleNode(std::move(other), what), m_(other.m_) { }

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

  B(int m, std::weak_ptr<GraphTracker> root_graph, char const* what) : RectangleNode(root_graph, what), m_(m) { }

  B(B const& other, char const* what) : RectangleNode(other, what), m_(other.m_) { }
  B(B&& other, char const* what) : RectangleNode(std::move(other), what), m_(other.m_) { }

  void node_attributes(dot::AttributeList& list) override
  {
    if (label_.empty())
      label_ = "B:" + std::to_string(m_);
    RectangleNode::node_attributes(list);
  }
};

struct C : Class<C>
{
  A a_;
  B b_;
  LastClassMember lcm_;

  C(int a, int b, std::weak_ptr<GraphTracker> root_graph, char const* what) :
    Class<C>(root_graph, what), a_(a, root_graph, "C::a_"), b_(b, root_graph, "C::b_") { }

  C(C const& other, char const* what) : Class<C>(other, what), a_(other.a_, "C::a_"), b_(other.b_, "C::b_") { }
  C(C&& other, char const* what) : Class<C>(std::move(other), what), a_(std::move(other.a_), "C::a_"), b_(std::move(other.b_), "C::b_") { }

 protected:
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

struct D : Class<D>
{
  utils::Array<A, 3, AIndex> as_ = { {1, "D::as_[0]"}, {2, "D::as_[1]"}, {3, "D::as_[2]"} };
  B b_{1000, "D::b_"};

  TableNodePtr as_table_node_;
  IndexedContainerSet<AIndex>& as_container_set_;

  LastClassMember lcm_;

  D(IndexedContainerSet<AIndex>& as_container_set) : as_container_set_(as_container_set)
  {
    // as_ --> TableNode --> IndexedContainerSet --> Graph
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

  Graph g0("g0");

#if -0
  IndexedContainerSet<AIndex> container_set("AIndex");
  utils::Array<A, 3, AIndex> as = { {20, "as[0]"}, {21, "as[1]"}, {22, "as[2]"} };
#endif

  Dout(dc::notice, "Constructing b4");
  B b(20, g0, "b");

  Dout(dc::notice, "Constructing c");
  C c(13, 42, g0, "c");

#if -0
  D d(container_set, g0, "d");
#endif

  Dout(dc::notice, "Constructing b2 from b");
  B b2(b, "b2");
  b2.set_label("b2");
  B b2m(std::move(b2), "b2m");

  C c2(c, "c2");

  {
    Dout(dc::notice, "Constructing b3");
    B b3(b, "b3");
    b3.set_label("b3");
    //b3.initialize();
//    Dout(dc::notice, "Adding b3 to g0");
//    g0.insert(b3);
    Dout(dc::notice, "Destructing b3");
  }

  {
    Dout(dc::notice, "Constructing b4");
    B b4(b, "b4");
    b4.set_label("b4");
    //b4.initialize();
    Dout(dc::notice, "Destructing b4");
  }

//  for (auto& a : as) g0.add(a);
//  g0.insert(d);
//  g0.insert(container_set);
  Dout(dc::notice, "Adding c to g0");

  Dout(dc::notice, "Calling write_dot");
  g0.write_dot(std::cout);

  Dout(dc::notice, "Leaving main.");
}
