//
//
//

// #include "StatisticsData.h"

class DataReader {
 public:

  explicit DataReader(const std::string & name,
                        const std::string & topic,
                        const std::string & library_name)
      : name_{name},
        topic_(topic),
        library_name_(library_name) {
  }

 private:
  const std::string name_{};
  const std::string topic_{};
  const std::string library_name_{};

  // Mode Flags
  //bool enable_listener_ = true;
  //bool enable_blocking_ = false;

  //StatisticData<unsigned long> tt_{0, 100};
};
