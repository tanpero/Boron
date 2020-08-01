#ifndef _BORON_H_
#define _BORON_H_

#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <cmath>
#include <functional>

namespace boron
{


struct Division;
struct Decimal;

class Data
{
	std::vector<uint32_t> data;

	// 0 为正，1 为负
	bool POS = 0, NEG = 1;
	bool sign;

public:
	Data();
	Data(const Data& b);
	Data(int8_t n);
	Data(uint8_t n);
	Data(int16_t n);
	Data(uint16_t n);
	Data(int32_t n);
	Data(uint32_t n);
	Data(int64_t n);
	Data(uint64_t n);
	Data(const char* s, int base = 10);
	Data(std::string s, int base = 10);

	~Data();

public:
	Data& operator=(const Data& b);
	Data& operator=(int8_t n);
	Data& operator=(uint8_t n);
	Data& operator=(int16_t n);
	Data& operator=(uint16_t n);
	Data& operator=(int32_t n);
	Data& operator=(uint32_t n);
	Data& operator=(int64_t n);
	Data& operator=(uint64_t n);
	Data& operator=(const char* s);
	Data& operator=(std::string s);

public:
	void clear();
	size_t digits()                                      const;
	size_t sectionAmount()                               const;
	uint32_t sectionAt(size_t offset)                    const;
	void moditySection(size_t offset, uint32_t newValue) const;
	void modityHighestSection(size_t offset, uint32_t newValue);
	uint32_t bitAt(size_t sec, size_t offset)            const;
	uint32_t bitAt(size_t offset)                        const;
	uint32_t highestSection()                            const;
	std::vector<uint32_t> getData()                      const;
	void eachSection(std::function<bool(size_t, uint32_t&)> execution);
};

class Boron
{

	Data data;

public:

	friend Boron operator++ (Boron, int);
	friend Boron operator-- (Boron);
	friend Boron operator-- (Boron, int);
	friend Boron operator-  (Boron);

#define make_uop_decl(op) \
	friend Boron operator##op##(const Boron& lhs);

#define make_bop_decl(op) \
	friend Boron operator##op##(const Boron& lhs, const Boron& rhs);

	make_uop_decl(-)
	make_bop_decl(+)
	make_bop_decl(-)
	make_bop_decl(*)
	make_bop_decl(/)
	make_bop_decl(%)
	make_bop_decl(<<)
	make_bop_decl(>>)
	make_bop_decl(&)
	make_bop_decl(|)
	make_bop_decl(^)
	make_uop_decl(~)

	make_bop_decl(+=)
	make_bop_decl(-=)
	make_bop_decl(*=)
	make_bop_decl(/=)
	make_bop_decl(%=)
	make_bop_decl(<<=)
	make_bop_decl(>>=)
	make_bop_decl(&=)
	make_bop_decl(|=)
	make_bop_decl(^=)

	make_bop_decl(&&)
	make_bop_decl(||)

#undef make_uop_decl
#undef make_bop_decl

public:
	Division divmod(Boron rhs);
	Decimal div(Boron precision = 8);
	Boron pow(Boron rhs);
	Boron modpow(Boron exponent, Boron modular);
	std::vector<Boron> factorize(Boron n);

public:
	friend bool operator>(const Boron& lhs, const Boron& rhs);
	friend bool operator>=(const Boron& lhs, const Boron& rhs);
	friend bool operator<(const Boron& lhs, const Boron& rhs);
	friend bool operator<=(const Boron& lhs, const Boron& rhs);
	friend bool operator==(const Boron& lhs, const Boron& rhs);
	friend bool operator!=(const Boron& lhs, const Boron& rhs);

	friend Boron pow(Boron& a, Boron& b);
	friend Boron gcd(Boron& a, Boron& b);
	friend Boron lcm(Boron& a, Boron& b);
	friend Boron intSqrt(Boron& n);
	friend Boron sqrt(Boron& n, Boron& accuracy);
	friend Boron max(Boron& a, Boron& b);
	friend Boron min(Boron& a, Boron& b);


public:
	std::string toString(int base = 10)       const;
	uint32_t getUInt32()                      const;
};

// 获取数值 x 第 n 位的值
constexpr inline uint32_t get_bit(unsigned x, unsigned n)
{
	return (x >> n) & 1;
}

// 将 x 第 n 位的值设置为 a
constexpr inline uint32_t set_bit(unsigned x, unsigned n, unsigned a)
{
	return x ^= (x & (1 << n)) ^ (a << n);
}

// 求 n 的二进制位数
constexpr inline uint32_t length_of_bits(unsigned n)
{
	int32_t pow_of_2[32] =
	{
				 1,           2,           4,           8,         16,          32,
				64,         128,         256,         512,       1024,        2048,
			  4096,        8192,       16384,       32768,      65536,      131072,
			262144,      524288,     1048576,     2097152,    4194304,     8388608,
		  16777216,    33554432,    67108864,   134217728,  268435456,   536870912,
		1073741824,  2147483648
	};

	int32_t left = 0,
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

