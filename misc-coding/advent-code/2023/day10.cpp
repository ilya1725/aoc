
#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <ranges>
#include <set>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

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

    constexpr bool issymbol(const char a) {
        return (a != '.' && !std::isdigit(a));
    }
    constexpr bool isgear(const char a) {
        return (a == '*');
    }

    std::vector<std::string> get_input(const std::string & path) {
        std::ifstream file_in(path);
        if (!file_in) {
            throw("Can't open input file");
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

    std::vector<std::vector<char>> convert_input(const std::vector<std::string> & input) {
        std::vector<std::vector<char>> output;

        for (const auto & line : input) {

            std::vector<char> processed_line{};
            for(const char & c : line) {
                processed_line.emplace_back(c);
            }
            output.push_back(processed_line);
        }
        return output;
    }

    std::vector<std::vector<int16_t>> convert_input(const std::vector<std::vector<char>> & input) {
        std::vector<std::vector<int16_t>> output;

        for (const auto & line : input) {
            output.push_back(std::vector<int16_t>(line.size(), -1));
        }
        return output;
    }

    std::pair<uint16_t, uint16_t> find_start(const std::vector<std::vector<char>> & input) {
        for (size_t y=0; y<input.size(); y++) {
            for(size_t x=0; x<input[0].size(); x++) {
                if (input[y][x] == 'S') {
                    return {y, x};
                }
            }
        }
        std::cout << "ERROR: S not found!" << std::endl;
        return {};
    }

    enum class dir {
        NORTH,
        SOUTH,
        EAST,
        WEST
    };

    bool traverse_tile(const std::tuple<uint16_t, uint16_t, dir> & input,
                        const std::vector<std::vector<char>> & map,
                        std::tuple<uint16_t, uint16_t, dir> & output) {

        const uint16_t y{std::get<0>(input)};
        const uint16_t x{std::get<1>(input)};
        const dir dir_in{std::get<2>(input)};

        uint16_t y_out, x_out;
        dir dir_out;

        switch (dir_in) {
            case dir::NORTH:
                if (y == 0) {
                    return false;
                }
                y_out = y-1;
                x_out = x;
            break;
            case dir::SOUTH:
                if (y == (map.size() - 1)) {
                    return false;
                }
                y_out = y+1;
                x_out = x;
            break;
            case dir::EAST:
                if (x == (map[0].size() - 1)) {
                    return false;
                }
                y_out = y;
                x_out = x+1;
            break;
            case dir::WEST:
                if (x == 0) {
                    return false;
                }
                y_out = y;
                x_out = x-1;
            break;
        }
        const auto tile{map[y_out][x_out]};

        if (tile == '.') {
            return false;
        } else if (tile == '|') {
            if (dir_in == dir::SOUTH) {
                dir_out = dir::SOUTH;
            } else if (dir_in == dir::NORTH) {
                dir_out = dir::NORTH;
            } else {
                return false;
            }
        } else if (tile == '-') {
            if (dir_in == dir::EAST) {
                dir_out = dir::EAST;
            } else if (dir_in == dir::WEST) {
                dir_out = dir::WEST;
            } else {
                return false;
            }
        } else if (tile == 'L') {
            if (dir_in == dir::SOUTH) {
                dir_out = dir::EAST;
            } else if (dir_in == dir::WEST) {
                dir_out = dir::NORTH;
            } else {
                return false;
            }
        } else if (tile == 'J') {
            if (dir_in == dir::SOUTH) {
                dir_out = dir::WEST;
            } else if (dir_in == dir::EAST) {
                dir_out = dir::NORTH;
            } else {
                return false;
            }
        } else if (tile == '7') {
            if (dir_in == dir::NORTH) {
                dir_out = dir::WEST;
            } else if (dir_in == dir::EAST) {
                dir_out = dir::SOUTH;
            } else {
                return false;
            }
        } else if (tile == 'F') {
            if (dir_in == dir::NORTH) {
                dir_out = dir::EAST;
            } else if (dir_in == dir::WEST) {
                dir_out = dir::SOUTH;
            } else {
                return false;
            }
        } else if (tile == 'S') {
            std::cout << "ERROR: S found!" << std::endl;
            return false;
        } else {
            std::cout << "ERROR: invalid character: " << tile << std::endl;
            return false;
        }

        output = {y_out, x_out, dir_out};
        return true;
    }

    // Switch internal/external flag depending on the values
    bool switch_internal(const std::vector<std::vector<char>> & map,
                        const std::pair<uint16_t, uint16_t> & input,
                        std::pair<uint16_t, uint16_t> & output) {
        output = {input.first, input.second};
        if (map[input.first][input.second] == '.') {
            return false;
        }
        if (map[input.first][input.second] == '|') {
            return true;
        }
        if (map[input.first][input.second] == 'F') {
            for (size_t x=input.second+1; x<map[0].size(); x++) {
                output = {input.first, x};
                if (map[input.first][x] == '-') {
                    continue;
                } else if (map[input.first][x] == 'J') {
                    return true;
                } else if (map[input.first][x] == '7') {
                    return false;
                } if (map[input.first][x] == '.') {
                    std::cout << "ERROR: " << map[input.first][x] << " found in [" << input.first << "][" << x << "]" << std::endl;
                    return false;
                }
            }
            return false;
        }
        if (map[input.first][input.second] == 'L') {
            for (size_t x=input.second+1; x<map[0].size(); x++) {
                output = {input.first, x};
                if (map[input.first][x] == '-') {
                    continue;
                } else if (map[input.first][x] == 'J') {
                    return false;
                } else if (map[input.first][x] == '7') {
                    return true;
                } if (map[input.first][x] == '.') {
                    std::cout << "ERROR: " << map[input.first][x] << " found in [" << input.first << "][" << x << "]" << std::endl;
                    return false;
                }
            }
            return false;
        }
        if (map[input.first][input.second] == '-') {
            std::cout << "ERROR: - found!" << std::endl;
            return false;
        }
        if (map[input.first][input.second] == '-') {
            std::cout << "ERROR: S found!" << std::endl;
            return false;
        }

        return false;
    }

    void print_map(const std::vector<std::vector<char>> & map) {
        for (size_t y=0; y<map.size(); y++) {
            for(size_t x=0; x<map[0].size(); x++) {
                std::cout << map[y][x];
            }
            std::cout << std::endl;
        }
    }

    void remove_unused(std::vector<std::vector<char>> & report, const std::vector<std::vector<int16_t>> & report_data) {
        for (size_t y=0; y<report_data.size(); y++) {
            for(size_t x=0; x<report_data[0].size(); x++) {
                if (report_data[y][x] == -1) {
                    report[y][x] = '.';
                }
            }
        }

        return;
    }

}

namespace ex {
    const std::vector<std::string>
        report{ {"..F7."},
                {".FJ|."},
                {"SJ.L7"},
                {"|F--J"},
                {"LJ..."}
                };

    const std::vector<std::string>
        report2{ {".F----7F7F7F7F-7...."},
                 {".|F--7||||||||FJ...."},
                 {".||.FJ||||||||L7...."},
                 {"FJL7L7LJLJ||LJ.L-7.."},
                 {"L--J.L7...LJS7F-7L7."},
                 {"....F-J..F7FJ|L7L7L7"},
                 {"....L7.F7||L7|.L7L7|"},
                 {".....|FJLJ|FJ|F7|.LJ"},
                 {"....FJL-7.||.||||..."},
                 {"....L---J.LJ.LJLJ..."}
        };

    const std::vector<std::string>
        report3{ {"FF7FSF7F7F7F7F7F---7"},
                 {"L|LJ||||||||||||F--J"},
                 {"FL-7LJLJ||||||LJL-77"},
                 {"F--JF--7||LJLJ7F7FJ-"},
                 {"L---JF-JLJ.||-FJLJJ7"},
                 {"|F|F-JF---7F7-L7L|7|"},
                 {"|FFJF7L7F-JF7|JL---7"},
                 {"7-L-JL7||F7|L7F-7F7|"},
                 {"L.L7LFJ|||||FJL7||LJ"},
                 {"L7JLJL-JLJLJL--JLJ.L"}
        };
}

namespace part_one {

}

namespace part_two {

}

int main(int argc, char *argv[]) {

    // Example
    {
        const auto report_ex{util::convert_input(ex::report)};
        const std::pair<uint16_t, uint16_t> start_ex{util::find_start(report_ex)};

        std::tuple<uint16_t, uint16_t, util::dir> output;

        auto report_data = util::convert_input(report_ex);
        report_data[start_ex.first][start_ex.second] = 0;
        std::tuple<uint16_t, uint16_t, util::dir> input = {start_ex.first, start_ex.second, util::dir::EAST};
        while (true) {
            const bool result{util::traverse_tile(input, report_ex, output)};
            const auto input_data{report_data[std::get<0>(input)][std::get<1>(input)]};
            if (result == false) {
                std::cout << "Done going East " << input_data << std::endl;
                break;
            } else {
                if (report_data[std::get<0>(output)][std::get<1>(output)] == 0) {
                    std::cout << "E: " << input_data + 1 << std::endl;
                    break;
                }
                report_data[std::get<0>(output)][std::get<1>(output)] = input_data + 1;
                input = output;
            }
        }

        report_data = util::convert_input(report_ex);
        report_data[start_ex.first][start_ex.second] = 0;
        input = {start_ex.first, start_ex.second, util::dir::WEST};
        while (true) {
            const bool result{util::traverse_tile(input, report_ex, output)};
            const auto input_data{report_data[std::get<0>(input)][std::get<1>(input)]};
            if (result == false) {
                std::cout << "Done going West " << input_data << std::endl;
                break;
            } else {
                if (report_data[std::get<0>(output)][std::get<1>(output)] == 0) {
                    std::cout << "W: " << input_data + 1 << std::endl;
                    break;
                }
                report_data[std::get<0>(output)][std::get<1>(output)] = input_data + 1;
                input = output;
            }
        }

        report_data = util::convert_input(report_ex);
        report_data[start_ex.first][start_ex.second] = 0;
        input = {start_ex.first, start_ex.second, util::dir::NORTH};
        while (true) {
            const bool result{util::traverse_tile(input, report_ex, output)};
            const auto input_data{report_data[std::get<0>(input)][std::get<1>(input)]};
            if (result == false) {
                std::cout << "Done going North " << input_data << std::endl;
                break;
            } else {
                if (report_data[std::get<0>(output)][std::get<1>(output)] == 0) {
                    std::cout << "N: " << input_data + 1 << std::endl;
                    break;
                }
                report_data[std::get<0>(output)][std::get<1>(output)] = input_data + 1;
                input = output;
            }
        }

        report_data = util::convert_input(report_ex);
        report_data[start_ex.first][start_ex.second] = 0;
        input = {start_ex.first, start_ex.second, util::dir::SOUTH};
        while (true) {
            const bool result{util::traverse_tile(input, report_ex, output)};
            const auto input_data{report_data[std::get<0>(input)][std::get<1>(input)]};
            if (result == false) {
                std::cout << "Done going South " << input_data << std::endl;
                break;
            } else {
                if (report_data[std::get<0>(output)][std::get<1>(output)] == 0) {
                    std::cout << "S: " << input_data + 1 << std::endl;
                    break;
                }
                report_data[std::get<0>(output)][std::get<1>(output)] = input_data + 1;
                input = output;
            }
        }
    }

    {
        auto report_ex2{util::convert_input(ex::report2)};
        const std::pair<uint16_t, uint16_t> start_ex{util::find_start(report_ex2)};

        // Process input
        auto report_data = util::convert_input(report_ex2);
        report_data[start_ex.first][start_ex.second] = 0;
        std::tuple<uint16_t, uint16_t, util::dir> input = {start_ex.first, start_ex.second, util::dir::EAST};
        std::tuple<uint16_t, uint16_t, util::dir> output;
        while (true) {
            const bool result{util::traverse_tile(input, report_ex2, output)};
            const auto input_data{report_data[std::get<0>(input)][std::get<1>(input)]};
            if (result == false) {
                std::cout << "Done going East " << input_data << std::endl;
                break;
            } else {
                if (report_data[std::get<0>(output)][std::get<1>(output)] == 0) {
                    std::cout << "E: " << input_data + 1 << std::endl;
                    break;
                }
                report_data[std::get<0>(output)][std::get<1>(output)] = input_data + 1;
                input = output;
            }
        }

        report_ex2[start_ex.first][start_ex.second] = 'F';

        // remove unused from input
        util::remove_unused(report_ex2, report_data);

        uint32_t main_count{0};
        for (size_t y=0; y<report_ex2.size(); y++) {
            bool fill_start{false};
            for (size_t x=0; x<report_ex2[0].size(); x++) {
                std::pair<uint16_t, uint16_t> input{y, x};
                std::pair<uint16_t, uint16_t> output{};

                if (fill_start == true) {
                    if (report_ex2[y][x] == '.') {
                        main_count++;
                        report_ex2[y][x] = '#';
                    } else {
                        const bool switch_flag{util::switch_internal(report_ex2, input, output)};
                        x = output.second;
                        if (switch_flag) {
                            fill_start = false;
                        }
                    }
                } else {
                    const bool switch_flag{util::switch_internal(report_ex2, input, output)};
                    x = output.second;
                    if (switch_flag) {
                        fill_start = true;
                    }
                }
            }
        }
        std::cout << "Cnt: " << main_count << std::endl;
        util::print_map(report_ex2);
    }
    {
        auto report_ex2{util::convert_input(ex::report3)};
        const std::pair<uint16_t, uint16_t> start_ex{util::find_start(report_ex2)};

        // Process input
        auto report_data = util::convert_input(report_ex2);
        report_data[start_ex.first][start_ex.second] = 0;
        std::tuple<uint16_t, uint16_t, util::dir> input = {start_ex.first, start_ex.second, util::dir::SOUTH};
        std::tuple<uint16_t, uint16_t, util::dir> output;
        while (true) {
            const bool result{util::traverse_tile(input, report_ex2, output)};
            const auto input_data{report_data[std::get<0>(input)][std::get<1>(input)]};
            if (result == false) {
                std::cout << "Done going South " << input_data << std::endl;
                break;
            } else {
                if (report_data[std::get<0>(output)][std::get<1>(output)] == 0) {
                    std::cout << "S: " << input_data + 1 << std::endl;
                    break;
                }
                report_data[std::get<0>(output)][std::get<1>(output)] = input_data + 1;
                input = output;
            }
        }

        report_ex2[start_ex.first][start_ex.second] = '7';

        // remove unused from input
        util::remove_unused(report_ex2, report_data);

        uint32_t main_count{0};
        for (size_t y=0; y<report_ex2.size(); y++) {
            bool fill_start{false};
            for (size_t x=0; x<report_ex2[0].size(); x++) {
                std::pair<uint16_t, uint16_t> input{y, x};
                std::pair<uint16_t, uint16_t> output{};

                if (fill_start == true) {
                    if (report_ex2[y][x] == '.') {
                        main_count++;
                        report_ex2[y][x] = '#';
                    } else {
                        const bool switch_flag{util::switch_internal(report_ex2, input, output)};
                        x = output.second;
                        if (switch_flag) {
                            fill_start = false;
                        }
                    }
                } else {
                    const bool switch_flag{util::switch_internal(report_ex2, input, output)};
                    x = output.second;
                    if (switch_flag) {
                        fill_start = true;
                    }
                }
            }
        }
        std::cout << "Cnt: " << main_count << std::endl;
        util::print_map(report_ex2);
    }

    const auto raw_input{util::get_input("./day10.input")};
    auto report_p1{util::convert_input(raw_input)};

    // Part one
    {
        const std::pair<uint16_t, uint16_t> start_p1{util::find_start(report_p1)};

        std::tuple<uint16_t, uint16_t, util::dir> output;
        std::vector<int16_t> record_data{};

        auto report_data = util::convert_input(report_p1);
        report_data[start_p1.first][start_p1.second] = 0;
        std::tuple<uint16_t, uint16_t, util::dir> input = {start_p1.first, start_p1.second, util::dir::EAST};
        while (true) {
            const bool result{util::traverse_tile(input, report_p1, output)};
            const auto input_data{report_data[std::get<0>(input)][std::get<1>(input)]};
            if (result == false) {
                std::cout << "Done going East " << input_data << std::endl;
                record_data.push_back(input_data+1);
                break;
            } else {
                if (report_data[std::get<0>(output)][std::get<1>(output)] == 0) {
                    std::cout << "E: " << input_data + 1 << std::endl;
                    record_data.push_back(input_data+1);
                    break;
                }
                report_data[std::get<0>(output)][std::get<1>(output)] = input_data + 1;
                input = output;
            }
        }

        report_data = util::convert_input(report_p1);
        report_data[start_p1.first][start_p1.second] = 0;
        input = {start_p1.first, start_p1.second, util::dir::WEST};
        while (true) {
            const bool result{util::traverse_tile(input, report_p1, output)};
            const auto input_data{report_data[std::get<0>(input)][std::get<1>(input)]};
            if (result == false) {
                std::cout << "Done going West " << input_data << std::endl;
                record_data.push_back(input_data+1);
                break;
            } else {
                if (report_data[std::get<0>(output)][std::get<1>(output)] == 0) {
                    std::cout << "W: " << input_data + 1 << std::endl;
                    record_data.push_back(input_data+1);
                    break;
                }
                report_data[std::get<0>(output)][std::get<1>(output)] = input_data + 1;
                input = output;
            }
        }

        report_data = util::convert_input(report_p1);
        report_data[start_p1.first][start_p1.second] = 0;
        input = {start_p1.first, start_p1.second, util::dir::NORTH};
        while (true) {
            const bool result{util::traverse_tile(input, report_p1, output)};
            const auto input_data{report_data[std::get<0>(input)][std::get<1>(input)]};
            if (result == false) {
                std::cout << "Done going North " << input_data << std::endl;
                record_data.push_back(input_data+1);
                break;
            } else {
                if (report_data[std::get<0>(output)][std::get<1>(output)] == 0) {
                    std::cout << "N: " << input_data + 1 << std::endl;
                    record_data.push_back(input_data+1);
                    break;
                }
                report_data[std::get<0>(output)][std::get<1>(output)] = input_data + 1;
                input = output;
            }
        }

        report_data = util::convert_input(report_p1);
        report_data[start_p1.first][start_p1.second] = 0;
        input = {start_p1.first, start_p1.second, util::dir::SOUTH};
        while (true) {
            const bool result{util::traverse_tile(input, report_p1, output)};
            const auto input_data{report_data[std::get<0>(input)][std::get<1>(input)]};
            if (result == false) {
                std::cout << "Done going South " << input_data << std::endl;
                record_data.push_back(input_data+1);
                break;
            } else {
                if (report_data[std::get<0>(output)][std::get<1>(output)] == 0) {
                    std::cout << "S: " << input_data + 1 << std::endl;
                    record_data.push_back(input_data+1);
                    break;
                }
                report_data[std::get<0>(output)][std::get<1>(output)] = input_data + 1;
                input = output;
            }
        }
        const auto max_data{std::max_element(record_data.begin(), record_data.end())};
        std::cout << "Output: " << *max_data << std::endl;

    }

    // Example.p2
    {
    }

    // Part two
    {
        const std::pair<uint16_t, uint16_t> start_ex{util::find_start(report_p1)};

        // Process input
        auto report_data = util::convert_input(report_p1);
        report_data[start_ex.first][start_ex.second] = 0;
        std::tuple<uint16_t, uint16_t, util::dir> input = {start_ex.first, start_ex.second, util::dir::SOUTH};
        std::tuple<uint16_t, uint16_t, util::dir> output;
        while (true) {
            const bool result{util::traverse_tile(input, report_p1, output)};
            const auto input_data{report_data[std::get<0>(input)][std::get<1>(input)]};
            if (result == false) {
                std::cout << "Done going South " << input_data << std::endl;
                break;
            } else {
                if (report_data[std::get<0>(output)][std::get<1>(output)] == 0) {
                    std::cout << "S: " << input_data + 1 << std::endl;
                    break;
                }
                report_data[std::get<0>(output)][std::get<1>(output)] = input_data + 1;
                input = output;
            }
        }

        report_p1[start_ex.first][start_ex.second] = '7';

        // remove unused from input
        util::remove_unused(report_p1, report_data);

        uint32_t main_count{0};
        for (size_t y=0; y<report_p1.size(); y++) {
            bool fill_start{false};
            for (size_t x=0; x<report_p1[0].size(); x++) {
                std::pair<uint16_t, uint16_t> input{y, x};
                std::pair<uint16_t, uint16_t> output{};

                if (fill_start == true) {
                    if (report_p1[y][x] == '.') {
                        main_count++;
                        report_p1[y][x] = '#';
                    } else {
                        const bool switch_flag{util::switch_internal(report_p1, input, output)};
                        x = output.second;
                        if (switch_flag) {
                            fill_start = false;
                        }
                    }
                } else {
                    const bool switch_flag{util::switch_internal(report_p1, input, output)};
                    x = output.second;
                    if (switch_flag) {
                        fill_start = true;
                    }
                }
            }
        }
        std::cout << "Cnt: " << main_count << std::endl;
        util::print_map(report_p1);
    }

    return 0;
}