//============================================================================
// Name        : doodles.cpp
// Author      : Ilya Katsnelson
// Version     :
// Copyright   : free to use
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <cfenv>
#include <climits>
#include <vector>
#include <array>
#include <algorithm>
#include <map>
#include <set>
#include <unordered_map>
#include <sstream>
#include <utility>

#include <arpa/inet.h>

#include "time_measure.hpp"
#include "MapCheck.h"

using namespace std;

uint8_t response[] = {
    0xff, 0xff,  // sync word
    0x00, 0xba,  // response_ header: address 0xab
    0x09, 0x00,  // actuator position: ~6.875 degrees
    0x00, 0x14,  // unit temperature: 20 degree Celsius
    0x02, 0x00,  // motor current: ~625 mA
    0x00, 0x01,  // received command message count
    0x00, 0x01,  // message count
    0x02, 0xc8   // WRONG checksum
};

uint16_t CalculateCrc(const uint8_t * d, unsigned size) {
    uint16_t crc = 0;
    for (unsigned i = 0; i < size; ++i)
      crc += d[i];

    return crc & 0x0fff;
}

template<class SRC, class DST>
DST scale(const std::pair<SRC, SRC> & src_range,
          const std::pair<DST, DST> & dst_range,
          const SRC value) {
  // One extra unit has to be added to the range if the type is integral
  // and have the same sign.
  // This is done because integral type ranges are inclusive.
  // I.e. [0, 5] - range is 6
  const bool value_extra = (std::is_integral<SRC>() &&
                            (src_range.second * src_range.first) >= 0);
  const bool result_extra = (std::is_integral<DST>() &&
                            (dst_range.second * dst_range.first) >= 0);

  const auto value_range = src_range.second - src_range.first + value_extra;
  const auto result_range = dst_range.second - dst_range.first + result_extra;

  auto result = (value - src_range.first) * (result_range/value_range) +
                dst_range.first;

  cout << value_range << "|" << result_range << "|" << value << "|" << result << endl;
  if (result < dst_range.first)
    result = dst_range.first;
  else if (result > dst_range.second)
    result = dst_range.second;

  return result;
}

template<class SRC, class DST>
DST scale(const std::pair<SRC, SRC> & src_range,
          const std::pair<DST, DST> & dst_range,
          const SRC value,
          const double scale_factor) {
  auto result = (value - src_range.first) * scale_factor + dst_range.first;

  if (std::is_integral<DST>())
    result = std::round(result);

  if (result < dst_range.first)
    result = dst_range.first;
  else if (result > dst_range.second)
    result = dst_range.second;

  return result;
}

//=============================================================================
struct President
{
    string name;
    string country;
    int year;

    President(string p_name, string p_country, int p_year)
        : name(move(p_name)), country(move(p_country)), year(p_year)
    {
        cout << "I am being constructed.\n";
    }
    President(const President& other)
        : name(other.name), country(other.country), year(other.year)
    {
        cout << "I am being copied.\n";
    }
    President(President&& other)
        : name(move(other.name)), country(move(other.country)), year(other.year)
    {
        cout << "I am being moved.\n";
    }
    President& operator=(const President& other) = default;

    friend ostream &operator<<(ostream &os, President const &p) {
      return os << "[" << p.name << "|" << p.country << "|" << p.year << "]";
    }
};

// bool vector_d() {
//   vector<President> pres;

//   for (auto i=0;i<4;i++){
//     President p("F1", "USA1", 1995+i);
//     pres.push_back(move(p));
//     cout << pres.size() << ":" << pres.capacity() << endl;
//   }

//   for (auto &i : pres) {
//     cout << i << endl;
//   }
//   return true;
// }

//=============================================================================
bool vector_c() {
  // array<const string, 5> log_level_strings = {
  //   {("DEBUG"), ("INFO"), ("WARNING"), ("ERROR"), ("FATAL")}
  // };

  // string item("INFO");
  // auto b = std::find(log_level_strings.begin(), log_level_strings.end(), item);
  // if (b == log_level_strings.end())
  //   return false;

  // cout << *b << endl;
  return true;
}

//=============================================================================
class Base {
 protected:
    int x;
 public:
    Base(int x_in) {
      x = x_in;
    }
    virtual ~Base() {};

    void nothing() {
      cout << "B this:" << this << endl;
    }

    virtual void something () = 0;
};

class Derived : public Base {
 public:
    Derived(int x_in)
      : Base(x_in) {}

    virtual ~Derived() {};

    void something() override {
        this->nothing();   // equivalent to `nothing()`

        cout << "D this:" << this << endl;
    }
};

//=============================================================================
bool map_d() {
  // std::map<uint8_t, std::string> motors_;

  // motors_[23] = "test1";
  // motors_[1] = "test3";

  // for (auto & p : motors_) {
  //   std::cout << (int)p.first << "|" << p.second << std::endl;
  // }
  return true;
}

//-----------------------------------------------------------------------------
struct CommandMsg {
  uint16_t sync_word;
  uint8_t address;
  uint8_t packed_bit_array;
  uint16_t payload;
  uint16_t message_count;
  uint16_t checksum;
};

// Word-wise network to host byte switching
template<class T>
void ntohsMsg(const T & from, T * to) {
  const uint16_t * pfrom = reinterpret_cast<const uint16_t*> (&from);
  uint16_t * pto = reinterpret_cast<uint16_t*> (to);

  for (unsigned i = 0; i < sizeof(T)/sizeof(uint16_t); i++) {
    pto[i] = ntohs(pfrom[i]);
  }
}

std::array<double, 5> online_variance() {
  std::vector<double> data =
    {0.370748816,
        0.602312483,
        0.875569608,
        0.601463073,
        0.90870928,
        0.295415135,
        0.388954106,
        0.908723661,
        0.149489457,
        0.309269764,
        0.439026809,
        0.626457386,
        0.613139638,
        0.662034279,
        0.643102256,
        0.481620467,
        0.868411163,
        0.472502088,
        0.733111284,
        0.780662289,
        0.410179533,
        0.255989372,
        0.463587665,
        0.096528408,
        0.350815241,
        0.33741896,
        0.902325497};

  unsigned n = 0;
  //double mean = 0.0;
  double M2 = 0.0;
  double min = 0.0;
  double max = 0.0;
  double avg = 0.0;

  for (auto x : data) {
    n += 1;
    if (n == 1) {
      min = avg = max = x;
    } else {
      if (x < min) min = x;
      if (x > max) max = x;
      auto delta1 = x - avg;
      avg = avg + (x - avg)/n;

      //mean += delta1/n;
      auto delta2 = x - avg;
      M2 += delta1*delta2;
    }
  }

  return (std::array<double, 5>{{min, avg, M2 / (n - 1), max, (double)n}});
}

//=============================================================================
template<class SRC, class DST>
class Range {
 public:
  std::pair<SRC, SRC> src_range;
  std::pair<DST, DST> dst_range;

  Range(SRC src_first, SRC src_second,
        DST dst_first, DST dst_second)
  : src_range(src_first, src_second),
    dst_range(dst_first, dst_second) {}

  Range() {}; // { Range(0.0); }

  Range(SRC value)
  : src_range(value, value),
    dst_range(value, value) {}

  bool operator()(const Range& lhs, const Range& rhs) const {
    return (lhs.src_range.first < rhs.src_range.first) && (lhs.src_range.second < rhs.src_range.second);
  }
};

bool mapping_test() {
  // Mapping
  std::set<Range<double,double>, Range<double, double>>
          z = {{-30.0, -20.0, -10.0, -5.0},
               {-20.0, 0, -5.0, 5.0},
               {0, 10.0, 5.0, 8.0}};

  // Find range that contains this value
  Range<double,double> f{8.0};

  auto d = z.find(f);
  if(d != z.end()) {
    std::cout << "Found {" << (*d).dst_range.first
                           << ","
                           << (*d).dst_range.second
                           << "}\n";
  }

  return true;
}

//-----------------------------------------------------------------------------
void map_check_thread1(const MapCheck & obj) {
  for (auto i=0; i<20; i++) {
    cout << dec << i << ":" << obj.Get("val") << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}
void map_check_thread2(MapCheck * obj) {
  for (auto i=0; i<20; i++) {
    obj->Set("val", i);
    std::this_thread::sleep_for(std::chrono::milliseconds(1001));
  }
}
void map_check_test() {
  MapCheck the_obj;

  //auto thr1 = std::thread(map_check_thread1, the_obj);
  const MapCheck & b = the_obj;
  auto thr2 = std::thread(map_check_thread2, &the_obj);

  for (auto i=0; i<20; i++) {
    cout << dec << i << ":" << b.Get("val") << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  thr2.join();

  return;
}

//-----------------------------------------------------------------------------
template<class T>
bool foo_vector(const std::vector<std::array<T,2>> & data) {
  for (auto & d : data) {
    std::cout << "{" << d[0] << ":" << d[1] << "}" << std::endl;
  }
  return true;
}

template<class T>
class Table {
  public:
    template<class D=T, std::size_t size>
    Table(const std::array<std::array<D,2>, size> & data)
      {
        for (auto & d : data) {
          std::cout << "[" << d[0] << "-" << d[1] << "]" << std::endl;
          data_.push_back(d);
        }
      }
    template<class D=T>
    Table(const std::vector<std::array<D,2>> & data)
      : data_(data)
      {
        for (auto & d : data) {
          std::cout << "[" << d[0] << ":" << d[1] << "]" << std::endl;
        }
      }
  private:
    std::vector<std::array<T,2>> data_;
};

//-----------------------------------------------------------------------------
int main() {

  cout << hex << "0x" << (uint16_t)CalculateCrc(response, 14) << endl;

  auto now = chrono::system_clock::now();
  auto now_c = chrono::system_clock::to_time_t(now);

  cout << put_time(std::localtime(&now_c), "%F %T") << endl;

  const pair<double, double> s1{0.0, 10.0};
  const pair<double, double> s2{0.0, 5.0};
  const pair<int16_t, int16_t> i2{-5, 5};

  const std::pair<double, double> doubleRange{-2048.0,2048.0};
  const std::pair<double, double> doubleRange2{256.0,2048.0};
  const std::pair<double, double> doubleRange3{4.1,70.0};
  const std::pair<double, double> kPositionRange2(-45.0, 45.0);
  const std::pair<double, double> kPositionRange(-20.0, 150.0);
  const std::pair<double, double> kPositionRange3(0.0, 196.85);
  const std::pair<double, double> kPositionRange4(-6.35, +209.55);

  const std::pair<int16_t, int16_t> int16Range{-2048,2048};
  const std::pair<int16_t, int16_t> int16Range2{1024,2048};
  const std::pair<int16_t, int16_t> int16Range3{0,4095};
  const std::pair<uint16_t, uint16_t> kUint12bitPair{0, 4095};

  //double result = scale(int16Range, doubleRange, (int16_t)0);

  cout << scale(int16Range, doubleRange, (int16_t)0)
       << "|" << scale(doubleRange, int16Range3, 0.0) << endl;

  {
    double arr[] = { 20.0,
        45.0,
        70.0,
       100.0,
       170.0,
       196.8,
       270.0};

    for (auto d : arr) {
      auto scale1 = scale(kPositionRange3, kUint12bitPair, d, (4096.0/196.85));
      auto scale2 = scale(kUint12bitPair, kPositionRange4, (uint16_t)0x900, 1.0/(4096.0/209.55));
      cout << "Scale:" << d << "|" << hex << scale1 << ":" << scale2 << endl;
    }

    //scale1 = scale(kPositionRange, kUint12bitPair, 1.02, (4096.0f/170.0f));
    //scale2 = scale(kUint12bitPair, kPositionRange, scale1, 1.0/(4096.0f/170.0f));
    //cout << "Scale:" << dec << scale1 << ":" << scale2 << endl;
  }

  vector_c();
  map_d();

  auto f = make_pair(string("Boo"), President("1", "D", -32));
  cout << f.first << ":" << f.second << endl;

  auto b1 = scale(doubleRange3, int16Range3, 5.6);
  auto b2 = scale(int16Range3, doubleRange3, b1);
  cout << "5.6|" << hex << b1 << "|" << b2 << endl;

  //---------------------------------------------------------------------------
  CommandMsg b{0};
  b.address = 0xAA;
  b.packed_bit_array = 0x55;
  b.sync_word = 0xffff;

  ntohsMsg(b, &b);

  cout << hex << (int)b.address << "|" << (int)b.packed_bit_array << endl;

  ssize_t sz1 = -2;
  size_t sz2 = sz1;
  cout << "1: " << sz1 << ":" << sz2 << endl;

  std::vector<uint8_t> vector(100);
  vector.clear();
  printf("V: %p|%lu\n", vector.data(), vector.size());
  vector.data()[1] = 21;
  printf("V: %p|%lu|%d\n", vector.data(), vector.size(), vector[1]);

  vector.resize(6);
  printf("V: %p|%lu|%d\n", vector.data(), vector.size(), vector[1]);
  for (unsigned i=0;i<vector.size();i++) {
    vector[i] = i;
    cout << i;
  }
  cout << endl << "V4:" << endl;
  stringstream stream;
  for (auto & i : vector) {
    stream << " " << hex << (int)i;
    cout << (int)i << "=";
  }
  cout << "V2:" << stream.str() << endl;

  auto d = online_variance();
  cout << "[" << d[0] << ";" << d[1] << ";" << d[2] << ";" << d[3] << ";" << d[4] << "]\n";

  mapping_test();

  // Inheritance
  Derived der{3};
  der.something();

  //map_check_test();

  {
    std::vector<std::array<float, 2>> bb {{{1.0, 2.0}},{{2.0, 45.0}}};
    std::array<std::array<float,2>,3> arr{{{{1.0, 2.0}},{{2.0, 45.0}},{{3.0, 145.0}}}};
    foo_vector(bb);
    Table<float> t(arr);
  }
  return 0;
}


//  if (1){
//    std::stringstream stream;
//    for (unsigned i = 0; i < ret_count; i++) {
//      stream << " " << std::hex << (int)response_buffer_[i];
//    }
//    log_info("rd") << "[" << std::hex << (int)address_ << "] " << stream.str();
//  }
//
//
//  {
//    std::stringstream stream;
//    uint8_t * pData = reinterpret_cast<uint8_t*> (&command_msg_);
//    for (unsigned i = 0; i < sizeof(CommandMsg)/sizeof(uint8_t); i++) {
//      stream << " " << std::hex << (int)pData[i];
//    }
//    log_info("CRC") << "[" << std::hex << (int)Moog<CommandPayload, ResponsePayload>::GetAddress() << "]"
//                    << stream.str()
//                    << "|" << std::hex << moog::calculateCrc<CommandMsg>(command_msg_);
//  }
