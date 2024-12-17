
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>
#include <tuple>
#include <vector>

namespace part_one {
    const std::vector<std::string> input1 = {"7 6 4 2 1",
                                            "1 2 7 8 9",
                                            "9 7 6 2 1",
                                            "1 3 2 4 5",
                                            "8 6 4 4 1",
                                            "1 3 6 7 9"};

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

    std::vector<std::vector<short>> get_values(const std::vector<std::string> & input) {
        std::vector<std::vector<short>> result{};
        for (const std::string & line : input) {
            std::vector<short> result_line{};
            std::stringstream ss(line);
            short number;

            // Get left
             while(ss >> number) {
                result_line.push_back(number);
            }
            result.push_back(result_line);
        }

        return result;
    }

    bool process_report(const std::vector<short> & report) {
        const short max_step{3};

        bool safe{true};
        bool increasing{true};
        auto prev_level{report[0]};
        for (auto it = report.begin() + 1; it != report.end(); ++it) {
            // Check if the step is too large
            if (std::abs(*it - prev_level) > max_step) {
                safe = false;
                break;
            }

            // Check if the step is zero
            if (std::abs(*it - prev_level) == 0) {
                safe = false;
                break;
            }

            // First case is abit tricky
            if (it == (report.begin() + 1)) {
                if (prev_level <= *it) {
                    increasing = true;
                } else {
                    increasing = false;
                }
            } else {
                if (prev_level <= *it && increasing == false) {
                    safe = false;
                    break;
                }

                if (prev_level > *it && increasing == true) {
                    safe = false;
                    break;
                }
            }
            prev_level = *it;
        }
        return safe;
    }

    long process_data1(const std::vector<std::vector<short>> & input) {
        long accumulator{0};
        for (const auto & report: input) {
            if (process_report(report)) {
                accumulator++;
            }
        }

        return accumulator;
    }
}

namespace part_two {
    // using part_one::process_data1;

    long process_data1(const std::vector<std::vector<short>> & input) {
        long accumulator{0};
        for (const auto & report: input) {
            if (part_one::process_report(report)) {
                accumulator++;
            } else {
                // Go over the report to see if it can be recovered by one removal
                for (auto it = report.begin(); it != report.end(); ++it) {

                    std::vector<short> sub_report;
                    sub_report.reserve(report.size() - 1);

                    sub_report.insert(sub_report.end(), report.begin(), it);
                    sub_report.insert(sub_report.end(), it + 1, report.end());

                    if (part_one::process_report(sub_report)) {
                        accumulator++;
                        break;
                    }
                }
            }
        }

        return accumulator;
    }
}

int main() {
    const auto result{part_one::get_values(part_one::input1)};

    auto accumulator{part_one::process_data1(result)};
    std::cout << "Result1: " << accumulator << std::endl;

    const auto input1_p1{part_one::get_input("./day2.input")};

    const auto result1{part_one::get_values(input1_p1)};
    accumulator = part_one::process_data1(result1);
    std::cout << "Result2: " << accumulator << std::endl;

    accumulator = part_two::process_data1(result);
    std::cout << "Result1.2: " << accumulator << std::endl;

    accumulator = part_two::process_data1(result1);
    std::cout << "Result2.2: " << accumulator << std::endl;

    return 0;
}