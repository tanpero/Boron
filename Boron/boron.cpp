#include "boron.h"

namespace boron
{

Data::Data()
{
	sign = 0;
	data = { 0 };
}

Data::Data(const Data& d)
{
	sign = d.sign;
	data = std::move(d).data;
}
              
Data::Data(int8_t n)
{
	sign = n < 0;
	data = { static_cast<unsigned>(std::abs(n)) };
}

Data::Data(uint8_t n)
{
	sign = 0;
	data = { static_cast<unsigned>(n) };
}

Data::Data(int16_t n)
{
	sign = n < 0;
	data = { static_cast<unsigned>(std::abs(n)) };
}

Data::Data(uint16_t n)
{
	sign = 0;
	data = { static_cast<unsigned>(n) };
}

Data::Data(int32_t n)
{
	sign = n < 0;
	data = { (unsigned)std::abs(n) };
}

Data::Data(uint32_t n)
{
	sign = 0;
	data = { n };
}

Data::Data(int64_t _n)
{
	sign = _n < 0;
	uint64_t n = (uint64_t)std::abs(_n);
	uint64_t criticalValue = (uint64_t)UINT_MAX;
	if (n <= criticalValue)
	{
		data = { (unsigned)n };
	}
	else
	{
		data = { (unsigned)n >> 32, (unsigned)n };
	}
}

Data::Data(uint64_t n)
{
	sign = 0;
	uint64_t criticalValue = (uint64_t)UINT_MAX;
	if (n <= criticalValue)
	{
		data = { (unsigned)n };
	}
	else
	{
		data = { (unsigned)n >> 32, (unsigned)n };
	}
}

Data::Data(const char* s, int base)
{
}

Data::Data(std::string s, int base)
{
}

Data::~Data()
{
}


Data& Data::operator=(const Data& d)
{
	this->sign = d.sign;
	this->data = d.data;
	return *this;
}


Boron operator-() const
{
	Boron b = *this;
	b.sign = !b.sign;
	return b;
}

/*
Boron Boron::operator+(const Boron& rhs) const
{
	Boron temp = *this;
	temp += rhs;
	return temp;
}
*/

/*
Boron Boron::operator+=(const Boron& rhs)
{

	const std::vector<unsigned>& lhs_data = data;
	const std::vector<unsigned>& rhs_data = rhs.data;
	std::vector<unsigned> result_data = { 0 };
	size_t lhs_size = lhs_data.size();
	size_t rhs_size = rhs_data.size();

	if (sign == rhs.sign)
	{

		for (int i = 1; i <= std::min(lhs_size, rhs_size); i += 1)
		{
			unsigned lhs_section = lhs_data.at(lhs_size - i);
			unsigned rhs_section = rhs_data.at(rhs_size - i);
			unsigned result_section = result_data.at(result_data.size() - 1);

			// 预测两 section 直接相加是否溢出
			if (UINT_MAX - lhs_section > rhs_section)
			{
				// 最大值减去 lhs 段的差大于 rhs 段，则两段相加不会溢出
				result_data.insert(result_data.begin(), lhs_section + rhs_section + result_section);
			}
			else
			{
				// 将两段的最左位清零，以完成其余位的加法
				unsigned lhs_temp = lhs_section & (~(1 << 1)),
					rhs_temp = rhs_section & (~(1 << 1)),
					result_temp = lhs_temp + rhs_temp;

				result_data.insert(result_data.begin(), result_temp);

				// 向 result 的高位段进 1 位
				result_data.insert(result_data.begin(), 1);

			}
		}

		b.sign == b.sign || POS;

	}
	else
	{
		// TODO...
	}

	b.data = result_data;
	return b;
}
*/

void Boron::clear()
{
	data = { 0 };
	sign = 0;
}

size_t Boron::digits() const
{
	size_t size = sectionAmount();
	if (size == 1)
	{
		return length_of_bits(sectionAt(0));
	}
	else
	{
		return 32 * (size - 1) + length_of_bits(sectionAt(0));
	}
}

size_t Boron::sectionAmount() const
{
	return data.size();
}

// 段的逻辑索引，而非在 vector 中的实际位置
unsigned Boron::sectionAt(size_t offset) const
{
	return data.at(sectionAmount() - 1 - offset);
}

unsigned Boron::bitAt(size_t sec, size_t offset) const
{
	return get_bit(sectionAt(sec), offset);
}

unsigned Boron::bitAt(size_t offset) const
{
	if (offset < 32)
	{
		return get_bit(sectionAt(0), offset);
	} 
	size_t restOffset = offset % 32;
	size_t sec = (offset - restOffset) / 32;
	return bitAt(sec, restOffset);
}

uint32_t Boron::highestSection() const
{
	return sectionAt(sectionAmount() - 1);
}

std::vector<unsigned> Boron::getData() const
{
	return data;
}

void Boron::eachSection(std::function<bool(size_t, uint32_t&)> execution)
{
	for (size_t i = 0, last = sectionAmount(); i < last; i += 1)
	{
		if (!execution(i, data[last - 1 - i]))
		{
			return;
		}
	}
}


std::string Boron::toString(int base) const
{
	if (data.size() == 1)
	{
		return std::to_string(data.at(0));
	}
	std::string s;
	for (size_t i = 0, length = data.size(); i < length; i += 1)
	{
		std::cout << std::to_string(data.at(i)) << "\n";
		s += std::to_string(data.at(i));
	}
	return s;
}


#define make_bop_def(op) \
	Boron operator##op##(const Boron& lhs, const Boron& rhs)

make_bop_def(<<)
{
	Boron temp = lhs;
	temp <<= rhs;
	return temp;
}

make_bop_def(<<=)
{
	// 如果左移的位数不致超过最高段
	if (length_of_bits(lhs.data[0]) > rhs)
	{

		uint32_t _rhs = rhs.getUInt32();

		// 第一步：将最高段直接左移，为下一段腾出空间
		lhs.mo

		size_t amount = lhs.sectionAmount();

		if (amount == 1)
		{
			return *this;
		}

		// 第二步：
		eachSection([&](size_t i, uint32_t& sec) -> bool
			{
				for (size_t j = 0; j < _rhs; j++)
				{
					sec = set_bit(sec, j, get_bit(data.at(i + 1), 32 - 1 - j));
				}
				if (i == amount - 2)
				{
					sec <<= rhs;
					return false;
				}
				return true;
			});
	}
	else if (rhs <= 32)
	{
		eachSection([&](size_t i, uint32_t section) -> bool{

		});
	}
}

Boron Boron::modpow(Boron exponent, Boron modular)
{
	Boron base = *this;
	if (modular == 1)
	{
		return 0;
	}
	else
	{
		Boron r = 1;
		base %= modular;
		while (exponent > 0)
		{
			if (exponent % 2 == 1)
			{
				r = (r * base) % modular;
			}
			exponent >>= 1;
			base = (base * base) % modular;
		}
		return r;
	}
}

Boron gcd(Boron& a, Boron& b)
{
	return Boron();
	
}

bool operator>(const Boron& lhs, const Boron& rhs)
{
	size_t lhsSecAmount = lhs.sectionAmount();
	size_t rhsSecAmount = rhs.sectionAmount();

	// 如果段数不同，则段数更大的显然数值更大
	if (lhsSecAmount != rhsSecAmount)
	{
		return lhsSecAmount > rhsSecAmount;
	}
	else
	{
		uint32_t lhsHSec = lhs.highestSection();
		uint32_t rhsHSec = rhs.highestSection();

		// 若最高段不同，则最高端更大的显然数值更大
		if (lhsHSec != rhsHSec)
		{
			return lhsHSec > rhsHSec;
		}

		bool is_lhsGreater = false;

		lhs.eachSection([&](size_t i, uint32_t sec) -> bool
		{
			if (sec > rhs.sectionAt(i))
			{
				is_lhsGreater = true;
				return false;
			}
			return true;
		});

		return is_lhsGreater;
	}
}

bool operator>=(const Boron& lhs, const Boron& rhs)
{
	return lhs > rhs || lhs == rhs;
}

bool operator<(const Boron& lhs, const Boron& rhs)
{
	return !(rhs >= rhs);
}

bool operator<=(const Boron& lhs, const Boron& rhs)
{
	return !(lhs > rhs);
}

bool operator==(const Boron& lhs, const Boron& rhs)
{
	bool isEqual = true;
	lhs.eachSection([&](size_t i, uint32_t sec) -> bool
		{
			if (sec != rhs.sectionAt(i))
			{
				isEqual = false;
				return false;
			}
			return true;
		});
	return isEqual;
}

bool operator!=(const Boron& lhs, const Boron& rhs)
{
	return !(lhs == rhs);
}

}
