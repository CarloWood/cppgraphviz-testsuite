#include "sys.h"
#include "cppgraphviz/IndexedContainerSet.h"
#include <utils/Vector.h>
#include <utils/Array.h>
#include "debug.h"

using namespace cppgraphviz;

struct A : dot::NodePtr
{
  int m_;
  A(int m) : m_(m)
  {
    std::ostringstream oss;
    oss << "A:" << m << "";
    unlocked_type::wat{item()}->add_attribute({"label", oss.str()});
  }
};

struct B : dot::NodePtr
{
  int m_;
  B(int m) : m_(m)
  {
    std::ostringstream oss;
    oss << "B:" << m << "";
    unlocked_type::wat{item()}->add_attribute({"label", oss.str()});
  }
};

struct ACategory;
using AIndex = utils::VectorIndex<ACategory>;

int main()
{
  Debug(NAMESPACE_DEBUG::init());
  Debug(libcw_do.set_ostream(&std::cerr));

  utils::Vector<A, AIndex> container_of_A = { 2, 3, 5 };
  utils::Array<B, 3, AIndex> container_of_B = { 11, 13, 17 };

  dot::TableNodePtr table_A;
  dot::TableNodePtr table_B;

  {
    dot::TableNodePtr::unlocked_type::wat table_A_w{table_A.item()};
    table_A_w->add_attribute({"what", "table_A"});
    table_A_w->add_attribute({"what", "table_A"});
    table_A_w->copy_elements(container_of_A);
  }

  {
    dot::TableNodePtr::unlocked_type::wat table_B_w{table_B.item()};
    table_B_w->add_attribute({"what", "table_B"});
    table_B_w->link_container(container_of_B);
  }

  dot::DigraphPtr g0;
  dot::DigraphPtr::unlocked_type::wat g0_w{g0.item()};
  g0_w->add_attribute({"what", "g0"});
  g0_w->set_concentrate(true);

  IndexedContainerSet test1("AIndex", "test1");
  test1.add_container(table_A);
  test1.add_container(table_B);
  g0_w->insert(test1);

  dot::EdgePtr e1;
  {
    dot::EdgePtr::unlocked_type::wat e1_w{e1.item()};
    e1_w->add_attribute({"what", "e1"});
    g0_w->add(e1, e1_w);
    e1_w->set_nodes(table_A[1], table_B[2]);
  }

  g0_w->write_dot(std::cout);
}
