// class-hierarchy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>

using namespace std;

class A
{
public:
	A(int n = 2) : m_i(n)
	{
		cout << "Base Constructed with m_i "<<m_i<<std::endl;
	}

	virtual ~A()
	{
		cout << "Base Destructed with m_i"<<m_i<<std::endl; 
		cout << m_i;
	}

protected:
	int m_i;
};

class B : public A
{
public:
	B(int n) : m_a1(m_i + 1), m_a2(n)
	{
		cout << "Derived Constructed with m_i "<<m_i<<std::endl;
	}

	virtual ~B()
	{
		cout << "Derived Destructed with m_i"<<m_i<<std::endl; 
		cout << m_i;//2
		--m_i;
	}

private:
	A m_a1;//3
	A m_a2;//5
};

int main()
{
	{ B b(5); }
	std::cout << std::endl;
	return 0;
}