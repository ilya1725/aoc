
#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <string_view>
#include <tuple>
#include <vector>

void pretty_print(const std::vector<std::string> & input) {
    std::cout << std::endl;
    size_t count{0};
    for (const auto & line : input) {
        std::cout << std::format("{:5}:{}", count, line) << std::endl;
        count++;
    }
    return;
}

template<typename T>
typename std::enable_if<std::is_integral<T>::value, int>::type
count_digits(T number) {
    int count = 0;
    if(number == 0)
        return 1;

    while(number != 0) {
        number = number/10;
        count++;
    }
    return count;
}

std::vector<int>
partition_three(int num, const size_t digits) {
    std::vector<int> result(digits);
    for(int i = 0; i < digits; i++) {
        result[i] = num % 3;
        num /= 3;
    }
    return result;
}

class Data {
  public:
    const uint64_t test_value_{0};
    const std::vector<uint16_t> numbers_{};
    const std::vector<uint8_t> digits_{};

    Data(const uint64_t test_value, const std::vector<uint16_t> & numbers, const std::vector<uint8_t> & digits) :
        test_value_(test_value), numbers_(numbers), digits_(digits) {}
};

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

std::vector<Data> get_data(const std::vector<std::string> & input) {
    std::vector<Data> result{};

    for (const auto & line : input) {
        uint64_t test_value;
        std::vector<uint16_t> numbers;
        std::vector<uint8_t> digits;

        const size_t colon_pos{line.find(':')};
        if (colon_pos == std::string::npos) {
            throw std::invalid_argument("Invalid format (no colon found)");
        }

        test_value = std::stol(line.substr(0, colon_pos));

        std::stringstream ss(line.substr(colon_pos + 1));
        uint16_t num;
        while (ss >> num) {
            numbers.push_back(num);
            digits.push_back(count_digits<uint16_t>(num));
        }

        result.emplace_back(test_value, numbers, digits);
    }
    return result;
}


namespace part_one {

    const std::vector<std::string> input_ex1{"190: 10 19",
                                            "3267: 81 40 27",
                                            "83: 17 5",
                                            "156: 15 6",
                                            "7290: 6 8 6 15",
                                            "161011: 16 10 13",
                                            "192: 17 8 14",
                                            "21037: 9 7 18 13",
                                            "292: 11 6 16 20"
                                            };

    const std::vector<std::string> input_ex2{"190: 10 19",
                                            "3267: 81 40 27",
                                            "83: 17 5",
                                            "156: 15 6",
                                            "7290: 6 8 6 15",
                                            "161011: 16 10 13",
                                            "108: 4 5 3 9",
                                            "192: 17 8 14",
                                            "21037: 9 7 18 13",
                                            "292: 11 6 16 20",
                                            "108: 2 2 5 12"
                                                };

    bool check_valid(const Data & equation) {
        bool result{false};

        const uint32_t iterations{1 << (equation.numbers_.size() - 1)};
        const auto width{equation.numbers_.size() - 1};
        for (uint32_t i=0;i<iterations;i++) {
            uint64_t value{equation.numbers_[0]};
            for (uint32_t j=0; j<width; j++) {
                const auto op{ (i >> j) & 0x1 };
                // op: 1 is +, 0 is *
                if (op == 1) {
                    value = value + equation.numbers_[j + 1];
                } else {
                    value = value * equation.numbers_[j + 1];
                }
            }

            if (value == equation.test_value_) {
                result = true;
                break;
            }
        }
        return result;
    }

    std::vector<Data> find_correct(const std::vector<Data> & input) {
        std::vector<Data> result{};

        for (const auto & equation : input) {
            const bool valid{part_one::check_valid(equation)};

            if (valid) {
                result.push_back(equation);
            }
        }
        return result;
    }

    uint64_t summarize(const std::vector<Data> & input) {
        uint64_t result{};

        for (const auto & equation : input) {
            result += equation.test_value_;
        }
        return result;
    }
}

namespace part_two {
    // valid with 3 operations
    bool check_valid(const Data & equation) {
        bool result{false};

        // width in number of gaps
        const auto width{equation.numbers_.size() - 1};
        const uint32_t iterations{std::pow(3, width)};

        for (uint32_t i=0;i<iterations;i++) {
            uint64_t value{equation.numbers_[0]};

            const auto three_vector{partition_three(i, width)};

            for (uint32_t j=0; j<width; j++) {
                // op: 1 is +, 0 is *, 2 is ||
                const auto op{three_vector[j]};
                if (op == 1) {
                    value = value + equation.numbers_[j + 1];
                } else if (op == 0) {
                    value = value * equation.numbers_[j + 1];
                } else {
                    value = (value * std::pow(10, equation.digits_[j + 1])) + equation.numbers_[j + 1];
                }
            }

            if (value == equation.test_value_) {
                result = true;
                break;
            }
        }
        return result;
    }

    std::vector<Data> find_correct(const std::vector<Data> & input) {
        std::vector<Data> result{};

        for (const auto & equation : input) {
            const bool valid{part_two::check_valid(equation)};

            if (valid) {
                result.push_back(equation);
            }
        }
        return result;
    }
}

int main() {
    // P1
    {
        const auto data{get_data(part_one::input_ex2)};
        const auto correct{part_one::find_correct(data)};

        const auto result_ex1{part_one::summarize(correct)};
        std::cout << "Result ex1.1: " << result_ex1 << std::endl;

        const auto input1_p1{get_input("./day7.input")};
        const auto data_p1{get_data(input1_p1)};
        const auto correct_p1{part_one::find_correct(data_p1)};

        const auto result_p1{part_one::summarize(correct_p1)};
        std::cout << "Result1.1: " << result_p1 << " Correct result: 1153997401072" << std::endl;
    }

    // P2
    {
        const auto data{get_data(part_one::input_ex1)};
        const auto correct{part_two::find_correct(data)};

        const auto result_ex1{part_one::summarize(correct)};
        std::cout << "Result ex1.2: " << result_ex1 << std::endl;

        const auto input1_p2{get_input("./day7.input")};
        const auto data_p2{get_data(input1_p2)};
        const auto correct_p1{part_two::find_correct(data_p2)};

        // 97902809384118
        const auto result_p1{part_one::summarize(correct_p1)};
        std::cout << "Result1.2: " << result_p1 << " Correct result: 97902809384118" << std::endl;
    }

    return 0;
}