
#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <queue>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

const int16_t empty_block{-1};
const int16_t end_block{-2};

void pretty_print(const std::vector<std::string> & input) {
    std::cout << std::endl;
    size_t count{0};
    for (const auto & line : input) {
        std::cout << std::format("{:5}:{}", count, line) << std::endl;
        count++;
    }
    return;
}

void pretty_print(const std::vector<int16_t> & input) {
    for (const auto & d : input) {
        if (d == empty_block) {
            std::cout << ".";
        } else if (d == end_block) {
            break;
        } else {
            std::cout << std::format("{}", d);
        }
    }
    std::cout << std::endl;
    return;
}

void pretty_print(const std::list<std::pair<uint64_t, uint8_t>> & input) {
    for (const auto & p : input) {
        std::cout << std::format("{}:{};", p.first, p.second);
    }
    std::cout << std::endl;
    return;
}

std::vector<std::string> get_input(const std::string & path) {
    std::ifstream file_in(path);
    if (!file_in) {
        std::exit(-1);
    }

    std::string line;
    std::vector<std::string> output;
    while (std::getline(file_in, line))
    {
        output.push_back(line);
    }
    return output;
}

std::vector<uint64_t> process_input(const std::string & input) {
    std::vector<uint64_t> result{};
    std::stringstream ss(input);
    std::string token;

    while (ss >> token) {
        try {
            size_t pos;
            const uint64_t num{static_cast<uint64_t>(std::stoll(token, &pos))};
            if (pos != token.length()) {
                throw std::invalid_argument("Invalid number format");
            }
            result.push_back(num);
        } catch (const std::exception& e) {
            throw std::invalid_argument("Failed to parse number: " + token);
        }
    }

    return result;
}

uint8_t count_digits(uint64_t value) {
    if (value == 0)
        return 1;

    uint8_t count{0};

    while (value > 0) {
        value /= 10;
        count++;
    }
    return count;
}

// Custom hash function for pair
struct PairHash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& pair) const {
        auto h1 = std::hash<T1>{}(pair.first);
        auto h2 = std::hash<T2>{}(pair.second);

        // Combining hashes
        return h1 ^ (h2 << 1);
    }
};

namespace part_one {
    const std::string input_ex1{"0 1 10 99 999"};

    const std::string input_ex2{"125 17"};

    std::vector<uint64_t> process_stones(const std::vector<uint64_t> & input, const uint8_t steps) {
        std::vector<uint64_t> data1{input};
        std::vector<uint64_t> data2{input};

        std::vector<uint64_t> * source = &data1;
        std::vector<uint64_t> * destination = &data2;

        for (uint8_t i=0; i<steps; i++) {
            destination->clear();

            for (size_t j=0; j<source->size(); j++) {
                const auto value{source->at(j)};
                if (value == 0) {
                    destination->push_back(1);
                } else if ((count_digits(value) & 0x1) == 0x0) {
                    const auto digits{count_digits(value)/2};
                    const uint64_t right{value % (static_cast<long>(std::pow(10, digits)))};
                    const uint64_t left{value / (static_cast<long>(std::pow(10, digits)))};
                    destination->push_back(left);
                    destination->push_back(right);
                } else {
                    destination->push_back(2024 * value);
                }
            }
            auto tmp = source;
            source = destination;
            destination = tmp;
        }

        return {*source};
    }
}

namespace part_two {
    uint64_t process_stones(const std::vector<uint64_t> & input, const uint8_t steps) {
        std::unique_ptr<std::list<uint64_t>> result = std::make_unique<std::list<uint64_t>>();

        for (const auto i : input) {
            result->push_back(i);
        }

        for (uint8_t i=0; i<steps; i++) {
            for (auto it = result->begin(); it != result->end(); ++it) {
                const auto value{*it};
                if (value == 0) {
                    *it = 1;
                } else if ((count_digits(value) & 0x1) == 0x0) {
                    const auto digits{count_digits(value)/2};
                    const uint64_t right{value % (static_cast<long>(std::pow(10, digits)))};
                    const uint64_t left{value / (static_cast<long>(std::pow(10, digits)))};

                    *it = left;  // update the current element

                    ++it;  // move past the inserted element
                    it = result->insert(it, right);
                } else {
                    *it = (2024 * value);
                }
            }
            std::cout << static_cast<int>(i) << ":" << result->size() << std::endl;
        }

        return result->size();
    }
    uint64_t process_stones2(const std::vector<uint64_t> & input, const uint8_t steps) {
        // first - value; second - step, 0-based
        std::list<std::pair<uint64_t, uint8_t>> result{};
        uint64_t count{0};

        for (const auto i : input) {
            result.push_back({i, 0});
        }

        //
        while (result.size() > 0) {
            auto right = result.back();
            result.pop_back();

            for (uint8_t i=right.second; i<steps; i++) {
                if (right.first == 0) {
                    right.first = 1;
                } else if ((count_digits(right.first) & 0x1) == 0x0) {
                    const auto digits{count_digits(right.first)/2};
                    const uint64_t split_right{right.first % (static_cast<long>(std::pow(10, digits)))};
                    const uint64_t split_left{right.first / (static_cast<long>(std::pow(10, digits)))};

                    // continue right, stash the left
                    right.first = split_right;
                    result.push_back({split_left, i+1});
                } else {
                    right.first = (2024 * right.first);
                }
            }
            count++;
            if ((count % 10000000) == 0) {
                std::cout << count << ":" << result.size() << std::endl;
                pretty_print(result);
            }
        }
        return count;
    }

    // Define the unordered_map
    std::unordered_map<
        std::pair<uint64_t, uint8_t>,  // key type
        uint64_t,                      // value type
        PairHash                       // custom hash function
    > pair_map;

    uint64_t process_stone_tree(const uint64_t value, const uint8_t steps) {
        uint64_t count{0};

        // exit recursion
        if (steps == 0) {
            return 1;
        }

        // access cache
        if (pair_map.find({value, steps}) != pair_map.end()) {
            count = pair_map[{value, steps}];
            return count;
        }

        if (value == 0) {
            count += process_stone_tree(1, steps - 1);
        } else if ((count_digits(value) & 0x1) == 0x0) {
            const auto digits{count_digits(value)/2};
            const uint64_t right{value % (static_cast<long>(std::pow(10, digits)))};
            const uint64_t left{value / (static_cast<long>(std::pow(10, digits)))};

            count += process_stone_tree(right, steps - 1);
            count += process_stone_tree(left, steps - 1);
        } else {
            count += process_stone_tree(2024 * value, steps - 1);
        }

        // Update cache
        if (pair_map.find({value, steps}) != pair_map.end()) {
            std::cout << "Found item: {" << value << ":" << static_cast<uint16_t>(steps) << "}" << std::endl;
        } else {
            pair_map[{value, steps}] = count;
        }

        return count;
    }
    uint64_t process_stones3(const std::vector<uint64_t> & input, const uint8_t steps) {
        uint64_t count{0};

        for (const auto i : input) {
            count += process_stone_tree(i, steps);
        }

        return count;
    }
}

int main(int argc, char* argv[]) {
    // P1
    {
        const auto data{process_input(part_one::input_ex2)};
        const auto result{part_one::process_stones(data, 25)};
        std::cout << "P1: " << result.size() << std::endl;

        {
            const auto input1_p1{get_input("./day11.input")};
            const auto data_p1{process_input(input1_p1[0])};
            const auto result_p1{part_one::process_stones(data_p1, 25)};
            std::cout << "P1: " << result_p1.size() << ". Correct answer: 220999" << std::endl;
        }
    }

    // P2
    {
        const auto data{process_input(part_one::input_ex2)};
        const auto result{part_two::process_stones3(data, 25)};
        std::cout << "P2: " << result << std::endl;

        {
            const auto input1_p1{get_input("./day11.input")};
            const auto data_p1{process_input(input1_p1[0])};
            const auto result_p2{part_two::process_stones3(data_p1, 25)};
            std::cout << "P2: " << result_p2 << ". Correct answer: 220999" << std::endl;

            // Potential: 65,601,038,650,482
            const auto result_p2_2{part_two::process_stones3(data_p1, 75)};
            std::cout << "P2: " << result_p2_2 << ". Correct answer: 261936432123724" << std::endl;
        }
    }

    return 0;
}

// 261,936,432,123,724
// 65,601,038,650,482