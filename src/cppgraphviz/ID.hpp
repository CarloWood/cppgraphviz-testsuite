#pragma once

#include <utils/UniqueID.h>     // Add https://github.com/CarloWood/ai-utils.git to the root of your project.
#include <cstdint>

namespace cppgraphviz {

using ID_type = uint32_t;
extern utils::UniqueIDContext<ID_type> s_unique_id_context;

} // namespace cppgraphviz
