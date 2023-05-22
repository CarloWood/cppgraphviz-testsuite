#include "sys.h"
#include "Item.hpp"

namespace cppgraphviz {

//static
thread_local MemoryAreaToGraphLinker Item::current_graph_linker_;

} // namespace cppgraphviz
