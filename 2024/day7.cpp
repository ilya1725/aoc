
#include <algorithm>
#include <array>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
#include <sstream>
#include <stack>
#include <string_view>
#include <tuple>
#include <vector>
#include <unordered_set>
#include <set>

void pretty_print(const std::vector<std::string> & input) {
    std::cout << std::endl;
    size_t count{0};
    for (const auto & line : input) {
        std::cout << std::format("{:5}:{}", count, line) << std::endl;
        count++;
    }
    return;
}
namespace part_one {

    const std::vector<std::string> input = {"....#.....",
                                            ".........#",
                                            "..........",
                                            "..#.......",
                                            ".......#..",
                                            "..........",
                                            ".#..^.....",
                                            "........#.",
                                            "#.........",
                                            "......#..."
                                            };

    const std::vector<std::string> input_ex1 = {"##..",
                                                "...#",
                                                "....",
                                                "^.#."
                                                    };

    const std::vector<std::string> input_ex2 = {"###.",
                                                "#.^.",
                                                "..#."
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

    std::pair<size_t, size_t> get_start(const std::vector<std::string> & input) {
        size_t x, y = 0;
        for (const auto & line : input) {
            // Exit on empty line
            if (line.length() == 0) {
                break;
            }

            x= line.find('^');
            if (x != std::string::npos) {
                break;
            }
            y++;
        }

        return {x, y};
    }

}

namespace part_two {
}

int main() {
    // P1
    const auto start{part_one::get_start(part_one::input)};
    // const auto route_map{part_one::get_route(part_one::input, start)};

    // const auto input1_p1{part_one::get_input("./day7.input")};

    // const auto start1{part_one::get_start(input1_p1)};
    // const auto route_map1{part_one::get_route(input1_p1, start1)};

    // {
    //     const auto final_count{part_one::get_path_count(route_map)};

    //     std::cout << "Result1.1: " << final_count << std::endl;

    //     const auto final_count1{part_one::get_path_count(route_map1)};

    //     std::cout << "Result1.1: " << final_count1 << " Correct answer is 5129" << std::endl;
    // }

    // // P2
    // {
    //     const auto final_count{part_two::get_route_loop_count(route_map, start)};

    //     std::cout << "Result2.1: " << final_count << std::endl;

    //     // pretty_print(route_map1);

    //     const auto final_count2{part_two::get_route_loop_count(route_map1, start1)};

    //     // 1946 is too high
    //     // 1884 is wrong.
    //     // 1886 is wrong.
    //     // 1887 is wrong.
    //     // 1586 is too low
    //     std::cout << "Result2.2: " << final_count2 << " Correct answer is 1888" << std::endl;
    // }

    return 0;
}