
#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <queue>
#include <ranges>
#include <set>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "time_measure.hpp"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

namespace util {
    template < typename StringT,
           typename DelimiterT = char,
           typename ContainerT = std::vector<std::string_view> >
    ContainerT split(StringT const& str, DelimiterT const& delimiters = ' ', bool trimEmpty = true, ContainerT&& tokens = {})
    {
        typename StringT::size_type pos, lastPos = 0, length = str.length();

        while (lastPos < length + 1)
        {
            pos = str.find_first_of(delimiters, lastPos);
            if (pos == StringT::npos)
                pos = length;

            if (pos != lastPos || !trimEmpty)
                tokens.emplace_back(str.data() + lastPos, pos - lastPos);

            lastPos = pos + 1;
        }

        return std::forward<ContainerT>(tokens);
    }

    enum class direction {
        up,
        down,
        left,
        right
    };

    class item {
      public:
        item(direction d, uint16_t row, uint16_t column) : d_(d), row_(row), column_(column) {};

        direction d_;
        uint16_t row_;
        uint16_t column_;
    };

    // Return vector of strings
    std::vector<std::string> get_input(const std::string & path) {
        std::ifstream file_in(path);
        if (!file_in) {
            std::cerr << "Can't open input file: " << path << std::endl;
            std::exit(-1);
        }

        std::string line;
        std::vector<std::string> output{};
        while (std::getline(file_in, line))
        {
            output.push_back(line);
        }
        return output;
    }

    // Return vector of strings
    std::vector<std::string> get_initial(const std::vector<std::string> & input) {
        std::vector<std::string> output{};

        for (const auto & line : input) {
            output.push_back(std::string(line.length(), '.'));
        }
        return output;
    }

    uint32_t energized(const std::vector<std::string> & input) {
        uint32_t output{0};

        for (const auto & line : input) {
            for (const char l : line) {
                if (l == '#') {
                    output++;
                }
            }
        }

        return output;
    }

    std::vector<item> process_path(const item & vector, const std::vector<std::string> & data, std::vector<std::string> & track) {

        const auto hight{data.size()};
        const auto width{data[0].length()};
        std::vector<item> output{};

        if (vector.column_ > width || vector.row_ > hight) {
            std::cerr << "ERROR: the size is wrong" << std::endl;
            return output;
        }

        auto curr_row{vector.row_};
        auto curr_column{vector.column_};
        auto curr_direction{vector.d_};

        while(true) {
            if (curr_direction == direction::down || curr_direction == direction::up) {
                if (data[curr_row][curr_column] == '-') {
                    if (track[curr_row][curr_column] != '#') {
                        track[curr_row][curr_column] = '#';
                        output.push_back({direction::left, curr_row, curr_column});
                        output.push_back({direction::right, curr_row, curr_column});
                    }
                    break;
                }

                if ((curr_direction == direction::down && data[curr_row][curr_column] == '/') ||
                    (curr_direction == direction::up && data[curr_row][curr_column] == '\\')) {
                    curr_direction = direction::left;
                }
                if ((curr_direction == direction::down && data[curr_row][curr_column] == '\\') ||
                    (curr_direction == direction::up && data[curr_row][curr_column] == '/')) {
                    curr_direction = direction::right;
                }
            } else {
                if (data[curr_row][curr_column] == '|') {
                    if (track[curr_row][curr_column] != '#') {
                        track[curr_row][curr_column] = '#';
                        output.push_back({direction::up, curr_row, curr_column});
                        output.push_back({direction::down, curr_row, curr_column});
                    }
                    break;
                }

                if ((curr_direction == direction::left && data[curr_row][curr_column] == '/') ||
                    (curr_direction == direction::right && data[curr_row][curr_column] == '\\')) {
                    curr_direction = direction::down;
                }
                if ((curr_direction == direction::left && data[curr_row][curr_column] == '\\') ||
                    (curr_direction == direction::right && data[curr_row][curr_column] == '/')) {
                    curr_direction = direction::up;
                }
            }

            track[curr_row][curr_column] = '#';
            if (curr_direction == direction::down) {
                if (curr_row == (hight-1)) {
                    break;
                }
                curr_row++;
            } else if (curr_direction == direction::up) {
                if (curr_row == 0) {
                    break;
                }
                curr_row--;
            } else if (curr_direction == direction::left) {
                if (curr_column == 0) {
                    break;
                }
                curr_column--;
            } else {
                if (curr_column == (width-1)) {
                    break;
                }
                curr_column++;
            }
        }

        return output;
    }

    uint32_t calc_energized(const item & vector, const std::vector<std::string> & data) {

        auto data_copy{util::get_initial(data)};

        std::queue<util::item> belt{};
        belt.push(vector);

        while (belt.size() != 0) {
            const auto item{belt.front()};

            auto vectors{util::process_path(item, data, data_copy)};

            belt.pop();
            for (auto vector : vectors) {
                belt.push(vector);
            }
        }

        const auto result{util::energized(data_copy)};

        return result;
    }
}

namespace ex {
}

namespace part_one {
}

namespace part_two {
}

int main(int argc, char *argv[]) {

    std::string data_file{"./day16.input"};
    if (argc == 2) {
        data_file = {argv[1]};
    }

    // Example
    {
        auto ex_input{util::get_input("./day16.ex.input")};

        const auto result{util::calc_energized(util::item(util::direction::right, 0, 0), ex_input)};
        std::cout << "E1:" << result << std::endl;
    }
    std::cout << std::endl;

    auto raw_input{util::get_input(data_file)};

    // P1
    const auto data_input{util::get_input(data_file)};
    if (1) {

        const auto result{util::calc_energized(util::item(util::direction::right, 0, 0), data_input)};
        std::cout << "T1:" << result << std::endl;
    }
    std::cout << std::endl;

    // Example
    {
        auto ex_input{util::get_input("./day16.ex.input")};

        std::vector<uint32_t> results{};

        for (uint16_t c=0; c<ex_input[0].length(); c++) {
            const auto result{util::calc_energized(util::item(util::direction::down, 0, c), ex_input)};
            results.push_back(result);

            std::cout << "E2: [" << 0 << "," << c << "] " << result << std::endl;
        }
        for (uint16_t c=0; c<ex_input[0].length(); c++) {
            const auto result{util::calc_energized(util::item(util::direction::up, ex_input.size()-1, c), ex_input)};
            results.push_back(result);

            std::cout << "E2: [" << ex_input.size()-1 << "," << c << "] " << result << std::endl;
        }

        for (uint16_t r=0; r<ex_input.size(); r++) {
            const auto result{util::calc_energized(util::item(util::direction::right, r, 0), ex_input)};
            results.push_back(result);

            std::cout << "E2: [" << r << "," << 0 << "] " << result << std::endl;
        }
        for (uint16_t r=0; r<ex_input.size(); r++) {
            const auto result{util::calc_energized(util::item(util::direction::left, r, ex_input[0].length()-1), ex_input)};
            results.push_back(result);

            std::cout << "E2: [" << r << "," << ex_input[0].length()-1 << "] " << result << std::endl;
        }

        std::cout << "E2 max:" << *std::max_element(results.begin(), results.end()) << std::endl;
    }

    std::cout << std::endl;

    // P2
    if (1) {
        std::vector<uint32_t> results{};

        for (uint16_t c=0; c<data_input[0].length(); c++) {
            const auto result{util::calc_energized(util::item(util::direction::down, 0, c), data_input)};
            results.push_back(result);

            std::cout << "T2: [" << 0 << "," << c << "] " << result << std::endl;
        }
        for (uint16_t c=0; c<data_input[0].length(); c++) {
            const auto result{util::calc_energized(util::item(util::direction::up, data_input.size()-1, c), data_input)};
            results.push_back(result);

            std::cout << "T2: [" << data_input.size()-1 << "," << c << "] " << result << std::endl;
        }

        for (uint16_t r=0; r<data_input.size(); r++) {
            const auto result{util::calc_energized(util::item(util::direction::right, r, 0), data_input)};
            results.push_back(result);

            std::cout << "T2: [" << r << "," << 0 << "] " << result << std::endl;
        }
        for (uint16_t r=0; r<data_input.size(); r++) {
            const auto result{util::calc_energized(util::item(util::direction::left, r, data_input[0].length()-1), data_input)};
            results.push_back(result);

            std::cout << "T2: [" << r << "," << data_input[0].length()-1 << "] " << result << std::endl;
        }

        std::cout << "T2 max:" << *std::max_element(results.begin(), results.end()) << std::endl;
    }
    std::cout << std::endl;

    return 0;
}