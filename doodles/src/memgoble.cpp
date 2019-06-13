//============================================================================
// Name        : memgoble.cpp
// Author      : Ilya Katsnelson
// Version     :
// Copyright   : free to use
// Description : The application will continuesly allocate memory till the end
//============================================================================

#include <array>
#include <algorithm>
#include <csignal>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <cfenv>
#include <set>
#include <unordered_map>
#include <sstream>
#include <utility>
#include <thread>

#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

// Handle some of the user signals
void SignalHandler(int signal) {
  std::cout << "Got a signal " << signal << std::endl;

  // Terminate program
  std::exit(signal);
}

int main(int argc, char *argv[]) {
  using namespace std::literals::chrono_literals;

  int mem_step = 1048575;
  if (argc >= 2) {
    std::stringstream convert(argv[1]);
    if (!(convert >> mem_step))
      mem_step = 1048575; // if conversion fails, set mem_step to a default value
  }

  std::cout << "Got memory step: " << mem_step << '\n';

  // Install a signal handlers
  std::signal(SIGSEGV, SignalHandler);
  std::signal(SIGFPE, SignalHandler);
  std::signal(SIGILL, SignalHandler);

  unsigned long * data;
  const auto size = mem_step/sizeof(unsigned long);
  const auto pid = getpid();
  const std::string command = "head -1 /proc/" + std::to_string(pid) + "/statm";

  unsigned long count = 0;
  while(1) {
    data = new unsigned long[size];
    if (data == nullptr) {
      std::cout << "Error allocating!\n";
      break;
    }

    for (unsigned i=0; i<size; i++) {
      data[i] = static_cast<unsigned long>(-1);
    }
    count++;

    //std::cout << "C: " << count << " ";
    system(command.c_str());

    std::this_thread::sleep_for(500ms);
  }

  return 0;
}
