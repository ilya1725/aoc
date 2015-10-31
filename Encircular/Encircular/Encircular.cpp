// Encircular.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// Enum class describing direction
enum class direction {
	north,
	south,
	east,
	west
};

// Enum class for a point management
class point {
private:
	direction m_direction;
	long      m_x;
	long      m_y;

public:
	point(int x = 0, int y = 0, direction d = direction::north): m_x(x), m_y(y), m_direction(d) {};
};

int main()
{
    return 0;
}

