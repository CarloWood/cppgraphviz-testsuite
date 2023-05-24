#include "sys.h"
#include "ItemTracker.hpp"
#include "Item.hpp"

namespace cppgraphviz {

void ItemTracker::set_parent_graph_tracker(utils::Badge<Graph, Node>, std::weak_ptr<GraphTracker> parent_graph_tracker)
{
  item_->set_parent_graph_tracker(std::move(parent_graph_tracker));
}

} // namespace cppgraphviz
