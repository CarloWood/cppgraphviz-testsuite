#pragma once

#include <string_view>
#include <string>
#include <iostream>

namespace cppgraphviz {

class Attribute
{
 private:
  std::string key_;
  mutable std::string value_;

 public:
  Attribute(std::string_view key, std::string_view value) :
    key_(key), value_(value) { }

  // Attribute is stored in a std::set.
  friend bool operator<(Attribute const& lhs, Attribute const& rhs)
  {
    return lhs.key_ < rhs.key_;
  }

  void print_on(std::ostream& os) const;
};

inline std::ostream& operator<<(std::ostream& os, Attribute const& attribute)
{
  attribute.print_on(os);
  return os;
}

} // namespace cppgraphviz
