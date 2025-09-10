
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>
#include <tuple>
#include <vector>

namespace part_one {
    const std::vector<std::string> input1 = {"3   4",
                                            "4   3",
                                            "2   5",
                                            "1   3",
                                            "3   9",
                                            "3   3"};

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

    std::tuple<std::vector<long>, std::vector<long>> get_values(const std::vector<std::string> & input) {
        std::vector<long> result_left{};
        std::vector<long> result_right{};
        for (const std::string & line : input) {
            std::stringstream ss(line);
            long number;

            // Get left
            ss >> number;
            result_left.push_back(number);

            // Get right
            ss >> number;
            result_right.push_back(number);
        }

        return {result_left, result_right};
    }
}

int main() {
    const auto result{part_one::get_values(part_one::input1)};
    auto left{std::get<0>(result)};
    auto right{std::get<1>(result)};

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    long accumulator{0};

    for (auto it1 = left.begin(), it2 = right.begin();
            it1 != left.end() && it2 != right.end();
            ++it1, ++it2)
    {
        accumulator += std::abs(*it1 - *it2);
    }

    std::cout << "Result1: " << accumulator << std::endl;

    const auto input1_p1{part_one::get_input("./day1.input")};

    const auto result1{part_one::get_values(input1_p1)};
    auto left1{std::get<0>(result1)};
    auto right1{std::get<1>(result1)};

    std::sort(left1.begin(), left1.end());
    std::sort(right1.begin(), right1.end());

    accumulator = 0;

    for (auto it1 = left1.begin(), it2 = right1.begin();
            it1 != left1.end() && it2 != right1.end();
            ++it1, ++it2)
    {
        accumulator += std::abs(*it1 - *it2);
    }

    std::cout << "Result2: " << accumulator << std::endl;

    // Second part
    accumulator = 0;

    for (auto it1 = left.begin();
            it1 != left.end();
            ++it1)
    {
        accumulator += *it1 * std::count (right.begin(), right.end(), *it1);
    }

    std::cout << "Result1.2: " << accumulator << std::endl;

    accumulator = 0;

    for (auto it1 = left1.begin();
            it1 != left1.end();
            ++it1)
    {
        accumulator += *it1 * std::count (right1.begin(), right1.end(), *it1);
    }

    std::cout << "Result2.2: " << accumulator << std::endl;

    return 0;
}