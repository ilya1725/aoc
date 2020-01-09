//============================================================================
// Name        : file-search.cpp
// Author      : Ilya Katsnelson
// Version     :
// Copyright   : free to use
// Description : The application implements Dijkstra algorithm
//============================================================================

#include <algorithm>
#include <array>
#include <csignal>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <cfenv>
#include <regex>
#include <set>
#include <unordered_map>
#include <sstream>
#include <string>
#include <utility>
#include <queue>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <stdio.h>
#include <stdlib.h>

const int obstacle_code = 9;

struct point {
    int x;
    int y;
};

auto
path(std::vector<std::vector<int>> & map) {
    int distance = -1;
    if (map.size() == 0)
        return distance;
    for (auto & line : map) {
        if (line.size() == 0) {
            std::cout << "One line is zero" << std::endl;
            return -1;
        }
    }

    // Create the visited map
    std::vector<std::vector<bool>> visited;
    visited.resize(map.size(), std::vector<bool>(map[0].size(), false));

    // Create the distance map
    std::vector<std::vector<int>> dist;
    dist.resize(map.size(), std::vector<int>(map[0].size(), -1));

    std::queue<point> elements;
    elements.push({0,0});
    point obstacle{-1,-1};

    while (!elements.empty()) {
        point p = elements.front();
        elements.pop();
        visited[p.x][p.y] = true;
        std::cout << "Popped: {" << p.x << ";" << p.y << "}\n";

        if (map[p.x][p.y] == obstacle_code) {
            obstacle = {p.x, p.y};
        }
        if (p.x > 0) {
            if (map[p.x-1][p.y] != 0) {
                if (!visited[p.x-1][p.y]) {
                    elements.push({p.x-1,p.y});
                }
                if (dist[p.x-1][p.y] > dist[p.x][p.y]+1 || dist[p.x-1][p.y] == -1) {
                    dist[p.x-1][p.y] = dist[p.x][p.y]+1;
                }
            }
        }
        if (p.x < map[0].size()-1) {
            if (map[p.x+1][p.y] != 0) {
                if (!visited[p.x+1][p.y]) {
                    elements.push({p.x+1,p.y});
                }
                if (dist[p.x+1][p.y] > dist[p.x][p.y]+1 || dist[p.x+1][p.y] == -1) {
                    dist[p.x+1][p.y] = dist[p.x][p.y]+1;
                }
            }
        }
        if (p.y > 0) {
            if (map[p.x][p.y-1] != 0) {
                if (!visited[p.x][p.y-1]) {
                    elements.push({p.x,p.y-1});
                }
                if (dist[p.x][p.y-1] > dist[p.x][p.y]+1 || dist[p.x][p.y-1] == -1) {
                    dist[p.x][p.y-1] = dist[p.x][p.y]+1;
                }
            }
        }
        if (p.y < map.size()-1) {
            if (map[p.x][p.y+1] != 0) {
                if (!visited[p.x][p.y+1]) {
                    elements.push({p.x,p.y+1});
                }
                if (dist[p.x][p.y+1] > dist[p.x][p.y]+1 || dist[p.x][p.y+1] == -1) {
                    dist[p.x][p.y+1] = dist[p.x][p.y]+1;
                }
            }
        }
    }

    if (obstacle.x != -1) {
        std::cout << "Obstacle: {" << obstacle.x << ";" << obstacle.y << "}\n";
        return dist[obstacle.x][obstacle.y];
    }

    return -1;
}

int main(int argc, char *argv[]) {

    std::vector<std::vector<int>> map = {{1,1,0,1},
                                        {0,1,1,1},
                                        {0,1,0,1},
                                        {0,0,9,1},
                                        {0,1,0,0}
                                        };

    std::cout << "Path:" << path(map) << std::endl;

    return 0;
}
