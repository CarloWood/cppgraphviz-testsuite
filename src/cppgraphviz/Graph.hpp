#pragma once

#include <utils/UniqueID.h>     // Add https://github.com/CarloWood/ai-utils.git to the root of your project.
#include <iosfwd>
#include <cstdint>

namespace cppgraphviz {

class Graph
{
  using ID_type = uint32_t;

 private:
  // Configuration.
  bool digraph_ = false;
  bool strict_;
  // The unique dot ID of this graph.
  utils::UniqueID<ID_type> dot_id_ = s_unique_id_context.get_id();

  static utils::UniqueIDContext<ID_type> s_unique_id_context;

 public:
  Graph(bool strict = false) : strict_(strict) { }

 protected:
  Graph(bool digraph, bool strict = false) : digraph_(digraph), strict_(strict) { }

 public:
  void set_digraph(bool digraph = true) { digraph_ = digraph; }
  void set_strict(bool strict = true) { strict_ = strict; }

  // Write graph to os in dot format.
  void write_dot(std::ostream& os) const;

  // Accessors.
  bool is_digraph() const { return digraph_; }
  bool is_strict() const { return strict_; }
};

class Digraph : public Graph {
 public:
  Digraph(bool strict = false) : Graph(true, strict) { }
};

} // namespace cppgraphviz
