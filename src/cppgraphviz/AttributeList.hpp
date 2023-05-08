#pragma once

#include "Attribute.hpp"
#include <iosfwd>
#include <set>

namespace cppgraphviz {

class AttributeList
{
 private:
  std::set<Attribute> attributes_;

 public:
  void add(Attribute&& attribute)
  {
    attributes_.insert(std::move(attribute));
  }

  bool has_key(std::string const& key) const;
  std::string const& get_value(std::string const& key) const;

  std::string const& get(std::string const& key, std::string const& default_value) const
  {
    return has_key(key) ? get_value(key) : default_value;
  }

  operator bool() const { return !attributes_.empty(); }

  void print_on(std::ostream& os) const;
};

inline std::ostream& operator<<(std::ostream& os, AttributeList const& attribute_list)
{
  attribute_list.print_on(os);
  return os;
}

} // namespace cppgraphviz
