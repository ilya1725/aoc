#pragma once

#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream> 
#include <iostream>
#include <string>

using namespace std;

class location_train
{
public:
	// local enum class of errors
	enum class error_code {
		no_error = 0,
		err_data_file = -1,
		err_no_data = -2,
		err_inv_data = -3
	};

	// local type describing one location point
	struct point_t {
		unsigned long x;
		unsigned long y;
	};

public:
	location_train();
	virtual ~location_train();

	error_code load(const string data_file);

	error_code verify(point_t ul, point_t lr) const;

	const size_t getCount() const { return m_trace.size(); };

private:

	vector<point_t>	m_trace;
};

