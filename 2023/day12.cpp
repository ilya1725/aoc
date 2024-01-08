
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

    const uint8_t count_errors(const std::string & line) {
        uint8_t count{0};
        for (const char a: line) {
            if (a == '?') {
                count++;
            }
        }
        return count;
    }

    bool fill_errors(std::string & line, const uint64_t value) {
        for (size_t i=0, j=0; i<line.length(); i++) {
            if (line[i] == '?') {
                const auto flag{(value >> j) & 0x1};
                line[i] = ((flag == 1) ? '#' : '.');
                j++;
            }
        }
        return true;
    }

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

    std::vector<std::pair<std::string, std::vector<uint8_t>>> convert_input(const std::vector<std::string> & input) {
        std::vector<std::pair<std::string, std::vector<uint8_t>>> output;

        for (const auto & line : input) {
            const auto split_vector{util::split(line, ' ', true, std::vector<std::string>())};
            const auto split_counts{util::split(split_vector[1], ',', true, std::vector<std::string>())};

            std::pair<std::string, std::vector<uint8_t>> data{split_vector[0], {}};

            for (const auto & item : split_counts) {
                data.second.push_back(std::atoi(item.c_str()));
            }
            output.push_back(data);
        }
        return output;
    }

    std::vector<std::pair<std::string, std::vector<uint8_t>>> convert_input(const std::vector<std::pair<std::string,
                                                                            std::vector<uint8_t>>> & input,
                                                                            const uint8_t multiply = 5) {
        std::vector<std::pair<std::string, std::vector<uint8_t>>> output;

        for (const auto & line : input) {
            std::pair<std::string, std::vector<uint8_t>> data{line.first, line.second};
            for (uint8_t m=1; m<multiply; m++) {
                data.first = data.first + "?" + line.first;
                data.second.insert(data.second.end(), line.second.begin(), line.second.end());
            }
            output.push_back(data);
        }
        return output;
    }

    bool verify(const std::string & input, const std::vector<uint8_t> & list) {
        bool in_group{false};
        uint8_t group_cnt{0};
        size_t list_index{0};
        for (const char s : input) {
            if (s == '#') {
                in_group = true;
                group_cnt++;
            } else if (s == '.') {
                if (in_group) {
                    in_group = false;
                    if (list_index == list.size()) {
                        return false;
                    }
                    if (group_cnt != list[list_index]) {
                        return false;
                    }
                    list_index++;
                    group_cnt = 0;
                }
            } else {
                std::cout << "Error: " << s << std::endl;
                return false;
            }
        }

        // There may be a tail
        if (in_group) {
            if (list_index == (list.size()-1) && group_cnt == list[list_index]) {
                return true;
            }
        } else {
            if (list_index == list.size()) {
                return true;
            }
        }
        return false;
    }

    bool verify(const std::string & input, const std::vector<uint8_t> & list, const uint64_t flags) {
        bool in_group{false};
        uint8_t group_cnt{0};
        size_t list_index{0};
        size_t j{0};

        for (const char s : input) {
            if (s == '#') {
                in_group = true;
                group_cnt++;
            } else if (s == '?') {
                const auto flag{(flags >> j) & 0x1};
                j++;

                if (flag) {
                    in_group = true;
                    group_cnt++;
                } else {
                    if (in_group) {
                        in_group = false;
                        if (list_index == list.size()) {
                            return false;
                        }
                        if (group_cnt != list[list_index]) {
                            return false;
                        }
                        list_index++;
                        group_cnt = 0;
                    }
                }
            } else if (s == '.') {
                if (in_group) {
                    in_group = false;
                    if (list_index == list.size()) {
                        return false;
                    }
                    if (group_cnt != list[list_index]) {
                        return false;
                    }
                    list_index++;
                    group_cnt = 0;
                }
            } else {
                std::cout << "Error: " << s << std::endl;
                return false;
            }
        }

        // There may be a tail
        if (in_group) {
            if (list_index == (list.size()-1) && group_cnt == list[list_index]) {
                return true;
            }
        } else {
            if (list_index == list.size()) {
                return true;
            }
        }

        return false;
    }

    // Verify the string from the now till the end of the possible vector of vectors.
    // begin_possible points to the first vector of begin indexes
    // begin_length points to the first element of the length
    uint64_t count(const std::string_view input,
                    const size_t start_location,
                    std::vector<std::vector<uint64_t>>::const_iterator begin_possible,
                    std::vector<std::vector<uint64_t>>::const_iterator end_possible,
                    std::vector<uint8_t>::const_iterator begin_length,
                    std::vector<uint8_t>::const_iterator end_length) {
        uint64_t result{0};

        if (begin_possible == end_possible) {
            return result;
        }

        const auto curr_length{*begin_length};
        int i{0};
        for (const auto index : *begin_possible) {
            if (index < start_location) {
                continue;
            }
            if (start_location == 0) {
                std::cout << std::string(start_location, ' ') << index << ":"
                            << static_cast<uint16_t>(curr_length) << ":" << i++ << std::endl;
            }

            // No #s between start and begin
            bool hash_clear{true};
            for (size_t h=start_location; h<index; h++) {
                if (input[h] == '#') {
                    hash_clear = false;
                }
            }
            if (!hash_clear) {
                // std::cout << "#\n";
                continue;
            }

            // String from this index for length should be ? or #, not .
            size_t k=index;
            for (; k<(index + *begin_length) && k<input.length(); k++) {
                if (input[k] == '.') {
                    std::cout << "Error: " << k << std::endl;
                    return 0;
                }
            }

            // Check if the last possibility
            if ((begin_possible + 1) == end_possible) {
                // There should be no more #s left
                bool end_clear{true};
                for (size_t e=k; e<input.length(); e++) {
                    if (input[e] == '#') {
                        end_clear = false;
                    }
                }

                if (end_clear) {
                    // std::cout << "L\n";
                    result++;
                }
            } else {
                // start next only if it is higher (or further)
                result += count(input, k+1, begin_possible+1, end_possible, begin_length+1, end_length);
            }
        }

        return result;
    }

    // Verify the string from the now till the end of the possible vector of vectors with caching.
    // The chache stores the results of the same calculation so no need to do it again
    // begin_possible points to the first vector of begin indexes
    // begin_length points to the first element of the length
    uint64_t count(const std::string_view input,
                    const size_t start_location,
                    std::unordered_map<size_t, std::unordered_map<uint8_t, uint64_t>> & cache,
                    std::vector<std::vector<uint64_t>>::const_iterator begin_possible,
                    std::vector<std::vector<uint64_t>>::const_iterator end_possible,
                    std::vector<uint8_t>::const_iterator begin_length,
                    std::vector<uint8_t>::const_iterator end_length) {
        uint64_t result{0};

        if (begin_possible == end_possible) {
            return result;
        }

        const auto cache_location{std::distance(begin_possible, end_possible)};
        auto cache_it{cache.find(cache_location)};

        const auto curr_length{*begin_length};
        int i{0};
        for (const auto index : *begin_possible) {
            uint64_t local_result{0};

            if (index < start_location) {
                continue;
            }
            if (start_location == 0) {
                std::cout << std::string(start_location, ' ') << index << ":"
                            << static_cast<uint16_t>(curr_length) << ":" << i++ << std::endl;
            }
            if (cache_it != cache.end()) {
                auto value_it{cache_it->second.find(index)};
                if (value_it != cache_it->second.end()) {
                    // std::cout << std::string(start_location, ' ') << "C:" << value_it->second << std::endl;
                    result += value_it->second;
                    continue;
                }
            }

            // No #s between start and begin
            bool hash_clear{true};
            for (size_t h=start_location; h<index; h++) {
                if (input[h] == '#') {
                    hash_clear = false;
                }
            }
            if (!hash_clear) {
                // std::cout << "#\n";
                continue;
            }

            // String from this index for length should be ? or #, not .
            size_t k=index;
            for (; k<(index + *begin_length) && k<input.length(); k++) {
                if (input[k] == '.') {
                    std::cout << "Error: " << k << std::endl;
                    return 0;
                }
            }

            // Check if the last possibility
            if ((begin_possible + 1) == end_possible) {
                // There should be no more #s left
                bool end_clear{true};
                for (size_t e=k; e<input.length(); e++) {
                    if (input[e] == '#') {
                        end_clear = false;
                    }
                }

                if (end_clear) {
                    // std::cout << "L\n";
                    local_result = 1;
                }
            } else {
                // start next only if it is higher (or further)
                local_result = count(input, k+1, cache, begin_possible+1, end_possible, begin_length+1, end_length);
            }

            // Update cache
            cache[cache_location][index] = local_result;

            result += local_result;
        }

        return result;
    }

    std::vector<uint64_t> possible_placements(const uint64_t offset,
                                            const uint64_t width,
                                            const std::string & data) {
        std::vector<uint64_t>  result;

        uint64_t i = offset;
        uint64_t j = i;

        while (i < data.length()) {
            // find next start
            while (data[i] == '.' && i < data.length()) {
                i++;
                j++;
            }

            if (i == data.length()) {
                break;
            }
            while (data[j] != '.' && j < data.length()) {
                j++;
            }

            for (int64_t k=i; k<=static_cast<int64_t>(j-width); k++) {
                if ((k-1 == -1 || data[k-1] != '#') &&
                    (k+width == data.length() || data[k+width] != '#')) {
                    result.push_back(k);
                }
                if (i == 0 && data[i] == '#') {
                    return result;
                }
            }

            i = j;
        }

        return result;
    }

    std::vector<std::vector<uint64_t>> all_placements(const std::pair<std::string, std::vector<uint8_t>> & data) {
        std::vector<std::vector<uint64_t>> result;
        std::string suffix{data.first};
        uint64_t offset{0};

        for (auto v : data.second) {
            result.push_back(possible_placements(offset, v, suffix));

            if (result.back().size() == 0) {
                offset = suffix.length();
            } else {
                offset = result.back()[0] + v + 1;
            }
        }
        return result;
    }
}

namespace ex {
    const std::vector<std::string>
        data1{  {"#.#.### 1,1,3"},
                {".#...#....###. 1,1,3"},
                {".#.###.#.###### 1,3,1,6"},
                {"####.#...#... 4,1,1"},
                {"#....######..#####. 1,6,5"},
                {".###.##....# 3,2,1"}
                };

    const std::vector<std::string>
        data{   {"???.### 1,1,3"},
                {".??..??...?##. 1,1,3"},
                {"?#?#?#?#?#?#?#? 1,3,1,6"},
                {"????.#...#... 4,1,1"},
                {"????.######..#####. 1,6,5"},
                {"?###???????? 3,2,1"}
                };

}

namespace part_one {

}

namespace part_two {

}

int main(int argc, char *argv[]) {

    std::string data_file{"./day12.input"};
    if (argc == 2) {
        data_file = {argv[1]};
    }
    // Example
    {
        const auto data_ex{util::convert_input(ex::data)};

        // const auto data_ex1{util::convert_input(ex::data1)};
        // for (const auto & d : data_ex1) {
        //     bool result{util::verify(d.first, d.second)};
        //     std::cout << d.first << ":" << result << std::endl;
        // }

        std::vector<uint32_t> accepted{};
        for (const auto & line : data_ex) {
            const auto error_count{util::count_errors(line.first)};

            uint32_t accepted_cnt{0};
            for (uint32_t i=0; i<(1 << error_count); i++) {
                // auto line_copy{line.first};
                // util::fill_errors(line_copy, i);

                if (util::verify(line.first, line.second, i)) {
                    accepted_cnt++;
                }
            }
            accepted.push_back(accepted_cnt);

            std::cout << line.first << ": " << accepted_cnt << std::endl;
        }
        std::cout << "T:" << std::accumulate(accepted.begin(), accepted.end(), 0) << std::endl;
    }
    std::cout << std::endl;

    // Example.m2
    {
        const auto data_ex{util::convert_input(ex::data)};
        std::vector<uint32_t> accepted{};

        for (const auto & line : data_ex) {
            const auto possible{util::all_placements(line)};
            std::unordered_map<size_t, std::unordered_map<uint8_t, uint64_t>> cache{};

            const auto accepted_cnt{util::count(line.first, 0, cache,
                                                possible.cbegin(), possible.cend(),
                                                line.second.cbegin(), line.second.cend())};
            accepted.push_back(accepted_cnt);

            std::cout << line.first << ": " << accepted_cnt << std::endl;
        }
        std::cout << "T:" << std::accumulate(accepted.begin(), accepted.end(), 0) << std::endl;
    }
    std::cout << std::endl;

    const auto raw_input{util::get_input(data_file)};
    const auto report_p1{util::convert_input(raw_input)};

    // P1
    if (0) {
        TimeMsr time_measure;

        std::vector<uint32_t> accepted{};
        for (const auto & line : report_p1) {
            time_measure.start();
            const auto error_count{util::count_errors(line.first)};

            uint32_t accepted_cnt{0};
            // std::string line_copy{};
            for (uint32_t i=0; i<(1 << error_count); i++) {
                // line_copy = line.first;
                // util::fill_errors(line_copy, i);

                if (util::verify(line.first, line.second, i)) {
                    accepted_cnt++;
                }
            }
            accepted.push_back(accepted_cnt);
            time_measure.sample(0);

            std::cout << line.first << ": " << accepted_cnt << std::endl;
        }
        std::cout << "T:" << std::accumulate(accepted.begin(), accepted.end(), 0) << std::endl;
        std::cout << time_measure << std::endl;
    }
    std::cout << std::endl;

    if (1) {
        TimeMsr time_measure;

        std::vector<uint32_t> accepted{};
        for (const auto & line : report_p1) {
            const auto possible{util::all_placements(line)};
            std::unordered_map<size_t, std::unordered_map<uint8_t, uint64_t>> cache{};

            const auto accepted_cnt{util::count(line.first, 0, cache,
                                                possible.cbegin(), possible.cend(),
                                                line.second.cbegin(), line.second.cend())};
            accepted.push_back(accepted_cnt);
            time_measure.sample(2);

            std::cout << line.first << ": " << accepted_cnt << std::endl;
        }
        std::cout << "T:" << std::accumulate(accepted.begin(), accepted.end(), 0) << std::endl;
        std::cout << time_measure << std::endl;
    }
    std::cout << std::endl;

    // Example 2
    {
        const auto data_ex{util::convert_input(ex::data)};
        const auto data_ex2{util::convert_input(data_ex)};
        TimeMsr time_measure;

        std::vector<uint32_t> accepted{};
        for (const auto & line : data_ex2) {
            const auto possible{util::all_placements(line)};
            std::unordered_map<size_t, std::unordered_map<uint8_t, uint64_t>> cache{};

            const auto accepted_cnt{util::count(line.first, 0, cache,
                                                possible.cbegin(), possible.cend(),
                                                line.second.cbegin(), line.second.cend())};
            accepted.push_back(accepted_cnt);
            time_measure.sample(3);

            std::cout << line.first << ": " << accepted_cnt << std::endl;
        }
        std::cout << "T:" << std::accumulate(accepted.begin(), accepted.end(), 0) << std::endl;
        std::cout << time_measure << std::endl;
    }
    std::cout << std::endl;

    // P2
    {
        const auto report_p2{util::convert_input(report_p1)};

        TimeMsr time_measure;

        std::vector<uint64_t> accepted{};
        for (size_t idx = 0; const auto & line : report_p2) {
            const auto possible{util::all_placements(line)};
            std::unordered_map<size_t, std::unordered_map<uint8_t, uint64_t>> cache{};

            const uint64_t accepted_cnt{util::count(line.first, 0, cache,
                                                possible.cbegin(), possible.cend(),
                                                line.second.cbegin(), line.second.cend())};
            accepted.push_back(accepted_cnt);
            time_measure.sample(2);

            std::cout << idx++ << ":" << line.first << ": " << accepted_cnt << std::endl;
        }

        std::cout << "T:" << std::accumulate(accepted.begin(), accepted.end(), 0ULL) << std::endl;
        std::cout << time_measure << std::endl;
    }
    std::cout << std::endl;

    return 0;
}