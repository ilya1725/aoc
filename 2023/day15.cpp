
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <set>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "time_measure.hpp"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

namespace util {
    template < typename StringT,
           typename DelimiterT = char,
           typename ContainerT = std::vector<std::string_view> >
    ContainerT split(StringT const& str, DelimiterT const& delimiters = ' ', bool trimEmpty = true, ContainerT&& tokens = {})
    {
        typename StringT::size_type pos, lastPos = 0, length = str.length();

        while (lastPos < length + 1)
        {
            pos = str.find_first_of(delimiters, lastPos);
            if (pos == StringT::npos)
                pos = length;

            if (pos != lastPos || !trimEmpty)
                tokens.emplace_back(str.data() + lastPos, pos - lastPos);

            lastPos = pos + 1;
        }

        return std::forward<ContainerT>(tokens);
    }

    constexpr bool issymbol(const char a) {
        return (a != '.' && !std::isdigit(a));
    }
    constexpr bool isgear(const char a) {
        return (a == '*');
    }

    // Return vector of strings
    std::vector<std::string> get_input(const std::string & path) {
        std::ifstream file_in(path);
        if (!file_in) {
            std::cerr << "Can't open input file: " << path << std::endl;
            std::exit(-1);
        }

        std::string line;
        std::vector<std::string> output{};
        while (std::getline(file_in, line))
        {
            output.push_back(line);
        }
        return output;
    }

    const uint64_t calculate_hash(const std::string & input) {
        uint64_t result{0};

        for (const auto a : input) {
            result += static_cast<uint64_t>(a);
            result *= 17;
            result = result & 0xFF;
        }
        return result;
    }

    const int64_t get_lens(const std::vector<std::pair<std::string, uint8_t>> & box,
                            const std::string & lens_label) {
        int64_t result{-1};
        for (int64_t idx=0; const auto & l : box) {
            if (l.first == lens_label) {
                result = idx;
                break;
            }
        }

        return result;
    }

    const int64_t add_lens(std::vector<std::pair<std::string, uint8_t>> & box,
                            const std::string & lens_label, const uint8_t value) {
        int64_t result{-1};
        auto it = box.begin();
        for(;it != box.end(); it++){
            if (it->first == lens_label) {
                it->second = value;
                break;
            }
        }

        if (it == box.end()) {
            box.push_back({lens_label, value});
        }

        return result;
    }

    const int64_t remove_lens(std::vector<std::pair<std::string, uint8_t>> & box,
                            const std::string & lens_label) {
        int64_t result{-1};
        for(auto it = box.begin();it != box.end(); it++){
            if (it->first == lens_label) {
                box.erase(it);
                break;
            }
        }

        return result;
    }

    const int64_t calculate_lens(const std::vector<std::pair<std::string, uint8_t>> & box,
                                const size_t box_index) {
        int64_t result{0};
        for (uint64_t idx=1; const auto & l : box) {
            result += ((box_index + 1) * idx * l.second);
            idx++;
        }

        return result;
    }

}

namespace ex {
    const std::string data1{"HASH"};

    const std::string data2{"rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7"};

}

namespace part_one {

}

namespace part_two {

}

int main(int argc, char *argv[]) {

    std::string data_file{"./day15.input"};
    if (argc == 2) {
        data_file = {argv[1]};
    }

    // Example
    {
        const auto result{util::calculate_hash(ex::data1)};

        std::cout << "E1:" << result << std::endl;
    }
    {
        const auto split_data{util::split(ex::data2, ',', true, std::vector<std::string>())};

        std::vector<uint64_t>result_vector{};
        for (const auto & d : split_data) {
            result_vector.emplace_back(util::calculate_hash(d));
        }

        const auto total{std::reduce(result_vector.begin(), result_vector.end(), 0ULL)};

        std::cout << "E1:" << total << std::endl;
    }
    std::cout << std::endl;

    auto raw_input{util::get_input(data_file)};

    // P1
    if (1) {
        TimeMsr time_measure;

        const auto split_data{util::split(raw_input[0], ',', true, std::vector<std::string>())};

        std::vector<uint64_t>result_vector{};
        for (const auto & d : split_data) {
            result_vector.emplace_back(util::calculate_hash(d));
        }

        const auto total{std::reduce(result_vector.begin(), result_vector.end(), 0ULL)};

        std::cout << "T1:" << total << std::endl;
        std::cout << time_measure << std::endl;
    }
    std::cout << std::endl;

    // Example
    {
        const auto split_data{util::split(ex::data2, ',', true, std::vector<std::string>())};
        std::array<std::vector<std::pair<std::string, uint8_t>>, 256> boxes{};

        for (const auto & d : split_data) {
            size_t substr_count{0};
            uint8_t lens_value{0};
            bool lens_add{false};

            if (d[d.length()-1] == '-') {
                substr_count = d.length() - 1;
            } else {
                substr_count = d.length() - 2;
                lens_add = true;
                lens_value = std::atoi(&d[d.length()-1]);
            }
            const auto lens_label{d.substr(0, substr_count)};
            const auto box_number(util::calculate_hash(lens_label));

            if (lens_add) {
                // add new lens to the box
                const auto location{util::add_lens(boxes[box_number], lens_label, lens_value)};
            } else {
                // remove the lens from the box
                const auto location{util::remove_lens(boxes[box_number], lens_label)};
            }
        }

        // Calculate
        std::vector<uint64_t>result_vector{};
        for (uint64_t idx=0; const auto & b : boxes) {
            result_vector.emplace_back(util::calculate_lens(b, idx));
            idx++;
        }

        const auto total{std::reduce(result_vector.begin(), result_vector.end(), 0ULL)};

        std::cout << "E2:" << total << std::endl;
    }
    std::cout << std::endl;

    // P2
    if (1) {
        TimeMsr time_measure;

        const auto split_data{util::split(raw_input[0], ',', true, std::vector<std::string>())};
        std::array<std::vector<std::pair<std::string, uint8_t>>, 256> boxes{};

        for (const auto & d : split_data) {
            size_t substr_count{0};
            uint8_t lens_value{0};
            bool lens_add{false};

            if (d[d.length()-1] == '-') {
                substr_count = d.length() - 1;
            } else {
                substr_count = d.length() - 2;
                lens_add = true;
                lens_value = std::atoi(&d[d.length()-1]);
            }
            const auto lens_label{d.substr(0, substr_count)};
            const auto box_number(util::calculate_hash(lens_label));

            if (lens_add) {
                // add new lens to the box
                const auto location{util::add_lens(boxes[box_number], lens_label, lens_value)};
            } else {
                // remove the lens from the box
                const auto location{util::remove_lens(boxes[box_number], lens_label)};
            }
        }

        // Calculate
        std::vector<uint64_t>result_vector{};
        for (uint64_t idx=0; const auto & b : boxes) {
            result_vector.emplace_back(util::calculate_lens(b, idx));
            idx++;
        }

        const auto total{std::reduce(result_vector.begin(), result_vector.end(), 0ULL)};

        std::cout << "T2:" << total << std::endl;
    }
    std::cout << std::endl;

    return 0;
}