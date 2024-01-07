// Copyright (C) 2017 A^3 - All Rights Reserved
//
// Template container to abstract a spline lookup table.
// The table will contain a set of pairs of ranges indicating the translation
// from source range to destination range. A value from possible source
// range could be passed to find the source range it belongs to
// and the destination range it maps to.
//

#ifndef VAHANA_LIB_UTILS_RANGE_LOOKUP_H_
#define VAHANA_LIB_UTILS_RANGE_LOOKUP_H_

#include <array>
#include <cmath>
#include <set>
#include <utility>
#include <type_traits>
#include <limits>
#include <vector>

namespace vahana_lib {

// Template class containing one pair of ranges with search support
template<class SRC, class DST,
         class Enable = typename
                          std::enable_if<std::is_arithmetic<SRC>::value &&
                                         std::is_arithmetic<DST>::value>::type>
class Range {
 public:
  // Comparison operator. With it this class could be used in containers
  bool operator< (const Range& rhs) const {
    return (src_range.first < rhs.src_range.first) &&
           (src_range.second < rhs.src_range.second);
  }

  Range(SRC src_first, SRC src_second,
        DST dst_first, DST dst_second)
  : src_range(src_first, src_second),
    dst_range(dst_first, dst_second) {}

  // Constructor to only initialize the source range
  // Used in search
  explicit Range(SRC src)
  : src_range(src, src) {}

  Range(SRC src, DST dst)
  : src_range(src, src),
    dst_range(dst, dst) {}

  std::pair<SRC, SRC> src_range{};
  std::pair<DST, DST> dst_range{};
};

// Template container class representing the entire lookup table
template<class SRC, class DST>
class LookupTable {
 public:
  typedef Range<SRC, DST> RangeType;
  typedef std::set<RangeType> TableType;

  LookupTable() {}
  template<class S = SRC, class D = DST, std::size_t size>
  explicit LookupTable(const std::array<Range<S, D>, size> & source_data) {
    for (auto & s : source_data) {
      table_.insert(s);
    }
  }
  explicit LookupTable(const std::vector<RangeType> & source_data) {
    for (auto & s : source_data) {
      table_.insert(s);
    }
  }

  void insert(const Range<SRC, DST> & item) {
    table_.insert(item);
  }

  // Finds the range that corresponds to the passed value.
  // If the value doesn't fit exactly, linear interpolation is used.
  // In the exact found case the runtime is log(N).
  // In the near case - log(N)
  const RangeType find(const SRC & value) const {
    const RangeType key{value};
    auto range = table_.find(key);
    if (range == table_.end()) {
      // No exact match - find the nearest range
      if (key < *(table_.begin())) {
        return *(table_.begin());
      }
      if (*(--table_.end()) < key) {
        return *(--table_.end());
      }

      // Find the two neighboring ranges.
      // In this case a pair of iterators is returned pointing to the next
      // greater than element. The first in the pair won't point to the
      // not less element because in this condition the key won't match
      // anything.
      auto pair = table_.equal_range(key);
      auto & less_element = *(--pair.first);
      auto & more_element = *(pair.second);

      // The 'nearest' range will be the one within shortest distance
      auto distance_less = std::abs(value - less_element.src_range.second);
      auto distance_more = std::abs(value - more_element.src_range.first);
      if (distance_less < distance_more) {
        return less_element;
      }
      return more_element;
    }
    return *range;
  }

 private:
  TableType table_{};
};

}  // namespace vahana_lib

#endif  // VAHANA_LIB_UTILS_RANGE_LOOKUP_H_
