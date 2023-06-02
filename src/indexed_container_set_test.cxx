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
    item().add_attribute({"label", oss.str()});
  }
};

struct B : dot::NodePtr
{
  int m_;
  B(int m) : m_(m)
  {
    std::ostringstream oss;
    oss << "B:" << m << "";
    item().add_attribute({"label", oss.str()});
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
  table_A->add_attribute({"what", "table_A"});
  dot::TableNodePtr table_B;
  table_B->add_attribute({"what", "table_B"});

//  container_of_A.push_back(7);
  table_A->copy_elements(container_of_A);
  table_B->link_container(container_of_B);
//  container_of_B.push_back(19);

  dot::DigraphPtr g0;
  g0->add_attribute({"what", "g0"});
  g0->set_concentrate(true);

  IndexedContainerSet<AIndex> test1("AIndex", "test1");
  test1.add_container(table_A);
  test1.add_container(table_B);
  g0->insert(test1);

  dot::EdgePtr e1;
  e1->add_attribute({"what", "e1"});
  g0->add(e1);
  e1->set_nodes(table_A[1], table_B[2]);

  g0->write_dot(std::cout);
}
