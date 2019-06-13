/*
 * MapCheck.h
 *
 *  Created on: Apr 19, 2017
 *      Author: ilyakatsnelson
 */

#ifndef MAPCHECK_H_
#define MAPCHECK_H_

#include <map>
#include <thread>
#include <string>
#include <mutex>

class MapCheck {
public:
  MapCheck();
  virtual ~MapCheck();

  bool Set(const std::string & name, const double value);
  double Get(const std::string & name) const;

private:
  std::map<std::string, double> map_;

  std::mutex data_mutex_;
};

#endif /* MAPCHECK_H_ */
