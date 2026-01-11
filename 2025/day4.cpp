
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
    const std::vector<std::string> input1 = {"..@@.@@@@.",
                                            "@@@.@.@.@@",
                                            "@@@@@.@.@@",
                                            "@.@@@@..@.",
                                            "@@.@@@@.@@",
                                            ".@@@@@@@.@",
                                            ".@.@.@.@@@",
                                            "@.@@@.@@@@",
                                            ".@@@@@@@@.",
                                            "@.@.@@@.@."};

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

    uint32_t process_input(const std::vector<std::string> & input) {
        uint32_t result{0};

        for (size_t x=0; x<input[0].length(); x++) {
            for (size_t y=0; y<input.size(); y++) {
                if(input[y][x] == '@') {
                    uint8_t count{0};
                    if ((y > 0 && x > 0) && input[y-1][x-1] == '@') {
                        count++;
                    }
                    if ((y > 0) && input[y-1][x] == '@') {
                        count++;
                    }
                    if ((y > 0 && x < (input[0].length()-1)) && input[y-1][x+1] == '@') {
                        count++;
                    }

                    if ((x > 0) && input[y][x-1] == '@') {
                        count++;
                    }
                    if ((x < (input[0].length()-1)) && input[y][x+1] == '@') {
                        count++;
                    }

                    if ((y < (input.size()-1) && x > 0) && input[y+1][x-1] == '@') {
                        count++;
                    }
                    if ((y < (input.size()-1)) && input[y+1][x] == '@') {
                        count++;
                    }
                    if ((y < (input.size()-1) && x < (input[0].length()-1)) && input[y+1][x+1] == '@') {
                        count++;
                    }

                    if (count < 4) {
                        result++;
                    }
                }
            }
        }
        return result;
    }

}

namespace part_two {
    uint32_t process_input(std::vector<std::string> & input) {
        uint32_t result{0};
        std::vector<std::pair<uint8_t, uint8_t>> roll{};

        for (size_t x=0; x<input[0].length(); x++) {
            for (size_t y=0; y<input.size(); y++) {
                if(input[y][x] == '@') {
                    uint8_t count{0};
                    if ((y > 0 && x > 0) && input[y-1][x-1] == '@') {
                        count++;
                    }
                    if ((y > 0) && input[y-1][x] == '@') {
                        count++;
                    }
                    if ((y > 0 && x < (input[0].length()-1)) && input[y-1][x+1] == '@') {
                        count++;
                    }

                    if ((x > 0) && input[y][x-1] == '@') {
                        count++;
                    }
                    if ((x < (input[0].length()-1)) && input[y][x+1] == '@') {
                        count++;
                    }

                    if ((y < (input.size()-1) && x > 0) && input[y+1][x-1] == '@') {
                        count++;
                    }
                    if ((y < (input.size()-1)) && input[y+1][x] == '@') {
                        count++;
                    }
                    if ((y < (input.size()-1) && x < (input[0].length()-1)) && input[y+1][x+1] == '@') {
                        count++;
                    }

                    if (count < 4) {
                        result++;
                        roll.push_back({y,x});
                    }
                }
            }
        }

        // Update the input
        for(auto r : roll) {
            input[r.first][r.second] = '.';
        }
        return result;
    }
}

int main() {
    const auto input1_p1{part_one::get_input("./day4.input")};

    {
        const auto result{part_one::process_input(part_one::input1)};

        std::cout << "Total 1.1: " << result << std::endl;

        const auto result2{part_one::process_input(input1_p1)};

        // 1320
        std::cout << "Total 1.2: " << result2 << std::endl;
    }
    {
        uint32_t result{0};

        uint64_t count{0};
        auto input_p1 = part_one::input1;
        do {
            count = part_two::process_input(input_p1);
            result += count;
        } while(count != 0);
        std::cout << "Total 2.1: " << result << std::endl;

        count = 0;
        result = 0;
        auto input_p2 = input1_p1;
        do {
            count = part_two::process_input(input_p2);
            result += count;
        } while(count != 0);

        // 8354
        std::cout << "Total 2.2: " << result << std::endl;
    }
    return 0;
}