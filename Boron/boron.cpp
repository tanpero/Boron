#include "boron.h"

namespace boron
{

SectionView::SectionView()
{
	sign = 0;
	data = { 0 };
}

SectionView::SectionView( SectionView& sv)
{
	sign = sv.sign;
	data = std::move(sv).data;
}
              
SectionView::SectionView(int8_t n)
{
	sign = n < 0;
	data = { static_cast<unsigned>(std::abs(n)) };
}

SectionView::SectionView(uint8_t n)
{
	sign = 0;
	data = { static_cast<unsigned>(n) };
}

SectionView::SectionView(int16_t n)
{
	sign = n < 0;
	data = { static_cast<unsigned>(std::abs(n)) };
}

SectionView::SectionView(uint16_t n)
{
	sign = 0;
	data = { static_cast<unsigned>(n) };
}

SectionView::SectionView(int32_t n)
{
	sign = n < 0;
	data = { (unsigned)std::abs(n) };
}

SectionView::SectionView(uint32_t n)
{
	sign = 0;
	data = { n };
}

SectionView::SectionView(int64_t _n)
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

SectionView::SectionView(uint64_t n)
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

SectionView::SectionView( char* s, int base)
{
}

SectionView::SectionView(std::string s, int base)
{
}

SectionView::~SectionView()
{
}


SectionView& SectionView::operator=( SectionView& sv)
{
	*this = sv;
	return *this;
}


/*
Boron Boron::operator+( Boron& rhs) 
{
	Boron temp = *this;
	temp += rhs;
	return temp;
}
*/

/*
Boron Boron::operator+=( Boron& rhs)
{

	 std::vector<unsigned>& lhs_data = data;
	 std::vector<unsigned>& rhs_data = rhs.data;
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

void SectionView::clear()
{
	data = { 0 };
	sign = 0;
}

void SectionView::negate()
{
	sign = !sign;
}

size_t SectionView::digits()
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

size_t SectionView::sectionAmount()
{
	return data.size();
}

// 段的逻辑索引，而非在 vector 中的实际位置
unsigned SectionView::sectionAt(size_t offset)
{
	return data.at(sectionAmount() - 1 - offset);
}

void SectionView::modifySection(size_t offset, uint32_t newValue)
{
	data[sectionAmount() - offset - 1] = newValue;
}

void SectionView::modifyHighestSection(uint32_t newValue)
{
	data[0] = newValue;
}

unsigned SectionView::bitAt(size_t sec, size_t offset)
{
	return get_bit(sectionAt(sec), offset);
}

unsigned SectionView::bitAt(size_t offset)
{
	if (offset < 32)
	{
		return get_bit(sectionAt(0), offset);
	} 
	size_t restOffset = offset % 32;
	size_t sec = (offset - restOffset) / 32;
	return bitAt(sec, restOffset);
}

uint32_t SectionView::highestSection()
{
	return sectionAt(sectionAmount() - 1);
}

std::vector<unsigned> SectionView::getData()
{
	return data;
}

void SectionView::eachSection(std::function<bool(size_t, uint32_t&)> execution)
{
	for (size_t i = 0, last = sectionAmount(); i < last; i += 1)
	{
		if (!execution(i, data[last - 1 - i]))
		{
			return;
		}
	}
}



std::string Boron::toString(int base) 
{
	size_t amount = sectionView.sectionAmount();
	if (amount == 1)
	{
		return std::to_string(sectionView.sectionAt(0));
	}
	std::string s;
	sectionView.eachSection([&](size_t i, uint32_t& sec) -> bool
		{
			s = std::to_string(sec) + s;
			return true;
		});
	return s;
}


#define make_bop_def(op) \
	Boron operator##op##(Boron lhs, Boron rhs)

make_bop_def(<<)
{
	Boron temp = lhs;
	temp <<= rhs;
	return temp;
}

make_bop_def(<<=)
{
	auto lsv = lhs.sectionView;

	// 如果左移的位数不致超过最高段
	if (length_of_bits(lsv.highestSection()) > rhs)
	{

		uint32_t _rhs = rhs.getUInt32();

		// 第一步：将最高段直接左移，为下一段腾出空间
		lsv.modifyHighestSection(lsv.highestSection() << _rhs);

		size_t amount = lsv.sectionAmount();

		if (amount == 1)
		{
			return lhs;
		}

		// 第二步：
		lsv.eachSection([&](size_t i, uint32_t& sec) -> bool
			{
				uint32_t next = lsv.sectionAt(i + 1);
				for (size_t j = 0; j < _rhs; j++)
				{
					sec = set_bit(sec, j, get_bit(next, 32 - 1 - j));
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
		lsv.eachSection([&](size_t i, uint32_t section) -> bool{

			// TODO...
			return false;
		});
	}
}

Boron::Boron( SectionView& sv)
{
	sectionView = std::move(sv);
}

Boron::Boron( Boron& b)
{
	sectionView = std::move(b.sectionView);
}

Boron& Boron::operator=(Boron& b)
{
	*this = b;
	return *this;
}

Boron::~Boron()
{
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

Boron& operator++(Boron& b)
{
	b += 1;
	return b;
}

Boron& operator++(Boron& b, int)
{
	Boron& temp = b;
	b += 1;
	return temp;
}

Boron& operator--(Boron& b)
{
	b += 1;
	return b;
}

Boron& operator--(Boron& b, int)
{
	Boron& temp = b;
	b += 1;
	return temp;
}

Boron& operator-( Boron& b)
{
	Boron temp = b;
	temp.sectionView.negate();
	return temp;
}

bool operator>(Boron lhs, Boron rhs)
{
	SectionView lsv = lhs.sectionView;
	SectionView rsv = rhs.sectionView;
	size_t lhsSecAmount = lsv.sectionAmount();
	size_t rhsSecAmount = rsv.sectionAmount();

	// 如果段数不同，则段数更大的显然数值更大
	if (lhsSecAmount != rhsSecAmount)
	{
		return lhsSecAmount > rhsSecAmount;
	}
	else
	{
		uint32_t lhsHSec = lsv.highestSection();
		uint32_t rhsHSec = rsv.highestSection();

		// 若最高段不同，则最高端更大的显然数值更大
		if (lhsHSec != rhsHSec)
		{
			return lhsHSec > rhsHSec;
		}

		bool is_lhsGreater = false;

		lsv.eachSection([&](size_t i, uint32_t sec) -> bool
		{
			if (sec > rsv.sectionAt(i))
			{
				is_lhsGreater = true;
				return false;
			}
			return true;
		});

		return is_lhsGreater;
	}
}

bool operator>=(Boron lhs, Boron rhs)
{
	return lhs > rhs || lhs == rhs;
}

bool operator<(Boron lhs, Boron rhs)
{
	return !(rhs >= rhs);
}

bool operator<=(Boron lhs, Boron rhs)
{
	return !(lhs > rhs);
}

bool operator==(Boron lhs, Boron rhs)
{
	bool isEqual = true;
	auto rsv = rhs.sectionView;
	lhs.sectionView.eachSection([&](size_t i, uint32_t sec) -> bool
		{
			if (sec != rsv.sectionAt(i))
			{
				isEqual = false;
				return false;
			}
			return true;
		});
	return isEqual;
}

bool operator!=(Boron lhs, Boron rhs)
{
	return !(lhs == rhs);
}

}
