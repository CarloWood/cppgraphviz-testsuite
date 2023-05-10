#pragma once

#include "AttributeList.hpp"
#include "GraphItemID.hpp"
#include <utils/AIRefCount.h>
#include <utils/Badge.h>
#include <boost/intrusive_ptr.hpp>

namespace cppgraphviz::dot {

class GraphData;
class NodeData;
class EdgeData;
class TableNodeData;
class Port;
class TableElement;

// Base class of GraphData, EdgeData and NodeData.
//
// This class provides a unique id for each graph (item) and a general attribute list.
class GraphItemData : public AIRefCount, public GraphItemID
{
 public:
  GraphItemData() : GraphItemID(s_unique_id_context.get_id()) { }
  GraphItemData(GraphItemData const& other) = delete;
  GraphItemData(GraphItemData&& other) = delete;
};

// A wrapper around a boost::intrusive_ptr.
// GraphItem's can be cheaply moved and even copied: changes made
// to one GraphItem affect all other GraphItem that are/were copies.
//
// The (shaded) data that a GraphItem points to (Data) will be
// either GraphData, NodeData or EdgeData.
template<typename Data>
class GraphItem
{
 private:
  // Use a smart pointer to the actual data, so that GraphItem remains movable and copyable.
  boost::intrusive_ptr<Data> data_;

 public:
  // Create a new Data object.
  GraphItem();
  // Make a copy of the GraphItem base class of other.
  GraphItem(GraphItem const& other) = default;
  // Move constructor.
  GraphItem(GraphItem&& other) = default;
  // Increment reference count of data and become a pointer to it.
  GraphItem(Data const* data) : data_(data) { }

  virtual ~GraphItem() = default;

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
GraphItem<Data>::GraphItem()
{
  data_ = new Data;
}

} // namespace cppgraphviz::dot
