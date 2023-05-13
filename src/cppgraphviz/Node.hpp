#pragma once

#include "NodeTracker.hpp"
#include <boost/intrusive_ptr.hpp>
#include <memory>
#include "debug.h"

namespace cppgraphviz {

class GraphData;

class Node
{
 private:
  std::shared_ptr<NodeTracker> tracker_;
  boost::intrusive_ptr<GraphData> graph_data_;

#ifdef CWDEBUG
 protected:
  bool copied_{false};
  bool destructed_{false};
#endif

 public:
  Node();
  Node(Node const& other);
  Node(Node&& other) noexcept;
#ifdef CWDEBUG
  virtual ~Node()
  {
    // Call copied() from the copy constructor of a derived class that already has all required virtual functions defined.
    ASSERT(!copied_);
    // Call destructed() from the destructor of that same derived class.
    ASSERT(destructed_);
  }
#else
  virtual ~Node() = default;
#endif

  void add_to_graph(GraphData& graph_data);
  void copied();
  void destructed();
  void set_graph_data(boost::intrusive_ptr<GraphData> graph_data);

  boost::intrusive_ptr<GraphData> const& get_graph_data() const { return graph_data_; }
  std::shared_ptr<NodeTracker> const& get_tracker() const { return tracker_; }

  operator NodeTracker const&() const { return *tracker_; }

  virtual void initialize() = 0;
  virtual void add_to_graph_impl(GraphData* graph_data) = 0;
  virtual void remove_from_graph_impl(GraphData* graph_data) = 0;
};

} // namespace cppgraphviz
