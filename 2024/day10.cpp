
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

    // Locate all 0s in the map
    // {X;Y}
    std::vector<std::pair<uint8_t, uint8_t>> get_all_starts(const std::vector<std::vector<int8_t>> & input) {
        std::vector<std::pair<uint8_t, uint8_t>> result{};

        size_t Y{0};
        for (const auto & line : input) {
            size_t X{0};
            for (auto altitude : line) {
                if (altitude == 0) {
                    result.push_back({X, Y});
                }
                X++;
            }
            Y++;
        }

        return result;
    }

    // vector of points
    // take the tail, if 9 put the vector into result vector,
    // take a tail, find the neighboring steps going up one step.
    //  if only one, add to vector, push the vector to the back of queue
    //  if more than 1, make a copy, add, push to the back of the queue
    //
    // {X;Y}
    std::vector<std::vector<std::pair<uint8_t, uint8_t>>> get_paths(const std::pair<uint8_t, uint8_t> & start,
                                                                    const std::vector<std::vector<int8_t>> & data) {
        std::vector<std::vector<std::pair<uint8_t, uint8_t>>> result{};
        const auto width{data[0].size()};
        const auto hight{data.size()};

        std::queue<std::vector<std::pair<uint8_t, uint8_t>>> queue{};
        {
            std::vector<std::pair<uint8_t, uint8_t>> path{start};
            queue.push(path);
        }

        while (queue.size() != 0) {
            auto path{queue.front()};

            const auto back{path.back()};
            if (data[back.second][back.first] == 9) {
                result.push_back(path);
                queue.pop();
                continue;
            }

            // Check the next step
            // UP
            if (back.second > 0) {
                if (data[back.second-1][back.first] == data[back.second][back.first]+1) {
                    auto path_copy{path};
                    path_copy.push_back({back.first, back.second-1});
                    queue.push(path_copy);
                }
            }

            // DOWN
            if (back.second < (hight-1)) {
                if (data[back.second+1][back.first] == data[back.second][back.first]+1) {
                    auto path_copy{path};
                    path_copy.push_back({back.first, back.second+1});
                    queue.push(path_copy);
                }
            }

            // LEFT
            if (back.first > 0) {
                if (data[back.second][back.first-1] == data[back.second][back.first]+1) {
                    auto path_copy{path};
                    path_copy.push_back({back.first-1, back.second});
                    queue.push(path_copy);
                }
            }

            // RIGHT
            if (back.first < (width-1)) {
                if (data[back.second][back.first+1] == data[back.second][back.first]+1) {
                    auto path_copy{path};
                    path_copy.push_back({back.first+1, back.second});
                    queue.push(path_copy);
                }
            }
            queue.pop();
        }
        return result;
    }
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
        const auto starts_data{part_one::get_all_starts(data)};

        std::vector<std::set<std::pair<uint8_t, uint8_t>>> peaks{};
        for (const auto start : starts_data) {
            const auto paths{part_one::get_paths(start, data)};

            std::set<std::pair<uint8_t, uint8_t>> start_peaks{};
            for (const auto & path : paths) {
                const auto peak{path.back()};
                start_peaks.insert(peak);
            }
            peaks.push_back(start_peaks);
        }

        uint32_t total_count{0};
        for (size_t i=0; i<starts_data.size(); i++) {
            const auto peaks_for_start{peaks[i]};
            total_count += peaks_for_start.size();
            std::cout << "Head: {" << static_cast<uint16_t>(starts_data[i].first) << ":" <<
                                    static_cast<uint16_t>(starts_data[i].second) << "} " <<
                                    peaks_for_start.size() << "|";

            for (const auto & peak : peaks_for_start) {
                std::cout << "{" << static_cast<uint16_t>(peak.first) << ":" <<
                                    static_cast<uint16_t>(peak.second) << "}";
            }
            std::cout << std::endl;
        }
        std::cout << "Total: " << total_count << std::endl;

        {
            const auto input1_p1{get_input("./day10.input")};
            const auto data_p1{process_input(input1_p1)};
            const auto starts_data{part_one::get_all_starts(data_p1)};

            std::vector<std::set<std::pair<uint8_t, uint8_t>>> peaks{};
            for (const auto start : starts_data) {
                const auto paths{part_one::get_paths(start, data_p1)};

                std::set<std::pair<uint8_t, uint8_t>> start_peaks{};
                for (const auto & path : paths) {
                    const auto peak{path.back()};
                    start_peaks.insert(peak);
                }
                peaks.push_back(start_peaks);
            }

            uint32_t total_count{0};
            for (size_t i=0; i<starts_data.size(); i++) {
                const auto peaks_for_start{peaks[i]};
                total_count += peaks_for_start.size();
                std::cout << "Head: {" << static_cast<uint16_t>(starts_data[i].first) << ":" <<
                                        static_cast<uint16_t>(starts_data[i].second) << "} " <<
                                        peaks_for_start.size() << "|";

                for (const auto & peak : peaks_for_start) {
                    std::cout << "{" << static_cast<uint16_t>(peak.first) << ":" <<
                                        static_cast<uint16_t>(peak.second) << "}";
                }
                std::cout << std::endl;
            }
            std::cout << "Total: " << total_count << " Correct: 798" << std::endl;
        }
    }

    // P2
    {
        const auto data{process_input(part_two::input_ex4)};
        const auto starts_data{part_one::get_all_starts(data)};

        std::map<std::pair<uint8_t, uint8_t>, uint16_t> paths_map{};
        uint16_t total{0};
        for (const auto start : starts_data) {
            const auto paths{part_one::get_paths(start, data)};
            paths_map[start] = paths.size();
            total += paths.size();
        }

        for (const auto & path_info_pair : paths_map) {
            std::cout << "{" << static_cast<uint16_t>(path_info_pair.first.first) << ":" <<
                                static_cast<uint16_t>(path_info_pair.first.second) << "} - " <<
                                path_info_pair.second << std::endl;
        }
        std::cout << total << std::endl;

        {
            const auto input1_p1{get_input("./day10.input")};
            const auto data_p1{process_input(input1_p1)};
            const auto starts_data{part_one::get_all_starts(data_p1)};

            std::map<std::pair<uint8_t, uint8_t>, uint16_t> paths_map{};
            uint16_t total{0};
            for (const auto start : starts_data) {
                const auto paths{part_one::get_paths(start, data_p1)};
                paths_map[start] = paths.size();
                total += paths.size();
            }

            for (const auto & path_info_pair : paths_map) {
                std::cout << "{" << static_cast<uint16_t>(path_info_pair.first.first) << ":" <<
                                    static_cast<uint16_t>(path_info_pair.first.second) << "} - " <<
                                    path_info_pair.second << std::endl;
            }
            std::cout << total << " Correct: 1816" << std::endl;
        }
    }

    return 0;
}