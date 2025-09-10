
#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <cstdint>
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

    const std::vector<std::string> input_ex1{"2333133121414131402"
                                            };

    const std::vector<std::string> input_ex2{"12345"
                                            };

    const std::vector<std::string> input_ex3{"0110"
                                            };

    const std::vector<std::string> input_ex4{"233313312141413140202333133121414131402"};

    const std::vector<std::string> input_ex5{"1313165"};
    const std::vector<std::string> input_ex6{"9953877292941"};

    std::vector<int16_t> spread_disk(const std::vector<std::string> & input) {
        std::vector<int16_t> result{};

        uint32_t block_id{0};
        bool empty{false};

        for (const uint8_t c : input[0]) {
            const auto real_count{c - '0'};
            if (empty == true) {
                result.insert(result.end(), real_count, empty_block);
                empty = false;
            } else {
                result.insert(result.end(), real_count, block_id);
                block_id++;
                empty = true;
            }
        }

        return result;
    }

    std::vector<int16_t> compact_disk(const std::vector<int16_t> & input, const bool print = false) {
        std::vector<int16_t> result{input};

        std::vector<int16_t>::iterator gap_it{result.begin()};
        std::vector<int16_t>::iterator end_it{result.end()-1};

        while (true) {
            // locate the next gap
            while (*gap_it != empty_block && gap_it != result.end() && gap_it != end_it) {
                gap_it++;
            }
            if (gap_it == result.end()) {
                break;
            }
            if (gap_it == end_it) {
                break;
            }

            *gap_it = *end_it;
            *end_it = end_block;
            end_it--;

            if (print) {
                pretty_print(result);
            }
        }

        return result;
    }

    uint64_t checksum(const std::vector<int16_t> & input) {
        uint64_t result{0};

        for (size_t i{0}; i<input.size() && input[i]!=end_block; i++) {
            if (input[i] != empty_block) {
                result += (i*input[i]);
            }
        }
        return result;
    }
}

namespace part_two {
    // going left
    int16_t find_block_size(const std::vector<int16_t> & input,
                            size_t end_it) {
        const auto value{input[end_it]};
        int16_t size{1};

        while (end_it != 0) {
            end_it--;
            if (input[end_it] != value) {
                break;
            }
            size++;
        }
        return size;
    }

    // going left.
    // return {size, pointer}
    std::pair<int16_t, size_t> find_block_size2(const std::vector<int16_t> & input,
                                                size_t end_it) {
        int16_t size{1};

        while (input[end_it] == empty_block && end_it != 0) {
            end_it--;
        }

        const auto value{input[end_it]};
        while (end_it != 0) {
            if (input[end_it-1] != value) {
                break;
            }
            end_it--;
            size++;
        }
        return {size, end_it};
    }

    // going right
    std::pair<int16_t, size_t> find_next_empty_block_size(const std::vector<int16_t> & input,
                                                        const size_t start_it,
                                                        const size_t end_it) {
        int16_t size{0};
        auto empty_it{start_it};

        while (empty_it != end_it) {
            if (input[empty_it] == empty_block) {
                break;
            }
            empty_it++;
        }

        const auto begin_empty_it{empty_it};

        while (empty_it != end_it) {
            if (input[empty_it] != empty_block) {
                break;
            }
            empty_it++;
            size++;
        }

        return {size, begin_empty_it};
    }

    // Find the location of an empty block of the specified size. Return end_it if not found
    // going right
    size_t find_next_empty_block_size(const std::vector<int16_t> & input,
                                        const size_t size,
                                        const size_t start_it,
                                        const size_t end_it) {
        auto empty_it{start_it};

        while (empty_it != end_it) {
            bool found_block{true};
            for (auto i=empty_it; i<(empty_it+size); i++) {
                if (input[i] != empty_block) {
                    found_block = false;
                    break;
                }
            }

            if (found_block == true) {
                break;
            }
            empty_it++;
        }

        return empty_it;
    }

    // Note: this version is incorrect
    std::vector<int16_t> compact_disk(const std::vector<int16_t> & input,
                                    const bool print = false) {
        std::vector<int16_t> result{input};

        size_t gap_it{0};   // result.begin()
        size_t end_it{result.size()-1};

        auto block_size{find_block_size(result, end_it)};
        while (true) {

            auto empty_candidate{find_next_empty_block_size(result, gap_it, end_it)};

            if (empty_candidate.second == end_it) {
                if (empty_candidate.first == 0 && gap_it == 0) {
                    break;
                }

                // no candidate to relocate
                end_it -= block_size;
                while (result[end_it] == empty_block && end_it != 0) {
                    end_it--;
                }
                if (end_it == 0) {
                    break;
                }
                gap_it = 0;

            } else if (empty_candidate.first >= block_size) {
                // relocate candidate
                for (int16_t i{0}; i<block_size; i++) {
                    result[empty_candidate.second] = result[end_it];
                    result[end_it] = empty_block;
                    end_it--;
                    empty_candidate.second++;
                }

                // Find next block
                while (result[end_it] == empty_block && end_it != 0) {
                    end_it--;
                }
                if (print) {
                    pretty_print(result);
                }

            } else {
                // keep searching
                gap_it = empty_candidate.second + empty_candidate.first;

                if (gap_it == end_it) {
                    break;
                }
                continue;
            }

            block_size = find_block_size(result, end_it);
        }

        return result;
    }

    std::vector<int16_t> compact_disk2(const std::vector<int16_t> & input, const bool print = false) {
        std::vector<int16_t> result{input};

        size_t gap_it{0};   // result.begin()
        size_t end_it{result.size()-1};

        if (print) {
            pretty_print(result);
        }

        while(true) {
            auto candidate_block{find_block_size2(result, end_it)};

            auto candidate_spot{find_next_empty_block_size(result, candidate_block.first, 0, candidate_block.second)};

            if (candidate_spot == candidate_block.second) {
                // didn't find any
            } else {
                // copy the data
                for (auto i=0; i<candidate_block.first; i++) {
                    const auto tmp_value{result[i+candidate_spot]};
                    result[i+candidate_spot] = result[i+candidate_block.second];
                    result[i+candidate_block.second] = tmp_value;
                }

                if (print) {
                    pretty_print(result);
                }
            }
            if (candidate_block.second == 0) {
                std::cout << "Block at the beginning" << std::endl;
                break;
            }
            end_it = candidate_block.second - 1;
        }

        return result;
    }

}

int main() {
    // P1
    {
        const auto data{part_one::spread_disk(part_one::input_ex1)};
        const auto compact_data{part_one::compact_disk(data, true)};

        const auto result_ex1{part_one::checksum(compact_data)};
        std::cout << "Result ex1.1: " << result_ex1 << std::endl;

        const auto input1_p1{get_input("./day9.input")};
        const auto data_p1{part_one::spread_disk(input1_p1)};
        const auto compact_data_p1{part_one::compact_disk(data_p1)};

        const auto result_p1{part_one::checksum(compact_data_p1)};
        std::cout << "Result1.1: " << result_p1 << " Correct result: 6378826667552" << std::endl;
    }

    // P2
    {
        // ex1
        {
            const auto data{part_one::spread_disk(part_one::input_ex1)};
            const auto compact_data{part_two::compact_disk2(data, true)};

            const auto result_ex1{part_one::checksum(compact_data)};
            std::cout << "Result ex1.2: " << result_ex1 << std::endl;
        }

        // ex2
        {
            const auto data{part_one::spread_disk(part_one::input_ex6)};
            const auto compact_data{part_two::compact_disk2(data, true)};

            const auto result_ex1{part_one::checksum(compact_data)};
            std::cout << "Result ex5.2: " << result_ex1 << std::endl;
        }

        const auto input1_p1{get_input("./day9.input")};
        const auto data_p1{part_one::spread_disk(input1_p1)};
        const auto compact_data_p1{part_two::compact_disk2(data_p1)};

        const auto result_p1{part_one::checksum(compact_data_p1)};

        // 7379300303875 is too high
        std::cout << "Result2.1: " << result_p1 << " Correct result: 6413328569890" << std::endl;
    }

    return 0;
}