
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>
#include <tuple>
#include <vector>

#include <stdint.h>

namespace part_one {
    const std::vector<std::string> input1 = {"L68",
                                            "L30",
                                            "R48",
                                            "L5",
                                            "R60",
                                            "L55",
                                            "L1",
                                            "L99",
                                            "R14",
                                            "L82"
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

    uint32_t get_values1(const std::vector<std::string> & input) {
        uint32_t count{0};
        int16_t pos{50};
        const uint16_t max_value{99};
        const uint16_t total_value{100};

        for (const std::string & line : input) {
            const char direction{line[0]};
            const int16_t value{std::stoi(line.substr(1))};

            if (direction == 'R')
            {
                pos += value;
                pos = (pos % total_value);
            }
            else if (direction == 'L')
            {
                pos -= value;
                if (pos < 0)
                {
                    pos = (max_value + pos % total_value + 1) % total_value;
                }
            }
            else
            {
                std::abort();
            }

            if (pos == 0)
            {
                std::cout << line << std::endl;
                count++;
            }
        }

        return count;
    }

    uint32_t get_values2(const std::vector<std::string> & input) {
        uint32_t count{0};
        int16_t pos{50};
        const uint16_t max_value{99};
        const uint16_t total_value{100};

        for (const std::string & line : input) {
            const char direction{line[0]};
            const int16_t value{std::stoi(line.substr(1))};

            if (direction == 'R')
            {
                pos += value;
                count += std::abs(pos / total_value);
                pos = (pos % total_value);
            }
            else if (direction == 'L')
            {
                int add = (pos == 0 ? 0 : 1);
                pos -= value;
                if (pos <= 0)
                {
                    count += std::abs(pos / total_value) + add;
                    pos = (max_value + pos % total_value + 1) % total_value;
                }
            }
            else
            {
                std::abort();
            }
        }

        return count;
    }

}

int main() {
    {
        const auto result{part_one::get_values1(part_one::input1)};

        std::cout << "Result1.0: " << result << std::endl;

        const auto input1_p1{part_one::get_input("./day1.input")};
        const auto result1{part_one::get_values1(input1_p1)};

        // 256 is too low
        // 1060 is too low
        std::cout << "Result1.1: " << result1 << std::endl;
    }

    {
        const auto result{part_one::get_values2(part_one::input1)};

        std::cout << "Result2.0: " << result << std::endl;

        const auto input1_p1{part_one::get_input("./day1.input")};
        const auto result1{part_one::get_values2(input1_p1)};

        // 7031 is too high
        std::cout << "Result2.1: " << result1 << std::endl;
    }

    return 0;
}