
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
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
        cards = {
                    "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53",
                    "Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19",
                    "Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1",
                    "Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83",
                    "Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36",
                    "Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11"
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

    class Card {
      public:
        Card(const std::string & input) {
            const auto split_card = util::split(input, ':', true, std::vector<std::string>());
            const auto split_parts = util::split(split_card[1], '|', true, std::vector<std::string>());

            const auto split_win_numbers = util::split(split_parts[0], ' ', true, std::vector<std::string>());
            const auto split_have_numbers = util::split(split_parts[1], ' ', true, std::vector<std::string>());

            for (const auto n : split_win_numbers) {
                win_numbers.emplace_back(std::atoi(n.c_str()));
            }
            for (const auto n : split_have_numbers) {
                have_numbers.emplace_back(std::atoi(n.c_str()));
            }
        }

        uint16_t GetPoints() {

            for (const auto h : have_numbers) {
                if ( std::find(win_numbers.begin(), win_numbers.end(), h) != win_numbers.end() ) {
                    if (points == 0) {
                        points = 1;
                    } else {
                        points *= 2;
                    }
                    win_count++;
                }
            }
            return points;
        }

        uint16_t GetWinCount() { return win_count; }

      private:
        std::vector<uint16_t> win_numbers{};
        std::vector<uint16_t> have_numbers{};

        uint16_t win_count{0};
        uint16_t points{0};
    };
}


int main() {

    std::vector<part_one::Card> cards{};
    for (const auto input : part_one::cards) {
        cards.emplace_back(part_one::Card(input));
    }

    uint32_t final_result{0};
    std::vector<uint32_t> p2_cnt (cards.size(),1);

    for (auto card : cards) {
        static uint16_t cnt = 0;
        const auto result{card.GetPoints()};
        const auto win_count{card.GetWinCount()};
        const auto card_count{p2_cnt[cnt]};
        final_result += result;
        std::cout << cnt++ << " " << result << win_count << " " << card_count << std::endl;

        // Part 2
        for (uint16_t i=0; i<card.GetWinCount(); i++) {
            p2_cnt[cnt + i] += card_count;
        }
    }
    const auto total = std::reduce(p2_cnt.begin(), p2_cnt.end(), 0);
    std::cout << "\tResult4: " << final_result << " " << total << std::endl;

    const auto input4_p1{part_one::get_input("./day4.input")};
    cards.clear();
    for (const auto input : input4_p1) {
        cards.emplace_back(part_one::Card(input));
    }

    final_result = 0;
    p2_cnt = std::vector<uint32_t>(cards.size(),1);
    for (auto card : cards) {
        static uint16_t cnt = 0;
        const auto result{card.GetPoints()};
        const auto win_count{card.GetWinCount()};
        const auto card_count{p2_cnt[cnt]};
        final_result += result;
        std::cout << cnt++ << " " << result << " " << win_count << " " << card_count << std::endl;

        // Part 2
        for (uint16_t i=0; i<win_count; i++) {
            p2_cnt[cnt + i] += card_count;
        }
    }
    const auto total2 = std::reduce(p2_cnt.begin(), p2_cnt.end(), 0);
    std::cout << "\tResult4 p1: " << final_result << " " << total2 << std::endl;

    return 0;
}