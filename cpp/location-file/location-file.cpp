// location-file.cpp : Defines the entry point for the console application.
//

#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream> 
#include <iostream>
#include <string>

using namespace std;

class location {
public:
	// local enum class of errors
	enum class error_code {
		no_error = 0,
		err_data_file = -1
	};

public:
	location::location() {};

	virtual location::~location() {};
	
	error_code location::load(const string data_file) {
		// Open the file and process the data points
		// Normal format: X Y\n
		ifstream infile(data_file);
		if (infile.bad()) {
			cout << "File opening failed\n";
			return error_code::err_data_file;
		}

		unsigned long x, y;
		string line;
		while (getline(infile, line)) {

			if (line.length()) {
				istringstream is(line);
				is >> x >> y;

				if (is.fail()) {
					cout << "Unexpected format in line '" << line << "'. It will be skipped\n";
				} else if (!is.eof()) {
					// There is some more data left in the string
					cout << "Unexpected format in line '" << line << "'. Only first two values were read\n";
				} else {
					// add data to the vector
					m_trace.push_back(point_t{ x, y });
				}
			}
		}

		// We are done reading, just verify the status
		if (infile.bad()) {
			cout << "I/O error while reading\n";
			return error_code::err_data_file;
		} else if (infile.eof()) {
			cout << "End of file reached successfully\n";
		} else if (infile.fail()) {
			cout << "Non-integer data encountered\n";
			return error_code::err_data_file;
		}

		return error_code::no_error;
	}

private:
	struct point_t {
		unsigned long x;
		unsigned long y;
	};

	vector<point_t>	m_trace;
};

int main(int argc, char ** argv)
{
	// Read the file name passed as the first argument
	location location_data;

	if (argc < 2) {
		cout << "Not enough parameters\n";
		return -1;
	}

	if (location_data.load(argv[1]) != location::error_code::no_error) {
		return -1;
	}

    return 0;
}

