
#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <queue>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

const int16_t empty_block{-1};
const int16_t end_block{-2};

void pretty_print(const std::vector<std::string> & input) {
    std::cout << std::endl;
    size_t count{0};
    for (const auto & line : input) {
        std::cout << std::format("{:5}:{}", count, line) << std::endl;
        count++;
    }
    return;
}

void pretty_print(const std::vector<int16_t> & input) {
    for (const auto & d : input) {
        if (d == empty_block) {
            std::cout << ".";
        } else if (d == end_block) {
            break;
        } else {
            std::cout << std::format("{}", d);
        }
    }
    std::cout << std::endl;
    return;
}

void pretty_print(const std::list<std::pair<uint64_t, uint8_t>> & input) {
    for (const auto & p : input) {
        std::cout << std::format("{}:{};", p.first, p.second);
    }
    std::cout << std::endl;
    return;
}

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

// Custom hash function for pair
struct PairHash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& pair) const {
        auto h1 = std::hash<T1>{}(pair.first);
        auto h2 = std::hash<T2>{}(pair.second);

        // Combining hashes
        return h1 ^ (h2 << 1);
    }
};

namespace part_one {
    const std::vector<std::string> input_ex1{"AAAA",
                                            "BBCD",
                                            "BBCC",
                                            "EEEC"};

    const std::vector<std::string> input_ex2{"OOOOO",
                                            "OXOXO",
                                            "OOOOO",
                                            "OXOXO",
                                            "OOOOO"};

    const std::vector<std::string> input_ex3{"RRRRIICCFF",
                                            "RRRRIICCCF",
                                            "VVRRRCCFFF",
                                            "VVRCCCJFFF",
                                            "VVVVCJJCFE",
                                            "VVIVCCJJEE",
                                            "VVIIICJJEE",
                                            "MIIIIIJJEE",
                                            "MIIISIJEEE",
                                            "MMMISSJEEE"};

    class Plot {
      public:

        Plot(const std::pair<uint8_t, uint8_t> & start) : start_(start) {
            plot_area_set.clear();
        };
        virtual ~Plot() {};

        std::unordered_set<std::pair<uint8_t, uint8_t>, PairHash> FindPlot(const std::vector<std::string> & full_map) {
            // A set that contains new starting points
            std::unordered_set<std::pair<uint8_t, uint8_t>, PairHash> start_set{};
            std::unordered_set<std::pair<uint8_t, uint8_t>, PairHash> unprocessed_tiles_set;

            // X - columns, Y - rows
            const auto max_X{full_map[0].length()-1};
            const auto max_Y{full_map.size()-1};
            const auto tile_letter{full_map[start_.second][start_.first]};

            unprocessed_tiles_set.insert(start_);
            while (unprocessed_tiles_set.size() != 0) {
                auto it = unprocessed_tiles_set.begin();
                const auto current_tile{*it};
                unprocessed_tiles_set.erase(it);

                // 4 corners from the current
                //  ← Left
                if (current_tile.first > 0) {
                    std::pair<uint8_t, uint8_t> next_tile{current_tile.first-1,current_tile.second};
                    if (full_map[next_tile.second][next_tile.first] == tile_letter) {
                        if (plot_area_set.count(next_tile) == 0) {
                            unprocessed_tiles_set.insert(next_tile);
                        }
                    } else {
                        start_set.insert(next_tile);
                    }
                }
                // → Right
                if (current_tile.first < max_X) {
                    std::pair<uint8_t, uint8_t> next_tile{current_tile.first+1,current_tile.second};
                    if (full_map[current_tile.second][current_tile.first+1] == tile_letter) {
                        if (plot_area_set.count(next_tile) == 0) {
                            unprocessed_tiles_set.insert(next_tile);
                        }
                    } else {
                        start_set.insert(next_tile);
                    }
                }
                // ↑ Top
                if (current_tile.second > 0) {
                    std::pair<uint8_t, uint8_t> next_tile{current_tile.first,current_tile.second-1};
                    if (full_map[next_tile.second][next_tile.first] == tile_letter) {
                        if (plot_area_set.count(next_tile) == 0) {
                            unprocessed_tiles_set.insert(next_tile);
                        }
                    } else {
                        start_set.insert(next_tile);
                    }
                }
                // ↓ Bottom
                if (current_tile.second < max_Y) {
                    std::pair<uint8_t, uint8_t> next_tile{current_tile.first,current_tile.second+1};
                    if (full_map[next_tile.second][next_tile.first] == tile_letter) {
                        if (plot_area_set.count(next_tile) == 0) {
                            unprocessed_tiles_set.insert(next_tile);
                        }
                    } else {
                        start_set.insert(next_tile);
                    }
                }
                plot_area_set.insert(current_tile);
            }

            return start_set;
        }

        const std::unordered_set<std::pair<uint8_t, uint8_t>, PairHash> & GetPlotAreaSet() const { return plot_area_set; }
        const std::pair<uint8_t, uint8_t> GetStart() const { return start_; }

        uint32_t GetPlotArea() const { return plot_area_set.size(); }
        uint32_t GetPlotPerimeter() {
            if (perimeter_ > 0) {
                return perimeter_;
            }
            // Go over the plots. Find which ones have no neighbors
            uint32_t perimeter{0};
            for (auto plot : plot_area_set) {
                // left-right
                {
                    std::pair<uint8_t, uint8_t> top{plot.first-1, plot.second};
                    if (plot_area_set.find(top) == plot_area_set.end()) {
                        perimeter++;
                    }

                    std::pair<uint8_t, uint8_t> bottom{plot.first+1, plot.second};
                    if (plot_area_set.find(bottom) == plot_area_set.end()) {
                        perimeter++;
                    }
                }

                // top-bottom
                {
                    std::pair<uint8_t, uint8_t> left{plot.first, plot.second-1};
                    if (plot_area_set.find(left) == plot_area_set.end()) {
                        perimeter++;
                    }

                    std::pair<uint8_t, uint8_t> right{plot.first, plot.second+1};
                    if (plot_area_set.find(right) == plot_area_set.end()) {
                        perimeter++;
                    }
                }
            }
            perimeter_ = perimeter;
            return perimeter_;
        }
        uint32_t GetPlotSides() {
            if (sides_ > 0) {
                return sides_;
            }
            // Go over the plots. Find which ones have no neighbors
            uint32_t sides{0};
            for (auto plot : plot_area_set) {
                // left-right
                {
                    std::pair<uint8_t, uint8_t> top{plot.first-1, plot.second};
                    if (plot_area_set.find(top) == plot_area_set.end()) {
                        vertical_border_l_[plot.first].emplace_back(plot.second);
                    }

                    std::pair<uint8_t, uint8_t> bottom{plot.first+1, plot.second};
                    if (plot_area_set.find(bottom) == plot_area_set.end()) {
                        vertical_border_r_[plot.first].emplace_back(plot.second);
                    }
                }

                // top-bottom
                {
                    std::pair<uint8_t, uint8_t> left{plot.first, plot.second-1};
                    if (plot_area_set.find(left) == plot_area_set.end()) {
                        horizontal_border_t_[plot.second].emplace_back(plot.first);
                    }

                    std::pair<uint8_t, uint8_t> right{plot.first, plot.second+1};
                    if (plot_area_set.find(right) == plot_area_set.end()) {
                        horizontal_border_b_[plot.second].emplace_back(plot.first);
                    }
                }
            }

            // sort the maps
            for (auto & pair : vertical_border_l_) {
                std::vector<uint8_t> & vec = pair.second;

                // Sort the vector
                std::sort(vec.begin(), vec.end());
            }
            for (auto & pair : vertical_border_r_) {
                std::vector<uint8_t> & vec = pair.second;

                // Sort the vector
                std::sort(vec.begin(), vec.end());
            }

            for (auto & pair : horizontal_border_t_) {
                std::vector<uint8_t> & vec = pair.second;

                // Sort the vector
                std::sort(vec.begin(), vec.end());
            }
            for (auto & pair : horizontal_border_b_) {
                std::vector<uint8_t> & vec = pair.second;

                // Sort the vector
                std::sort(vec.begin(), vec.end());
            }

            // go over the border vectors and find gaps
            for (const auto & pair : vertical_border_l_) {
                auto local_sides = CountGaps(pair.second) + 1;
                sides += local_sides;
            }
            for (const auto & pair : vertical_border_r_) {
                auto local_sides = CountGaps(pair.second) + 1;
                sides += local_sides;
            }
            for (const auto & pair : horizontal_border_t_) {
                auto local_sides = CountGaps(pair.second) + 1;
                sides += local_sides;
            }
            for (const auto & pair : horizontal_border_b_) {
                auto local_sides = CountGaps(pair.second) + 1;
                sides += local_sides;
            }

            sides_ = sides;
            return sides_;
        }

      private:
        std::unordered_set<std::pair<uint8_t, uint8_t>, PairHash> plot_area_set;

        std::unordered_map<uint8_t, std::vector<uint8_t>> vertical_border_l_;
        std::unordered_map<uint8_t, std::vector<uint8_t>> vertical_border_r_;
        std::unordered_map<uint8_t, std::vector<uint8_t>> horizontal_border_t_;
        std::unordered_map<uint8_t, std::vector<uint8_t>> horizontal_border_b_;

        // X, Y
        std::pair<uint8_t, uint8_t> start_;
        uint32_t perimeter_{0};
        uint32_t sides_{0};

        // Sorted input vectors expected
        uint8_t CountGaps(const std::vector<uint8_t> & input) {
            uint8_t result{0};
            if (input.size() <= 1) {
                return result;
            }

            auto prev_value{input[0]};
            for (size_t i=1; i<input.size(); i++) {
                // The steps w/o gaps are only 1
                if (input[i] > (prev_value+1)) {
                    result++;
                }
                prev_value = input[i];
            }
            return result;
        }
    };

    bool find_plots(const std::vector<std::string> & full_map,
                    std::vector<Plot> & plots) {
        bool result{false};
        std::unordered_set<std::pair<uint8_t, uint8_t>, PairHash> starts_set{{0, 0}};
        std::unordered_set<std::pair<uint8_t, uint8_t>, PairHash> processed_set{};

        plots.clear();
        while (starts_set.size() != 0) {
            auto it = starts_set.begin();   // Iterator pointing to the beginning element
            const auto start_place{*it};    // Get the value
            starts_set.erase(it);           // Remove the beginning element

            Plot working_plot(start_place);
            const auto found_start_set = working_plot.FindPlot(full_map);
            plots.push_back(working_plot);

            // Get the found plot and add to processed tiles. Also remove from starting tiles if possible
            for (const auto tile : working_plot.GetPlotAreaSet()) {
                processed_set.insert(tile);

                auto it = starts_set.find(tile);
                if (it != starts_set.end()) {
                    starts_set.erase(it);
                }
            }

            // Add all found starts to the future processing if they weren't processed yet
            for (const auto start_tile : found_start_set) {
                if (processed_set.count(start_tile) == 0) {
                    starts_set.insert(start_tile);
                }
            }
        }

        return result;
    };

    // Global map containers
    std::vector<std::string> full_map;

    std::vector<Plot> plots;
}

namespace part_two {
    const std::vector<std::string> input_ex1{"EEEEE",
                                            "EXXXX",
                                            "EEEEE",
                                            "EXXXX",
                                            "EEEEE"};
    const std::vector<std::string> input_ex2{"AAAAAA",
                                            "AAABBA",
                                            "AAABBA",
                                            "ABBAAA",
                                            "ABBAAA",
                                            "AAAAAA"};

}

int main(int argc, char* argv[]) {
    // P1
    {
        part_one::full_map = part_one::input_ex2;
        part_one::find_plots(part_one::full_map, part_one::plots);
        uint64_t result{0};
        for (auto plot : part_one::plots) {
            std::cout << part_one::full_map[plot.GetStart().second][plot.GetStart().first]
                        << ":" << plot.GetPlotArea() << "*" << plot.GetPlotPerimeter() << std::endl;
            result += plot.GetPlotArea() * plot.GetPlotPerimeter();
        }
        std::cout << "P1: " << result << std::endl;

        {
            const auto input1_p1{get_input("./day12.input")};
            part_one::find_plots(input1_p1, part_one::plots);
            uint64_t result{0};
            for (auto plot : part_one::plots) {
                std::cout << input1_p1[plot.GetStart().second][plot.GetStart().first]
                            << ":" << plot.GetPlotArea() << "*" << plot.GetPlotPerimeter() << std::endl;
                result += plot.GetPlotArea() * plot.GetPlotPerimeter();
            }
            std::cout << "P1: " << result << " Correct: 1415378" << std::endl;
        }
    }

    // P2
    {
        part_one::full_map = part_one::input_ex1;
        part_one::find_plots(part_one::full_map, part_one::plots);
        uint64_t result{0};
        for (auto plot : part_one::plots) {
            std::cout << part_one::full_map[plot.GetStart().second][plot.GetStart().first]
                        << ":" << plot.GetPlotArea() << "*" << plot.GetPlotSides() << std::endl;
            result += plot.GetPlotArea() * plot.GetPlotSides();
        }
        std::cout << "P2: " << result << std::endl;

        {
            const auto input1_p2{get_input("./day12.input")};
            part_one::find_plots(input1_p2, part_one::plots);
            uint64_t result{0};
            for (auto plot : part_one::plots) {
                std::cout << input1_p2[plot.GetStart().second][plot.GetStart().first]
                            << ":" << plot.GetPlotArea() << "*" << plot.GetPlotSides() << std::endl;
                result += plot.GetPlotArea() * plot.GetPlotSides();
            }
            std::cout << "P2: " << result << " Correct: 862714" << std::endl;
        }
    }

    return 0;
}

