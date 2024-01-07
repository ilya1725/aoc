
#include <algorithm>
#include <array>
#include <chrono>
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

#include "/home/ilya/archer/work/ProjectBits/doodles/src/time_measure.hpp"

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
}

namespace ex {
    const std::vector<std::string>
        data1{  {"O....#...."},
                {"O.OO#....#"},
                {".....##..."},
                {"OO.#O....O"},
                {".O.....O#."},
                {"O.#..O.#.#"},
                {"..O..#O..O"},
                {".......O.."},
                {"#....###.."},
                {"#OO..#...."},
                };

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
        auto ex_data{ex::data1};
        util::tiltN(ex_data);

        const auto result{util::calculate_load(ex_data)};

        std::cout << "E1:" << result << std::endl;
    }
    std::cout << std::endl;

    auto raw_input{util::get_input(data_file)};

    // P1
    if (1) {
        TimeMsr time_measure;

        util::tiltN(raw_input);

        const auto result{util::calculate_load(raw_input)};

        std::cout << "T1:" << result << std::endl;
        std::cout << time_measure << std::endl;
    }
    std::cout << std::endl;

    // Example
    {
        auto ex_data{ex::data1};
        std::unordered_map<uint64_t, uint64_t> cycle_data{};
        std::unordered_map<uint64_t, uint64_t> cycle_index{};

        uint64_t result_index{0};

        for (uint32_t i=0; i<1000000000; i++) {
            util::tiltN(ex_data);
            util::tiltW(ex_data);
            util::tiltS(ex_data);
            util::tiltE(ex_data);

            const auto resultX{util::calculate_load(ex_data)};
            std::cout << "E:" << i << ":" << resultX << std::endl;

            const auto data_sha{util::calculate_sha(ex_data)};
            if (cycle_data.find(data_sha) == cycle_data.end()) {
                cycle_data[data_sha] = resultX;
                cycle_index[data_sha] = i;
            } else {
                // found repeat
                const auto previous_index{cycle_index[data_sha]};
                const auto cycle{i - previous_index};
                result_index = (1000000000 - previous_index) % cycle + previous_index - 1;
                break;
            }
        }

        uint64_t final_result{0};
        for (auto data : cycle_index) {
            if (data.second == result_index) {
                final_result = cycle_data[data.first];
                break;
            }
        }

        std::cout << "E2:" << result_index << " " << final_result << std::endl;
    }
    std::cout << std::endl;

    // P2
    if (1) {
        auto ex_data{raw_input};
        std::unordered_map<uint64_t, uint64_t> cycle_data{};
        std::unordered_map<uint64_t, uint64_t> cycle_index{};

        uint64_t result_index{0};

        for (uint32_t i=0; i<1000000000; i++) {
            util::tiltN(ex_data);
            util::tiltW(ex_data);
            util::tiltS(ex_data);
            util::tiltE(ex_data);

            const auto resultX{util::calculate_load(ex_data)};
            std::cout << "E:" << i << ":" << resultX << std::endl;

            const auto data_sha{util::calculate_sha(ex_data)};
            if (cycle_data.find(data_sha) == cycle_data.end()) {
                cycle_data[data_sha] = resultX;
                cycle_index[data_sha] = i;
            } else {
                // found repeat
                const auto previous_index{cycle_index[data_sha]};
                const auto cycle{i - previous_index};
                result_index = (1000000000 - previous_index) % cycle + previous_index - 1;
                break;
            }
        }

        uint64_t final_result{0};
        for (auto data : cycle_index) {
            if (data.second == result_index) {
                final_result = cycle_data[data.first];
                break;
            }
        }

        std::cout << "P2:" << result_index << " " << final_result << std::endl;
    }
    std::cout << std::endl;

    return 0;
}