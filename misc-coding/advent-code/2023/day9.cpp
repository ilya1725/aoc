
#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
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

    std::vector<std::vector<int32_t>> convert_input(const std::vector<std::string> & input) {
        std::vector<std::vector<int32_t>> output;

        for (const auto & line : input) {
            const auto hand_str{util::split(line, ' ', true, std::vector<std::string>())};

            std::vector<int32_t> processed_line{};
            for (const auto & measurement : hand_str) {
                processed_line.emplace_back(std::atol(measurement.c_str()));
            }
            output.push_back(processed_line);
        }
        return output;
    }

    std::vector<int32_t> find_deltas(const std::vector<int32_t> & input) {
        std::vector<int32_t> output;

        for (size_t i=1; i<input.size(); i++) {
            output.emplace_back(input[i] - input[i-1]);
        }
        return output;
    }
}

namespace ex {
    const std::vector<std::string>
        report{{"0 3 6 9 12 15"},
                {"1 3 6 10 15 21"},
                {"10 13 16 21 30 45"}
                };
}

namespace part_one {

    std::unordered_map<std::string, std::pair<std::string, std::string>> convert_map(const std::vector<std::string> & input) {
        std::unordered_map<std::string, std::pair<std::string, std::string>> output{};

        for (const auto & line : input) {
            const auto line_split{util::split(line, ' ', true, std::vector<std::string>())};
            const std::string left(line_split[2].begin() + 1, line_split[2].end() - 1);
            const std::string right(line_split[3].begin(), line_split[3].end() - 1);
            output.emplace(std::make_pair(line_split[0], std::make_pair(left, right)));
        }
        return output;
    }
}

namespace part_two {

}

int main(int argc, char *argv[]) {

    // Example
    {
        auto report_ex1{util::convert_input(ex::report)};

        std::vector<int32_t> tails{};
        for (const auto & line : report_ex1) {
            std::vector<std::vector<int32_t>> process{line};

            while(true) {
                const auto reduction{util::find_deltas(process[process.size()-1])};
                process.push_back(reduction);

                if (std::all_of(reduction.cbegin(), reduction.cend(), [](int i) { return i == 0; })) {
                    break;
                }
            }

            // Processing next
            for (int32_t i=(process.size()-2); i>=0; i--) {
                const auto curr_tail{process[i][process[i].size()-1]};
                const auto next_tail{process[i+1][process[i+1].size()-1]};
                process[i].push_back(curr_tail + next_tail);
            }
            tails.push_back(process[0][process[0].size()-1]);
        }

        std::cout << "T: ";
        int32_t result{0};
        for (const auto tail : tails) {
            result += tail;
            std::cout << tail << " ";
        }
        std::cout << "|" << result << std::endl;
    }

    const auto raw_input{util::get_input("./day9.input")};
    auto report_p1{util::convert_input(raw_input)};

    // Part one
    {
        std::vector<int32_t> tails{};
        for (const auto & line : report_p1) {
            std::vector<std::vector<int32_t>> process{line};

            while(true) {
                const auto reduction{util::find_deltas(process[process.size()-1])};
                process.push_back(reduction);

                if (std::all_of(reduction.cbegin(), reduction.cend(), [](int i) { return i == 0; })) {
                    break;
                }
            }

            // Processing next
            for (int32_t i=(process.size()-2); i>=0; i--) {
                const auto curr_tail{process[i][process[i].size()-1]};
                const auto next_tail{process[i+1][process[i+1].size()-1]};
                process[i].push_back(curr_tail + next_tail);
            }
            tails.push_back(process[0][process[0].size()-1]);
        }

        std::cout << "T: ";
        int32_t result{0};
        for (const auto tail : tails) {
            result += tail;
            std::cout << tail << " ";
        }
        std::cout << "|" << result << std::endl;
    }

    // Example.p2
    {
        auto report_ex1{util::convert_input(ex::report)};

        std::vector<int32_t> tails{};
        for (const auto & line : report_ex1) {
            std::vector<std::vector<int32_t>> process{line};

            while(true) {
                const auto reduction{util::find_deltas(process[process.size()-1])};
                process.push_back(reduction);

                if (std::all_of(reduction.cbegin(), reduction.cend(), [](int i) { return i == 0; })) {
                    break;
                }
            }

            // Processing next
            process[process.size()-1].insert(process[process.size()-1].begin(), 0);

            for (int32_t i=(process.size()-2); i>=0; i--) {
                const auto curr_tail{process[i][0]};
                const auto next_tail{process[i+1][0]};
                process[i].insert(process[i].begin(), curr_tail - next_tail);
            }
            tails.push_back(process[0][0]);
        }

        std::cout << "T: ";
        int32_t result{0};
        for (const auto tail : tails) {
            result += tail;
            std::cout << tail << " ";
        }
        std::cout << "|" << result << std::endl;
    }

    // Part two
    {
        std::vector<int32_t> tails{};
        for (const auto & line : report_p1) {
            std::vector<std::vector<int32_t>> process{line};

            while(true) {
                const auto reduction{util::find_deltas(process[process.size()-1])};
                process.push_back(reduction);

                if (std::all_of(reduction.cbegin(), reduction.cend(), [](int i) { return i == 0; })) {
                    break;
                }
            }

            // Processing next
            process[process.size()-1].insert(process[process.size()-1].begin(), 0);

            for (int32_t i=(process.size()-2); i>=0; i--) {
                const auto curr_tail{process[i][0]};
                const auto next_tail{process[i+1][0]};
                process[i].insert(process[i].begin(), curr_tail - next_tail);
            }
            tails.push_back(process[0][0]);
        }

        std::cout << "T: ";
        int32_t result{0};
        for (const auto tail : tails) {
            result += tail;
            std::cout << tail << " ";
        }
        std::cout << "|" << result << std::endl;
    }

    return 0;
}