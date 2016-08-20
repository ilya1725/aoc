/**
 * Implenetation of the location_train class.
 */
#include "location_train.h"

location_train::location_train()
{
}


location_train::~location_train()
{
}

/**
 * Load the location data file.
 * The data is exppected in this format: 
 * each line contains coordinates of one point in <X> <Y> format.
 * The function will skip lines formatted incorrectly.
 * The function will only read the first two numbers from a line that contains more.
 */
location_train::error_code 
location_train::load(const string data_file) {
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
			}
			else if (!is.eof()) {
				// There is some more data left in the string
				cout << "Unexpected format in line '" << line << "'. Only first two values were read\n";
			}
			else {
				// add data to the vector
				m_trace.push_back(point_t{ x, y });
			}
		}
	}

	// We are done reading, just verify the status
	if (infile.eof()) {
		cout << "End of file reached successfully\n";
	}
	else if (infile.bad()) {
		cout << "I/O error while reading\n";
		return error_code::err_data_file;
	}
	else if (infile.fail()) {
		cout << "Non-integer data encountered\n";
		return error_code::err_data_file;
	}

	return error_code::no_error;
}

/**
 * Verify that all location values are in the specied range
 * ul - upper left point's coordinates
 * lr - lower right point's coordinates
 */
location_train::error_code
location_train::verify(point_t ul, point_t lr) const {
	if (!m_trace.size()) {
		return error_code::err_no_data;
	}

	for (auto p : m_trace) {
		if ((p.x < ul.x || p.x > lr.x) || (p.y > ul.y || p.y < lr.y)) {
			return error_code::err_inv_data;
		}
	}

	return error_code::no_error;
}

