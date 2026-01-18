
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
    const std::vector<std::string> input1 = {"3-5",
                                            "10-14",
                                            "16-20",
                                            "12-18",
                                            "",
                                            "1",
                                            "5",
                                            "8",
                                            "11",
                                            "17",
                                            "32"};

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

    std::pair<std::vector<std::pair<uint64_t, uint64_t>>, std::vector<uint64_t>> process_input(const std::vector<std::string> & input) {
        std::vector<std::pair<uint64_t, uint64_t>> ranges{};
        std::vector<uint64_t> ids{};

        bool ranges_state{true};
        for (const auto & line : input) {
            if(line.length() == 0) {
                ranges_state = false;
                continue;
            }

            if (ranges_state) {
                uint64_t a = 0, b = 0;
                sscanf(line.c_str(), "%llu-%llu", &a, &b);
                ranges.push_back({a,b});
            } else {
                ids.push_back(std::atoll(line.c_str()));
            }
        }

        // Sort the vector using the default std::pair comparison
        std::sort(ranges.begin(), ranges.end());

        return {ranges, ids};
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
    const auto input1_p1{part_one::get_input("./day5.input")};

    {
        uint64_t result{0};
        const auto [ranges, ids] = part_one::process_input(part_one::input1);

        for (const auto & id : ids) {
            for (const auto & range : ranges) {
                if (id >= range.first && id <= range.second) {
                    result++;
                    break;
                }
            }
        }
        std::cout << "Total 1.1: " << result << std::endl;

        uint64_t result2{0};
        const auto [ranges2, ids2] = part_one::process_input(input1_p1);

        for (const auto & id : ids2) {
            for (const auto & range : ranges2) {
                if (id >= range.first && id <= range.second) {
                    result2++;
                    break;
                }
            }
        }
        // 623
        std::cout << "Total 1.2: " << result2 << std::endl;
    }
    {
        uint32_t result{0};
        const auto [ranges, ids] = part_one::process_input(part_one::input1);

        // find the range
        uint64_t min, max;
        min = ranges[0].first;
        max = ranges[0].second;

        for (const auto & range : ranges) {
            if (min > range.first) {
                min = range.first;
            }
            if (max < range.second) {
                max = range.second;
            }
        }
        std::cout << "Range 2.1: " << min << "-" << max << std::endl;
        bool found_range_flag{false};
        std::pair<uint64_t, uint64_t> found_range{};
        for (uint64_t i=min; i<=max; i++) {
            if (found_range_flag == true) {
                if ((found_range.first <= i) && (i <= found_range.second)) {
                    result += (found_range.second - i + 1);
                    i = found_range.second;
                    continue;
                } else {
                    found_range_flag = false;
                }
            }
            for (size_t r=0; r<ranges.size(); r++) {
                const auto range = ranges[r];
                if ((range.first <= i) && (i <= range.second)) {
                    found_range = range;
                    found_range_flag = true;
                    result++;
                    break;
                } else {
                    if (r < (ranges.size() - 1)) {
                        if ((i > range.second) && (i < ranges[r+1].first)) {
                            i = ranges[r+1].first - 1;
                            break;
                        }
                    }
                }
            }
        }
        std::cout << "Total 2.1: " << result << std::endl;

        uint64_t result2{0};
        const auto [ranges2, ids2] = part_one::process_input(input1_p1);

        min = ranges2[0].first;
        max = ranges2[0].second;

        for (const auto & range : ranges2) {
            if (min > range.first) {
                min = range.first;
            }
            if (max < range.second) {
                max = range.second;
            }
        }
        std::cout << "Range 2.2: " << min << "-" << max << std::endl;
        found_range_flag = false;

        for (uint64_t i=min; i<=max; i++) {
            if (found_range_flag == true) {
                if ((found_range.first <= i) && (i <= found_range.second)) {
                    result2 += (found_range.second - i + 1);
                    i = found_range.second;
                    continue;
                } else {
                    found_range_flag = false;
                }
            }
            for (size_t r=0; r<ranges2.size(); r++) {
                const auto range = ranges2[r];
                if ((range.first <= i) && (i <= range.second)) {
                    found_range = range;
                    found_range_flag = true;
                    result2++;
                    break;
                } else {
                    if (r < (ranges2.size() - 1)) {
                        if ((i > range.second) && (i < ranges2[r+1].first)) {
                            i = ranges2[r+1].first - 1;
                            break;
                        }
                    }
                }
            }
        }

        // 353507173555373
        std::cout << "Total 2.2: " << result2 << std::endl;
    }
    return 0;
}