
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

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

    bool add_gear_location(const uint8_t l, const uint8_t c, std::vector<uint16_t> & gear_locations) {
        const uint16_t coord = l << 8 | c;
        if ( std::find(gear_locations.begin(), gear_locations.end(), coord) == gear_locations.end() ) {
            gear_locations.emplace_back(coord);
        }
        return true;
    }
}

namespace part_one {

    const std::vector<std::string>
        schematic = {
                    "467..114..",
                    "...*......",
                    "..35..633.",
                    "......#...",
                    "617*......",
                    ".....+.58.",
                    "..592.....",
                    "......755.",
                    "...$.*....",
                    ".664.598.."
                    };

    std::vector<std::string> get_input(const std::string & path) {
        std::ifstream file_in(path);
        if (!file_in) {
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

    long process_schematic(const std::vector<std::string> & input) {
        long result{0};

        for (size_t l=0; l < input.size(); l++) {
            const auto & line{input[l]};
            long value{0};
            bool near_gear{false};

            std::cout << l << ":";

            for (size_t i=0; i<line.length(); i++) {

                if (line[i] == '.' or util::issymbol(line[i])) {
                    if (value != 0) {
                        std::cout << " " << value << (near_gear ? '*' : ' ');
                    }
                    if (near_gear) {
                        result += value;
                    }
                    value = 0;
                    near_gear = false;
                    continue;
                }

                // What's left is digits
                value = (line[i] - static_cast<char>('0')) + value * 10;

                // Verify what's near
                if (i > 0) {
                    if (util::issymbol(line[i-1])) {
                        near_gear = true;
                    }
                }
                if (i < (line.length() -1)) {
                    if (util::issymbol(line[i+1])) {
                        near_gear = true;
                    }
                }

                if (l > 0) {
                    if (util::issymbol(input[l-1][i])) {
                        near_gear = true;
                    }
                    if (i > 0) {
                        if (util::issymbol(input[l-1][i-1])) {
                            near_gear = true;
                        }
                    }
                    if (i < (line.length() -1)) {
                        if (util::issymbol(input[l-1][i+1])) {
                            near_gear = true;
                        }
                    }
                }
                if (l < (input.size()-1)) {
                    if (util::issymbol(input[l+1][i])) {
                        near_gear = true;
                    }
                    if (i > 0) {
                        if (util::issymbol(input[l+1][i-1])) {
                            near_gear = true;
                        }
                    }
                    if (i < (line.length() -1)) {
                        if (util::issymbol(input[l+1][i+1])) {
                            near_gear = true;
                        }
                    }
                }
            }

            if (value != 0) {
                std::cout << " " << value << (near_gear ? '*' : ' ');
            }
            if (near_gear) {
                result += value;
            }
            std::cout << std::endl;
        }
        return result;
    }

    std::unordered_map<uint16_t, std::vector<long>> process_schematic_p2(const std::vector<std::string> & input) {

        // map of pairs to vector of numbers
        // key is 16 bits of <line><column>
        std::unordered_map<uint16_t, std::vector<long>> gears{};

        for (size_t l=0; l < input.size(); l++) {
            const auto & line{input[l]};
            long value{0};
            bool near_gear{false};
            std::vector<uint16_t> gear_locations{};

            std::cout << l << ":";

            for (size_t i=0; i<line.length(); i++) {

                if (line[i] == '.' or util::issymbol(line[i])) {
                    if (value != 0) {
                        std::cout << " " << value << (near_gear ? '*' : ' ');
                    }
                    if (near_gear) {
                        for (const auto gear_coord : gear_locations) {
                            auto gear_string = gears[gear_coord];
                            gear_string.emplace_back(value);
                            gears[gear_coord] = gear_string;
                        }
                    }
                    value = 0;
                    near_gear = false;
                    gear_locations.clear();
                    continue;
                }

                // What's left is digits
                value = (line[i] - static_cast<char>('0')) + value * 10;

                // Verify what's near
                if (i > 0) {
                    if (util::isgear(line[i-1])) {
                        near_gear = true;
                        util::add_gear_location(l, i-1, gear_locations);
                    }
                }
                if (i < (line.length() -1)) {
                    if (util::isgear(line[i+1])) {
                        near_gear = true;
                        util::add_gear_location(l, i+1, gear_locations);
                    }
                }

                if (l > 0) {
                    if (util::isgear(input[l-1][i])) {
                        near_gear = true;
                        util::add_gear_location(l-1, i, gear_locations);
                    }
                    if (i > 0) {
                        if (util::isgear(input[l-1][i-1])) {
                            near_gear = true;
                            util::add_gear_location(l-1, i-1, gear_locations);
                        }
                    }
                    if (i < (line.length() -1)) {
                        if (util::isgear(input[l-1][i+1])) {
                            near_gear = true;
                            util::add_gear_location(l-1, i+1, gear_locations);
                        }
                    }
                }
                if (l < (input.size()-1)) {
                    if (util::isgear(input[l+1][i])) {
                        near_gear = true;
                        util::add_gear_location(l+1, i, gear_locations);
                    }
                    if (i > 0) {
                        if (util::isgear(input[l+1][i-1])) {
                            near_gear = true;
                            util::add_gear_location(l+1, i-1, gear_locations);
                        }
                    }
                    if (i < (line.length() -1)) {
                        if (util::isgear(input[l+1][i+1])) {
                            near_gear = true;
                            util::add_gear_location(l+1, i+1, gear_locations);
                        }
                    }
                }

                if (i == (line.length() - 1)) {
                    if (near_gear) {
                        for (const auto gear_coord : gear_locations) {
                            auto gear_string = gears[gear_coord];
                            gear_string.emplace_back(value);
                            gears[gear_coord] = gear_string;
                        }
                    }
                }

            }

            if (value != 0) {
                std::cout << " " << value << (near_gear ? '*' : ' ');
            }
            std::cout << std::endl;
        }
        return gears;
    }

    long process_gears(const std::unordered_map<uint16_t, std::vector<long>> & gears) {
        long result{0};

        for (const auto [coord, list] : gears) {
            if (list.size() == 2) {
                const long gear_ratio = list[0] * list[1];
                result += gear_ratio;

                std::cout << (coord >> 8) << ":" << (coord & 0xFF) << "[" << list[0] << "," << list[1] << "]" << std::endl;
            } else {
                std::cout << (coord >> 8) << ":" << (coord & 0xFF) << "<" << list.size() << ">" << std::endl;
            }
        }
        return result;
    }
}


int main() {
    const auto result{part_one::process_schematic(part_one::schematic)};
    std::cout << "\tResult3: " << result << std::endl;

    const auto input3_p1{part_one::get_input("./day3.input")};
    const auto result3_p1{part_one::process_schematic(input3_p1)};
    std::cout << "\tResult3 p1: " << result3_p1 << std::endl;

    const auto gears1{part_one::process_schematic_p2(part_one::schematic)};
    const auto result2{part_one::process_gears(gears1)};
    std::cout << "\tResult3: " << result2 << std::endl;

    const auto gears2{part_one::process_schematic_p2(input3_p1)};
    const auto result3_p2{part_one::process_gears(gears2)};
    std::cout << "\tResult3 p2: " << result3_p2 << std::endl;

    return 0;
}