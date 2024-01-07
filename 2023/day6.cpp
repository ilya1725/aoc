
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>
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
}

namespace part_one {

    namespace ex {
        const std::vector<uint32_t>  times{7, 15, 30};
        const std::vector<uint32_t>  distance{9, 40, 200};
    }
    namespace input {
        const std::vector<uint32_t>  times{55, 82, 64, 90};
        const std::vector<uint32_t>  distance{246, 1441, 1012, 1111};
    }

    std::vector<uint32_t> HoldTimesToWin(const uint32_t time, const uint32_t distance) {
        std::vector<uint32_t> output{};

        for (size_t i=1; i<time; i++) {
            if ((i * (time-i)) > distance) {
                output.push_back(i);
            }
        }
        return output;
    }
}

namespace part_two {

    namespace ex {
        const uint32_t  times{71530};
        const uint64_t  distance{940200};
    }
    namespace input {
        const uint32_t  times{55826490};
        const uint64_t  distance{246144110121111};
    }

    uint64_t HoldTimesToWin(const uint32_t time, const uint64_t distance) {
        uint64_t output{0};
        uint64_t threshold1{0};
        uint64_t threshold2{0};

        for (size_t i=1; i<time; i++) {
            if ((i * (time-i)) > distance) {
                threshold1 = i;
                break;
            }
        }
        for (size_t i=(time-1); i>threshold1; i--) {
            if ((i * (time-i)) > distance) {
                threshold2 = i;
                break;
            }
        }
        return (threshold2 - threshold1 + 1);
    }
}

int main() {

    {
        uint32_t result_ex{1};
        for (size_t i=0; i<part_one::ex::times.size(); i++) {
            const auto data{part_one::HoldTimesToWin(part_one::ex::times[i], part_one::ex::distance[i])};

            result_ex *= data.size();
            std::cout << data.size() << " ";
        }
        std::cout << "|" << result_ex << std::endl;

        uint32_t result_p1{1};
        for (size_t i=0; i<part_one::input::times.size(); i++) {
            const auto data{part_one::HoldTimesToWin(part_one::input::times[i], part_one::input::distance[i])};

            result_p1 *= data.size();
            std::cout << data.size() << " ";
        }
        std::cout << "|" << result_p1 << std::endl;
    }

    {
        uint64_t result_ex{part_two::HoldTimesToWin(part_two::ex::times, part_two::ex::distance)};
        std::cout << "|" << result_ex << std::endl;

        uint32_t result_p2{part_two::HoldTimesToWin(part_two::input::times, part_two::input::distance)};
        std::cout << "|" << result_p2 << std::endl;
    }

    return 0;
}