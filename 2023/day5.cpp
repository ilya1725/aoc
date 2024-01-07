
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>
// #include <format>

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
}

namespace part_one {

    const std::string  maps{
R"example(seeds: 79 14 55 13

seed-to-soil map:
50 98 2
52 50 48

soil-to-fertilizer map:
0 15 37
37 52 2
39 0 15

fertilizer-to-water map:
49 53 8
0 11 42
42 0 7
57 7 4

water-to-light map:
88 18 7
18 25 70

light-to-temperature map:
45 77 23
81 45 19
68 64 13

temperature-to-humidity map:
0 69 1
1 0 69

humidity-to-location map:
60 56 37
56 93 4)example"
                    };

    class Range {
      public:
        Range(const std::string & setting) {
            const auto split_range{util::split(setting, ' ', true, std::vector<std::string>())};

            if (split_range.size() != 3) {
                throw("Invalid input format: {}");
            }

            destination = std::atoll(split_range[0].c_str());
            source = std::atoll(split_range[1].c_str());
            length = std::atoll(split_range[2].c_str());

            if (destination < 0 || source < 0 || length < 0) {
                throw("Input value is negative: {}.{}.{}");
            }
        }

        uint64_t GetDestination() const { return destination; }
        uint64_t GetSource() const { return source; }
        uint64_t GetLength() const { return length; }

        // Check if the input is in current range
        bool IsInRange(const uint64_t value) const {
            // All values are positive
            if (GetSource() <= value && value < (GetSource() + GetLength())) {
                return true;
            }

            return false;
        }

        // Scale the input value.
        // The function assumes that the input is in range
        uint64_t Scale(const uint64_t value) const {
            // All values are positive
            return (GetDestination() + (value - GetSource()));
        }

      private:
        uint64_t destination{0};
        uint64_t source{0};
        uint64_t length{0};
    };

    class Map {
      public:
        Map(const std::vector<std::string> & input) {
            // Vector of data. First line is the name
            name = input[0];
            size_t i{0};
            for (const auto & line: input) {
                if (i++ == 0) {
                    continue;
                }
                range_vector.emplace_back(line);
            }
        }

        uint64_t Traverse(const uint64_t input) const {
            // Algorithm: the ranges indicated chunks of the input. Need to check teh value in all ranges, in fot it'll just pass through
            uint64_t output{input};
            // size_t i=0;
            for (const auto & range : range_vector) {
                if (range.IsInRange(input)) {
                    output = range.Scale(output);
                    // std::cout << "" << i << " ";
                    break;
                }
                // i++;
            }
            return output;
        }

      private:
        std::vector<Range> range_vector{};
        std::string name{};
    };

    std::vector<Map> CreateMaps(const std::vector<std::string> & input) {
        auto map_start = input.end();

        std::vector<Map> output{};

        for (auto it = input.begin(); it != input.end(); ++it) {

            if (it->length() == 0) {
                if (map_start == input.end()) {
                    map_start = it;
                } else {
                    const std::vector<std::string> current_map{map_start+1, it};
                    output.emplace_back(current_map);
                    map_start = it;
                }
            } else if (it+1 == input.end()) {
                const std::vector<std::string> current_map{map_start+1, it+1};
                output.emplace_back(current_map);
            }
        }
        return output;
    }
}


int main() {

    const auto almanac_ex{util::split(part_one::maps, '\n', false, std::vector<std::string>())};
    const auto maps_ex{part_one::CreateMaps(almanac_ex)};

    const auto seed_str_ex{util::split(almanac_ex[0], ' ', false, std::vector<std::string>())};
    std::vector<uint64_t> seeds_ex{};
    for (size_t i=1; i<seed_str_ex.size(); i++) {
        seeds_ex.push_back(std::atol(seed_str_ex[i].c_str()));
    }

    // Go through maps
    std::vector<uint64_t> results_ex{};
    for (const auto seed : seeds_ex) {
        uint64_t output{seed};
        for (auto & map : maps_ex) {
            output = map.Traverse(output);
        }
        results_ex.push_back(output);

        std::cout << seed << "|-> " << output << std::endl;
    }
    const auto min_ex{std::min_element(results_ex.begin(), results_ex.end())};
    std::cout << "Min: " << *min_ex << std::endl;

    // Part one
    const auto almanac{util::get_input("./day5.input")};
    const auto maps{part_one::CreateMaps(almanac)};

    const auto seed_str{util::split(almanac[0], ' ', false, std::vector<std::string>())};
    std::vector<uint64_t> seeds{};
    for (size_t i=1; i<seed_str.size(); i++) {
        seeds.push_back(std::atoll(seed_str[i].c_str()));
    }

    // Go through maps
    std::vector<uint64_t> results_p1{};
    for (const auto seed : seeds) {
        uint64_t output{seed};
        std::cout << seed << "|-> ";
        for (auto & map : maps) {
            output = map.Traverse(output);
            std::cout << output << " -> ";
        }
        results_p1.push_back(output);

        std::cout << std::endl;
    }
    const auto min_p1{std::min_element(results_p1.begin(), results_p1.end())};
    std::cout << "Min: " << *min_p1 << std::endl;

    // Part two - ranges of seeds
    // example
    results_ex.clear();
    for (size_t s=0; s<seeds_ex.size(); s+=2) {
        uint64_t min_result{0};

        for (uint64_t seed=seeds_ex[s]; seed<(seeds_ex[s] + seeds_ex[s+1]); seed++) {
            uint64_t output{seed};
            for (auto & map : maps_ex) {
                output = map.Traverse(output);
            }
            if (min_result == 0 || min_result > output) {
                min_result = output;
            }

            std::cout << seed << "|-> " << output << std::endl;
        }
        std::cout << "[" << seeds_ex[s] << "," << seeds_ex[s+1] << "]Min: " << min_result << std::endl;
        results_ex.push_back(min_result);
    }
    const auto min_ex_p2{std::min_element(results_ex.begin(), results_ex.end())};
    std::cout << "Min: " << *min_ex_p2 << std::endl;

    // real data
    results_p1.clear();
    for (size_t s=0; s<seeds.size(); s+=2) {
        uint64_t min_result{0};

        for (uint64_t seed=seeds[s]; seed<(seeds[s] + seeds[s+1]); seed++) {
            uint64_t output{seed};
            for (auto & map : maps) {
                output = map.Traverse(output);
            }
            if (min_result == 0 || min_result > output) {
                min_result = output;
            }

            // std::cout << seed << "|-> " << output << std::endl;
        }
        std::cout << "[" << seeds[s] << "," << seeds[s+1] << "]Min: " << min_result << std::endl;
        results_p1.push_back(min_result);
    }
    const auto min_p2{std::min_element(results_p1.begin(), results_p1.end())};
    std::cout << "Min: " << *min_p2 << std::endl;

    return 0;
}