// constructors.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

using namespace std;

class myclass
{
public:
	myclass(int x, int y)
	{
		cout << "In Constructor with arguments" << endl;
	}

	myclass()
	{
		cout << "In Constructor" << endl;
	}

	~myclass()
	{
		cout << "In Destructor" << endl;
	}

	myclass(const myclass &obj)
	{
		cout << "In Copy Constructor" << endl;
	}

	myclass &operator=(const myclass &obj)
	{
		cout << "Overloading = operator" << endl;
		return *this;
	}
};

int main()
{
	cout << "obj" << endl;
	myclass obj((myclass()));
	cout << "obj1" << endl;
	myclass obj1(myclass(2, 3));
}

