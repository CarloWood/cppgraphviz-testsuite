#pragma once

#include "Node.hpp"

namespace cppgraphviz {

template<typename T>
class Class : public Node
{
 private:
  Graph subgraph_;

 public:
  Class(std::weak_ptr<GraphTracker> root_graph, char const* what) : Node(root_graph, what), subgraph_(root_graph, "Class::subgraph_")
  {
    // Class must be the first base class of T; therefore `this` should point to the beginning of T.
    Node::current_graph_linker_.start_new_subgraph_for({reinterpret_cast<char const*>(static_cast<T const*>(this)), sizeof(T)}, subgraph_);
  }

  Class(Class const& other, char const* what) : Node(other, what), subgraph_(other.subgraph_, "Class::subgraph_") { }
  Class(Class&& other, char const* what) : Node(std::move(other), what), subgraph_(std::move(other.subgraph_), "Class::subgraph_") { }
};

class LastClassMember
{
 public:
  LastClassMember()
  {
    Node::current_graph_linker_.end_subgraph();
  }
};

} // namespace cppgraphviz
