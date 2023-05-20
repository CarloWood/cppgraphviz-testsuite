#include "sys.h"
#include "Node.hpp"

namespace cppgraphviz {

//static
thread_local MemoryAreaToGraphLinker Node::current_graph_linker_;

} // namespace cppgraphviz
