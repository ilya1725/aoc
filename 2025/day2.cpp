
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <print>
#include <sstream>
#include <string_view>
#include <tuple>
#include <vector>

#include <stdint.h>

namespace part_one {
    const std::vector<std::string> input1 = {"11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,446443-446449,38593856-38593862,565653-565659,824824821-824824827,2121212118-2121212124"};

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

    std::vector<std::tuple<uint64_t, uint64_t>> process_input(const std::vector<std::string> & input) {
        std::vector<std::tuple<uint64_t, uint64_t>> output{};

        for (const auto & line: input) {
            std::stringstream ss(line);
            std::string segment;

            // 1. Split string by comma
            while (std::getline(ss, segment, ',')) {
                std::stringstream segmentStream(segment);
                uint64_t start, end;
                char dash;

                // 2. Parse "Number-Number"
                // The extraction operator (>>) handles the conversion
                if (segmentStream >> start >> dash >> end) {
                    output.emplace_back(start, end);
                }
            }
        }
        return output;
    }

    uint8_t count_digits(const uint64_t number) {
        if (number < 10) {
            return 1;
        }
        if (number < 100) {
            return 2;
        }

        uint8_t result{2};
        uint64_t div{100};

        uint64_t front{number / div};
        while (front != 0)
        {
            result++;
            div *= 10;
            front = (number / div);
        }

        return result;
    }

    bool id_valid1(const uint64_t id) {
        const auto digits{count_digits(id)};

        if ((digits % 2) != 0) {
            return true;
        }

        bool res{true};

        const uint64_t divider{static_cast<uint64_t>(std::pow(10u, (digits/2)))};
        const uint64_t left{id / divider};
        const uint64_t right{id % divider};

        if (left == right) {
            res = false;
            std::cout << "Inv: " << id << " [" << divider << "]" << std::endl;
        }
        return res;
    }

    bool id_valid2(const uint64_t id) {
        const auto digits{count_digits(id)};

        bool res{true};
        for (uint8_t i=1; i<digits; i++) {
            if ((digits % i) == 0) {
                const uint64_t divider{static_cast<uint64_t>(std::pow(10u, i))};
                const auto steps{digits / i};

                const auto initial{id % divider};
                bool matched{true};
                for (uint8_t j=1; j<steps; j++) {
                    // Shift, then divide to find reminder
                    const auto next{ ( id / (static_cast<uint64_t>(std::pow(divider, j))) ) % divider};
                    if (next != initial) {
                        matched = false;
                        break;
                    }
                }
                if (matched == true) {
                    res = false;
                    std::cout << "Inv: " << id << " [" << divider << "]" << std::endl;
                    break;
                }
            }
        }

        return res;
    }
}

int main() {
    const auto input1_p1{part_one::get_input("./day2.input")};

    {
        const auto result{part_one::process_input(part_one::input1)};
        uint64_t count{0};

        for (const auto & range : result) {
            const auto start{std::get<0>(range)};
            const auto stop{std::get<1>(range)};

            for (uint64_t i=start; i<=stop; i++) {
                if (part_one::id_valid1(i) == false) {
                    count += i;
                }
            }
        }

        std::cout << "Total 1.1: " << count << std::endl;

        const auto result2{part_one::process_input(input1_p1)};
        count = 0;

        for (const auto & range : result2) {
            const auto start{std::get<0>(range)};
            const auto stop{std::get<1>(range)};

            for (uint64_t i=start; i<=stop; i++) {
                if (part_one::id_valid1(i) == false) {
                    count += i;
                }
            }
        }

        // 19219508902
        std::cout << "Total 1.2: " << count << std::endl;
    }
    {
        const auto result{part_one::process_input(part_one::input1)};
        uint64_t count{0};

        for (const auto & range : result) {
            const auto start{std::get<0>(range)};
            const auto stop{std::get<1>(range)};

            for (uint64_t i=start; i<=stop; i++) {
                if (part_one::id_valid2(i) == false) {
                    count += i;
                }
            }
        }

        std::cout << "Total 2.1: " << count << std::endl;

        const auto result2{part_one::process_input(input1_p1)};
        count = 0;

        for (const auto & range : result2) {
            const auto start{std::get<0>(range)};
            const auto stop{std::get<1>(range)};

            for (uint64_t i=start; i<=stop; i++) {
                if (part_one::id_valid2(i) == false) {
                    count += i;
                }
            }
        }

        // 27180728081
        std::cout << "Total 2.2: " << count << std::endl;
    }

    return 0;
}