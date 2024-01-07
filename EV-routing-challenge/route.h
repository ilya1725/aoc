// ------------------------------------------------------------------------- //
// Class to implement route storage and calculation.
// It will contain a vector of stations representing a route.
// All the stations are indexes to the network array.
// ------------------------------------------------------------------------- //
#pragma once

#include <array>
#include <vector>
#include <unordered_map>
#include <utility>

#include "network.h"

template <size_t size>
class route {
    // structure describing a point in the route
    struct point {
        size_t id;
        double charge_time;                     // amount of time vehicle should spend there charging
        double distance;                        // distance it had to travel to get there
        double initial_charge;                  // the amount of charge, in km, vehicle had when reaching this point
    };

    // structure describing a node in the search graph
    struct node {
        size_t id;
        double value;                           // some kind of figure of merit value for this node

        bool operator<(const node& other) const {
            return other.value < value;
        }
    };

  public:
    // NOTE: constructor presumes that the passed indexes are correct
    explicit route (const typename std::array<row, size>::size_type start,
                    const typename std::array<row, size>::size_type finish,
                    const double initial_charge) :
        m_start(start), m_finish(finish), m_initial_charge(initial_charge) {};

    static constexpr double deg2rad(const double deg);

    static double calculate_station_distance(const row & a, const row & b);
    static double calculate_station_time(const row & a, const row & b);

    const double get_route_time() const { return m_route_time; }
    const double get_full_route_time();

    // Route finding methods
    int find_fastest2full();
    int find_shortest();
    int find_random();
    int find_dijkstra();

  private:
    static constexpr double m_earth_radius{6356.752};       // km
    static constexpr double m_max_speed{105.0};             // km/h
    static constexpr double m_charge_margin{5.0};           // Extra 5 km
    static constexpr double m_max_charge{320.0};            // km

    // Since the network is fixed in a constant std::array, we can just use the
    // indexes to it to indicate points
    const typename std::array<row, size>::size_type m_start;
    const typename std::array<row, size>::size_type m_finish;
    const double m_initial_charge;
    double m_route_time{0.0};

    std::vector<point> m_points{};

    template <size_t friend_size>
    friend std::ostream& operator<<(std::ostream &strm, const route<friend_size> &a) {
        strm << network[a.m_start].name << ", ";

        for (auto & p : a.m_points) {
            strm << network[p.id].name << ", "
                << p.charge_time << ", ";
        }
        return strm << network[a.m_finish].name;
    }

    void add_point(const typename std::array<row, size>::size_type id,
                    const double charge_time,
                    const double distance,
                    const double initial_charge);
    void clear();
};
