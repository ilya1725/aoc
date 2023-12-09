
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

    std::vector<std::tuple<std::string, uint16_t>> convert_input(const std::vector<std::string> & input) {
        std::vector<std::tuple<std::string, uint16_t>> output;

        for (const auto & line : input) {
            const auto hand_str{util::split(line, ' ', true, std::vector<std::string>())};

            output.emplace_back(hand_str[0], std::atol(hand_str[1].c_str()));
        }
        return output;
    }

    std::vector<uint64_t> factors(uint64_t n) {
        uint64_t z{2};
        std::vector<uint64_t> output{};
        for (int i = 2; i * i <= n; i += 1 + (i > 2)) {
            while ((n % i) == 0) {
                output.push_back(i);
                n /= i;
            }
        }
        if (n != 1){
            output.push_back(n);
        }

        return output;
    }
}

namespace ex {
    const std::string direction1{"RL"};
    const std::vector<std::string>
        network1{{"AAA = (BBB, CCC)"},
                {"BBB = (DDD, EEE)"},
                {"CCC = (ZZZ, GGG)"},
                {"DDD = (DDD, DDD)"},
                {"EEE = (EEE, EEE)"},
                {"GGG = (GGG, GGG)"},
                {"ZZZ = (ZZZ, ZZZ)"}
                };

    const std::string direction2{"LLR"};
    const std::vector<std::string>
        network2{{"AAA = (BBB, BBB)"},
                 {"BBB = (AAA, ZZZ)"},
                 {"ZZZ = (ZZZ, ZZZ)"}
                };

    const std::string direction_p2{"LR"};
    const std::vector<std::string>
        network_P2{ {"11A = (11B, XXX)"},
                    {"11B = (XXX, 11Z)"},
                    {"11Z = (11B, XXX)"},
                    {"22A = (22B, XXX)"},
                    {"22B = (22C, 22C)"},
                    {"22C = (22Z, 22Z)"},
                    {"22Z = (22B, 22B)"},
                    {"XXX = (XXX, XXX)"}
                    };
}

namespace part_one {

    std::unordered_map<std::string, std::pair<std::string, std::string>> convert_map(const std::vector<std::string> & input) {
        std::unordered_map<std::string, std::pair<std::string, std::string>> output{};

        for (const auto & line : input) {
            const auto line_split{util::split(line, ' ', true, std::vector<std::string>())};
            const std::string left(line_split[2].begin() + 1, line_split[2].end() - 1);
            const std::string right(line_split[3].begin(), line_split[3].end() - 1);
            output.emplace(std::make_pair(line_split[0], std::make_pair(left, right)));
        }
        return output;
    }
}

namespace part_two {
    uint32_t str2int(const std::string & input) {
        uint32_t output{0};
        output = (static_cast<uint8_t>(input[0]) << 16 ) |
                (static_cast<uint8_t>(input[1]) << 8 ) |
                (static_cast<uint8_t>(input[2]));
        return output;
    }
    std::string int2str(const uint32_t & input) {
        std::string output{};
        output.push_back(static_cast<char>((input >> 16) & 0xFF));
        output.push_back(static_cast<char>((input >> 8) & 0xFF));
        output.push_back(static_cast<char>((input) & 0xFF));

        return output;
    }

    std::unordered_map<uint32_t, std::pair<uint32_t, uint32_t>> convert_map(const std::unordered_map<std::string, std::pair<std::string, std::string>> & input) {
        std::unordered_map<uint32_t, std::pair<uint32_t, uint32_t>> output{};

        for (const auto & [key,value] : input) {
            const auto new_key{str2int(key)};
            const auto new_left{str2int(std::get<0>(value))};
            const auto new_right{str2int(std::get<1>(value))};

            output.emplace(std::make_pair(new_key, std::make_pair(new_left, new_right)));
        }
        return output;
    }
}

int main(int argc, char *argv[]) {

    // Example
    {
        auto map_ex1{part_one::convert_map(ex::network1)};

        const std::string start{"AAA"};
        const std::string finish{"ZZZ"};
        uint32_t steps{0};
        uint32_t direction_idex{0};
        std::string step_name{start};

        while (true) {
            steps++;
            const auto & step{map_ex1[step_name]};
            if (ex::direction1[direction_idex] == 'L') {
                step_name = std::get<0>(step);
            } else {
                step_name = std::get<1>(step);
            }

            if (step_name == finish) {
                break;
            }
            direction_idex++;
            if (direction_idex == ex::direction1.length()) {
                direction_idex = 0;
            }
        }
        std::cout << "Ex1: " << steps << std::endl;

        auto map_ex2{part_one::convert_map(ex::network2)};

        steps = 0;
        direction_idex = 0;
        step_name = start;

        while (true) {
            steps++;
            const auto & step{map_ex2[step_name]};
            if (ex::direction2[direction_idex] == 'L') {
                step_name = std::get<0>(step);
            } else {
                step_name = std::get<1>(step);
            }

            if (step_name == finish) {
                break;
            }
            direction_idex++;
            if (direction_idex == ex::direction2.length()) {
                direction_idex = 0;
            }
        }
        std::cout << "Ex2: " << steps << std::endl;

    }

    const auto raw_input{util::get_input("./day8.input")};
    const std::string directions{raw_input[0]};
    auto map_p1{part_one::convert_map({raw_input.begin()+2, raw_input.end()})};

    // Part one
    {
        const std::string start{"AAA"};
        const std::string finish{"ZZZ"};
        uint32_t steps{0};
        uint32_t direction_idex{0};
        std::string step_name{start};

        while (true) {
            steps++;
            const auto & step{map_p1[step_name]};
            if (directions[direction_idex] == 'L') {
                step_name = std::get<0>(step);
            } else {
                step_name = std::get<1>(step);
            }

            if (step_name == finish) {
                break;
            }
            direction_idex++;
            if (direction_idex == directions.length()) {
                direction_idex = 0;
                // std::cout << "Restart" << std::endl;
            }
        }
        std::cout << "P1: " << steps << std::endl;
    }

    // Part two
    {
        auto map_ex_p2{part_one::convert_map(ex::network_P2)};
        std::vector<std::string> starts{};
        for (const auto [key, value] : map_ex_p2) {
            if (key[2] == 'A') {
                std::cout << key << std::endl;
                starts.push_back(key);
            }
        }

        {
            uint32_t steps{0};
            uint32_t direction_idex{0};
            std::vector<std::string> steps_name{starts};

            std::vector<std::string> new_steps{};
            while (true) {
                steps++;
                for (const auto & step : steps_name) {
                    const auto & new_step{map_ex_p2[step]};
                    if (ex::direction_p2[direction_idex] == 'L') {
                        new_steps.push_back(std::get<0>(new_step));
                    } else {
                        new_steps.push_back(std::get<1>(new_step));
                    }
                }
                steps_name = new_steps;
                new_steps.clear();

                // all steps need to end in Z
                bool all_end{true};
                for (const auto & step : steps_name) {
                    if (step[2] != 'Z') {
                        all_end = false;
                        break;
                    }
                }
                if (all_end) {
                    break;
                }

                direction_idex++;
                if (direction_idex == ex::direction_p2.length()) {
                    direction_idex = 0;
                }
            }
            std::cout << "Ex P2: " << steps << std::endl;
        }

        starts.clear();
        for (const auto [key, value] : map_p1) {
            if (key[2] == 'A') {
                std::cout << key << std::endl;
                starts.push_back(key);
            }
        }

        if (0) {
            std::cout << "- String based code" << std::endl;
            uint32_t steps{0};
            uint32_t direction_idex{0};
            std::vector<std::string> steps_name{starts};

            std::chrono::time_point<std::chrono::high_resolution_clock> t1;
            std::chrono::time_point<std::chrono::high_resolution_clock> t2;
            std::vector<std::string> new_steps{steps_name.size()};
            while (true) {
                steps++;

                // Time profile
                if (steps % 10000000 == 0) {
                    t1 = high_resolution_clock::now();
                }

                bool left_right{true};
                if (directions[direction_idex] == 'L') {
                    left_right = true;
                } else {
                    left_right = false;
                }
                size_t index{0};
                for (const auto & step : steps_name) {
                    const auto & new_step{map_p1[step]};
                    if (left_right) {
                        new_steps[index] = std::get<0>(new_step);
                    } else {
                        new_steps[index] = std::get<1>(new_step);
                    }
                    index++;
                }
                steps_name.swap(new_steps);

                // all steps need to end in Z
                uint16_t z_end{0};
                for (const auto & step : steps_name) {
                    if (step[2] == 'Z') {
                        z_end++;
                    }
                }
                if (z_end > 2) {
                    std::cout << steps << ":" << z_end << ":";
                    for (const auto & step : steps_name) {
                        std::cout << step << " ";
                    }
                    std::cout << std::endl;
                    if (z_end == steps_name.size()) {
                        for (const auto & step : steps_name) {
                            std::cout << step << std::endl;
                        }
                        break;
                    }
                }

                direction_idex++;
                if (direction_idex == directions.length()) {
                    direction_idex = 0;
                }

                // Time profile
                if (steps % 10000000 == 0) {
                    t2 = high_resolution_clock::now();
                    const duration<double, std::milli> ms_double = t2 - t1;
                    std:: cout << "T:" << ms_double.count() << std::endl;
                }
            }
            std::cout << "P2: " << steps << std::endl;
        }

        auto map_p2{part_two::convert_map(map_p1)};
        std::vector<uint32_t> starts_int{};

        std::cout << "P2 int: " << std::endl;
        for (const auto [key, value] : map_p2) {
            if ((key & 0xFF) == static_cast<uint8_t>('A')) {
                std::cout << part_two::int2str(key) << std::endl;
                starts_int.push_back(key);
            }
        }

        if (1) {
            std::cout << "- Integer based code. Find the loop counts. Apparently ghosts travel in loops" << std::endl;
            uint32_t steps{0};
            uint32_t direction_idex{0};
            std::vector<uint32_t> steps_name{starts_int};
            std::vector<uint32_t> loop_counts = std::vector<uint32_t>(starts_int.size(), 0);
            uint8_t loop_count{0};

            std::chrono::time_point<std::chrono::high_resolution_clock> t1;
            std::chrono::time_point<std::chrono::high_resolution_clock> t2;
            std::vector<uint32_t> new_steps{steps_name};
            while (true) {
                steps++;

                // Time profile
                if (steps % 10000000 == 0) {
                    t1 = high_resolution_clock::now();
                }

                bool left_right{true};
                if (directions[direction_idex] == 'L') {
                    left_right = true;
                } else {
                    left_right = false;
                }
                size_t index{0};
                for (const auto & step : steps_name) {
                    const auto & new_step{map_p2[step]};
                    if (left_right) {
                        new_steps[index] = std::get<0>(new_step);
                    } else {
                        new_steps[index] = std::get<1>(new_step);
                    }
                    index++;
                }
                steps_name.swap(new_steps);

                // all steps need to end in Z
                uint16_t z_end{0};
                for (size_t i=0; i<steps_name.size(); i++) {
                    if ((steps_name[i] & 0xFF) == static_cast<uint8_t>('Z')) {
                        if (loop_counts[i] == 0) {
                            loop_counts[i] = steps;
                            loop_count++;
                        }
                        z_end++;
                    }
                }
                if (z_end > 0) {
                    std::cout << steps << ":" << z_end << ":";
                    for (const auto & step : steps_name) {
                        std::cout << part_two::int2str(step) << " ";
                    }
                    std::cout << std::endl;
                    if (z_end == steps_name.size()) {
                        for (const auto & step : steps_name) {
                            std::cout << part_two::int2str(step) << std::endl;
                        }
                        break;
                    }
                }
                if (loop_count == (steps_name.size())) {
                    break;
                }

                direction_idex++;
                if (direction_idex == directions.length()) {
                    direction_idex = 0;
                }

                // Time profile
                if (steps % 10000000 == 0) {
                    t2 = high_resolution_clock::now();
                    const duration<double, std::milli> ms_double = t2 - t1;
                    std:: cout << "T:" << ms_double.count() << std::endl;
                }
            }

            // counts for loops
            std::set<int> factors{};
            for (size_t i=0; i<loop_counts.size(); i++){
                const auto loop_factor{util::factors(loop_counts[i])};
                std:: cout << i << ":" << loop_counts[i] << ":";
                for (const auto f : loop_factor) {
                    std::cout << f << " ";
                    factors.insert(f);
                }
                std::cout << std::endl;
            }

            uint64_t final_value{1};
            std:: cout << "Common: ";
            for (const auto f : factors) {
                std::cout << f << " ";
                factors.insert(f);
                final_value *= f;
            }
            std::cout << ":" << final_value << std::endl;

        }

    }

    return 0;
}