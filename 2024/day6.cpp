
#include <algorithm>
#include <array>
#include <cstdint>
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

    // x - first, y - second
    std::vector<std::string> get_route(const std::vector<std::string> & input, const std::pair<size_t, size_t> & start) {
        std::vector<std::string> result{input};
        auto current{start};

        while(true) {
            const auto current_char{result[current.second][current.first]};
            result[current.second][current.first] = 'X';

            if (current_char == '^') {
                if (current.second == 0) {
                    std::cout << "top\n";
                    break;
                }
                if (input[current.second-1][current.first] == '#'){
                    if (current.first == result[0].size()-1) {
                        std::cout << "right\n";
                        break;
                    }
                    result[current.second][current.first] = '>';
                } else {
                    current.second--;
                    result[current.second][current.first] = '^';
                }
            } else if (current_char == 'v') {
                if (current.second == result.size()-1) {
                    std::cout << "bottom\n";
                    break;
                }
                if (input[current.second+1][current.first] == '#'){
                    if (current.first == 0) {
                        std::cout << "left\n";
                        break;
                    }
                    result[current.second][current.first] = '<';
                } else {
                    current.second++;
                    result[current.second][current.first] = 'v';
                }
            } else if (current_char == '>') {
                if (current.first == result[0].size()-1) {
                    std::cout << "right\n";
                    break;
                }
                if (input[current.second][current.first+1] == '#'){
                    if (current.second == result.size()-1) {
                        std::cout << "bottom\n";
                        break;
                    }
                    result[current.second][current.first] = 'v';
                } else {
                    current.first++;
                    result[current.second][current.first] = '>';
                }
            } else {
                // <
                if (current.first == 0) {
                    std::cout << "left\n";
                    break;
                }
                if (input[current.second][current.first-1] == '#'){
                    if (current.second == 0) {
                        std::cout << "top\n";
                        break;
                    }
                    result[current.second][current.first] = '^';
                } else {
                    current.first--;
                    result[current.second][current.first] = '<';
                }
            }
        }

        return result;
    }

    uint32_t get_path_count(const std::vector<std::string> & input, const char key = 'X') {
        uint32_t count{0};

        for (const auto & line : input) {
            for (const auto c : line) {
                if (c == key) {
                    count++;
                }
            }
        }
        return count;
    }
}

namespace part_two {
    // x - first, y - second
    bool get_route_loop(const std::vector<std::string> & input,
                        const std::pair<size_t, size_t> & start,
                        const std::pair<size_t, size_t> & obstacle) {
        std::vector<std::string> result{input};
        result[start.second][start.first] = '^';
        result[obstacle.second][obstacle.first] = 'O';
        std::cout << "Obs: [" << obstacle.first << ":" << obstacle.second << "] ";

        bool loop{false};
        std::set<std::array<size_t, 4>> loop_set;

        auto current{start};
        uint16_t count{0};

        while(true) {
            const auto current_char{result[current.second][current.first]};
            result[current.second][current.first] = '*';
            const auto start_current{current};
            count++;

            if (current_char == '^') {
                if (current.second == 0) {
                    std::cout << "top[" << current.second << ":" << current.first << "]" << "\n";
                    break;
                }
                if (input[current.second-1][current.first] == '#' || result[current.second-1][current.first] == 'O'){
                    if (current.first == result[0].size()-1) {
                        std::cout << "right[" << current.second << ":" << current.first << "]" << "\n";
                        break;
                    }
                    result[current.second][current.first] = '>';
                } else {
                    current.second--;
                    result[current.second][current.first] = '^';
                }
            } else if (current_char == 'v') {
                if (current.second == result.size()-1) {
                    std::cout << "bottom[" << current.second << ":" << current.first << "]" << "\n";
                    break;
                }
                if (input[current.second+1][current.first] == '#' || result[current.second+1][current.first] == 'O'){
                    if (current.first == 0) {
                        std::cout << "left[" << current.second << ":" << current.first << "]" << "\n";
                        break;
                    }
                    result[current.second][current.first] = '<';
                } else {
                    current.second++;
                    result[current.second][current.first] = 'v';
                }
            } else if (current_char == '>') {
                if (current.first == result[0].size()-1) {
                    std::cout << "right[" << current.second << ":" << current.first << "]" << "\n";
                    break;
                }
                if (input[current.second][current.first+1] == '#' || result[current.second][current.first+1] == 'O'){
                    if (current.second == result.size()-1) {
                        std::cout << "bottom[" << current.second << ":" << current.first << "]" << "\n";
                        break;
                    }
                    result[current.second][current.first] = 'v';
                } else {
                    current.first++;
                    result[current.second][current.first] = '>';
                }
            } else if (current_char == '<') {
                // <
                if (current.first == 0) {
                    std::cout << "left[" << current.second << ":" << current.first << "]" << "\n";
                    break;
                }
                if (input[current.second][current.first-1] == '#' || result[current.second][current.first-1] == 'O'){
                    if (current.second == 0) {
                        std::cout << "top[" << current.second << ":" << current.first << "]" << "\n";
                        break;
                    }
                    result[current.second][current.first] = '^';
                } else {
                    current.first--;
                    result[current.second][current.first] = '<';
                }
            } else {
                std::cerr << "How did we get here?" << std::endl;
                std::exit(-1);
            }

            // if we locate the sae element in the set, there was a loop
            // if (loop_set.find({start_current.first, start_current.second, current.first, current.second}) == loop_set.end()) {
            //     loop_set.insert({start_current.first, start_current.second, current.first, current.second});
            // } else {
            //     std::cout << "Loop. " << count << ":" << loop_set.size() << std::endl;
            //     // pretty_print(result);
            //     loop = true;
            //     break;
            // }
            if (count > 10000) {
                std::cout << "Loop\n";
                loop = true;
                break;
            }
        }

        return loop;
    }

    // x - first, y - second
    uint16_t get_route_loop_count(const std::vector<std::string> & input,
                                    const std::pair<size_t, size_t> & start) {
        uint16_t count{0};
        uint16_t attempts_count{0};
        std::vector<std::string> obstacles{input};
        obstacles[start.second][start.first] = '^';

        for (size_t y=0; y<input.size(); y++) {
            for (size_t x=0; x<input[0].size(); x++) {
                if (start.second == y && start.first == x) {
                    std::cout << input[y][x] << std::endl;
                    continue;
                }
                if (input[y][x] == 'X') {
                    attempts_count++;
                    const auto loop{get_route_loop(input, start, {x, y})};
                    if (loop) {
                        count++;
                        obstacles[y][x] = 'O';
                    }
                }
            }
        }
        pretty_print(obstacles);
        std::cout << part_one::get_path_count(obstacles, 'O') << ":" << attempts_count << std::endl;

        return count;
    }

}

int main() {
    // P1
    const auto start{part_one::get_start(part_one::input)};
    const auto route_map{part_one::get_route(part_one::input, start)};

    const auto input1_p1{part_one::get_input("./day6.input")};

    const auto start1{part_one::get_start(input1_p1)};
    const auto route_map1{part_one::get_route(input1_p1, start1)};

    {
        const auto final_count{part_one::get_path_count(route_map)};

        std::cout << "Result1.1: " << final_count << std::endl;

        const auto final_count1{part_one::get_path_count(route_map1)};

        std::cout << "Result1.1: " << final_count1 << " Correct answer is 5129" << std::endl;
    }

    // P2
    {
        const auto final_count{part_two::get_route_loop_count(route_map, start)};

        std::cout << "Result2.1: " << final_count << std::endl;

        // pretty_print(route_map1);

        const auto final_count2{part_two::get_route_loop_count(route_map1, start1)};

        // 1946 is too high
        // 1884 is wrong.
        // 1886 is wrong.
        // 1887 is wrong.
        // 1586 is too low
        std::cout << "Result2.2: " << final_count2 << " Correct answer is 1888" << std::endl;
    }

    return 0;
}