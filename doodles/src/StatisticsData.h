//
// Helper class to store statistical data of a measured value.
// It is intended to contain the distribution of the
//

#ifndef STATISTIC_DATA_H_
#define STATISTIC_DATA_H_

#include <atomic>
#include <cmath>
#include <limits>
#include <type_traits>

// Values are [min, avg, max, variance, cnt]
template <class DATA_TYPE, class Enable = typename
          std::enable_if<std::is_arithmetic<DATA_TYPE>::value>::type>
class StatisticData {
 public:
  explicit StatisticData(DATA_TYPE value)
    : min_(value),
      avg_(value),
      max_(value),
      last_sample_(value),
      min_limit_(std::numeric_limits<DATA_TYPE>::min()),
      max_limit_(std::numeric_limits<DATA_TYPE>::max()),
      variance_(0),
      cnt_(1),
      num_overruns_(0),
      num_underruns_(0) {}

  StatisticData(DATA_TYPE min_limit, DATA_TYPE max_limit)
      : min_(0),
        avg_(0),
        max_(0),
        last_sample_(0),
        min_limit_(min_limit),
        max_limit_(max_limit),
        variance_(0),
        cnt_(0),                            // will force a Reset on next Sample
        num_overruns_(0),
        num_underruns_(0) {}

  StatisticData(DATA_TYPE value,
                DATA_TYPE min_limit,
                DATA_TYPE max_limit)
      : min_(value),
        avg_(value),
        max_(value),
        last_sample_(value),
        min_limit_(min_limit),
        max_limit_(max_limit),
        variance_(0),
        cnt_(1),
        num_overruns_(0),
        num_underruns_(0) {}

  StatisticData(const StatisticData & other) {
    *this = other;
  }

  StatisticData & operator=(const StatisticData & other) {
    if (&other == this) {
      return *this;
    }

    min_ = other.GetMin();
    avg_ = other.GetAvg();
    max_ = other.GetMax();
    last_sample_ = other.GetLastSample();
    variance_ = other.GetVariance()*(other.GetCnt()-1);
    cnt_ = other.GetCnt();
    num_overruns_ = other.GetNumOverruns();
    num_underruns_ = other.GetNumUnderruns();
    min_limit_ = other.GetMinLimit();
    max_limit_ = other.GetMaxLimit();
    lock_.clear();

    return *this;
  }

  const DATA_TYPE GetMin() const { return min_; }
  const DATA_TYPE GetAvg() const { return avg_; }
  const DATA_TYPE GetMax() const { return max_; }
  const DATA_TYPE GetLastSample() const { return last_sample_; }
  const DATA_TYPE GetVariance() const {
    if (cnt_ < 2) {
      return variance_;
    }
    return (variance_/(cnt_-1));
  }
  const auto GetCnt() const { return cnt_; }
  const auto GetNumOverruns() const { return num_overruns_; }
  const auto GetNumUnderruns() const { return num_underruns_; }
  const auto GetMinLimit() const { return min_limit_; }
  const auto GetMaxLimit() const { return max_limit_; }

  void SetMinLimit(const DATA_TYPE &period) { min_limit_ = period; }
  void SetMaxLimit(const DATA_TYPE &period) { max_limit_ = period; }

  // Measure the min, max, avg, & variance
  // Variance is calculated using Welford's algorithm
  // https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Online_algorithm
  StatisticData & Sample(const DATA_TYPE value) {
    // If the reset was requested, proceed here.
    // std::atomic_flag doesn't have load and store operations
    if (lock_.test_and_set() || cnt_ == 0) {
      last_sample_ = min_ = avg_ = max_ = value;
      variance_ = DATA_TYPE{0};
      cnt_ = 1;
      num_overruns_  = (value > max_limit_) ? 1 : 0;
      num_underruns_ = (value < min_limit_) ? 1 : 0;

      lock_.clear();
      return *this;
    } else {
      lock_.clear();
    }

    last_sample_ = value;

    if (value > max_limit_) {
      num_overruns_++;
    }

    if (value < min_limit_) {
      num_underruns_++;
    }

    if (min_ > value) {
      min_ = value;
    } else if (max_ < value) {
      max_ = value;
    }

    // Sample count
    cnt_ += 1;

    // Calculation is done depending on the type
    calculate(value);

    return *this;
  }

  // Methods to calculate the average and variance
  // They are overloaded depending on the type of DATA_TYPE
  template<class Q = DATA_TYPE>
  typename std::enable_if<std::is_unsigned<Q>::value, bool>::type
  calculate(const Q value) {
    const typename std::make_signed<Q>::type delta1 = value - avg_;
    const typename std::make_signed<decltype(cnt_)>::type cnt = cnt_;
    // avg
    avg_ = (avg_ + lround(delta1/cnt));

    // variance
    const typename std::make_signed<Q>::type delta2 = value - avg_;
    variance_ += (delta1*delta2);

    return true;
  }

  template<class Q = DATA_TYPE>
  typename std::enable_if<!std::is_unsigned<Q>::value, bool>::type
  calculate(const Q value) {
    const Q delta1 = value - avg_;
    const typename std::make_signed<decltype(cnt_)>::type cnt = cnt_;
    // avg
    avg_ = (avg_ + delta1/cnt);

    // variance
    const Q delta2 = value - avg_;
    variance_ += (delta1*delta2);

    return true;
  }

  // Reset the state of the statistics data to the initial value
  // In order for this method to work reliably it should only be called
  // from a lower priority thread.
  // This method just sets the flag. The actual data will be written
  // in the next Sample call
  void Reset() {
    lock_.test_and_set();
  }

 private:
  DATA_TYPE min_;
  DATA_TYPE avg_;
  DATA_TYPE max_;
  DATA_TYPE last_sample_;
  DATA_TYPE min_limit_;  // Minimum sample limit
  DATA_TYPE max_limit_;  // Maximum sample limit
  DATA_TYPE variance_;   // Incomplete variance. To get full variance this
                         // value has to be divided by (cnt_-1)
  uint32_t cnt_;
  uint32_t num_overruns_;
  uint32_t num_underruns_;

  // Flag to control access to this object
  std::atomic_flag lock_ = ATOMIC_FLAG_INIT;
};

#endif  // STATISTIC_DATA_H_
