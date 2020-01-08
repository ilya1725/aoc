
// ------------------------------------------------------------------------- //
// Testing code to verify the solution to the minimal path problem
//
// ------------------------------------------------------------------------- //

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <regex>

#include "network.h"

const double initial_charge = 320;  // in km that the vehicle can travel

std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

int main(int argc, char** argv)
{
    const std::string not_converge("Algorithm didn't converge");
    const std::regex re("[+-]?([0-9]*[.])?[0-9]+");

    uint32_t test_count = 0;
    uint32_t converge_count = 0;
    uint32_t error_count = 0;
    uint32_t reference_more_count = 0;
    uint32_t reference_less_count = 0;

    // Iterate through all possible paths and verify:
    // 1. do the algorithm actually converge
    // 2. how well it converges

    // NOTE: this will run for a while. Could be multithreaded
    for (auto & start : network) {
        for (auto & finish : network) {
            // NOTE: the solution can process direction from the same place to the same place
            // but the checker crashes on it, so we'll skip it
            if (start.name.compare(finish.name) == 0) {
                continue;
            }

            std::string command = "./solution " + start.name + " " + finish.name;

            // replace ' in the name with \' to work in the shell
            command = std::regex_replace(command, std::regex("\'"), "\\'");
            std::cout << command << std::endl;
            std::string output{};
            try {
                output = exec(command.c_str());
            } catch (const std::exception &exc) {
                std::cerr << exc.what();
                break;
            }
            //std::cout << output << std::endl;
            test_count++;
            if (output.find(not_converge) == std::string::npos) {
                converge_count++;
            }

            command = "./checker_linux \"" + output + "\"";
            //std::cout << command << std::endl;
            try {
                output = exec(command.c_str());
            } catch (const std::exception &exc) {
                std::cerr << exc.what();
                break;
            }
            std::cout << output << std::endl;

            if (output.find("Error") != std::string::npos) {
                error_count++;
            }
            // Locate the cost strings
            try {
                std::smatch m;
                uint8_t line = 0;
                double reference_cost, solution_cost;

                while (std::regex_search(output, m, re)) {
                    if (line == 0) {
                        reference_cost = atof(m[0].str().c_str());
                        line++;
                    } else {
                        solution_cost = atof(m[0].str().c_str());
                        if (solution_cost < reference_cost) {
                            reference_more_count++;
                        } else {
                            reference_less_count++;
                        }
                    }
                    output = m.suffix();
                }

            } catch (std::regex_error& exc) {
                std::cerr << exc.what();
                break;
            }
        }
    }

    std::cout << "Total tests run: " << test_count << std::endl;
    std::cout << "Tests didn't converge: " << (test_count - converge_count) << std::endl;
    std::cout << "Tests failed: " << (error_count) << std::endl;
    std::cout << "Tests solution more expensive: " << reference_less_count << std::endl;
    std::cout << "Tests reference more expensive: " << reference_more_count << std::endl;

    return 0;
}