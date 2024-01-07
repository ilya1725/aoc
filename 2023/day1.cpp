
#include <iostream>
#include <algorithm>
#include <string_view>
#include <vector>
#include <fstream>

namespace part_one {
    const std::vector<std::string> input1 = {"1abc2",
                                            "pqr3stu8vwx",
                                            "a1b2c3d4e5f",
                                            "treb7uchet"};

    const std::vector<std::string> input_p2 = {"two1nine",
                                            "eightwothree",
                                            "abcone2threexyz",
                                            "xtwone3four",
                                            "4nineeightseven2",
                                            "zoneight234",
                                            "7pqrstsixteen"};

    const std::vector<std::string> digit_str = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

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

    std::vector<long> get_values(const std::vector<std::string> & input, bool part2 = false) {
        std::vector<long> result = {};
        for (const auto line : input) {
            long first_digit{-1};
            long last_digit{-1};
            for (size_t i=0; i<line.length(); i++) {
                if (std::isdigit(line[i])) {
                    if (first_digit == -1) {
                        first_digit = line[i] - '0';
                    }
                    last_digit = line[i] - '0';
                } else {
                    if (part2) {
                        for (size_t j=0; j<digit_str.size();j++) {
                            if (line.compare(i, digit_str[j].length(), digit_str[j]) == 0) {
                                if (first_digit == -1) {
                                    first_digit = j+1;
                                }
                                last_digit = j+1;
                                j += digit_str[j].length();
                            }
                        }
                    }
                }
            }
            const long number = first_digit * 10 + last_digit;
            result.push_back(number);
        }

        return result;
    }
}

int main() {
    const auto result{part_one::get_values(part_one::input1)};
    long final1{0};
    for (auto v : result) {
        std::cout << v << std::endl;
        final1 += v;
    }
    std::cout << "Result1: " << final1 << std::endl;

    const auto result_p2{part_one::get_values(part_one::input_p2)};
    long final_p2{0};
    for (auto v : result_p2) {
        std::cout << v << std::endl;
        final_p2 += v;
    }
    std::cout << "Result p2: " << final_p2 << std::endl;

    const auto input2_p1{part_one::get_input("./day1.input")};
    const auto result2_p1{part_one::get_values(input2_p1)};
    long final2_p1{0};
    for (auto v : result2_p1) {
        // std::cout << v << std::endl;
        final2_p1 += v;
    }
    std::cout << "Result2 p1: " << final2_p1 << std::endl;

    const auto result2_p2{part_one::get_values(input2_p1, true)};
    long final2_p2{0};
    for (auto v : result2_p2) {
        // std::cout << v << std::endl;
        final2_p2 += v;
    }
    std::cout << "Result2 p2: " << final2_p2 << std::endl;

    return 0;
}