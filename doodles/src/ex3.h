#ifndef EX3_H_
#define EX3_H_

#include <memory>

#include "DataReader.h"

class Manager {
 public:
  Manager();

  virtual ~Manager();

 private:
  // Topic Constants
  static constexpr const char* kActuatorsCommand = "ActuatorsCommand";
  static constexpr const char* kActuatorsProxy = "ActuatorsProxy";

  std::unique_ptr<DataReader> faults_;
};

#endif // EX3_H_
