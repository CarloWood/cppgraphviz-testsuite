#include "sys.h"
#include "cppgraphviz/Array.h"
#include "cppgraphviz/RectangleNode.h"
#include "cppgraphviz/Class.h"
#include "cppgraphviz/IndexedContainerSet.h"
#include "cppgraphviz/dot/Graph.h"
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

  A(int m, std::weak_ptr<GraphTracker> const& root_graph COMMA_WHAT(std::string_view what)) :
    RectangleNode(root_graph COMMA_WHAT(what)), m_(m)
  {
    DoutEntering(dc::notice, "A(" << m << ", " << root_graph << ", \"" << what << "\") [" << this << "]");
  }

  // Used for members of a Class and/or elements of an Array-- in that case it should not be necessary to pass the root graph.
  A(int m COMMA_WHAT(std::string_view what)) : RectangleNode(WHAT(what)), m_(m)
  {
    DoutEntering(dc::notice, "A(" << m << ", \"" << what << "\") [" << this << "]");
  }

  // Copy constructor.
  A(A const& other COMMA_WHAT(std::string_view what)) : RectangleNode(other COMMA_WHAT(what)), m_(other.m_)
  {
    DoutEntering(dc::notice, "A(A const& " << &other << ", \"" << what << "\") [" << this << "]");
  }

  // Move constructor.
  A(A&& other COMMA_WHAT(std::string_view what)) : RectangleNode(std::move(other) COMMA_WHAT(what)), m_(other.m_)
  {
    DoutEntering(dc::notice, "A(A&& " << &other << ", \"" << what << "\") [" << this << "]");
  }

#ifdef CPPGRAPHVIZ_USE_WHAT
  A(A const& other) : A(other, "A(A const&) of " + other.get_what())
  {
    DoutEntering(dc::notice, "A(A const& " << &other << ") [" << this << "]");
  }

  A(A&& other) : A(std::move(other), "A(A&&) of " + other.get_what())
  {
    DoutEntering(dc::notice, "A(A&& " << &other << ") [" << this << "]");
  }
#endif

  ~A()
  {
    DoutEntering(dc::notice, "~A() [" << this << "]");
  }

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

  B(int m, std::weak_ptr<GraphTracker> const& root_graph COMMA_WHAT(std::string_view what)) :
    RectangleNode(root_graph COMMA_WHAT(what)), m_(m) { }

  // Used for members of some class-- in that case it should not be necessary to pass the root graph.
  B(int m COMMA_WHAT(std::string_view what)) : RectangleNode(WHAT(what)), m_(m) { }

  B(B const& other COMMA_WHAT(std::string_view what)) : RectangleNode(other COMMA_WHAT(what)), m_(other.m_) { }
  B(B&& other COMMA_WHAT(std::string_view what)) : RectangleNode(std::move(other) COMMA_WHAT(what)), m_(other.m_) { }

#ifdef CPPGRAPHVIZ_USE_WHAT
  B(B const& other) : B(other, "B(B const&) of " + other.get_what()) { }
  B(B&& other) : B(std::move(other), "B(B&&) of " + other.get_what()) { }
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
  C(int a, int b, std::weak_ptr<GraphTracker> const& root_graph COMMA_WHAT(std::string_view what)) :
    Class<C>(root_graph COMMA_WHAT(what)),
    a_(a COMMA_WHAT("C::a_")),
    b_(b COMMA_WHAT("C::b_"))
  {
    DoutEntering(dc::notice, "C(" << a << ", " << b << ", " << root_graph << ", \"" << what << "\") [" << this << "]");
    a_.set_label("a_");
    b_.set_label("b_");
  }

  // Copy constructor.
  C(C const& other COMMA_WHAT(std::string_view what)) :
    Class<C>(other COMMA_WHAT(what)),
    a_(other.a_ COMMA_WHAT("C::a_")),
    b_(other.b_ COMMA_WHAT("C::b_"))
  {
    DoutEntering(dc::notice, "C(C const& " << &other << WHAT(", \"" << what <<) "\") [" << this << "]");
  }

  // Move constructor.
  C(C&& other COMMA_WHAT(std::string_view what)) :
    Class<C>(std::move(other) COMMA_WHAT(what)),
    a_(std::move(other.a_) COMMA_WHAT("C::a_")),
    b_(std::move(other.b_) COMMA_WHAT("C::b_"))
  {
    DoutEntering(dc::notice, "C(C&& " << &other << WHAT(", \"" << what <<) "\") [" << this << "]");
  }

#ifdef CPPGRAPHVIZ_USE_WHAT
  C(C const& other) : C(other, "C(C const&) of " + other.get_what()) { }
  C(C&& other) : C(std::move(other), "C(C&&) of " + other.get_what()) { }
#endif

  ~C()
  {
    DoutEntering(dc::notice, "~C() [" << this << "]");
  }

 protected:
  void item_attributes(dot::AttributeList& list) override
  {
    list += {
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

// Specialize cppgraphviz::get_index_label.
template<>
std::string cppgraphviz::get_index_label<AIndex>()
{
  return "AIndex";
}

struct D : Class<D>
{
  cppgraphviz::Array<A, 3, AIndex> as_;
  B b_{1000 COMMA_WHAT("D::b_")};

  D(std::weak_ptr<GraphTracker> const& root_graph COMMA_WHAT(std::string_view what)) :
    Class<D>(root_graph COMMA_WHAT(what)),
    as_(root_graph, {
      {1 COMMA_WHAT("D::as_[0]")},
      {2 COMMA_WHAT("D::as_[1]")},
      {3 COMMA_WHAT("D::as_[2]")} }
      COMMA_WHAT(std::string("as_ of ") + std::string(what)))
  {
    as_.set_label("as_");
    b_.set_label("b_");
  }

  D(D const& other COMMA_WHAT(std::string_view what)) :
    Class<D>(other COMMA_WHAT(what)),
    as_(other.as_),
    b_(other.b_)
  {
  }

  D(D&& other COMMA_WHAT(std::string_view what)) :
    Class<D>(std::move(other) COMMA_WHAT(what)),
    as_(std::move(other.as_)),
    b_(std::move(other.b_))
  {
  }

 private:
  void item_attributes(dot::AttributeList& list) override
  {
    list += {
      {"color", "red"},
    };
    Class::item_attributes(list);
  }
};

int main()
{
  Debug(NAMESPACE_DEBUG::init());
  Debug(libcw_do.set_ostream(&std::cerr));
#if THREADSAFE_TRACK_UNLOCKED
  Debug(if (!dc::tracked.is_on()) dc::tracked.on());
#endif

  {
    Dout(dc::notice, "Constructing g0");
    Graph g0{WHAT("g0")};

    Dout(dc::notice, "Constructing as");
    cppgraphviz::Array<A, 3, AIndex> as(g0, { {20 COMMA_WHAT("as[0]")}, {21 COMMA_WHAT("as[1]")}, {22 COMMA_WHAT("as[2]")} } COMMA_WHAT("as"));
    as.set_label("as");

    cppgraphviz::Array<A, 3, AIndex> as2(std::move(as) COMMA_WHAT("as2"));
    as2.set_label("as2");

    as2[as2.ibegin() + 1].m_ += 100;

    Dout(dc::notice, "Constructing b");
    B b(20, g0 COMMA_WHAT("b"));
    b.set_label("b");

    {
      Dout(dc::notice, "Constructing c");
      C c(13, 42, g0 COMMA_WHAT("c"));
      c.set_label("c");

      A ca(c.a_ COMMA_WHAT("ca"));
      ca.set_label("ca");

      D d(g0 COMMA_WHAT("d"));
      d.set_label("d");

      D d2(std::move(d) COMMA_WHAT("d2"));
      d2.set_label("d2");
      d2.as_[d2.as_.ibegin() + 1].m_ += 100;

      Dout(dc::notice, "Constructing b2 from b");
      B b2(b COMMA_WHAT("b2"));
      b2.set_label("b2");
      B b2m(std::move(b2) COMMA_WHAT("b2m"));
      b2m.set_label("b2m");

      {
        Dout(dc::notice, "Constructing c2");
        C c2(c COMMA_WHAT("c2"));
        c2.set_label("c2");

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
