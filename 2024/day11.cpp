
#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <queue>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <string_view>
#include <tuple>
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

std::vector<std::vector<int8_t>> process_input(const std::vector<std::string> & input) {
    std::vector<std::vector<int8_t>> result{};

    for (const auto & line : input) {
        std::vector<int8_t> number_line{};
        for (auto c : line) {
            int8_t value{empty_block};
            if (std::isdigit(c)) {
                value = c - '0';
            }
            number_line.push_back(value);
        }
        result.push_back(number_line);
    }

    return result;
}

namespace part_one {
    const std::vector<std::string> input_ex1{"0123",
                                            "1234",
                                            "8765",
                                            "9876"
                                            };

    const std::vector<std::string> input_ex2{"...0...",
                                            "...1...",
                                            "...2...",
                                            "6543456",
                                            "7.....7",
                                            "8.....8",
                                            "9.....9"};

    const std::vector<std::string> input_ex3{"..90..9",
                                            "...1.98",
                                            "...2..7",
                                            "6543456",
                                            "765.987",
                                            "876....",
                                            "987...."};

    const std::vector<std::string> input_ex4{"10..9..",
                                            "2...8..",
                                            "3...7..",
                                            "4567654",
                                            "...8..3",
                                            "...9..2",
                                            ".....01"};

    const std::vector<std::string> input_ex5{"89010123",
                                            "78121874",
                                            "87430965",
                                            "96549874",
                                            "45678903",
                                            "32019012",
                                            "01329801",
                                            "10456732"};
}

namespace part_two {
    const std::vector<std::string> input_ex1{".....0.",
                                            "..4321.",
                                            "..5..2.",
                                            "..6543.",
                                            "..7..4.",
                                            "..8765.",
                                            "..9...."
                                            };
    const std::vector<std::string> input_ex2{"..90..9",
                                            "...1.98",
                                            "...2..7",
                                            "6543456",
                                            "765.987",
                                            "876....",
                                            "987...."
                                            };
    const std::vector<std::string> input_ex3{"012345",
                                            "123456",
                                            "234567",
                                            "345678",
                                            "4.6789",
                                            "56789."
                                            };
    const std::vector<std::string> input_ex4{"89010123",
                                            "78121874",
                                            "87430965",
                                            "96549874",
                                            "45678903",
                                            "32019012",
                                            "01329801",
                                            "10456732"
                                            };
}

int main(int argc, char* argv[]) {
    // P1
    {
        const auto data{process_input(part_one::input_ex5)};
        // const auto starts_data{part_one::get_all_starts(data)};

        {
            const auto input1_p1{get_input("./day10.input")};
            const auto data_p1{process_input(input1_p1)};
            // const auto starts_data{part_one::get_all_starts(data_p1)};
        }
    }

    // P2
    {
        const auto data{process_input(part_two::input_ex4)};
        // const auto starts_data{part_one::get_all_starts(data)};

        {
            const auto input1_p1{get_input("./day10.input")};
            const auto data_p1{process_input(input1_p1)};
            // const auto starts_data{part_one::get_all_starts(data_p1)};

        }
    }

    return 0;
}