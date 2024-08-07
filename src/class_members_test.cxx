#include "sys.h"
#include "cppgraphviz/Array.h"
#include "cppgraphviz/Vector.h"
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
  A(threadsafe::LockFinalCopy<A> other COMMA_WHAT(std::string_view what)) : RectangleNode(other COMMA_WHAT(what)), m_(other->m_)
  {
    DoutEntering(dc::notice, "A(A const& " << other.operator->() << ", \"" << what << "\") [" << this << "]");
  }
  A(A const& other COMMA_WHAT(std::string_view what)) : A(threadsafe::LockFinalCopy<A>{other} COMMA_WHAT(what)) { }

  // Move constructor.
  A(threadsafe::LockFinalMove<A> other COMMA_WHAT(std::string_view what)) : RectangleNode(std::move(other) COMMA_WHAT(what)), m_(other->m_)
  {
    DoutEntering(dc::notice, "A(A&& " << other.operator->() << ", \"" << what << "\") [" << this << "]");
  }
  A(A&& other COMMA_WHAT(std::string_view what)) : A(threadsafe::LockFinalMove<A>{std::move(other)} COMMA_WHAT(what)) { }

#ifdef CPPGRAPHVIZ_USE_WHAT
  A(threadsafe::LockFinalCopy<A> other) : A(other, "A(A const&) of " + other->get_what())
  {
    DoutEntering(dc::notice, "A(A const& " << other.operator->() << ") [" << this << "]");
  }
  A(A const& other) : A(threadsafe::LockFinalCopy<A>{other}) { }

  A(threadsafe::LockFinalMove<A> other) : A(std::move(other), "A(A&&) of " + other->get_what())
  {
    DoutEntering(dc::notice, "A(A&& " << other.operator->() << ") [" << this << "]");
  }
  A(A&& other) : A(threadsafe::LockFinalMove<A>{std::move(other)}) { }
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

  B(threadsafe::LockFinalCopy<B> other COMMA_WHAT(std::string_view what)) : RectangleNode(other COMMA_WHAT(what)), m_(other->m_) { }
  B(B const& other COMMA_WHAT(std::string_view what)) : B(threadsafe::LockFinalCopy<B>{other} COMMA_WHAT(what)) { }
  B(threadsafe::LockFinalMove<B> other COMMA_WHAT(std::string_view what)) : RectangleNode(std::move(other) COMMA_WHAT(what)), m_(other->m_) { }
  B(B&& other COMMA_WHAT(std::string_view what)) : B(threadsafe::LockFinalMove<B>{std::move(other)} COMMA_WHAT(what)) { }

#ifdef CPPGRAPHVIZ_USE_WHAT
  B(threadsafe::LockFinalCopy<B> other) : B(other, "B(B const&) of " + other->get_what()) { }
  B(B const& other) : B(threadsafe::LockFinalCopy<B>{other}) { }
  B(threadsafe::LockFinalMove<B> other) : B(std::move(other), "B(B&&) of " + other->get_what()) { }
  B(B&& other) : B(threadsafe::LockFinalMove<B>{std::move(other)}) { }
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
  C(threadsafe::LockFinalCopy<C> other COMMA_WHAT(std::string_view what)) :
    Class<C>(other COMMA_WHAT(what)),
    a_(other->a_ COMMA_WHAT("C::a_")),
    b_(other->b_ COMMA_WHAT("C::b_"))
  {
    DoutEntering(dc::notice, "C(C const& " << other.operator->() << WHAT(", \"" << what <<) "\") [" << this << "]");
  }
  C(C const& other COMMA_WHAT(std::string_view what)) : C(threadsafe::LockFinalCopy<C>{other} COMMA_WHAT(what)) { }

  // Move constructor.
  C(threadsafe::LockFinalMove<C> other COMMA_WHAT(std::string_view what)) :
    Class<C>(std::move(other) COMMA_WHAT(what)),
    a_(std::move(other->a_) COMMA_WHAT("C::a_")),
    b_(std::move(other->b_) COMMA_WHAT("C::b_"))
  {
    DoutEntering(dc::notice, "C(C&& " << other.operator->() << WHAT(", \"" << what <<) "\") [" << this << "]");
  }
  C(C&& other COMMA_WHAT(std::string_view what)) : C(threadsafe::LockFinalMove<C>{std::move(other)} COMMA_WHAT(what)) { }

#ifdef CPPGRAPHVIZ_USE_WHAT
  C(threadsafe::LockFinalCopy<C> other) : C(other, "C(C const&) of " + other->get_what()) { }
  C(C const& other) : C(threadsafe::LockFinalCopy<C>{other}) { }
  C(threadsafe::LockFinalMove<C> other) : C(std::move(other), "C(C&&) of " + other->get_what()) { }
  C(C&& other) : C(threadsafe::LockFinalMove<C>{std::move(other)}) { }
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

  D(threadsafe::LockFinalCopy<D> other COMMA_WHAT(std::string_view what)) :
    Class<D>(other COMMA_WHAT(what)),
    as_(other->as_),
    b_(other->b_)
  {
  }
  D(D const& other COMMA_WHAT(std::string_view what)) : D(threadsafe::LockFinalCopy<D>{other} COMMA_WHAT(what)) { }

  D(threadsafe::LockFinalMove<D> other COMMA_WHAT(std::string_view what)) :
    Class<D>(std::move(other) COMMA_WHAT(what)),
    as_(std::move(other->as_)),
    b_(std::move(other->b_))
  {
  }
  D(D&& other COMMA_WHAT(std::string_view what)) : D(threadsafe::LockFinalMove<D>{std::move(other)} COMMA_WHAT(what)) { }

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
  //Debug(if (!dc::tracked.is_on()) dc::tracked.on());
#endif

  {
    Dout(dc::notice, "Constructing g0");
    Graph g0{WHAT("g0")};

#if 0
    A item1(42, "item1");
    item1.set_label("item1");

    A item2(42, g0, "item2");
    item2.set_label("item2");

    A item3(item1);
    item3.set_label("item3");

    A item4(item2);
    item4.set_label("item4");

    cppgraphviz::Array<A, 2, AIndex> array1(g0, { {42, "array1[0]"}, {43, "array1[1]"} }, "array1");
    array1.set_label("array1");

    ASSERT(!A::rat{array1[AIndex{0}]}->parent_graph_tracker());

    cppgraphviz::Array<A, 3, AIndex> array2(g0, { item1, item2, array1[array1.ibegin()] }, "array2");
    array2.set_label("array2");

    ASSERT(!A::rat{array2[AIndex{0}]}->parent_graph_tracker());
    ASSERT(!A::rat{array2[AIndex{1}]}->parent_graph_tracker());
    ASSERT(!A::rat{array2[AIndex{2}]}->parent_graph_tracker());

    cppgraphviz::Array<A, 3, AIndex> array3(g0, { array2[array2.ibegin()], array2[array2.ibegin() + 1], array2[array2.ibegin() + 2] }, "array3");
    array3.set_label("array3");

    ASSERT(!A::rat{array3[AIndex{0}]}->parent_graph_tracker());
    ASSERT(!A::rat{array3[AIndex{1}]}->parent_graph_tracker());
    ASSERT(!A::rat{array3[AIndex{2}]}->parent_graph_tracker());

    cppgraphviz::Array<A, 2, AIndex> array4(array1);
    array4.set_label("array4");

    ASSERT(!A::rat{array4[AIndex{0}]}->parent_graph_tracker());
    ASSERT(!A::rat{array4[AIndex{1}]}->parent_graph_tracker());

    cppgraphviz::Array<A, 3, AIndex> array5(array2);
    array5.set_label("array5");

    ASSERT(!A::rat{array5[AIndex{0}]}->parent_graph_tracker());
    ASSERT(!A::rat{array5[AIndex{1}]}->parent_graph_tracker());
    ASSERT(!A::rat{array5[AIndex{2}]}->parent_graph_tracker());

    cppgraphviz::Array<A, 3, AIndex> array6(array3);
    array6.set_label("array6");

    ASSERT(!A::rat{array6[AIndex{0}]}->parent_graph_tracker());
    ASSERT(!A::rat{array6[AIndex{1}]}->parent_graph_tracker());
    ASSERT(!A::rat{array6[AIndex{2}]}->parent_graph_tracker());
#endif

    Dout(dc::notice, "Constructing as");
    cppgraphviz::Array<A, 3, AIndex> as(g0, { {20 COMMA_WHAT("as[0]")}, {21 COMMA_WHAT("as[1]")}, {22 COMMA_WHAT("as[2]")} } COMMA_WHAT("as"));
    as.set_label("as");

#if 0
    cppgraphviz::Array<A, 3, AIndex> as2(std::move(as) COMMA_WHAT("as2"));
    as2.set_label("as2");
#endif

    Dout(dc::notice, "Constructing va");
    cppgraphviz::Vector<A, AIndex> va(g0, { {30 COMMA_WHAT("va[0]")}, {31 COMMA_WHAT("va[1]")}, {32 COMMA_WHAT("va[2]")} } COMMA_WHAT("va"));
    va.set_label("va");

    for (int n = 33; n < 36; ++n)
    {
      std::string what = "va[" + std::to_string(n) + "]";
      va.push_back({n, what});
    }

    Dout(dc::notice, "Calling write_dot");
    g0.write_dot(std::cout);

#if 0
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
#endif
    Dout(dc::notice, "Destructing g0");
  }

  Dout(dc::notice, "Leaving main.");
}
