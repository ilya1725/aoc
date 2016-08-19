// location-file.cpp : Defines the entry point for the console application.
//

#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream> 
#include <iostream>
#include <string>

#include "location_train.h"

using namespace std;

int main(int argc, char ** argv)
{
	// Read the file name passed as the first argument
	location_train location_data;

	if (argc < 2) {
		cout << "Not enough parameters\n";
		return -1;
	}

	if (location_data.load(argv[1]) != location_train::error_code::no_error) {
		return -1;
	}

	cout << "Total: " << location_data.getCount() << endl;

    return 0;
}

