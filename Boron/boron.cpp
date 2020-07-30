#include "boron.h"

namespace boron
{

Boron::Boron()
{
	sign = 0;
	data = { 0 };
}

Boron::Boron(const Boron& b)
{
	sign = b.sign;
	data = std::move(b).data;
}
              
Boron::Boron(int8_t n)
{
	sign = n < 0;
	data = { static_cast<unsigned>(std::abs(n)) };
}

Boron::Boron(uint8_t n)
{
	sign = 0;
	data = { static_cast<unsigned>(n) };
}

Boron::Boron(int16_t n)
{
	sign = n < 0;
	data = { static_cast<unsigned>(std::abs(n)) };
}

Boron::Boron(uint16_t n)
{
	sign = 0;
	data = { static_cast<unsigned>(n) };
}

Boron::Boron(int32_t n)
{
	sign = n < 0;
	data = { (unsigned)std::abs(n) };
}

Boron::Boron(uint32_t n)
{
	sign = 0;
	data = { n };
}

Boron::Boron(int64_t _n)
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

Boron::Boron(uint64_t n)
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

Boron::Boron(const char* s, int base)
{
}

Boron::Boron(std::string s, int base)
{
}

Boron::~Boron()
{
}


Boron& Boron::operator=(const Boron& b)
{
	this->sign = b.sign;
	this->data = b.data;
	return *this;
}


#define make_assignment(type)     \
Boron& Boron::operator=(type n)   \
{                                 \
	*this = std::move(Boron(n));  \
	return *this;                 \
} 

make_assignment(int8_t)
make_assignment(uint8_t)
make_assignment(int16_t)
make_assignment(uint16_t)
make_assignment(int32_t)
make_assignment(uint32_t)
make_assignment(int64_t)
make_assignment(uint64_t)
make_assignment(int64_t)
make_assignment(uint64_t)

#undef make_assignment

Boron Boron::operator-() const
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

unsigned Boron::sectionAt(size_t offset) const
{
	return data.at(offset);
}

unsigned Boron::bitAt(size_t sec, size_t offset) const
{
	return get_bit(sectionAt(sectionAmount() - 1 - sec), offset);
}

unsigned Boron::bitAt(size_t offset) const
{
	if (offset < 32)
	{
		return get_bit(sectionAt(sectionAmount() - 1), offset);
	}
	size_t restOffset = offset % 32;
	size_t sec = (offset - restOffset) / 32;
	return bitAt(sec, restOffset);
}

std::vector<unsigned> Boron::getData() const
{
	return data;
}

void Boron::eachSection(std::function<bool(size_t&, uint32_t)> execution) const
{
	for (size_t i = 0, last = sectionAmount(); i < last; i += 1)
	{
		size_t cursor = last - 1 - i;
		if (!execution(i, sectionAt(cursor)))
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

Boron Boron::operator<<(const Boron& rhs) const
{
	Boron temp = *this;
	temp <<= rhs;
	return temp;
}

Boron Boron::operator<<=(const Boron& rhs)
{
	// 如果左移的位数不致超过最高段
	if (length_of_bits(data[0]) > rhs)
	{
		data[0] <<= rhs.getUInt32();
	}
	else if (rhs <= 32)
	{
		eachSection([&](size_t i, uint32_t section) -> bool{

		});
	}
}

}
