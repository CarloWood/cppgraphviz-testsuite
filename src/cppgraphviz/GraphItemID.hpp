#pragma once

#include "ID.hpp"

namespace cppgraphviz {

class GraphItemID
{
 private:
  // The unique dot ID of this item.
  utils::UniqueID<ID_type> dot_id_;
  // Attribute list of this item.
  AttributeList attribute_list_;

 public:
  GraphItemID(utils::UniqueID<ID_type> dot_id) : dot_id_(dot_id) { }

  // Accessors.
  ID_type id() const { return dot_id_; }
  AttributeList const& attribute_list() const { return attribute_list_; }
  AttributeList& attribute_list() { return attribute_list_; }
};

} // namespace cppgraphviz
