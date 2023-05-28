#include "sys.h"
#include "Item.hpp"
#include "Graph.hpp"

namespace cppgraphviz {

//static
thread_local MemoryAreaToGraphLinker Item::current_graph_linker_;

void Item::extract_root_graph()
{
  std::shared_ptr<GraphTracker> parent_graph_tracker = parent_graph_tracker_.lock();
  if (parent_graph_tracker)
  {
    // The parent_graph_tracker_ points to the tracker of the subgraph associated with the class containing this Node.
    Graph const& subgraph = parent_graph_tracker->get_graph();
    // Get the root graph from that subgraph.
    root_graph_tracker_ = subgraph.root_graph_tracker();
  }
}

} // namespace cppgraphviz
