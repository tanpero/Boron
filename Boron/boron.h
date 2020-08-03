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

class SectionView
{
	std::vector<uint32_t> data;

	// 0 为正，1 为负
	bool POS = 0, NEG = 1;
	bool sign;

public:
	SectionView();
	SectionView(std::vector<uint32_t> vec);
	SectionView(SectionView& sv);
	SectionView(int8_t n);
	SectionView(uint8_t n);
	SectionView(int16_t n);
	SectionView(uint16_t n);
	SectionView(int32_t n);
	SectionView(uint32_t n);
	SectionView(int64_t n);
	SectionView(uint64_t n);
	SectionView(char* s, int base = 10);
	SectionView(std::string s, int base = 10);

	~SectionView();

public:
	SectionView& operator=( SectionView& sv);

public:
	void clear();
	void negate();
	size_t digits();
	size_t sectionAmount();
	uint32_t sectionAt(size_t offset);
	void modifySection(size_t offset, uint32_t newValue);
	void modifyHighestSection(uint32_t newValue);
	void expandSection(uint32_t sec);
	uint32_t bitAt(size_t sec, size_t offset);
	uint32_t bitAt(size_t offset);
	uint32_t highestSection();
	std::vector<uint32_t> getData();
	void eachSection(std::function<bool(size_t, uint32_t&)> execution);
};

inline SectionView construct(size_t size)
{
	return SectionView(std::vector<uint32_t>(size));
}

class Boron
{

public:
	SectionView sectionView;

public:
	Boron();
	Boron(SectionView& sv);
	Boron(Boron& b);

	Boron(int8_t n) : sectionView(n) {}
	Boron(uint8_t n) : sectionView(n) {}
	Boron(int16_t n) : sectionView(n) {}
	Boron(uint16_t n) : sectionView(n) {}
	Boron(int32_t n) : sectionView(n) {}
	Boron(uint32_t n) : sectionView(n) {}
	Boron(int64_t n) : sectionView(n) {}
	Boron(uint64_t n) : sectionView(n) {}
	Boron& operator=(Boron& b);
	~Boron();

public:
	friend Boron& operator++ (Boron&);
	friend Boron& operator++ (Boron&, int);
	friend Boron& operator-- (Boron&);
	friend Boron& operator-- (Boron&, int);

#define make_uop_decl(op) \
	friend Boron operator##op##(Boron lhs);

#define make_bop_decl(op) \
	friend Boron operator##op##(Boron lhs, Boron rhs);

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

#define make_asn_decl(op) \
	friend Boron& operator##op##(Boron lhs, Boron rhs);

	make_asn_decl(+=)
	make_asn_decl(-=)
	make_asn_decl(*=)
	make_asn_decl(/=)
	make_asn_decl(%=)
	make_asn_decl(<<=)
	make_asn_decl(>>=)
	make_asn_decl(&=)
	make_asn_decl(|=)
	make_asn_decl(^=)

	make_bop_decl(&&)
	make_bop_decl(||)

#undef make_uop_decl
#undef make_bop_decl
#undef make_asn_decl

public:

public:
	friend bool operator>(Boron lhs,  Boron rhs);
	friend bool operator>=(Boron lhs,  Boron rhs);
	friend bool operator<(Boron lhs,  Boron rhs);
	friend bool operator<=(Boron lhs,  Boron rhs);
	friend bool operator==(Boron lhs,  Boron rhs);
	friend bool operator!=(Boron lhs,  Boron rhs);

	friend std::vector<Boron> factorize(Boron& n);
	friend Boron modpow(Boron base, Boron exponent, Boron modular);
	friend Boron pow(Boron& a, Boron& b);
	friend Boron gcd(Boron& a, Boron& b);
	friend Boron lcm(Boron& a, Boron& b);
	friend Boron intSqrt(Boron& n);
	friend Boron sqrt(Boron& n, Boron& accuracy);
	friend Boron max(Boron& a, Boron& b);
	friend Boron min(Boron& a, Boron& b);


public:
	std::string toString(int base = 10)       ;
	uint32_t getUInt32()                      ;
};

// 获取数值 x 第 n 位的值
constexpr inline uint32_t get_bit(uint32_t x, uint32_t n)
{
	return (x >> n) & 1;
}

// 将 x 第 n 位的值设置为 a
constexpr inline uint32_t set_bit(uint32_t x, uint32_t n, uint32_t a)
{
	return x ^= (x & (1 << n)) ^ (a << n);
}

// 求 n 的二进制位数
constexpr inline uint32_t length_of_bits(uint32_t n)
{
	uint32_t pow_of_2[32] =
	{
				 1,           2,           4,           8,         16,          32,
				64,         128,         256,         512,       1024,        2048,
			  4096,        8192,       16384,       32768,      65536,      131072,
			262144,      524288,     1048576,     2097152,    4194304,     8388608,
		  16777216,    33554432,    67108864,   134217728,  268435456,   536870912,
		1073741824,  2147483648
	};

	uint32_t left = 0,
		right = 31;

	while (left <= right)
	{
		uint32_t mid = (left + right) / 2;
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

