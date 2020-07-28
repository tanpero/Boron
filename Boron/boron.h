#ifndef _BORON_H_
#define _BORON_H_

#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <cmath>

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
	Boron operator++ ();
	Boron operator++ (int);
	Boron operator-- ();
	Boron operator-- (int);
	Boron operator-  ()                 const;
	Boron operator+  (const Boron& rhs) const;
	Boron operator-  (const Boron& rhs) const;
	Boron operator*  (const Boron& rhs) const;
	Boron operator/  (const Boron& rhs) const;
	Boron operator%  (const Boron& rhs) const;
	Boron operator<< (const Boron& rhs) const;
	Boron operator>> (const Boron& rhs) const;
	Boron operator&  (const Boron& rhs) const;
	Boron operator|  (const Boron& rhs) const;
	Boron operator^  (const Boron& rhs) const;
	Boron operator~  ()                 const;
	Boron operator+= (const Boron& rhs);
	Boron operator-= (const Boron& rhs);
	Boron operator*= (const Boron& rhs);
	Boron operator/= (const Boron& rhs);
	Boron operator%= (const Boron& rhs);
	Boron operator<<=(const Boron& rhs);
	Boron operator>>=(const Boron& rhs);
	Boron operator&= (const Boron& rhs);
	Boron operator|= (const Boron& rhs);
	Boron operator^= (const Boron& rhs);

public:
	bool operator>(const Boron& rhs)  const;
	bool operator>=(const Boron& rhs) const;
	bool operator<(const Boron& rhs)  const;
	bool operator<=(const Boron& rhs) const;
	bool operator==(const Boron& rhs) const;
	bool operator!=(const Boron& rhs) const;

public:
	friend Boron pow(Boron& a, Boron& b);
	friend Boron gcd(Boron& a, Boron& b);
	friend Boron lcm(Boron& a, Boron& b);
	friend Boron intSqrt(Boron& n);
	friend Boron sqrt(Boron& n, Boron& accuracy);
	friend Boron max(Boron& a, Boron& b);
	friend Boron min(Boron& a, Boron& b);


public:
	void clear();
	size_t digits()                           const;
	size_t sectionAmount()                         const;
	unsigned sectionAt(size_t offset)         const;
	unsigned bitAt(size_t sec, size_t offset) const;
	unsigned bitAt(size_t offset)             const;
	std::vector<unsigned> getData()           const;

public:
	std::string toString(int base = 10);
};

// 获取数值 x 第 n 位的值
constexpr inline unsigned get_bit(unsigned x, unsigned n)
{
	return (x >> n) & 1;
}

// 将 x 第 n 位的值设置为 a
constexpr inline unsigned set_bit(unsigned x, unsigned n, unsigned a)
{
	return x ^= (x & (1 << n)) ^ (a << n);
}

// 求 n 的二进制位数
constexpr inline unsigned length_of_bits(unsigned n)
{
	int pow_of_2[32] =
	{
				 1,           2,           4,           8,         16,          32,
				64,         128,         256,         512,       1024,        2048,
			  4096,        8192,       16384,       32768,      65536,      131072,
			262144,      524288,     1048576,     2097152,    4194304,     8388608,
		  16777216,    33554432,    67108864,   134217728,  268435456,   536870912,
		1073741824,  2147483648
	};

	int left = 0,
		right = 31;

	while (left <= right)
	{
		int mid = (left + right) / 2;
		if (pow_of_2[mid] <= n)
		{
			if (pow_of_2[mid + 1] > n)
			{
				return mid + 1;
			}
			else
			{
				left = mid + 1;
			}
		}
		else
		{
			right = mid - 1;
		}
	}
}

}


#endif // !_BORON_H_

