
#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
#include <variant>
#include <functional>
// #include <format>

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
}

enum class hand_type {
    five_of_kind,
    four_of_kind,
    full_house,
    three_of_kind,
    two_pair,
    one_pair,
    high_card
};

namespace part_one {

    namespace ex {
        const std::vector<std::tuple<std::string, uint16_t>>
            hands{  {"32T3K", 765},
                    {"T55J5", 684},
                    {"KK677", 28},
                    {"KTJJT", 220},
                    {"QQQJA", 483}
                    };
    }

    const std::unordered_map<char, uint8_t>
        card_values{{'A', 1},
                    {'K', 2},
                    {'Q', 3},
                    {'J', 4},
                    {'T', 5},
                    {'9', 6},
                    {'8', 7},
                    {'7', 8},
                    {'6', 9},
                    {'5', 10},
                    {'4', 11},
                    {'3', 12},
                    {'2', 13}};

    hand_type get_type(const std::string & input) {
        hand_type output{hand_type::high_card};

        // Find counts
        std::vector<uint8_t> max_count{};
        std::vector<uint8_t> skip_index{};
        for (size_t i=0; i<input.size(); i++) {
            uint8_t count{0};
            if (std::find(skip_index.begin(), skip_index.end(), i) != skip_index.end()) {
                continue;
            }
            for (size_t j=0; j<input.size(); j++) {
                if (i != j) {
                    if (input[i] == input[j]) {
                        count++;
                        skip_index.push_back(j);
                    }
                }
            }
            max_count.push_back(count);
        }
        std::sort (max_count.begin(), max_count.end(), std::greater<uint8_t>());

        if (max_count[0] == 4) {
            return hand_type::five_of_kind;
        }
        if (max_count[0] == 3) {
            return hand_type::four_of_kind;
        }
        if (max_count[0] == 2 && max_count[1] == 1) {
            return hand_type::full_house;
        }
        if (max_count[0] == 2 && max_count[1] == 0) {
            return hand_type::three_of_kind;
        }
        if (max_count[0] == 1 && max_count[1] == 1) {
            return hand_type::two_pair;
        }
        if (max_count[0] == 1 && max_count[1] == 0) {
            return hand_type::one_pair;
        }

        return output;
    }

    struct {
        // returns ​true if the first argument is less than (i.e. is ordered before) the second.
        bool operator()(const std::tuple<std::string, uint16_t, hand_type> & a, const std::tuple<std::string, uint16_t, hand_type> & b) const {

            if (std::get<2>(a) < std::get<2>(b)) {
                return false;
            } else if (std::get<2>(a) > std::get<2>(b)) {
                return true;
            }

            const std::string data_a{std::get<0>(a)};
            const std::string data_b{std::get<0>(b)};

            for (size_t i=0; i<data_a.length(); i++) {
                if (data_a[i] == data_b[i]) {
                    continue;
                } else if (card_values.at(data_a[i]) < card_values.at(data_b[i])) {
                    return false;
                } else {
                    break;
                }
            }
            return true;
        }
    } hand_sort;

    const std::vector<std::tuple<std::string, uint16_t, hand_type>> set_hand_types(const std::vector<std::tuple<std::string, uint16_t>> & input) {
        std::vector<std::tuple<std::string, uint16_t, hand_type>> output;

        for (const auto & hand : input) {
            const std::tuple<std::string, uint16_t, hand_type> output_hand{std::get<0>(hand), std::get<1>(hand), get_type(std::get<0>(hand))};
            output.push_back(output_hand);
        }
        return output;
    }

}

namespace part_two {
    const std::unordered_map<char, uint8_t>
        card_values{{'A', 1},
                    {'K', 2},
                    {'Q', 3},
                    {'T', 4},
                    {'9', 5},
                    {'8', 6},
                    {'7', 7},
                    {'6', 8},
                    {'5', 9},
                    {'4', 10},
                    {'3', 11},
                    {'2', 12},
                    {'J', 13}};

    hand_type get_type(const std::string & input) {
        hand_type output{hand_type::high_card};

        // Find counts
        std::vector<uint8_t> max_count{};
        std::vector<uint8_t> skip_index{};
        for (size_t i=0; i<input.size(); i++) {
            uint8_t count{0};
            if (std::find(skip_index.begin(), skip_index.end(), i) != skip_index.end()) {
                continue;
            }
            for (size_t j=0; j<input.size(); j++) {
                if (i != j) {
                    if (input[i] == input[j]) {
                        count++;
                        skip_index.push_back(j);
                    }
                }
            }
            max_count.push_back(count);
        }
        std::sort (max_count.begin(), max_count.end(), std::greater<uint8_t>());

        uint8_t joker_cnt{0};
        for (size_t i=0; i<input.size(); i++) {
            if (input[i] == 'J') {
                joker_cnt++;
            }
        }

        if (max_count[0] == 4) {
            output = hand_type::five_of_kind;
        } else if (max_count[0] == 3) {
            // Can have 4, 1, or 0 jokers
            if (joker_cnt == 4) {
                output =  hand_type::five_of_kind;
            } else if (joker_cnt == 1) {
                output =  hand_type::five_of_kind;
            } else {
                output =  hand_type::four_of_kind;
            }
        } else if (max_count[0] == 2 && max_count[1] == 1) {
            // Can have 3, 2, or 0 jokers
            if (joker_cnt == 3) {
                output =  hand_type::five_of_kind;
            } else if (joker_cnt == 2) {
                output =  hand_type::five_of_kind;
            } else {
                output =  hand_type::full_house;
            }
        } else if (max_count[0] == 2 && max_count[1] == 0) {
            // Can have 3, 1, or 0 jokers
            if (joker_cnt == 3) {
                output =  hand_type::four_of_kind;
            } else if (joker_cnt == 1) {
                output =  hand_type::four_of_kind;
            } else {
                output =  hand_type::three_of_kind;
            }
        } else if (max_count[0] == 1 && max_count[1] == 1) {
            // Can have 2, 1, or 0 jokers
            if (joker_cnt == 2) {
                output =  hand_type::four_of_kind;
            } else if (joker_cnt == 1) {
                output =  hand_type::full_house;
            } else {
                output =  hand_type::two_pair;
            }
        } else if (max_count[0] == 1 && max_count[1] == 0) {
            // Can have 2, 1, or 0 jokers
            if (joker_cnt == 2) {
                output =  hand_type::three_of_kind;
            } else if (joker_cnt == 1) {
                output =  hand_type::three_of_kind;
            } else {
                output =  hand_type::one_pair;
            }
        } else {
            if (joker_cnt == 1) {
                output = hand_type::one_pair;
            }
        }

        return output;
    }

    struct {
        // returns ​true if the first argument is less than (i.e. is ordered before) the second.
        bool operator()(const std::tuple<std::string, uint16_t, hand_type> & a, const std::tuple<std::string, uint16_t, hand_type> & b) const {

            if (std::get<2>(a) < std::get<2>(b)) {
                return false;
            } else if (std::get<2>(a) > std::get<2>(b)) {
                return true;
            }

            const std::string data_a{std::get<0>(a)};
            const std::string data_b{std::get<0>(b)};

            for (size_t i=0; i<data_a.length(); i++) {
                if (data_a[i] == data_b[i]) {
                    continue;
                } else if (card_values.at(data_a[i]) < card_values.at(data_b[i])) {
                    return false;
                } else {
                    break;
                }
            }
            return true;
        }
    } hand_sort;

    const std::vector<std::tuple<std::string, uint16_t, hand_type>> set_hand_types(const std::vector<std::tuple<std::string, uint16_t>> & input) {
        std::vector<std::tuple<std::string, uint16_t, hand_type>> output;

        for (const auto & hand : input) {
            const std::tuple<std::string, uint16_t, hand_type> output_hand{std::get<0>(hand), std::get<1>(hand), get_type(std::get<0>(hand))};
            output.push_back(output_hand);
        }
        return output;
    }

}

int main(int argc, char *argv[]) {

    // Example
    {
        auto hands_ex{part_one::set_hand_types(part_one::ex::hands)};
        std::sort(hands_ex.begin(), hands_ex.end(), part_one::hand_sort);
        uint32_t result{0};
        for (size_t i=0; i<hands_ex.size(); i++) {
            result += ((i+1) * std::get<1>(hands_ex[i]));
        }

        std::cout << "Result ex: " << result << std::endl;
    }

    // Part one
    const auto raw_input{util::get_input("./day7.input")};
    {
        const auto hands_input{util::convert_input(raw_input)};
        auto hands_p1{part_one::set_hand_types(hands_input)};
        std::sort(hands_p1.begin(), hands_p1.end(), part_one::hand_sort);

        uint32_t result{0};
        for (size_t i=0; i<hands_p1.size(); i++) {
            result += ((i+1) * std::get<1>(hands_p1[i]));
        }

        std::cout << "Result p1: " << result << std::endl;
    }

    // Part two
    {
        auto hands_ex{part_two::set_hand_types(part_one::ex::hands)};
        std::sort(hands_ex.begin(), hands_ex.end(), part_two::hand_sort);
        uint32_t result{0};
        for (size_t i=0; i<hands_ex.size(); i++) {
            result += ((i+1) * std::get<1>(hands_ex[i]));
        }

        std::cout << "Result ex: " << result << std::endl;
    }

    {
        const auto hands_input{util::convert_input(raw_input)};
        auto hands_p1{part_two::set_hand_types(hands_input)};
        std::sort(hands_p1.begin(), hands_p1.end(), part_two::hand_sort);

        uint32_t result{0};
        for (size_t i=0; i<hands_p1.size(); i++) {
            result += ((i+1) * std::get<1>(hands_p1[i]));
        }

        std::cout << "Result p2: " << result << std::endl;
    }

    return 0;
}