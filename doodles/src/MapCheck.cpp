/*
 * MapCheck.cpp
 *
 *  Created on: Apr 19, 2017
 *      Author: ilyakatsnelson
 */

#include "MapCheck.h"
#include <cmath>

MapCheck::MapCheck() {
}

MapCheck::~MapCheck() {
}

bool MapCheck::Set(const std::string & name, const double value) {
  std::lock_guard<std::mutex> lock(data_mutex_);

  map_[name] = value;
  return true;
}

double MapCheck::Get(const std::string & name) const {
  //std::lock_guard<std::mutex> lock(data_mutex_);

  auto element = map_.find(name);
  if (element == map_.end())
    return NAN;

  return element->second;
}
