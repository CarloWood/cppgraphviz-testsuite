#pragma once

#include <iosfwd>
#include <memory>

namespace cppgraphviz {

class GraphTracker;

} // namespace cppgraphviz

namespace std {

std::ostream& operator<<(std::ostream& os, std::shared_ptr<cppgraphviz::GraphTracker> const& graph_tracker);
std::ostream& operator<<(std::ostream& os, std::weak_ptr<cppgraphviz::GraphTracker> const& weak_graph_tracker);

} // namespace std
