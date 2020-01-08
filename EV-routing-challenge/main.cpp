
// ------------------------------------------------------------------------- //
// The problem at hand is the standard application of the Dijkstra's algorithm.
// However, for the size of the data it could be a bit resource taxing.
// But it could be sub-optimally solved in a brute-force manner using
// the shortest path from point to point.
//
// ------------------------------------------------------------------------- //

#include <cstring>
#include <cmath>

#include "network.h"
#include "route.h"

const double initial_charge = 320;  // in km that the vehicle can travel

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        // Let's make it a bit user friendly and print the network, if requested
        // Very simple comparisson
        if (argc == 2 &&
            (std::strcmp(argv[1], "-?") == 0 || std::strcmp(argv[1], "--help") == 0 || std::strcmp(argv[1], "-h") == 0)) {
            std::cout << "List of stations:" << std::endl;
            for (auto & station : network) {
                std::cout << " " << station.name << ": " << station.lat << " " << station.lon << ":" << station.rate << std::endl;
            }
            return 0;
        }

        std::cout << "Error: requires initial and final supercharger names" << std::endl;

        return -1;
    }

    std::string initial_charger_name = argv[1];
    std::string goal_charger_name = argv[2];

    // Sanity check of the input parameters
    int found_initial = -1;
    int found_goal = -1;
    for (auto i = 0; i < network.size(); i++) {
        if (found_initial == -1 && initial_charger_name.compare(network[i].name) == 0) {
            found_initial = i;
        }
        if (found_goal == -1 && goal_charger_name.compare(network[i].name) == 0) {
            found_goal = i;
        }
    }
    if (found_initial == -1 || found_goal == -1) {
        std::cout << "Station location is invalid" << std::endl;
        std::cout << "I:" << initial_charger_name << ":" << goal_charger_name << std::endl;
        return -1;
    }

    // Find the route
    route potential_route{static_cast<std::array<row, 303>::size_type>(found_initial),
                            static_cast<std::array<row, 303>::size_type>(found_goal),
                            initial_charge};
    if (potential_route.find_dijkstra() < 0) {
        std::cout << "Algorithm didn't converge!" << std::endl;
        return -1;
    };

    std::cout << potential_route;

    return 0;
}