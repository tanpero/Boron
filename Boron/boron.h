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
	Boron& operator=(const Boron& b);
	Boron& operator=(signed char n);
	Boron& operator=(unsigned char n);
	Boron& operator=(signed short n);
	Boron& operator=(unsigned short n);
	Boron& operator=(signed int n);
	Boron& operator=(unsigned int n);
	Boron& operator=(signed long n);
	Boron& operator=(unsigned long n);
	Boron& operator=(signed long long n);
	Boron& operator=(unsigned long long n);
	Boron& operator=(const char* s);
	Boron& operator=(std::string s);

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

public:
	bool operator>(const Boron& rhs);
	bool operator>=(const Boron& rhs);
	bool operator<(const Boron& rhs);
	bool operator<=(const Boron& rhs);
	bool operator==(const Boron& rhs);
	bool operator!=(const Boron& rhs);

public:
	Boron pow() const;
	Boron gcd() const;
	Boron lcm() const;

public:
	std::string toString(int base = 10);
};

}


#endif // !_BORON_H_

