#include "sys.h"
#include "NodeTracker.hpp"
#include "Node.hpp"

namespace cppgraphviz {

NodeTracker::NodeTracker(utils::Badge<NodeTracker>, Node* node) : ItemTracker(node)
{
}

Node const& NodeTracker::get_node() const
{
  return static_cast<Node const&>(*item_);
}

Node& NodeTracker::get_node()
{
  return static_cast<Node&>(*item_);
}

} // namespace cppgraphviz
