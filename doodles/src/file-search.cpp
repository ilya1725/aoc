//============================================================================
// Name        : file-search.cpp
// Author      : Ilya Katsnelson
// Version     :
// Copyright   : free to use
// Description : The application will search directory for a file pattern
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
#include <thread>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <stdio.h>
#include <stdlib.h>

// Handle some of the user signals
void SignalHandler(int signal) {
  std::cout << "Got a signal " << signal << std::endl;

  // Terminate program
  std::exit(signal);
}

int main(int argc, char *argv[]) {
  std::string dir_name{"/data/"};

  auto directory = opendir(dir_name.c_str());
  if (directory == NULL) {
    std::cout << "Cannot open directory: " << dir_name << std::endl;
    return errno;
  }

  struct dirent *ent;
  struct stat sb;

  try {
    const std::regex reg_ex("[\\w\\W_-]+_flight_data_\\d{4}-\\d{2}-\\d{2}_[0-9_]*");

    while ((ent = readdir(directory)) != NULL) {
      if (ent->d_type == DT_REG) {  // regular file
        // Find if the file matches the regex
        std::string s(ent->d_name);

        if (std::regex_match(ent->d_name, reg_ex)) {
          std::string file_name = dir_name + ent->d_name;

          if (stat(file_name.c_str(), &sb) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
          }

          std::cout << "Match: " << ent->d_name << ":" << sb.st_atime << std::endl;
        }
      }
    }
  } catch (const std::regex_error& e) {
    std::cout << "regex_error caught: " << e.what() << '\n';
    if (e.code() == std::regex_constants::error_brack) {
      std::cout << "The code was error_brack\n";
    }
  }

  closedir(directory);

  return 0;
}
