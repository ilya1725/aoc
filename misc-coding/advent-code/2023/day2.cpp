
#include <algorithm>
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

}

namespace part_one {
    const std::unordered_map<std::string, int> max = {{"red", 12},
                                                    {"green", 13},
                                                    {"blue", 14}};

    const std::vector<std::vector<std::unordered_map<std::string, int>>>
        games = {
                {{{"blue",3}, {"red", 4}},{{"red", 1}, {"green",2}, {"blue", 6},{"green", 2}}},
                {{{"blue",1}, {"green", 2}}, {{"green", 3}, {"blue", 4}, {"red", 1}}, {{"green", 1}, {"blue", 1}}},
                {{{"green", 8}, {"blue", 6}, {"red", 20}}, {{"blue", 5}, {"red", 4}, {"green", 13}}, {{"green", 5}, {"red",1}}},
                {{{"green", 1}, {"red", 3}, {"blue", 6}}, {{"green", 3}, {"red", 6}}, {{"green", 3}, {"blue", 15}, {"red", 14}}},
                {{{"red", 6}, {"blue", 1}, {"green", 3}}, {{"blue", 2}, {"red", 1}, {"green", 2}}}
                };

    std::vector<std::vector<std::unordered_map<std::string, int>>> get_input(const std::string & path) {
        std::ifstream file_in(path);
        if (!file_in) {
            std::exit(-1);
        }

        std::string line;
        std::vector<std::vector<std::unordered_map<std::string, int>>> output;
        while (std::getline(file_in, line))
        {
            std::vector<std::unordered_map<std::string, int>> game;

            const auto split_vector = util::split(line, ':', true, std::vector<std::string>());
            const auto split_sample = util::split(split_vector[1], ';', true, std::vector<std::string>());

            for (const auto & sample : split_sample) {
                const auto split_sample_data = util::split(sample, ',', true, std::vector<std::string>());

                std::unordered_map<std::string, int> sample_map;
                for (const auto color : split_sample_data) {
                    const auto color_data = util::split(color, ' ', true, std::vector<std::string>());
                    sample_map[color_data[1]] = std::atoi(color_data[0].c_str());
                }

                game.push_back(sample_map);
            }
            output.push_back(game);
        }
        return output;
    }

    std::vector<long> process_games(const std::vector<std::vector<std::unordered_map<std::string, int>>> & input) {
        std::vector<long> result{};

        for (size_t game=0; game < input.size(); game++) {
            bool can_play{true};

            for (const auto sample : input[game]) {
                for (const auto & [ key, value ] : sample) {
                    if (max.at(key) < value) {
                        // This value is higher than max - the game couldn't be played
                        can_play = false;
                        break;
                    }
                }

                if (can_play == false) {
                    break;
                }
            }

            if (can_play == true) {
                result.push_back(game+1);
            }
        }
        return result;
    }

    std::vector<long> process_games2(const std::vector<std::vector<std::unordered_map<std::string, int>>> & input) {
        std::vector<long> result{};

        for (size_t game=0; game < input.size(); game++) {
            std::unordered_map<std::string, int> max_value={{"red", 0},
                                                    {"green", 0},
                                                    {"blue", 0}};

            for (const auto sample : input[game]) {
                for (const auto & [ key, value ] : sample) {
                    if (max_value.at(key) < value) {
                        // If this value is higher than current max - that's teh value
                        max_value.at(key) = value;
                    }
                }
            }

            long power = 1;
            for (const auto & [ key, value ] : max_value) {
                power *= value;
            }

            result.push_back(power);
        }
        return result;
    }
}

namespace part_two {
}

int main() {
    const auto result{part_one::process_games(part_one::games)};
    long final1{0};
    for (auto v : result) {
        std::cout << v << std::endl;
        final1 += v;
    }
    std::cout << "Result1: " << final1 << std::endl;

    const auto input2_p1{part_one::get_input("./day2.input")};
    const auto result2_p1{part_one::process_games(input2_p1)};
    long final2_p1{0};
    for (auto v : result2_p1) {
        std::cout << v << std::endl;
        final2_p1 += v;
    }
    std::cout << "Result2 p1: " << final2_p1 << std::endl;

    // ---
    const auto result2{part_one::process_games2(part_one::games)};
    long final2{0};
    for (auto v : result2) {
        std::cout << v << std::endl;
        final2 += v;
    }
    std::cout << "Result2: " << final2 << std::endl;

    const auto result2_p2{part_one::process_games2(input2_p1)};
    long final2_p2{0};
    for (auto v : result2_p2) {
        std::cout << v << std::endl;
        final2_p2 += v;
    }
    std::cout << "Result2 p2: " << final2_p2 << std::endl;

    return 0;
}