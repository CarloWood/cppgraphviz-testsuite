#pragma once

#include "AttributeList.hpp"
#include <utils/Badge.h>
#include <boost/intrusive_ptr.hpp>
#include <concepts>

namespace cppgraphviz::dot {

class GraphData;
class NodeData;
class EdgeData;
class TableNodeData;
class Port;
class TableElement;

// A wrapper around a boost::intrusive_ptr.
// GraphItemPtr's can be cheaply moved and even copied: changes made
// to one GraphItemPtr affect all other GraphItemPtr that are/were copies.
//
// The (shaded) data that a GraphItemPtr points to (Data) will be
// either GraphData, NodeData or EdgeData.
template<typename Data>
class GraphItemPtr
{
 public:
  using data_type = Data;

 private:
  // Use a smart pointer to the actual data, so that GraphItemPtr remains movable and copyable.
  boost::intrusive_ptr<Data> data_;

 public:
  // Create a new Data object.
  GraphItemPtr();
  // Make a copy of the GraphItemPtr base class of other.
  GraphItemPtr(GraphItemPtr const& other) = default;
  // Move constructor.
  GraphItemPtr(GraphItemPtr&& other) = default;
  // Increment reference count of data and become a pointer to it.
  GraphItemPtr(Data const* data) : data_(data) { }
  // Allow downgrading a GraphItemPtr to point to a base class of D.
  template<typename D>
  requires std::derived_from<D, Data>
  GraphItemPtr(GraphItemPtr<D> const& other) : data_(other.data_) { }
  // Accessing other.data_ requires us to be a friend.
  template<typename D>
  friend class GraphItemPtr;

  virtual ~GraphItemPtr() = default;

  // Give access to the attribute list of the item, allowing the user to add attributes.
  AttributeList& attribute_list() { return data_->attribute_list(); }

  // Short-cut for convenience.
  void add_attribute(Attribute&& attribute) { data_->attribute_list().add(std::move(attribute)); }

  // Accessors for GraphData, NodeData, EdgeData and TableNodeData.
  Data const& data(utils::Badge<GraphData, NodeData, EdgeData, TableNodeData>) const { return *data_; }
  Data& data(utils::Badge<GraphData, NodeData, EdgeData, TableNodeData>) { return *data_; }

 protected:
  // Accessors for Graph, Node, Edge, Port and TableElement.
  friend class Port;
  friend class TableElement;
  Data const& data() const { return *data_; }
  Data& data() { return *data_; }
};

template<typename Data>
GraphItemPtr<Data>::GraphItemPtr()
{
  data_ = new Data;
}

} // namespace cppgraphviz::dot
