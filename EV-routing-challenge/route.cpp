// ------------------------------------------------------------------------- //
// Implementation of the route class
// ------------------------------------------------------------------------- //

#include <cmath>
#include <ctime>
#include <limits>
#include <iostream>
#include <queue>
#include <vector>

#include "route.h"

// ------------------------------------------------------------------------- //
// Convert degrees to radians
double route::deg2rad(const double deg) {
    return (deg * M_PI / 180.0);
}

// Calculate the distance on the spherical earth between two points.
// Lifted from: https://en.wikipedia.org/wiki/Great-circle_distance#Formulae
double route::calculate_station_distance(const row & a, const row & b) {
    // calculate the central angle
    const double delta_lambda = deg2rad(std::abs(a.lon - b.lon));
    double delta_sigma = std::acos(std::sin(deg2rad(a.lat))*std::sin(deg2rad(b.lat)) +
                                    std::cos(deg2rad(a.lat))*std::cos(deg2rad(b.lat))*std::cos(delta_lambda));

    //std::cout << a.name << ":" << a.lat << ":" << a.lon << "|"
    //        << b.name << ":" << b.lat << ":" << b.lon << "|" << (delta_sigma * route::m_earth_radius) << std::endl;
    return (delta_sigma * route::m_earth_radius);
}

// Utility function to return station distance in hours driven at max speed
double route::calculate_station_time(const row & a, const row & b) {
    return (calculate_station_distance(a, b) / m_max_speed);
}

// Add one more point to the route
void route::add_point(const std::array<row, 303>::size_type point_id, const double charge_time, const double distance, const double initial_charge) {
    m_points.push_back({point_id, charge_time, distance, initial_charge});
}

// Calculate the full route time from beginning to end
const double route::get_full_route_time() {
    auto previous_point = m_start;
    m_route_time = 0.0;
    for (auto & point : m_points) {
        m_route_time += (calculate_station_time(network[previous_point], network[point.id]) + point.charge_time);
        previous_point = point.id;
    }

    // add the last step to the finish
    m_route_time += (calculate_station_time(network[m_points.back().id], network[m_finish]) + m_points.back().charge_time);

    return (m_route_time);
}

// Remove all points from the route
void route::clear() {
    m_route_time = 0.0;
    m_points.clear();
}

// Find the route that will allow the shortest time to full battery closest to the finish,
// i.e. shortest time spend traveling with highest charging rate.
// Try the direct route first from the current station.
// The criterion to be closer to the finish point to avoid loops.
int route::find_fastest2full() {

    auto current_point = m_start;
    bool done_flag = false;
    std::vector<bool> visited(network.size(), false);

    // NOTE: keep in mind that the algorithm may not converge and run forever.
    // Need to figure out a way to break from it
    while (!done_flag) {
        // First check if we can get to finish directly
        double distance_finish = calculate_station_distance(network[current_point], network[m_finish]) + m_charge_margin;
        visited[current_point] = true;

        if (distance_finish <= m_max_charge) {
            // Update the previous' point's charge time to have enough to reach this point, with margin
            if (m_points.size() > 0) {
                double opt_charge_time = (distance_finish + m_charge_margin - m_points.back().initial_charge)/network[current_point].rate;
                m_points.back().charge_time = opt_charge_time;
            }

            done_flag = true;
            break;
        }

        // Loop through the network and locate a place that can get us to the full battery fastest
        auto fastest = m_start;
        double fastest_solution = std::numeric_limits<double>::max();   // goodness factor to use to find the best solution
        double fastest_charge_time = 0.0;
        double fastest_distance = 0.0;
        std::array<row, 303>::size_type fastest_id;

        for (auto i = 0; i < network.size(); i++) {
            // Skip the obvious
            if (i == m_start || i == m_finish || i == current_point || visited[i] == true) {
                continue;
            }

            const auto distance = calculate_station_distance(network[current_point], network[i]);
            const auto finish_distance = calculate_station_distance(network[i], network[m_finish]);

            // See if we can even get there
            // NOTE: Assume that the battery is always full, can be optimized later
            if (distance > m_max_charge) {
                continue;
            }
            auto time_to_full = (distance / m_max_speed) + distance / network[i].rate + finish_distance / m_max_speed;

            if (fastest_solution > time_to_full) {
                fastest_solution = time_to_full;
                fastest_charge_time = distance / network[i].rate;
                fastest_id = i;
                fastest_distance = distance;
            }
        }

        // Something bad happened and we can't get anywhere from the current point
        // NOTE: this will have to be optimized to make the algorithm converge
        if (fastest_solution == std::numeric_limits<double>::max()) {
            return -1;
        } else {
            // Initial possible travel from the start point
            double current_possible_travel = m_max_charge;

            // Update the previous' point's charge time to have enough to reach this point, with margin
            if (m_points.size() > 0) {
                double opt_charge_time = (fastest_distance + m_charge_margin - m_points.back().initial_charge)/network[current_point].rate;

                m_points.back().charge_time = opt_charge_time;
                current_possible_travel = (m_points.back().initial_charge + (network[current_point].rate * m_points.back().charge_time));
            }

            add_point(fastest_id, fastest_charge_time, fastest_distance, (current_possible_travel-fastest_distance));
            current_point = fastest_id;

            //std::cout << "F:" << fastest_id << ":" << network[fastest_id].name << ":" << fastest_charge_time << ":" << fastest_distance << std::endl;
        }
    }

    return 0;
}

// Find the route that will allow the shortest travel time closest to the finish,
// i.e. shortest time spend traveling.
// Try the direct route first from the current station.
// The criterion to be closer to the finish point to avoid loops.
int route::find_shortest() {

    auto current_point = m_start;
    bool done_flag = false;
    std::vector<bool> visited(network.size(), false);

    // NOTE: keep in mind that the algorithm may not converge and run forever.
    // Need to figure out a way to break from it
    while (!done_flag) {
        // First check if we can get to finish directly
        double distance_finish = calculate_station_distance(network[current_point], network[m_finish]) + m_charge_margin;
        visited[current_point] = true;

        if (distance_finish <= m_max_charge) {
            // Update the previous' point's charge time to have enough to reach this point, with margin
            if (m_points.size() > 0) {
                double opt_charge_time = (distance_finish + m_charge_margin - m_points.back().initial_charge)/network[current_point].rate;
                m_points.back().charge_time = opt_charge_time;
            }

            done_flag = true;
            break;
        }

        // Loop through the network and locate a place that can get us to the full battery fastest
        auto fastest = m_start;
        double fastest_distance = std::numeric_limits<double>::max();
        double fastest_solution = std::numeric_limits<double>::max();   // Goodness factor to use to find solution
        std::array<row, 303>::size_type fastest_id;

        for (auto i = 0; i < network.size(); i++) {
            // Skip the obvious
            if (i == m_start || i == m_finish || i == current_point || visited[i] == true) {
                continue;
            }

            const auto distance = calculate_station_distance(network[current_point], network[i]);
            const auto finish_distance = calculate_station_distance(network[i], network[m_finish]);

            // See if we can even get there
            // NOTE: Assume that the battery is always full, can be optimized later
            if (distance > m_max_charge) {
                continue;
            }

            if (fastest_solution > (distance + finish_distance)) {
                fastest_solution = (distance + finish_distance);
                fastest_id = i;
                fastest_distance = distance;
            }
        }

        // Something bad happened and we can't get anywhere from the current point
        // NOTE: this will have to be optimized to make the algorithm converge
        if (fastest_distance == std::numeric_limits<double>::max()) {
            return -1;
        } else {
            // Initial possible travel from the start point
            double current_possible_travel = m_max_charge;

            // Update the previous' point's charge time to have enough to reach this point, with margin
            if (m_points.size() > 0) {
                double opt_charge_time = (fastest_distance + m_charge_margin - m_points.back().initial_charge)/network[current_point].rate;

                m_points.back().charge_time = opt_charge_time;
                current_possible_travel = (m_points.back().initial_charge + (network[current_point].rate * m_points.back().charge_time));
            }

            add_point(fastest_id, 0.0, fastest_distance, (current_possible_travel-fastest_distance));
            current_point = fastest_id;
        }
    }

    return 0;
}

// Find the route using simple random selection.
// Sometimes this can be surprisingly effective for little effort. 'Can' is the keyword.
// Try the direct route first from the current station
int route::find_random() {
    auto current_point = m_start;
    bool done_flag = false;

    std::srand(std::time(nullptr));

    // NOTE: keep in mind that the algorithm may not converge and run forever.
    // Need to figure out a way to break from it
    while (!done_flag) {
        // First check if we can get to finish directly
        if (calculate_station_distance(network[current_point], network[m_finish]) <= m_initial_charge) {
            done_flag = true;
            break;
        }

        // Pick another point at random
        std::array<row, 303>::size_type random_id = std::rand() % network.size();

        // Skip the obvious
        if (random_id == m_start || random_id == m_finish || random_id == current_point) {
            continue;
        }

        auto distance = calculate_station_distance(network[current_point], network[random_id]);

        // See if we can even get there
        // NOTE: Assume that the battery is always full, can be optimized later
        if (distance > m_initial_charge) {
            continue;
        }
        auto time_to_full = (distance / m_max_speed) + (m_initial_charge - distance) / network[random_id].rate;
        auto charge_time = (m_initial_charge - distance) / network[random_id].rate;

        add_point(random_id, charge_time, distance, m_max_charge-distance);
        current_point = random_id;

        //std::cout << "F:" << random_id << ":" << network[random_id].name << ":" << charge_time << std::endl;
    }

    return 0;
}

// Use the common Dijkstra's algorithm to find the most optimal path
int route::find_dijkstra() {

    std::priority_queue<node> min_queue;
    std::vector<std::array<row, 303>::size_type> previous_id(network.size());

    // Initialize the min priority queue data structure
    for (std::array<row, 303>::size_type i = 0; i < network.size(); i++) {
        if (i == m_start) {
            min_queue.push({i, 0.0});
        } else {
            min_queue.push({i, std::numeric_limits<double>::max()});
        }
    }

    // Main Dijkstra's algorithm
    while (!min_queue.empty()) {
        auto min_node = min_queue.top();
        min_queue.pop();
        // std::cout << "D:" << min_node.id << ":" << min_queue.size() << std::endl;

        // If the minimum node is the finish, we are done
        if (min_node.id == m_finish) {
            break;
        }

        // For simplicity we concider that each point is connected to every point which is closer than max range
        // but only to the points still in the queue.
        // We will create a temporary min priority queue with new values that then will be swapped into the min_queue
        std::priority_queue<node> upd_min_queue;
        while (!min_queue.empty()) {
            auto child_node = min_queue.top();
            min_queue.pop();

            const auto distance = calculate_station_distance(network[min_node.id], network[child_node.id]);

            // We will use time as the figure of merit of a link.
            const auto value = distance/m_max_speed + distance/network[child_node.id].rate;
            const auto tot_value = value + min_node.value;
            if (distance < m_max_charge && tot_value < child_node.value) {
                child_node.value = tot_value;
                previous_id[child_node.id] = min_node.id;
            }
            upd_min_queue.push(child_node);
        }
        min_queue.swap(upd_min_queue);
    }

    // Set the route vector using the previous_id data
    auto current_point = m_finish;
    while (current_point != m_start) {
        const auto distance = calculate_station_distance(network[current_point], network[previous_id[current_point]]);
        m_points.insert(m_points.begin(), {current_point, 0.0, distance, 0.0});
        current_point = previous_id[current_point];
    }
    m_points.insert(m_points.begin(), {m_start, 0.0, 0.0, 0.0});

    // Update the route vector with proper charge times
    // Initial possible travel from the start point
    double current_possible_travel = m_max_charge;
    for (auto it = m_points.begin(); it != m_points.end(); it++) {
        if (it == m_points.begin()) {
            it->initial_charge = m_max_charge;
            continue;
        }

        it->initial_charge = ((it-1)->initial_charge + (it-1)->charge_time * network[(it-1)->id].rate - it->distance);
        if (it+1 != m_points.end()) {
            it->charge_time = ((it+1)->distance + m_charge_margin - it->initial_charge)/network[it->id].rate;
        }
    }

    // Remove the start and finish points so that the print method can work
    m_points.erase(m_points.begin());
    m_points.pop_back();

    return 0;
}

// Nicely and correctly print the route
std::ostream& operator<<(std::ostream &strm, const route &a) {
    strm << network[a.m_start].name << ", ";

    for (auto & p : a.m_points) {
        strm << network[p.id].name << ", "
             << p.charge_time << ", ";
    }
    return strm << network[a.m_finish].name;
}