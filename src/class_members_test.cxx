#include "sys.h"
#include "cppgraphviz/Array.h"
#include "cppgraphviz/RectangleNode.h"
#include "cppgraphviz/Class.h"
#include "cppgraphviz/IndexedContainerSet.h"
#include "cppgraphviz/dot/Graph.h"
#include "utils/Array.h"
#include "debug.h"
#ifdef CWDEBUG
#include "cppgraphviz/debug_ostream_operators.h"
#endif

using namespace cppgraphviz;

#ifdef CPPGRAPHVIZ_USE_WHAT
#define WHAT(what) what
#define COMMA_WHAT(what) , what
#else
#define WHAT(what)
#define COMMA_WHAT(what)
#endif

struct A : RectangleNode
{
  int m_;

  A(int m, std::weak_ptr<GraphTracker> const& root_graph COMMA_WHAT(char const* what)) : RectangleNode(root_graph COMMA_WHAT(what)), m_(m) { }

  // Used for members of a class-- in that case it should not be necessary to pass the root graph.
  A(int m COMMA_WHAT(char const* what)) : RectangleNode(WHAT(what)), m_(m) { }

  // Copy constructor.
  A(A const& other COMMA_WHAT(char const* what)) : RectangleNode(other COMMA_WHAT(what)), m_(other.m_) { }
  // Move constructor.
  A(A&& other COMMA_WHAT(char const* what)) : RectangleNode(std::move(other) COMMA_WHAT(what)), m_(other.m_) { }

#ifdef CPPGRAPHVIZ_USE_WHAT
  A(A const& other) : A(other, "default A copy") { }
  A(A&& other) : A(std::move(other), "default A move") { }
#endif

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

  B(int m, std::weak_ptr<GraphTracker> const& root_graph COMMA_WHAT(char const* what)) : RectangleNode(root_graph COMMA_WHAT(what)), m_(m) { }

  // Used for members of some class-- in that case it should not be necessary to pass the root graph.
  B(int m COMMA_WHAT(char const* what)) : RectangleNode(WHAT(what)), m_(m) { }

  B(B const& other COMMA_WHAT(char const* what)) : RectangleNode(other COMMA_WHAT(what)), m_(other.m_) { }
  B(B&& other COMMA_WHAT(char const* what)) : RectangleNode(std::move(other) COMMA_WHAT(what)), m_(other.m_) { }

#ifdef CPPGRAPHVIZ_USE_WHAT
  B(B const& other) : B(other, "default B copy") { }
  B(B&& other) : B(std::move(other), "default B move") { }
#endif

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

  // Constructor.
  C(int a, int b, std::weak_ptr<GraphTracker> const& root_graph COMMA_WHAT(char const* what)) :
    Class<C>(root_graph COMMA_WHAT(what)),
    a_(a COMMA_WHAT("C::a_")),
    b_(b COMMA_WHAT("C::b_"))
  {
    DoutEntering(dc::notice, "C(" << a << ", " << b << ", " << root_graph << ", \"" << what << "\") [" << this << "]");
  }

  // Copy constructor.
  C(C const& other COMMA_WHAT(char const* what)) :
    Class<C>(other COMMA_WHAT(what)),
    a_(other.a_ COMMA_WHAT("C::a_")),
    b_(other.b_ COMMA_WHAT("C::b_"))
  {
    DoutEntering(dc::notice, "C(C const& " << &other << WHAT(", \"" << what <<) "\") [" << this << "]");
  }

  // Move constructor.
  C(C&& other COMMA_WHAT(char const* what)) :
    Class<C>(std::move(other) COMMA_WHAT(what)),
    a_(std::move(other.a_) COMMA_WHAT("C::a_")),
    b_(std::move(other.b_) COMMA_WHAT("C::b_"))
  {
    DoutEntering(dc::notice, "C(C&& " << &other << WHAT(", \"" << what <<) "\") [" << this << "]");
  }

#ifdef CPPGRAPHVIZ_USE_WHAT
  C(C const& other) : C(other, "default C copy") { }
  C(C&& other) : C(std::move(other), "default C move") { }
#endif

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

struct D : Class<D>
{
  utils::Array<A, 3, AIndex> as_ = {
    {1 COMMA_WHAT("D::as_[0]")},
    {2 COMMA_WHAT("D::as_[1]")},
    {3 COMMA_WHAT("D::as_[2]")} };
  B b_{1000 COMMA_WHAT("D::b_")};

  dot::TableNodePtr as_table_node_;
  IndexedContainerSet<AIndex>& as_container_set_;

  D(IndexedContainerSet<AIndex>& as_container_set, std::weak_ptr<GraphTracker> const& root_graph COMMA_WHAT(char const* what)) :
    Class<D>(root_graph COMMA_WHAT(what)), as_container_set_(as_container_set)
  {
    // as_ --> TableNode --> IndexedContainerSet --> Graph
    as_table_node_->add_attribute({"what", "D::as_table_node_"});
    as_table_node_->link_container(as_);
    as_container_set_.add_container(as_table_node_);
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

int main()
{
  Debug(NAMESPACE_DEBUG::init());
  Debug(libcw_do.set_ostream(&std::cerr));
#if TRACK_GRAPHDATA
  Debug(if (!dc::tracked.is_on()) dc::tracked.on());
#endif

  {
    Dout(dc::notice, "Constructing g0");
    Graph g0{WHAT("g0")};

    IndexedContainerSet<AIndex> container_set{WHAT("AIndex")};
    cppgraphviz::Array<A, 3, AIndex> as = { {20, g0 COMMA_WHAT("as[0]")}, {21, g0 COMMA_WHAT("as[1]")}, {22, g0 COMMA_WHAT("as[2]")} };

    Dout(dc::notice, "Constructing b");
    B b(20, g0 COMMA_WHAT("b"));

    {
      Dout(dc::notice, "Constructing c");
      C c(13, 42, g0 COMMA_WHAT("c"));

      A ca(c.a_ COMMA_WHAT("ca"));
      ca.set_label("ca");

      D d(container_set, g0 COMMA_WHAT("d"));
      g0.tracker().graph_ptr()->insert(container_set);

      Dout(dc::notice, "Constructing b2 from b");
      B b2(b COMMA_WHAT("b2"));
      b2.set_label("b2");
      B b2m(std::move(b2) COMMA_WHAT("b2m"));

      {
        Dout(dc::notice, "Constructing c2");
        C c2(c COMMA_WHAT("c2"));

        {
          Dout(dc::notice, "Constructing b3");
          B b3(b COMMA_WHAT("b3"));
          b3.set_label("b3");
          Dout(dc::notice, "Destructing b3");
        }

        {
          Dout(dc::notice, "Constructing b4");
          B b4(b COMMA_WHAT("b4"));
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
