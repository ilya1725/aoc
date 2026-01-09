
#include <algorithm>
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
    const std::vector<std::string> input1 = {"987654321111111",
                                            "811111111111119",
                                            "234234234234278",
                                            "818181911112111"};

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

    std::vector<std::vector<uint8_t>> process_input(const std::vector<std::string> & input) {
        std::vector<std::vector<uint8_t>> output{};

        for (const auto & line: input) {
            std::vector<uint8_t> item{};

            for(const auto s : line) {
                item.push_back(s - '0');
            }
            output.push_back(item);
        }
        return output;
    }

    uint32_t find_joltage1(const std::vector<uint8_t> &input) {
        uint8_t result{0};
        size_t max_index{0};
        uint8_t max{input[max_index]};

        for (size_t i=(max_index+1);i<input.size();i++) {
            const auto curr{input[i]};
            if (curr > max) {
                max = curr;
                max_index = i;
            }
        }

        // Find maxes on both sides
        uint8_t max_left{input[0]};
        for (size_t i=1;i<max_index;i++) {
            const auto curr{input[i]};
            if (curr > max_left) {
                max_left = curr;
            }
        }
        uint32_t left{max_left*10 + max};
        if (max_index == (input.size()-1)) {
            return left;
        }

        uint8_t max_right{input[max_index+1]};
        for (size_t i=(max_index+1);i<input.size();i++) {
            const auto curr{input[i]};
            if (curr > max_right) {
                max_right = curr;
            }
        }

        uint32_t right{max*10 + max_right};
        if (max_index == 0) {
            return right;
        }
        return (left > right ? left : right);
    }

    std::vector<size_t> find_maxes(const std::vector<uint8_t> &input, const size_t digits, size_t start) {
        uint32_t result{0};

        if (start == input.size()) {
            return {};
        }
        size_t max_index{start};
        uint8_t max{input[max_index]};

        for (size_t i=(max_index+1);i<input.size();i++) {
            const auto curr{input[i]};
            if (curr > max) {
                max = curr;
                max_index = i;
            }
        }

        // find all possible maxes
        std::vector<size_t> maxes;
        for (size_t i=start; i<input.size()-digits;i++) {
            if (input[i] == max) {
                maxes.push_back(i);
            }
        }

        return maxes;
    }

    size_t find_max(const std::vector<uint8_t> &input, const size_t start, const size_t end) {

        if (start > end) {
            throw("Invalid input");
            return 0;
        }
        size_t max_index{start};
        uint8_t max{input[max_index]};

        for (size_t i=(max_index+1);i<end;i++) {
            const auto curr{input[i]};
            if (curr > max) {
                max = curr;
                max_index = i;
            }
        }

        return max_index;
    }

    uint64_t find_joltage2(const std::vector<uint8_t> &input) {
        uint64_t result{0};

        // Find max value from beginning till the edge of the digits
        size_t start{0};
        size_t end{input.size() - 11};
        for (int i=0;i<12;i++) {
            const auto max_index{find_max(input, start, end)};
            const auto max_value{input[max_index]};
            result = result * 10 + max_value;

            // std::cout << std::format("{}: {}-{}-{} {} {}\n", i, start, max_index, end, max_value, result);
            start = max_index + 1;
            end = end + 1;
        }

        return result;
    }

}

int main() {
    const auto input1_p1{part_one::get_input("./day3.input")};

    {
        const auto result{part_one::process_input(part_one::input1)};
        uint64_t count{0};

        for (const auto & range : result) {
            const auto max = part_one::find_joltage1(range);
            std::cout << " " << max << std::endl;
            count += max;
        }

        std::cout << "Total 1.1: " << count << std::endl;

        const auto result2{part_one::process_input(input1_p1)};
        uint64_t count2{0};

        for (const auto & range : result2) {
            const auto max = part_one::find_joltage1(range);
            std::cout << " " << max << std::endl;
            count2 += max;
        }

        // 17332
        std::cout << "Total 1.2: " << count2 << std::endl;
    }
    {
        const auto result{part_one::process_input(part_one::input1)};
        uint64_t count{0};

        for (const auto & range : result) {
            const auto max = part_one::find_joltage2(range);
            std::cout << " " << max << std::endl;
            count += max;
        }

        std::cout << "Total 2.1: " << count << std::endl;

        const auto result2{part_one::process_input(input1_p1)};
        uint64_t count2{0};

        for (const auto & range : result2) {
            const auto max = part_one::find_joltage2(range);
            std::cout << " " << max << std::endl;
            count2 += max;
        }

        // 172516781546707
        std::cout << "Total 2.2: " << count2 << std::endl;
    }
    return 0;
}