
#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
#include <sstream>
#include <stack>
#include <string_view>
#include <tuple>
#include <vector>

namespace part_one {

    const std::vector<std::string> input = {"47|53",
                                            "97|13",
                                            "97|61",
                                            "97|47",
                                            "75|29",
                                            "61|13",
                                            "75|53",
                                            "29|13",
                                            "97|29",
                                            "53|29",
                                            "61|53",
                                            "97|53",
                                            "61|29",
                                            "47|13",
                                            "75|47",
                                            "97|75",
                                            "47|61",
                                            "75|61",
                                            "47|29",
                                            "75|13",
                                            "53|13",
                                            "",
                                            "75,47,61,53,29",
                                            "97,61,53,29,13",
                                            "75,29,13",
                                            "75,97,47,61,53",
                                            "61,13,29",
                                            "97,13,75,29,47"
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

    std::vector<std::pair<uint8_t, uint8_t>> get_instructions(const std::vector<std::string> & input) {
        std::vector<std::pair<uint8_t, uint8_t>> result{};

        bool instruction_flag{false};
        for (const auto & line : input) {
            // Exit on empty line
            if (line.length() == 0) {
                break;
            }

            std::stringstream ss(line);
            uint32_t a, b;
            char pipe{'|'};
            ss >> a >> pipe >> b;
            result.emplace_back(a, b);
        }

        return result;
    }

    std::vector<std::vector<uint8_t>> get_updates(const std::vector<std::string> & input) {
        std::vector<std::vector<uint8_t>> result{};

        bool instruction_flag{false};
        for (const auto & line : input) {

            // Switch state on the empty line
            if (line.length() == 0 && instruction_flag == false) {
                instruction_flag = true;
                continue;
            }

            if (instruction_flag) {
                std::vector<uint8_t> instruction;
                std::stringstream ss(line);
                std::string item;

                while (std::getline(ss, item, ',')) {
                    instruction.push_back(std::stoi(item));
                }
                result.push_back(instruction);
            }
        }

        return result;
    }

    std::vector<std::vector<uint8_t>> find_correct(const std::vector<std::vector<uint8_t>> & updates, const std::vector<std::pair<uint8_t, uint8_t>> & instructions) {
        std::vector<std::vector<uint8_t>> result{};

        for (const auto & upd : updates) {
            size_t failed_count{0};
            for (const auto instr : instructions) {
                size_t first_found{upd.size()};
                size_t second_found{upd.size()};
                for (size_t index=0; index < upd.size(); index++) {
                    const auto & page{upd[index]};
                    if (instr.first == page) {
                        first_found = index;
                        continue;
                    }
                    if (instr.second == page) {
                        second_found = index;
                        continue;
                    }
                }

                // only if both found and they have right order
                if (first_found != upd.size() && second_found != upd.size()) {
                    if (first_found >= second_found) {
                        failed_count++;
                    }
                }
            }

            if (failed_count == 0) {
                // no failed instructions
                result.push_back(upd);
            }
        }

        return result;
    }
}

namespace part_two {
    std::vector<std::vector<uint8_t>> find_incorrect(const std::vector<std::vector<uint8_t>> & updates, const std::vector<std::pair<uint8_t, uint8_t>> & instructions) {
        std::vector<std::vector<uint8_t>> result{};

        for (auto upd : updates) {
            std::vector<std::pair<uint8_t, uint8_t>> failed_rules{};
            for (const auto instr : instructions) {
                size_t first_found{upd.size()};
                size_t second_found{upd.size()};
                for (size_t index=0; index < upd.size(); index++) {
                    const auto & page{upd[index]};
                    if (instr.first == page) {
                        first_found = index;
                        continue;
                    }
                    if (instr.second == page) {
                        second_found = index;
                        continue;
                    }
                }

                // only if both found and they have right order
                // if not - flip the values
                if (first_found != upd.size() && second_found != upd.size()) {
                    if (first_found >= second_found) {
                        failed_rules.emplace_back(first_found, second_found);
                    }
                }
            }

            if (!failed_rules.empty()) {
                for (const auto flip : failed_rules) {
                    auto tmp{upd[flip.first]};
                    upd[flip.first] = upd[flip.second];
                    upd[flip.second] = tmp;
                }
                result.push_back(upd);
            }
        }

        return result;
    }

    std::vector<std::vector<uint8_t>> find_incorrect2(const std::vector<std::vector<uint8_t>> & updates, const std::vector<std::pair<uint8_t, uint8_t>> & instructions) {
        std::vector<std::vector<uint8_t>> result{};

        uint16_t upd_index{0};
        for (auto  upd : updates) {
            uint32_t count{0};
            while (true) {
                bool exit_flag = true;
                for (const auto instr : instructions) {
                    size_t first_found{upd.size()};
                    size_t second_found{upd.size()};
                    for (size_t index=0; index < upd.size(); index++) {
                        const auto & page{upd[index]};
                        if (instr.first == page) {
                            first_found = index;
                            continue;
                        }
                        if (instr.second == page) {
                            second_found = index;
                            continue;
                        }
                    }

                    // only if both found and they have right order
                    // if not - flip the values
                    if (first_found != upd.size() && second_found != upd.size()) {
                        if (first_found >= second_found) {
                            auto tmp{upd[first_found]};
                            upd[first_found] = upd[second_found];
                            upd[second_found] = tmp;

                            // break to try again
                            exit_flag = false;
                            break;
                        }
                    }
                }
                count++;
                if (exit_flag == true) {
                    std::cout << upd_index++ << ":Exited at " << count << std::endl;
                    // skip the original correct lists
                    if (count != 1) {
                        result.push_back(upd);
                    }
                    break;
                }
            }
        }

        return result;
    }

}

int main() {
    // P1
    {
        const auto instructions{part_one::get_instructions(part_one::input)};
        const auto updates{part_one::get_updates(part_one::input)};

        const auto correct{part_one::find_correct(updates, instructions)};

        uint16_t final_count{0};
        for (const auto & c : correct) {
            if ((c.size() & 0x1) == 0) {
                std::cerr << "Something is not right" << std::endl;
                return 1;
            }
            final_count += (c[c.size()/2]);
        }
        std::cout << "Result1.1: " << final_count << std::endl;

        const auto input1_p1{part_one::get_input("./day5.input")};

        const auto instructions1{part_one::get_instructions(input1_p1)};
        const auto updates1{part_one::get_updates(input1_p1)};

        const auto correct1{part_one::find_correct(updates1, instructions1)};

        uint16_t final_count1{0};
        for (const auto & c : correct1) {
            if ((c.size() & 0x1) == 0) {
                std::cerr << "Something is not right" << std::endl;
                return 1;
            }
            final_count1 += (c[c.size()/2]);
        }
        std::cout << "Result1.2: " << final_count1 << " Correct answer is 4135" << std::endl;
    }

    // P2
    {
        const auto instructions{part_one::get_instructions(part_one::input)};
        const auto updates{part_one::get_updates(part_one::input)};

        const auto incorrect{part_two::find_incorrect2(updates, instructions)};
        const auto check{part_one::find_correct(incorrect, instructions)};

        uint16_t final_count{0};
        for (const auto & c : incorrect) {
            if ((c.size() & 0x1) == 0) {
                std::cerr << "Something is not right" << std::endl;
                return 1;
            }
            final_count += (c[c.size()/2]);
        }
        std::cout << "Result2.1: " << final_count << std::endl;

        const auto input1_p1{part_one::get_input("./day5.input")};

        const auto instructions1{part_one::get_instructions(input1_p1)};
        const auto updates1{part_one::get_updates(input1_p1)};

        const auto incorrect1{part_two::find_incorrect2(updates1, instructions1)};
        const auto check1{part_one::find_correct(incorrect1, instructions1)};

        uint16_t final_count1{0};
        for (const auto & c : incorrect1) {
            if ((c.size() & 0x1) == 0) {
                std::cerr << "Something is not right" << std::endl;
                return 1;
            }
            final_count1 += (c[c.size()/2]);
        }
        // 5511 is too high
        // 4135 is too low
        std::cout << "Result2.2: " << final_count1 << " Correct answer is 5285" << std::endl;
    }

    return 0;
}