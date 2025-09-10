
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

    std::vector<std::string> get_input(const std::string & path) {
        std::ifstream file_in(path);
        if (!file_in) {
            throw("Can't open input file");
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

    std::vector<std::vector<char>> convert_input(const std::vector<std::string> & input) {
        std::vector<std::vector<char>> output;

        for (const auto & line : input) {

            std::vector<char> processed_line{};
            for(const char & c : line) {
                processed_line.emplace_back(c);
            }
            output.push_back(processed_line);
        }
        return output;
    }

    std::vector<std::vector<int16_t>> convert_input(const std::vector<std::vector<char>> & input) {
        std::vector<std::vector<int16_t>> output;

        for (const auto & line : input) {
            output.push_back(std::vector<int16_t>(line.size(), -1));
        }
        return output;
    }

    std::vector<std::vector<char>> expand_input(const std::vector<std::vector<char>> & input) {
        std::vector<std::vector<char>> output;

        for (const auto & line : input) {
            if (std::all_of(line.cbegin(), line.cend(), [](char i) { return i == '.'; })) {
                output.push_back(line);
            }
            output.push_back(line);
        }

        for (size_t x=0; x<output[0].size(); x++) {
            bool all_same{true};
            for (size_t y=0; y<output.size(); y++) {
                if (output[y][x] != '.') {
                    all_same = false;
                    break;
                }
            }

            if (all_same) {
                for (size_t y=0; y<output.size(); y++) {
                    output[y].insert(std::next(output[y].begin(), x), '.');
                }
                x++;
            }
        }
        return output;
    }

    std::vector<std::vector<uint64_t>> expand_input(const std::vector<std::vector<char>> & input, const uint32_t step) {
        std::vector<std::vector<uint64_t>> output;

        const uint64_t default_point{1};
        for (const auto & line : input) {
            uint64_t point{default_point};
            if (std::all_of(line.cbegin(), line.cend(), [](char i) { return i == '.'; })) {
                point *= step;
            }

            std::vector<uint64_t> output_line{};
            for (size_t x=0; x<line.size(); x++) {
                output_line.push_back(point);
            }
            output.push_back(output_line);
        }

        //
        for (size_t x=0; x<output[0].size(); x++) {
            bool all_same{true};
            for (size_t y=0; y<output.size(); y++) {
                if (input[y][x] != '.') {
                    all_same = false;
                    break;
                }
            }

            if (all_same) {
                for (size_t y=0; y<output.size(); y++) {
                    if (output[y][x-1] == (default_point + step)) {
                        output[y][x] = output[y][x-1] * 2;
                    } else {
                        output[y][x] *= step;
                    }
                }
            }
        }
        return output;
    }

    std::vector<std::vector<int16_t>> count_galaxies(const std::vector<std::vector<char>> & input) {
        std::vector<std::vector<int16_t>> output;

        for (const auto & line : input) {
            output.push_back(std::vector<int16_t>(line.size(), 0));
        }

        int16_t cnt{1};
        for (size_t y=0; y<input.size(); y++) {
            for(size_t x=0; x<input[0].size(); x++) {
                if (input[y][x] == '#') {
                    output[y][x] = cnt;
                    cnt++;
                }
            }
        }
        return output;
    }

    std::vector<std::pair<uint8_t, uint8_t>> get_galaxies(const std::vector<std::vector<char>> & input) {
        std::vector<std::pair<uint8_t, uint8_t>> output;

         for (size_t y=0; y<input.size(); y++) {
            for(size_t x=0; x<input[0].size(); x++) {
                if (input[y][x] == '#') {
                    output.push_back({y, x});
                }
            }
        }
        return output;
    }

    template <typename T>
    void print_map(const std::vector<std::vector<T>> & map) {
        for (size_t y=0; y<map.size(); y++) {
            for(size_t x=0; x<map[0].size(); x++) {
                std::cout << map[y][x] << " ";
            }
            std::cout << std::endl;
        }
    }

    int32_t distance(const std::pair<uint8_t, uint8_t> & a, const std::pair<uint8_t, uint8_t> & b) {
        return {std::abs(a.first - b.first) + std::abs(a.second - b.second) };
    }

    int64_t distance(const std::pair<uint8_t, uint8_t> & a, const std::pair<uint8_t, uint8_t> & b, const std::vector<std::vector<uint64_t>> & map) {
        const auto delta_y{std::abs(a.first - b.first)};
        const auto delta_x{std::abs(a.second - b.second)};
        int64_t steps{0};

        auto start_y{a.first};
        if (a.first > b.first) {
            start_y = b.first;
        }
        for (uint16_t y=(start_y+1); y<(start_y + delta_y); y++) {
            steps += map[y][a.second];
        }

        auto start_x{a.second};
        if (a.second > b.second) {
            start_x = b.second;
        }
        uint16_t offset{0};
        if (delta_y == 0) {
            offset++;
        }
        for (uint16_t x=(start_x + offset); x<=(start_x + delta_x); x++) {
            steps += map[b.first][x];
        }

        return steps;
    }
}

namespace ex {
    const std::vector<std::string>
        data{   {"...#......"},
                {".......#.."},
                {"#........."},
                {".........."},
                {"......#..."},
                {".#........"},
                {".........#"},
                {".........."},
                {".......#.."},
                {"#...#....."}
                };

}

namespace part_one {

}

namespace part_two {

}

int main(int argc, char *argv[]) {

    // Example
    {
        const auto data_ex{util::convert_input(ex::data)};
        const auto data_expanded{util::expand_input(data_ex)};
        util::print_map(data_expanded);

        const auto data_counts{util::count_galaxies(data_expanded)};

        const auto galaxies{util::get_galaxies(data_expanded)};

        std::vector<int16_t> distances{};
        for (size_t d=0; d<galaxies.size(); d++) {
            for (size_t f=(d+1); f<galaxies.size(); f++) {
                distances.push_back(util::distance(galaxies[d], galaxies[f]));
            }
        }

        uint32_t total_distance{0};
        for (const auto d: distances) {
            std::cout << d << std::endl;
            total_distance += d;
        }
        std::cout << "T:" << total_distance << std::endl;
    }

    const auto raw_input{util::get_input("./day11.input")};
    const auto report_p1{util::convert_input(raw_input)};

    // P1
    {
        const auto data_expanded{util::expand_input(report_p1)};
        // util::print_map(data_expanded);

        const auto data_counts{util::count_galaxies(data_expanded)};

        const auto galaxies{util::get_galaxies(data_expanded)};

        std::vector<int16_t> distances{};
        for (size_t d=0; d<galaxies.size(); d++) {
            for (size_t f=(d+1); f<galaxies.size(); f++) {
                distances.push_back(util::distance(galaxies[d], galaxies[f]));
            }
        }

        uint32_t total_distance{0};
        for (const auto d: distances) {
            // std::cout << d << std::endl;
            total_distance += d;
        }
        std::cout << "T:" << total_distance << std::endl;
    }
    std::cout << std::endl;

    // Example 2
    {
        const auto data_ex{util::convert_input(ex::data)};
        const auto data_expanded{util::expand_input(data_ex, 100)};
        util::print_map(data_expanded);

        const auto data_counts{util::count_galaxies(data_ex)};

        const auto galaxies{util::get_galaxies(data_ex)};

        std::vector<int16_t> distances{};
        for (size_t d=0; d<galaxies.size(); d++) {
            for (size_t f=(d+1); f<galaxies.size(); f++) {
                distances.push_back(util::distance(galaxies[d], galaxies[f], data_expanded));
            }
        }

        uint32_t total_distance{0};
        for (const auto d: distances) {
            std::cout << d << std::endl;
            total_distance += d;
        }
        std::cout << "T:" << total_distance << std::endl;
    }

    // P2
    {
        const auto data_expanded{util::expand_input(report_p1, 1000000)};
        // util::print_map(data_expanded);

        const auto data_counts{util::count_galaxies(report_p1)};

        const auto galaxies{util::get_galaxies(report_p1)};

        std::vector<int64_t> distances{};
        for (size_t d=0; d<galaxies.size(); d++) {
            for (size_t f=(d+1); f<galaxies.size(); f++) {
                distances.push_back(util::distance(galaxies[d], galaxies[f], data_expanded));
            }
        }

        uint64_t total_distance{0};
        for (const auto d: distances) {
            // std::cout << d << std::endl;
            total_distance += d;
        }
        std::cout << "T:" << total_distance << std::endl;
    }
    std::cout << std::endl;

    return 0;
}