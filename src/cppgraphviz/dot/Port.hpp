#pragma once

#include "GraphItemPtr.hpp"
#include "DotID.hpp"
#include <optional>
#include <iosfwd>

namespace cppgraphviz::dot {

class NodeData;
using Node = GraphItemPtr<NodeData>;

class Port
{
 private:
  ID_type id_;
  std::optional<size_t> port_;

 public:
  Port() = default;
  Port(Node const& node);
  Port(ID_type id, size_t port) : id_(id), port_(port) { }

  void set_port(ID_type id) { id_ = id; port_.reset(); }
  void write_to(std::ostream& os) const;

  ID_type id() const { return id_; }
  bool has_port() const { return port_.has_value(); }
  size_t port() const { return port_.value(); }
};

inline std::ostream& operator<<(std::ostream& os, Port const& port)
{
  port.write_to(os);
  return os;
}

} // namespace cppgraphviz::dot
