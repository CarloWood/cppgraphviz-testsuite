#pragma once

namespace cppgraphviz {

class Node;

// See comment above GraphData::node_trackers_.
class NodeTracker
{
 private:
  Node* node_;

 public:
  explicit NodeTracker(Node* node) : node_(node) { }

  void set_node(Node* node)
  {
    node_ = node;
  }

  operator Node&() const
  {
    return *node_;
  }
};

} // namespace cppgraphviz
