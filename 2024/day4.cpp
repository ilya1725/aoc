
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stack>
#include <string_view>
#include <tuple>
#include <vector>

namespace part_one {
    enum class Direction : u_int8_t {
        N,
        NE,
        E,
        SE,
        S,
        SW,
        W,
        NW
    };

    // Direction change values, X, Y
    // 0----► X
    // |
    // |
    // ▼
    // Y
    //
    const std::array<std::pair<int8_t, int8_t>, 8u > Increases {{
        {0, -1},    // N,
        {1, -1},    // NE,
        {1,  0},    // E,
        {1,  1},    // SE,
        {0,  1},    // S,
        {-1, 1},    // SW,
        {-1, 0},    // W,
        {-1, -1}    // NW
    }};

    const std::string target_word{"XMAS"};

    const std::vector<std::string> input = {"MMMSXXMASM",
                                            "MSAMXMSMSA",
                                            "AMXSXMAAMM",
                                            "MSAMASMSMX",
                                            "XMASAMXAMM",
                                            "XXAMMXXAMA",
                                            "SMSMSASXSS",
                                            "SAXAMASAAA",
                                            "MAMMMXMMMM",
                                            "MXMXAXMASX"
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

    bool find_word(const std::vector<std::string> & input, const std::pair<size_t, size_t> start, const std::pair<int8_t, int8_t> dir) {
        const auto word_len{target_word.size()};
        bool result {true};

        size_t curr_x{start.first};
        size_t curr_y{start.second};

        // Check if the entire word would fit within boundaries
        if (dir.first < 0 && curr_x < word_len - 1) {
            return false;
        }
        if (dir.first > 0 && curr_x + word_len > input[0].size()) {
            return false;
        }
        if (dir.second < 0 && curr_y < word_len - 1) {
            return false;
        }
        if (dir.second > 0 && curr_y + word_len > input.size()) {
            return false;
        }

        for (size_t i=0; i<word_len; i++) {
            if (input[curr_y][curr_x] != target_word[i]) {
                result = false;
                break;
            }

            curr_x += dir.first;
            curr_y += dir.second;
        }
        return result;
    }

    bool fill_word(std::vector<std::string> & input, const std::pair<size_t, size_t> start, const std::pair<int8_t, int8_t> dir) {
        const auto word_len{target_word.size()};
        bool result{true};

        size_t curr_x{start.first};
        size_t curr_y{start.second};

        // Check if the entire word would fit within boundaries
        if (dir.first < 0 && curr_x < word_len - 1) {
            return false;
        }
        if (dir.first > 0 && curr_x + word_len > input[0].size()) {
            return false;
        }
        if (dir.second < 0 && curr_y < word_len - 1) {
            return false;
        }
        if (dir.second > 0 && curr_y + word_len > input.size()) {
            return false;
        }

        for (size_t i=0; i<word_len; i++) {
            input[curr_y][curr_x] = target_word[i];

            curr_x += dir.first;
            curr_y += dir.second;
        }
        return result;
    }

    std::pair<std::vector<std::string>, long> find_string(const std::vector<std::string> & input) {
        std::vector<std::string> result{};
        long total_count{0};

        const auto height{input.size()};
        const auto width{input[0].size()};

        for (size_t y=0; y<height; y++) {
             result.emplace_back(width, '.');
        }

        for (size_t x=0; x<width; x++) {
            for (size_t y=0; y<height; y++) {
                for (const auto direction : Increases) {
                    const bool found{find_word(input, {x, y}, direction)};

                    if (found) {
                        total_count++;
                        fill_word(result, {x, y}, direction);
                    }
                }
            }
        }

        return {result, total_count};
    }

}

namespace part_two {

    // Location of the left M going clockwise
    // M M
    //  A
    // S S
    enum class Direction : u_int8_t {
        left_top,
        right_top,
        left_bottom,
        right_bottom
    };

    const u_int8_t DirCount{4};
    const std::array<std::array<int8_t, 8>, DirCount> Adjustments {{
        {-1, -1, -1, +1, +1, +1, +1, -1},   // top left
        {-1, +1, +1, +1, +1, -1, -1, -1},   // top right
        {+1, -1, -1, -1, -1, +1, +1, +1},   // bot left
        {+1, +1, +1, -1, -1, -1, -1, +1},   // bot right
    }};

    // start point leaves enough space
    bool find_x(const std::vector<std::string> & input, const std::pair<size_t, size_t> start, const u_int8_t dir) {

        size_t curr_x{start.first};
        size_t curr_y{start.second};

        if (input[curr_y][curr_x] != 'A') {
            return false;
        }

        const auto adjustments{Adjustments[dir]};

        if (input[curr_y+adjustments[0]][curr_x+adjustments[1]] == 'M' &&
            input[curr_y+adjustments[2]][curr_x+adjustments[3]] == 'M' &&
            input[curr_y+adjustments[4]][curr_x+adjustments[5]] == 'S' &&
            input[curr_y+adjustments[6]][curr_x+adjustments[7]] == 'S') {
            return true;
        }

        return false;
    }

    // start point leaves enough space
    bool fill_x(std::vector<std::string> & input, const std::pair<size_t, size_t> start, const u_int8_t dir) {
        bool result{true};

        size_t curr_x{start.first};
        size_t curr_y{start.second};

        input[curr_y][curr_x] = 'A';
        const auto adjustments{Adjustments[dir]};

        input[curr_y+adjustments[0]][curr_x+adjustments[1]] = 'M';
        input[curr_y+adjustments[2]][curr_x+adjustments[3]] = 'M';
        input[curr_y+adjustments[4]][curr_x+adjustments[5]] = 'S';
        input[curr_y+adjustments[6]][curr_x+adjustments[7]] = 'S';

        return result;
    }

    std::pair<std::vector<std::string>, long> find_x(const std::vector<std::string> & input) {
        std::vector<std::string> result{};
        long total_count{0};

        const auto height{input.size()};
        const auto width{input[0].size()};

        for (size_t y=0; y<height; y++) {
             result.emplace_back(width, '.');
        }

        for (size_t x=1; x<width-1; x++) {
            for (size_t y=1; y<height-1; y++) {
                for (u_int8_t direction=0; direction<DirCount; direction++) {
                    const bool found{find_x(input, {x, y}, direction)};

                    if (found) {
                        total_count++;
                        fill_x(result, {x, y}, direction);
                    }
                }
            }
        }

        return {result, total_count};
    }
}

int main() {
    // P1
    {
        const auto result{part_one::find_string(part_one::input)};
        std::cout << "Result1.1: " << result.second << std::endl;

        const auto input1_p1{part_one::get_input("./day4.input")};

        const auto result1{part_one::find_string(input1_p1)};
        std::cout << "Result2.1: " << result1.second << " Correct value is 2685" << std::endl;
    }

    // P2
    {
        const auto result{part_two::find_x(part_one::input)};
        std::cout << "Result1.2: " << result.second << std::endl;

        const auto input1_p2{part_one::get_input("./day4.input")};

        const auto result1{part_two::find_x(input1_p2)};
        std::cout << "Result2.2: " << result1.second << " Correct value is 2685" << std::endl;
    }

    return 0;
}