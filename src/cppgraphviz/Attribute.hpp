#pragma once

#include <string_view>
#include <string>
#include <iostream>

namespace cppgraphviz {

class Attribute
{
 private:
  std::string key_;
  std::string value_;

 public:
  Attribute(std::string_view key, std::string_view value) :
    key_(key), value_(value) { }

  void print_on(std::ostream& os) const
  {
    // ID '=' ID
    os << '"' << key_ << '"' << '=' << '"' << value_ << '"';
  }
};

inline std::ostream& operator<<(std::ostream& os, Attribute const& attribute)
{
  attribute.print_on(os);
  return os;
}

} // namespace cppgraphviz
