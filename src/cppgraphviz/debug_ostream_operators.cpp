#include "sys.h"
#include "debug_ostream_operators.hpp"
#include "GraphTracker.hpp"
#include <iostream>

namespace std {

using namespace cppgraphviz;

std::ostream& operator<<(std::ostream& os, std::shared_ptr<GraphTracker> const& graph_tracker)
{
  if (graph_tracker)
    os << '"' << graph_tracker->graph_ptr().item().attribute_list().get("what", "<NO \"what\">") << '"';
  else
    os << "<empty std::shared_ptr<GraphTracker>>";
  return os;
}

std::ostream& operator<<(std::ostream& os, std::weak_ptr<GraphTracker> const& weak_graph_tracker)
{
  std::shared_ptr<GraphTracker> graph_tracker = weak_graph_tracker.lock();
  if (graph_tracker)
    os << graph_tracker;
  else
    os << "<deleted GraphTracker>";
  return os;
}

} // namespace std
