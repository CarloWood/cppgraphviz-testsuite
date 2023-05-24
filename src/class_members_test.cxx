#include "sys.h"
#include "cppgraphviz/RectangleNode.hpp"
#include "cppgraphviz/Class.hpp"
#include "cppgraphviz/IndexedContainerSet.hpp"
#include "cppgraphviz/dot/Graph.hpp"
#include "utils/Array.h"
#include "debug.h"
#ifdef CWDEBUG
#include "cppgraphviz/debug_ostream_operators.hpp"
#endif

using namespace cppgraphviz;

struct A : RectangleNode
{
  int m_;

  A(int m, std::weak_ptr<GraphTracker> const& root_graph, char const* what) : RectangleNode(root_graph, what), m_(m) { }

  A(A const& other, char const* what) : RectangleNode(other, what), m_(other.m_) { }
  A(A&& other, char const* what) : RectangleNode(std::move(other), what), m_(other.m_) { }

  void item_attributes(dot::AttributeList& list) override
  {
    if (label_.empty())
      label_ = "A:" + std::to_string(m_);
    RectangleNode::item_attributes(list);
  }
};

struct B : RectangleNode
{
  int m_;

  B(int m, std::weak_ptr<GraphTracker> const& root_graph, char const* what) : RectangleNode(root_graph, what), m_(m) { }

  B(B const& other, char const* what) : RectangleNode(other, what), m_(other.m_) { }
  B(B&& other, char const* what) : RectangleNode(std::move(other), what), m_(other.m_) { }

  void item_attributes(dot::AttributeList& list) override
  {
    if (label_.empty())
      label_ = "B:" + std::to_string(m_);
    RectangleNode::item_attributes(list);
  }
};

struct C : Class<C>
{
  A a_;
  B b_;

  C(int a, int b, std::weak_ptr<GraphTracker> const& root_graph, char const* what) :
    Class<C>(root_graph, what), a_(a, root_graph, "C::a_"), b_(b, root_graph, "C::b_")
  {
    DoutEntering(dc::notice, "C(" << a << ", " << b << ", " << root_graph << ", \"" << what << "\") [" << this << "]");
  }

  C(C const& other, char const* what) : Class<C>(other, what),
    a_(other.a_, "C::a_"), b_(other.b_, "C::b_")
  {
    DoutEntering(dc::notice, "C(C const& " << &other << ", \"" << what << "\") [" << this << "]");
  }

  C(C&& other, char const* what) : Class<C>(std::move(other), what),
    a_(std::move(other.a_), "C::a_"), b_(std::move(other.b_), "C::b_")
  {
    DoutEntering(dc::notice, "C(C&& " << &other << ", \"" << what << "\") [" << this << "]");
  }

  ~C()
  {
    DoutEntering(dc::notice, "~C() [" << this << "]");
  }

 protected:
  void item_attributes(dot::AttributeList& list) override
  {
    list += {
      {"label", "C"},
      {"shape", "folder"},
      {"color", "blue"},
//      {"style", "filled"},
//      {"fillcolor", "yellow"},
    };
    Class::item_attributes(list);
  }
};

struct ACategory;
using AIndex = utils::ArrayIndex<ACategory>;

#if -0
struct D : Class<D>
{
  utils::Array<A, 3, AIndex> as_ = { {1, "D::as_[0]"}, {2, "D::as_[1]"}, {3, "D::as_[2]"} };
  B b_{1000, "D::b_"};

  TableNodePtr as_table_node_;
  IndexedContainerSet<AIndex>& as_container_set_;

  D(IndexedContainerSet<AIndex>& as_container_set, std::weak_ptr<GraphTracker> const& root_graph, char const* what) :
    Class<D>(root_graph, what), as_container_set_(as_container_set)
  {
    // as_ --> TableNode --> IndexedContainerSet --> Graph
    as_table_node_->link_container(as_);
    as_container_set_.add_container(as_table_node_);
    Class::add_table_node_member(as_table_node_);
    // Also add normal member b_.
    Class::add_node_member(b_);
  }

 private:
  void item_attributes(dot::AttributeList& list) override
  {
    list += {
      {"label", "D"},
      {"color", "red"},
    };
    Class::item_attributes(list);
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

  {
    Dout(dc::notice, "Constructing g0");
    Graph g0("g0");

    IndexedContainerSet<AIndex> container_set("AIndex");
    utils::Array<A, 3, AIndex> as = { {20, g0, "as[0]"}, {21, g0, "as[1]"}, {22, g0, "as[2]"} };

    Dout(dc::notice, "Constructing b");
    B b(20, g0, "b");

    {
      Dout(dc::notice, "Constructing c");
      C c(13, 42, g0, "c");

#if -0
      D d(container_set, g0, "d");

      Dout(dc::notice, "Constructing b2 from b");
      B b2(b, "b2");
      b2.set_label("b2");
      B b2m(std::move(b2), "b2m");
#endif

      {
        Dout(dc::notice, "Constructing c2");
        C c2(c, "c2");

        {
          Dout(dc::notice, "Constructing b3");
          B b3(b, "b3");
          b3.set_label("b3");
          Dout(dc::notice, "Destructing b3");
        }

        {
          Dout(dc::notice, "Constructing b4");
          B b4(b, "b4");
          b4.set_label("b4");
          Dout(dc::notice, "Destructing b4");
        }

        Dout(dc::notice, "Calling write_dot");
        g0.write_dot(std::cout);

        Dout(dc::notice, "Destructing c2");
      }
      Dout(dc::notice, "Destructing c");
    }
    Dout(dc::notice, "Destructing g0");
  }

  Dout(dc::notice, "Leaving main.");
}
