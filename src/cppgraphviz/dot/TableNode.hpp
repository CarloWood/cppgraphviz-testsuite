#pragma once

#include "GraphItem.hpp"
#include "TableElement.hpp"
#include "Port.hpp"
#include <map>

namespace cppgraphviz::dot {

template <typename T>
concept ConceptIndexableContainer = requires(T t)
{
  { t.ibegin() } -> std::same_as<typename T::index_type>;
  { t.iend() } -> std::same_as<typename T::index_type>;
  { std::declval<T>()[std::declval<typename T::index_type>()] } -> std::same_as<typename T::reference>;
  { std::declval<T const>()[std::declval<typename T::index_type>()] } -> std::same_as<typename T::const_reference>;
};

class TableNodeData : public GraphItemData
{
 private:
  std::vector<TableElement> elements_;

 public:

  void add_elements(std::function<Node (size_t)> at, size_t size)
  {
    for (size_t i = 0; i < size; ++i)
      elements_.push_back(at(i));
  }

  template<ConceptIndexableContainer Container>
  void add_elements(Container const& container)
  {
    add_elements([&](size_t i){ return container[typename Container::index_type{i}]; }, container.size());
  }

  void write_html_to(std::ostream& os, std::string const& indentation) const;

  Port at(size_t index) const
  {
    return {dot_id(), index};
  }
};

class TableNode : public GraphItem<TableNodeData>
{
 public:
  template<ConceptIndexableContainer Container>
  void add_elements(Container const& container)
  {
    data().add_elements(container);
  }

  Port operator[](size_t index) const { return data().at(index); }
};

} // namespace cppgraphviz::dot
