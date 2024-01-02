
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

    const uint8_t count_errors(const std::string & line) {
        uint8_t count{0};
        for (const char a: line) {
            if (a == '?') {
                count++;
            }
        }
        return count;
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

    // Return vector of vectors of strings. Separation is by the empty lines
    std::vector<std::vector<std::string>> get_input2(const std::string & path) {
        std::ifstream file_in(path);
        if (!file_in) {
            std::cerr << "Can't open input file: " << path << std::endl;
            std::exit(-1);
        }

        std::string line;
        std::vector<std::vector<std::string>> output{};
        std::vector<std::string> sub_output{};

        while (std::getline(file_in, line))
        {
            if (line.length() == 0) {
                output.push_back(sub_output);
                sub_output.clear();
            } else {
                sub_output.push_back(line);
            }
        }

        if (sub_output.size() != 0) {
            output.push_back(sub_output);
        }
        return output;
    }

    // return the number of characters different. 0 is equal
    size_t equal_column(const std::vector<std::string> & data, const size_t a, const size_t b) {
        size_t result{0};

        if (b >= data[0].length()) {
            std::cout << "ERROR: " << b << ":" << data[0].length() << std::endl;
            return -1;
        }
        for (size_t row=0; row<data.size(); row++) {
            if (data[row][a] != data[row][b]) {
                result++;
            }
        }

        return result;
    }
    size_t equal_row(const std::vector<std::string> & data, const size_t a, const size_t b) {
        size_t result{0};

        if (b >= data.size()) {
            std::cout << "ERROR: " << b << ":" << data.size() << std::endl;
            return -1;
        }
        for (size_t column=0; column<data[0].length(); column++) {
            if (data[a][column] != data[b][column]) {
                result++;
            }
        }

        return result;
    }

    // Return: idex of the right location, size
    std::pair<size_t, uint16_t> longest_column(const std::vector<std::string> & data, const size_t diff=0) {
        std::pair<size_t, uint16_t> result{0, 0};

        size_t begin{0};
        while(true) {
            size_t diff_cnt{0};
            bool found{false};
            bool smudge_found{false};

            for (size_t column=begin+1; column<data[0].length(); column++) {
                diff_cnt = equal_column(data, column-1, column);
                if (diff_cnt <= diff) {
                    begin = column;
                    found = true;
                    break;
                }
            }
            if (found == false) {
                break;
            }
            if (diff_cnt == diff) {
                smudge_found = true;
            }

            // Lowest count
            size_t end_count{1};
            size_t count = (data[0].length() - begin) < (begin) ? (data[0].length() - begin) : begin;
            for (size_t column=1; column<count; column++) {
                diff_cnt = equal_column(data, begin - column - 1, begin + column);
                if (diff_cnt > diff) {
                    break;
                }
                if (diff_cnt == diff) {
                    smudge_found = true;
                }
                end_count++;
            }

            if (((begin - end_count) == 0 || (begin + end_count) == data[0].length()) && smudge_found) {
                result = {begin, end_count};
                break;
            }
        }

        return result;
    }

    // Return: idex of the bottom location, size
    std::pair<size_t, uint16_t> longest_row(const std::vector<std::string> & data, const size_t diff=0) {
        std::pair<size_t, uint16_t> result{0, 0};

        size_t begin{0};
        while(true) {
            size_t diff_cnt{0};
            bool found{false};
            bool smudge_found{false};

            for (size_t row=begin+1; row<data.size(); row++) {
                diff_cnt = equal_row(data, row-1, row);
                if (diff_cnt <= diff) {
                    begin = row;
                    found = true;
                    break;
                }
            }
            if (found == false) {
                break;
            }
            if (diff_cnt == diff) {
                smudge_found = true;
            }

            // Lowest count
            size_t end_count{1};
            size_t count = (data.size() - begin) < (begin) ? (data.size() - begin) : begin;
            for (size_t row=1; row<count; row++) {
                diff_cnt = equal_row(data, begin - row - 1, begin + row);
                if (diff_cnt > diff) {
                    break;
                }
                if (diff_cnt == diff) {
                    smudge_found = true;
                }
                end_count++;
            }

            if (((begin - end_count) == 0 || (begin + end_count) == data.size()) && smudge_found) {
                result = {begin, end_count};
                break;
            }
        }

        return result;
    }
}

namespace ex {
    const std::vector<std::string>
        data1{  {"#.##..##."},
                {"..#.##.#."},
                {"##......#"},
                {"##......#"},
                {"..#.##.#."},
                {"..##..##."},
                {"#.#.##.#."}
                };

    const std::vector<std::string>
        data2{  {"#...##..#"},
                {"#....#..#"},
                {"..##..###"},
                {"#####.##."},
                {"#####.##."},
                {"..##..###"},
                {"#....#..#"}
                };

}

namespace part_one {

}

namespace part_two {

}

int main(int argc, char *argv[]) {

    std::string data_file{"./day13.test.input"};
    if (argc == 2) {
        data_file = {argv[1]};
    }

    // Example
    {
        uint64_t result{0};
        const auto result_ex1c{util::longest_column(ex::data1)};
        const auto result_ex1r{util::longest_row(ex::data1)};

        // perfect mirror goes to the end
        if ((result_ex1c.first + result_ex1c.second == ex::data1[0].length()) || (result_ex1c.first - 1 - result_ex1c.second == 0)) {
            result += result_ex1c.first;
        } else {
            result += result_ex1r.first * 100;
        }

        const auto result_ex2c{util::longest_column(ex::data2)};
        const auto result_ex2r{util::longest_row(ex::data2)};

        if ((result_ex2c.first + result_ex2c.second == ex::data1[0].length()) || (result_ex2c.first - 1 - result_ex2c.second == 0)) {
            result += result_ex2c.first;
        } else {
            result += result_ex2r.first * 100;
        }

        std::cout << "E1:" << result << std::endl;
    }
    std::cout << std::endl;

    const auto raw_input{util::get_input2(data_file)};

    // P1
    if (1) {
        TimeMsr time_measure;
        uint64_t result{0};

        for (size_t idx=0; const auto & mirror : raw_input) {
            const auto result_c{util::longest_column(mirror)};
            const auto result_r{util::longest_row(mirror)};

            std::cout << "C:" << idx << ":" << result_c.first << "-" << result_c.second << "|"
                                            << result_r.first << "-" << result_r.second;
            if (result_c.second != 0) {
                result += result_c.first;
                std::cout << " c" << std::endl;
            } else {
                result += result_r.first * 100;
                std::cout << " r" << std::endl;
            }
            idx++;
            time_measure.sample(13);

        }

        std::cout << "T1:" << result << std::endl;
        std::cout << time_measure << std::endl;
    }
    std::cout << std::endl;

    // Example
    {
        uint64_t result{0};
        const auto result_ex1c2{util::longest_row(ex::data1, 1)};
        result += result_ex1c2.first * 100;

        const auto result_ex2c2{util::longest_row(ex::data2, 1)};
        result += result_ex2c2.first * 100;

        std::cout << "E2:" << result << std::endl;
    }
    std::cout << std::endl;

    // P2
    if (1) {
        TimeMsr time_measure;
        uint64_t result{0};

        for (size_t idx=0; const auto & mirror : raw_input) {
            const auto result_c{util::longest_column(mirror, 1)};
            const auto result_r{util::longest_row(mirror, 1)};

            std::cout << "C:" << idx << ":" << result_c.first << "-" << result_c.second << "|"
                                            << result_r.first << "-" << result_r.second;
            if (result_c.second != 0) {
                result += result_c.first;
                std::cout << " c" << std::endl;
            } else {
                result += result_r.first * 100;
                std::cout << " r" << std::endl;
            }
            idx++;
            time_measure.sample(13);
        }

        std::cout << "T2:" << result << std::endl;
        std::cout << time_measure << std::endl;
    }
    std::cout << std::endl;

    return 0;
}