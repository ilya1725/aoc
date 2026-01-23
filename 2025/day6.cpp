
#include <algorithm>
#include <charconv>
#include <cmath>
#include <format>
#include <fstream>
#include <iostream>
#include <print>
#include <sstream>
#include <string_view>
#include <tuple>
#include <vector>

#include <stdint.h>

namespace part_one {
    const std::vector<std::string> input1 = {"123 328  51 64 ",
                                             " 45 64  387 23 ",
                                             "  6 98  215 314",
                                             "*   +   *   +  "};

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

    std::vector<uint32_t> parse_numbers(const std::string& str) {
        std::vector<uint32_t> result;
        result.reserve(str.size() / 4);

        const char* ptr = str.data();
        const char* end = ptr + str.size();

        while (ptr < end) {
            // Skip whitespace
            while (ptr < end && std::isspace(*ptr)) {
                ++ptr;
            }

            if (ptr >= end) break;

            uint32_t num;
            auto [next_ptr, ec] = std::from_chars(ptr, end, num);

            if (ec == std::errc()) {
                result.push_back(num);
                ptr = next_ptr;
            } else {
                ++ptr; // Skip invalid character
            }
        }

        return result;
    }

    std::vector<bool> parse_operators(const std::string& str) {
        std::vector<bool> result;
        result.reserve(str.size() / 2); // Rough estimate

        for (char c : str) {
            if (c == '*') {
                result.push_back(true);
            } else if (c == '+') {
                result.push_back(false);
            }
        }

        return result;
    }

    std::pair<std::vector<std::vector<uint32_t>>, std::vector<bool>> process_input(const std::vector<std::string> & input) {
        std::vector<std::vector<uint32_t>> numbers{};
        std::vector<bool> operations{};

        bool ranges_state{true};
        for (const auto & line : input) {
            const auto numbers_line = parse_numbers(line);
            if (numbers_line.size() == 0) {
                // convert operations
                operations = parse_operators(line);
            } else {
                numbers.push_back(numbers_line);
            }
        }

        return {numbers, operations};
    }
}

namespace part_two {

    std::vector<std::pair<std::vector<uint32_t>, bool>> process_input(const std::vector<std::string> & input) {
        std::vector<std::pair<std::vector<uint32_t>, bool>> result{};

        // number of rows of numbers - total-1
        const size_t rows{input.size()-1};
        const size_t op_index{input.size()-1};

        // Iterate over the input right to left
        std::vector<uint32_t> numbers{};
        for (ssize_t i=(input[0].size()-1); i>=0; i--) {
            uint32_t value{0};
            bool operation{false};  // false is +

            for(size_t r=0; r<rows; r++) {
                if (input[r][i] != ' ') {
                    value = value*10 + (input[r][i] - '0');
                }
            }
            if (value != 0) {
                numbers.push_back(value);
            }

            if (input[rows][i] != ' ') {
                if (input[rows][i] == '*') {
                    operation = true;
                }

                // all space in the column, we are done with this batch
                result.push_back({numbers, operation});
                numbers.clear();
            }
        }

        return result;
    }
}

int main() {
    const auto input1_p1{part_one::get_input("./day6.input")};

    {
        uint64_t result{0};
        const auto [numbers, operations] = part_one::process_input(part_one::input1);

        for (size_t i=0; i<operations.size(); i++) {
            uint32_t op_result{0};
            if (operations[i] == true) {
                op_result = 1;
            }
            for (const auto & num : numbers) {
                const auto val{num[i]};
                if (operations[i] == true) {
                    op_result *= val;
                } else {
                    op_result += val;
                }
            }
            result += op_result;
        }
        std::cout << "Total 1.1: " << result << std::endl;

        uint64_t result2{0};
        const auto [numbers2, operations2] = part_one::process_input(input1_p1);

        std::cout << numbers2[0].size() << ":" << numbers2[1].size() << ":" << numbers2[2].size() << ":" << numbers2[3].size() << std::endl;

        for (size_t i=0; i<operations2.size(); i++) {
            uint64_t op_result{0};
            if (operations2[i] == true) {
                op_result = 1;
            }
            for (const auto & num : numbers2) {
                const auto val{num[i]};
                if (operations2[i] == true) {
                    op_result *= val;
                } else {
                    op_result += val;
                }
            }
            result2 += op_result;
        }
        // 5782351442566
        std::cout << "Total 1.2: " << result2 << std::endl;
    }

    {
        uint64_t result{0};
        const auto bunches = part_two::process_input(part_one::input1);

        for (const auto & bunch : bunches) {
            uint32_t op_result{0};
            if (bunch.second == true) {
                op_result = 1;
            }

            for (const uint32_t number : bunch.first) {
                if (bunch.second == true) {
                    op_result *= number;
                } else {
                    op_result += number;
                }
            }
            result += op_result;
        }

        std::cout << "Total 2.1: " << result << std::endl;

        uint64_t result2{0};
        const auto bunches2 = part_two::process_input(input1_p1);

        for (const auto & bunch : bunches2) {
            uint64_t op_result{0};
            if (bunch.second == true) {
                op_result = 1;
            }

            for (const uint32_t number : bunch.first) {
                if (bunch.second == true) {
                    op_result *= number;
                } else {
                    op_result += number;
                }
                std::cout << number << ":";
            }
            result2 += op_result;
                std::cout << (bunch.second ? '*' : '+') << " " << op_result << "\n";

        }
        std::cout << bunches2.size() << "\n";

        // 10194584711842
        std::cout << "Total 2.2: " << result2 << std::endl;
    }
    return 0;
}