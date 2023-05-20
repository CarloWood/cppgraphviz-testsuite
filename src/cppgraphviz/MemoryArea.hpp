#pragma once

namespace cppgraphviz {

class MemoryArea
{
 private:
  char const* begin_;
  char const* end_;

 public:
  MemoryArea(char const* begin, size_t size) : begin_(begin), end_(begin + size) { }

  bool operator==(MemoryArea const& other) const
  {
    return begin_ == other.begin_ && end_ == other.end_;
  }

  bool operator<(MemoryArea const& other) const
  {
    return end_ <= other.begin_;
  }

  bool overlaps_with(MemoryArea const& other) const
  {
    // The areas overlap when neither is less than the other.
    return !(end_ <= other.begin_) && !(other.end_ <= begin_);
  }

  bool lays_within(MemoryArea const& other) const
  {
    return begin_ >= other.begin_ && end_ <= other.end_;
  }
};

} // namespace cppgraphviz
