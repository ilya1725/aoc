// Time measurement

#ifndef TIME_MEASUREENT_H_
#define TIME_MEASUREENT_H_

#include <chrono>
#include <map>
#include <unordered_map>
#include <array>
#include <thread>
#include <atomic>
#include <ostream>

class TimeMsr {
  public:
    // Map of sample IDs.
    // Use unordered_map to make access fast.
    // But have to sort it for proper printing
    // Values are [min, avg, max, variance, cnt]
    typedef std::unordered_map<uint32_t, std::array<double, 5>> msr_map;
    typedef std::map<uint32_t, std::array<double, 5>> msr_map_ordered;
    static const unsigned kMinIndx = 0;
    static const unsigned kMeanIndx = 1;
    static const unsigned kMaxIndx = 2;
    static const unsigned kVarianceIndx = 3;
    static const unsigned kCntIndx = 4;

    TimeMsr()
        : out_cnt_(0) {
        start_ = std::chrono::high_resolution_clock::now();
        this_id_ = std::this_thread::get_id();
    }

    ~TimeMsr() {
    }

    // Start the measurement time
    bool start() {
        start_ = std::chrono::high_resolution_clock::now();
        this_id_ = std::this_thread::get_id();
        return true;
    }

    // Measure timing for this ID from the start.
    // Measure the min, max, avg, & variance
    bool sample(uint32_t id) {
        const auto now{std::chrono::high_resolution_clock::now()};
        const std::chrono::duration<double> diff{now-start_};

        return sample(id, diff.count());
    }

    // Measure the values of some variable, identified by the passed id
    // Measure the min, max, avg, & variance
    // Variance is calculated using Welford's algorithm
    bool sample(uint32_t id, double value) {
        auto range = data_values_[id];

        // On the first access to this id the std::map will create data array
        // with default values set
        if (range[kCntIndx] == 0.0) {
        range[kMinIndx] = range[kMeanIndx] = range[kMaxIndx] = value;
        range[kVarianceIndx] = 0.0;
        range[kCntIndx] = 1.0;
        } else {
        if (range[kMinIndx] > value) {
            range[kMinIndx] = value;
        } else if (range[kMaxIndx] < value) {
            range[kMaxIndx] = value;
        }

        // Sample count
        range[kCntIndx] += 1;

        const auto delta1{value - range[kMeanIndx]};
        // avg
        range[kMeanIndx] = range[kMeanIndx] +
                            (value - range[kMeanIndx])/range[kCntIndx];

        // variance
        auto delta2 = value - range[kMeanIndx];
        range[kVarianceIndx] += delta1*delta2;

        }
        data_values_[id] = range;

        return true;
    }

    bool IsPrint(uint32_t count) {
        out_cnt_++;
        if (out_cnt_ == count) {
        out_cnt_ = 0;
        }
        return (out_cnt_ == 0);
    }

    const msr_map & GetDataValues() const { return data_values_; }
    const msr_map_ordered GetOrderedDataValues() const {
        return msr_map_ordered{data_values_.begin(), data_values_.end()};
    }
    const std::thread::id GetThrId() const { return this_id_; }

  private:
    uint32_t out_cnt_;

    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
    std::thread::id this_id_;

    // A map of measured values.
    // array contains [min, agv, max] values
    // Time duration is stored in seconds
    msr_map data_values_;
};

// Delimiters are to make formatting easier in Excel
inline std::ostream& operator<<(std::ostream& os, const TimeMsr& tm) {

  os << tm.GetThrId() << "|";
  for (auto & m : tm.GetOrderedDataValues()) {
    os << m.first << "|" << m.second[TimeMsr::kMinIndx] << ";"
                         << m.second[TimeMsr::kMeanIndx] << ";"
                         << m.second[TimeMsr::kMaxIndx] << ";"
                         << (m.second[TimeMsr::kVarianceIndx]/(m.second[TimeMsr::kCntIndx]-1)) << "|";
  }
  return os;
}

#endif // TIME_MEASUREENT_H_
