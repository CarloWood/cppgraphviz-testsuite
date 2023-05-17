#pragma once

#include "AttributeList.hpp"
#include "GraphItem.hpp"
#include <utils/Badge.h>
#include <boost/intrusive_ptr.hpp>
#include <concepts>

namespace cppgraphviz::dot {

// A wrapper around a boost::intrusive_ptr<GraphItem>.
//
// GraphItemPtr's can be cheaply moved and even copied: changes made
// to one GraphItemPtr affect all other GraphItemPtr that are/were copies.
//
// The (shared) data that a GraphItemPtr points to (GraphItem) will be
// a base class of either GraphGraph, GraphNode or GraphEdge.
class ConstGraphItemPtr
{
 protected:
  // Use a smart pointer to the actual data, so that GraphItemPtr remains movable and copyable.
  boost::intrusive_ptr<GraphItem> shared_item_ptr_;

 public:
  // Create a ConstGraphItemPtr that points to an existing GraphItem object.
  // Increment the reference count of the GraphItem to keep it alive.
  ConstGraphItemPtr(GraphItem const* ptr) : shared_item_ptr_(const_cast<GraphItem*>(ptr)) { }

  // Derived classes have virtual functions.
  // Allow destruction by (base class) pointer in case this object was allocated with new.
  virtual ~ConstGraphItemPtr() = default;

  // Declaring the above destructor stops the compiler from generating move functions
  // (and deprecates the implicit generation of the copy constructor).
  // So we need to explicitly request them.
  ConstGraphItemPtr(ConstGraphItemPtr&&) = default;
  ConstGraphItemPtr& operator=(ConstGraphItemPtr&&) = default;
  // Declaring the move constructor stopped the compiler from generating the copy constructor
  // (so this is beyond a deprecation now).
  ConstGraphItemPtr(ConstGraphItemPtr const&) = default;
  ConstGraphItemPtr& operator=(ConstGraphItemPtr const&) = default;

  // Accessor for the pointed-to GraphItem. Only give const- access.
  GraphItem const& item() const { return *shared_item_ptr_; }

  // Give access to the attribute list of the item, allowing the user to read attributes.
  AttributeList const& attribute_list() const { return shared_item_ptr_->attribute_list(); }
};

class GraphItemPtr : public ConstGraphItemPtr
{
 public:
  // Create a pointer to a new graph item.
  template<typename T>
  requires std::derived_from<T, GraphItem>
  GraphItemPtr(std::type_identity<T>) : ConstGraphItemPtr(new T) { }

  // Increment reference count of the item and become a pointer to it.
  GraphItemPtr(GraphItem* item) : ConstGraphItemPtr(item) { }

  // Give access to the attribute list of the item, allowing the user to add attributes.
  using ConstGraphItemPtr::attribute_list;
  AttributeList& attribute_list() { return shared_item_ptr_->attribute_list(); }

  // Accessors.
  using ConstGraphItemPtr::item;
  GraphItem& item() { return *shared_item_ptr_; }
};

template<typename T>
struct GraphItemPtrTemplate : GraphItemPtr
{
  using item_type = T;

  GraphItemPtrTemplate() : GraphItemPtr(new T) { }

  // Accessors.
  T const& item() const { return static_cast<T const&>(*this->shared_item_ptr_); }
  T& item() { return static_cast<T&>(*this->shared_item_ptr_); }

  T const* operator->() const { return static_cast<T const*>(this->shared_item_ptr_.get()); }
  T* operator->() { return static_cast<T*>(this->shared_item_ptr_.get()); }
};

template<typename T>
struct ConstGraphItemPtrTemplate : ConstGraphItemPtr
{
  ConstGraphItemPtrTemplate(T const* item) : ConstGraphItemPtr(item) { }

  // Automatic conversion from GraphItemPtrTemplate.
  ConstGraphItemPtrTemplate(GraphItemPtrTemplate<T> const& other) : ConstGraphItemPtr(other) { }
  ConstGraphItemPtrTemplate(GraphItemPtrTemplate<T>&& other) : ConstGraphItemPtr(std::move(other)) { }

  // Accessor.
  T const& item() const { return static_cast<T const&>(*shared_item_ptr_); }

  T const* operator->() const { return static_cast<T const*>(this->shared_item_ptr_.get()); }
};

} // namespace cppgraphviz::dot
