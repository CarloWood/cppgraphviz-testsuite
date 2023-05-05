#pragma once

#include "ID.hpp"
#include "AttributeList.hpp"
#include <utils/AIRefCount.h>
#include <utils/Badge.h>
#include <boost/intrusive_ptr.hpp>

namespace cppgraphviz {

class Graph;
class Edge;

class GraphItemData : public AIRefCount
{
 private:
  // The unique dot ID of this item.
  utils::UniqueID<ID_type> dot_id_ = s_unique_id_context.get_id();
  // Attribute list of this item.
  AttributeList attribute_list_;

 public:
  GraphItemData() = default;
  GraphItemData(GraphItemData const& other) = delete;
  GraphItemData(GraphItemData&& other) = delete;

  // Accessors.
  ID_type id() const { return dot_id_; }
  AttributeList const& attribute_list() const { return attribute_list_; }
  AttributeList& attribute_list() { return attribute_list_; }
};

template<typename Data>
class GraphItem
{
 private:
  // Use a smart pointer to the actual data, so that  GraphItem remains movable and copyable.
  boost::intrusive_ptr<Data> data_;

 public:
  GraphItem();

  // Give access to the attribute list of the node, allowing the user to add attributes.
  AttributeList& attribute_list() { return data_->attribute_list(); }

  // Short-cut for convenience.
  void add_attribute(Attribute const& attribute) { data_->attribute_list().add(attribute); }

  // Accessors for Graph.
  Data const& data(utils::Badge<Graph, Edge>) const { return *data_; }
  Data& data(utils::Badge<Graph, Edge>) { return *data_; }
};

template<typename Data>
GraphItem<Data>::GraphItem()
{
  data_ = new Data;
}

} // namespace cppgraphviz

