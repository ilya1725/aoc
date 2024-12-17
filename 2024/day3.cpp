
#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stack>
#include <string_view>
#include <tuple>
#include <vector>

namespace part_one {
    const std::vector<std::string> input = {"xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))"};

    std::string get_input(const std::string & path) {
        std::ifstream file_in(path);
        if (!file_in) {
            std::exit(-1);
        }

        std::string line;
        std::string output;
        while (std::getline(file_in, line))
        {
            output += line;
        }
        return output;
    }

    std::vector<std::string> get_instructions(const std::string & input) {
        std::vector<std::string> result{};

        std::regex pattern(R"(mul\(\d{1,3},\d{1,3}\))");

        auto words_begin = std::sregex_iterator(input.begin(), input.end(), pattern);
        auto words_end = std::sregex_iterator();

        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            std::smatch match{*i};
            result.push_back(match.str());
        }

        return result;
    }

    std::vector<std::pair<short, short>> get_instructions(const std::vector<std::string> & input) {
        std::vector<std::pair<short, short>> result{};
        std::regex pattern(R"(\((\d+),\s*(\d+)\))");

        for (const auto & instruction : input) {
            std::smatch matches;

            if (std::regex_search(instruction, matches, pattern)) {
                result.push_back( {std::stoi(matches[1].str()), std::stoi(matches[2].str())});
            }
        }
        return result;
    }

    long process_data(const std::vector<std::pair<short, short>> & input) {
        long accumulator{0};
        for (const auto & instruction: input) {
            accumulator += (instruction.first * instruction.second);
        }

        return accumulator;
    }
}

namespace part_two {
    std::vector<std::string> input = {"xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))"};

    std::vector<std::string> get_instructions(const std::string & input) {
        std::vector<std::string> result{};

        std::regex pattern(R"(mul\(\d{1,3},\d{1,3}\)|do\(\)|don't\(\))");

        auto words_begin = std::sregex_iterator(input.begin(), input.end(), pattern);
        auto words_end = std::sregex_iterator();

        bool skip_flag{false};
        long count{0};
        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            const std::smatch match{*i};
            const auto match_str{match.str()};
            count++;

            if (match_str.compare("don't()") == 0) {
                std::cout << count << ". Start skipping" << std::endl;
                skip_flag = true;
            } else if (match_str.compare("do()") == 0) {
                std::cout << count << ". Stop skipping" << std::endl;
                skip_flag = false;
            } else {
                if (skip_flag == false) {
                    result.push_back(match_str);
                }
            }
        }

        return result;
    }
}

int main() {
    // P1
    {
        const auto result_str{part_one::get_instructions(part_one::input[0])};
        const auto result_int{part_one::get_instructions(result_str)};

        auto accumulator{part_one::process_data(result_int)};
        std::cout << "Result1.1: " << accumulator << std::endl;

        const auto input1_p1{part_one::get_input("./day3.input")};

        const auto result1_str{part_one::get_instructions(input1_p1)};
        const auto result1_int{part_one::get_instructions(result1_str)};
        accumulator = part_one::process_data(result1_int);
        std::cout << "Result2.1: " << accumulator << " Correct value is 188192787" << std::endl;
    }

    // P2
    {
        const auto result_str{part_two::get_instructions(part_two::input[0])};
        const auto result_int{part_one::get_instructions(result_str)};

        auto accumulator{part_one::process_data(result_int)};
        std::cout << "Result1.2: " << accumulator << std::endl;

        const auto input1_p1{part_one::get_input("./day3.input")};

        const auto result1_str{part_two::get_instructions(input1_p1)};
        const auto result1_int{part_one::get_instructions(result1_str)};
        accumulator = part_one::process_data(result1_int);
        std::cout << "Result2.2: " << accumulator << " Correct value is 113965544" << std::endl;
    }

    return 0;
}