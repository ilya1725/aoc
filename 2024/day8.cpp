
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

uint32_t get_path_count(const std::vector<std::string> & input, const char key = 'X', const bool flip = false) {
    uint32_t count{0};

    for (const auto & line : input) {
        for (const auto c : line) {
            if ((c == key && flip == false) || (c != key && flip == true)) {
                count++;
            }
        }
    }
    return count;
}

namespace part_one {

    const std::vector<std::string> input_ex1{"............",
                                            "........0...",
                                            ".....0......",
                                            ".......0....",
                                            "....0.......",
                                            "......A.....",
                                            "............",
                                            "............",
                                            "........A...",
                                            ".........A..",
                                            "............",
                                            "............"
                                            };

    const std::vector<std::string> input_ex2{""
                                                };

    std::map<char, std::vector<std::pair<uint8_t, uint8_t>>> process_data(const std::vector<std::string> & input) {
        std::map<char, std::vector<std::pair<uint8_t, uint8_t>>> result{};

        // first - x; second - y
        for (size_t l=0; l<input.size(); l++) {
            for (size_t c=0; c<input[l].size(); c++) {
                const auto frequency{input[l][c]};
                if (frequency != '.') {
                    auto locations{result.find(frequency)};
                    if (locations == result.end()) {
                        result[frequency] = {{c, l}};
                    } else {
                        locations->second.push_back({c, l});
                    }
                }
            }
        }
        return result;
    }

    bool is_on_map(const std::vector<std::string> & input,
                    const std::pair<int, int> & point) {
        bool result{true};

        if (point.first < 0 || point.second < 0) {
            return false;
        }

        const auto width{input[0].size()};
        const auto hight{input.size()};

        if (point.first >= width || point.second >= hight) {
            return false;
        }

        return result;
    }

    std::vector<std::string> find_antinodes(const std::vector<std::string> & input,
                    const std::map<char, std::vector<std::pair<uint8_t, uint8_t>>> & frequencies) {
        std::vector<std::string> result{input};

        // n(n-1) / 2*.
        for (const auto& [key, locations]: frequencies) {
            if (locations.size() == 1) {
                std::cout << "Skip antennas of size 1: " << key << std::endl;
            }
            std::vector<std::pair<std::pair<uint8_t, uint8_t>, std::pair<uint8_t, uint8_t>>> antenna_pairs{};

            for(size_t i = 0; i < locations.size(); ++i) {
                for(size_t j = i + 1; j < locations.size(); ++j) {
                    // populate the result map
                    // first - x; second - y
                    const auto delta_x{locations[j].first - locations[i].first};
                    const auto delta_y{locations[j].second - locations[i].second};

                    const std::pair<int, int> resonance1{locations[i].first - delta_x, locations[i].second - delta_y};
                    const std::pair<int, int> resonance2{locations[j].first + delta_x, locations[j].second + delta_y};

                    if (is_on_map(input, resonance1) ==true) {
                        result[resonance1.second][resonance1.first] = '#';
                    }
                    if (is_on_map(input, resonance2) ==true) {
                        result[resonance2.second][resonance2.first] = '#';
                    }
                }
            }
        }

        return result;
    }

}

namespace part_two {
    std::vector<std::string> find_antinodes(const std::vector<std::string> & input,
                    const std::map<char, std::vector<std::pair<uint8_t, uint8_t>>> & frequencies) {
        std::vector<std::string> result{input};

        // n(n-1) / 2*.
        for (const auto& [key, locations]: frequencies) {
            if (locations.size() == 1) {
                std::cout << "Skip antennas of size 1: " << key << std::endl;
            }
            std::vector<std::pair<std::pair<uint8_t, uint8_t>, std::pair<uint8_t, uint8_t>>> antenna_pairs{};

            for(size_t i = 0; i < locations.size(); ++i) {
                for(size_t j = i + 1; j < locations.size(); ++j) {
                    // populate the result map
                    // first - x; second - y
                    const auto delta_x{locations[j].first - locations[i].first};
                    const auto delta_y{locations[j].second - locations[i].second};

                    uint8_t harmonic{0};
                    while (true) {
                        harmonic++;
                        const std::pair<int, int> resonance1{locations[i].first - (delta_x*harmonic), locations[i].second - (delta_y*harmonic)};

                        if (part_one::is_on_map(input, resonance1) == true) {
                            result[resonance1.second][resonance1.first] = '#';
                        } else {
                            break;
                        }
                    }

                    harmonic = 0;
                    while (true) {
                        harmonic++;
                        const std::pair<int, int> resonance2{locations[j].first + (delta_x*harmonic), locations[j].second + (delta_y*harmonic)};

                        if (part_one::is_on_map(input, resonance2) == true) {
                            result[resonance2.second][resonance2.first] = '#';
                        } else {
                            break;
                        }
                    }
                }
            }
        }

        return result;
    }
}

int main() {
    // P1
    {
        const auto data{part_one::process_data(part_one::input_ex1)};
        const auto antinodes{part_one::find_antinodes(part_one::input_ex1, data)};

        const auto result_ex1{get_path_count(antinodes, '#')};
        std::cout << "Result ex1.1: " << result_ex1 << std::endl;
        pretty_print(antinodes);

        const auto input1_p1{get_input("./day8.input")};
        const auto data_p1{part_one::process_data(input1_p1)};
        const auto antinodes_p1{part_one::find_antinodes(input1_p1, data_p1)};

        const auto result_p1{get_path_count(antinodes_p1, '#')};
        std::cout << "Result1.1: " << result_p1 << " Correct result: 327" << std::endl;
        pretty_print(antinodes_p1);
    }

    // P2
    {
        const auto data{part_one::process_data(part_one::input_ex1)};
        const auto antinodes{part_two::find_antinodes(part_one::input_ex1, data)};

        const auto result_ex1{get_path_count(antinodes, '.', true)};
        std::cout << "Result ex1.2: " << result_ex1 << std::endl;
        pretty_print(antinodes);

        const auto input1_p1{get_input("./day8.input")};
        const auto data_p1{part_one::process_data(input1_p1)};
        const auto antinodes_p1{part_two::find_antinodes(input1_p1, data_p1)};

        const auto result_p1{get_path_count(antinodes_p1, '.', true)};
        std::cout << "Result2.1: " << result_p1 << " Correct result: 1233" << std::endl;
        pretty_print(antinodes_p1);
    }

    return 0;
}