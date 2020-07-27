#ifndef _BORON_H_
#define _BORON_H_

#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>

namespace boron
{


class Boron
{
	std::vector<unsigned> data;

	// 0 为正，1 为负
	bool POS = 0, NEG = 1;
	bool sign;

public:
	Boron();
	Boron(const Boron& b);
	Boron(signed char n);
	Boron(unsigned char n);
	Boron(signed short n);
	Boron(unsigned short n);
	Boron(signed int n);
	Boron(unsigned int n);
	Boron(signed long n);
	Boron(unsigned long n);
	Boron(signed long long n);
	Boron(unsigned long long n);
	Boron(const char* s, int base = 10);
	Boron(std::string s, int base = 10);

	~Boron();

public:
	Boron operator-();
	Boron operator+(const Boron& rhs);
	Boron operator-(const Boron& rhs);
	Boron operator*(const Boron& rhs);
	Boron operator/(const Boron& rhs);
	Boron operator%(const Boron& rhs);
	Boron operator<<(const Boron& rhs);
	Boron operator>>(const Boron& rhs);
	Boron operator&(const Boron& rhs);
	Boron operator|(const Boron& rhs);
	Boron operator+=(const Boron& rhs);
	Boron operator-=(const Boron& rhs);
	Boron operator*=(const Boron& rhs);
	Boron operator/=(const Boron& rhs);
	Boron operator%=(const Boron& rhs);
	Boron operator<<=(const Boron& rhs);
	Boron operator>>=(const Boron& rhs);
	Boron operator&=(const Boron& rhs);
	Boron operator|=(const Boron& rhs);
};

}


#endif // !_BORON_H_

