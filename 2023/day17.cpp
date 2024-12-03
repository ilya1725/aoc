
#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <queue>
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
        data1{  {"2413432311323"},
                {"3215453535623"},
                {"3255245654254"},
                {"3446585845452"},
                {"4546657867536"},
                {"1438598798454"},
                {"4457876987766"},
                {"3637877979653"},
                {"4654967986887"},
                {"4564679986453"},
                {"1224686865563"},
                {"2546548887735"},
                {"4322674655533"}
                };

}

namespace part_one {
}

namespace part_two {
}

int main(int argc, char *argv[]) {

    std::string data_file{"./day17.input"};
    if (argc == 2) {
        data_file = {argv[1]};
    }

    // Example
    {
         std::cout << "E1:" << result << std::endl;
    }
    std::cout << std::endl;

    auto raw_input{util::get_input(data_file)};

    // P1
    const auto data_input{util::get_input(data_file)};
    if (1) {

         std::cout << "T1:" << result << std::endl;
    }
    std::cout << std::endl;

    // Example
    {
        std::cout << "E2 max:" << *std::max_element(results.begin(), results.end()) << std::endl;
    }

    std::cout << std::endl;

    // P2
    if (1) {
        std::vector<uint32_t> results{};

        std::cout << "T2 max:" << *std::max_element(results.begin(), results.end()) << std::endl;
    }
    std::cout << std::endl;

    return 0;
}