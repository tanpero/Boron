#include "boron.h"

namespace boron
{

/*
 * class SectionView
 */

SectionView::SectionView()
{
    sign = 0;
    data = std::vector<uint32_t>{ 0 };
}

SectionView::SectionView(std::vector<uint32_t> vec)
{
    data = vec;
    sign = POS;
}

SectionView::SectionView(SectionView& sv)
{
    if (*this != sv)
    {
        sign = sv.sign;
        data = sv.data;
    }
}
              
SectionView::SectionView(int8_t n)
{
    sign = n < 0;
    data = { static_cast<uint32_t>(std::abs(n)) };
}

SectionView::SectionView(uint8_t n)
{
    sign = 0;
    data = { static_cast<uint32_t>(n) };
}

SectionView::SectionView(int16_t n)
{
    sign = n < 0;
    data = { static_cast<uint32_t>(std::abs(n)) };
}

SectionView::SectionView(uint16_t n)
{
    sign = 0;
    data = { static_cast<uint32_t>(n) };
}

SectionView::SectionView(int32_t n)
{
    sign = n < 0;
    data = { (uint32_t)std::abs(n) };
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
        data = { (uint32_t)n };
    }
    else
    {
        data = { (uint32_t)n >> 32, (uint32_t)n };
    }
}

SectionView::SectionView(uint64_t n)
{
    sign = 0;
    uint64_t criticalValue = (uint64_t)UINT_MAX;
    if (n <= criticalValue)
    {
        data = { (uint32_t)n };
    }
    else
    {
        data = { (uint32_t)n >> 32, (uint32_t)n };
    }
}

SectionView::SectionView(char* s, int base)
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
    if (*this != sv)
    {
        sign = sv.sign;
        data = std::move(sv.data);
    }
    return *this;
}


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
uint32_t SectionView::sectionAt(size_t offset)
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

void SectionView::expandSection(uint32_t sec)
{
    data.insert(data.begin(), sec);
}

uint32_t SectionView::bitAt(size_t sec, size_t offset)
{
    return get_bit(sectionAt(sec), offset);
}

uint32_t SectionView::bitAt(size_t offset)
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

std::vector<uint32_t> SectionView::getData()
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


/*
 * class Boron
 */

Boron::Boron()
{
}

Boron::Boron( SectionView& sv)
{
    sectionView = sv;
}

Boron::Boron(Boron& b)
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

/*
 * 用于完成基本计算
 */

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

#define make_uop_def(op) \
    Boron operator##op##(Boron lhs)

#define make_bop_def(op) \
    Boron operator##op##(Boron lhs, Boron rhs)

#define make_asn_def(op) \
    Boron& operator##op##(Boron lhs, Boron rhs)

make_uop_def(-)
{
    Boron temp = lhs;
    temp.sectionView.negate();
    return temp;
}

make_bop_def(+)
{
    size_t lhsLen = lhs.sectionView.sectionAmount(),
        rhsLen = rhs.sectionView.sectionAmount();
    SectionView temp = construct(std::max(lhsLen, rhsLen));
    uint64_t carry = 0;

    for (size_t i = 0, last = temp.sectionAmount(); i < last; i += 1)
    {
        if (i < lhsLen)
        {
            carry += lhs.sectionView.sectionAt(i);
        }
        if (i < rhsLen)
        {
            carry += rhs.sectionView.sectionAt(i);
        }
        temp.modifySection(i, static_cast<uint32_t>((carry)));
        carry >>= 32;
    }

    if (carry)
    {
        temp.expandSection(carry);
    }

    return std::move(temp);
}

make_bop_def(-)
{
    size_t lhsLen = lhs.sectionView.sectionAmount(),
        rhsLen = rhs.sectionView.sectionAmount();
    SectionView temp = construct(lhsLen);
    int64_t borrow = 0;

    for (size_t i = 0, last = temp.sectionAmount(); i < last; i += 1)
    {
        borrow += lhs.sectionView.sectionAt(i);
        borrow -= (i < rhsLen) ? rhs.sectionView.sectionAt(i) : 0;
        temp.modifySection(i, borrow);
        borrow >>= 32;
    }

    return std::move(temp);
}

make_bop_def(*)
{

    return Boron();
}

make_bop_def(/)
{

    return Boron();
}

make_bop_def(%)
{

    return Boron();
}

make_bop_def(<<)
{
    Boron temp = lhs;
    temp <<= rhs;
    return temp;
}

make_bop_def(>>)
{

    return Boron();
}

make_bop_def(&)
{

    return Boron();
}

make_bop_def(|)
{

    return Boron();
}

make_bop_def(^)
{

    return Boron();
}

make_uop_def(~)
{

    return Boron();
}

make_asn_def(+=)
{
    return Boron();
}

make_asn_def(-=)
{
    return Boron();
}

make_asn_def(*=)
{
    return Boron();
}

make_asn_def(/=)
{
    return Boron();
}

make_asn_def(%=)
{
    return Boron();
}

make_asn_def(<<=)
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
        lsv.eachSection([&](size_t i, uint32_t section) -> bool {

            // TODO...
            return false;
            });
    }
}

make_asn_def(>>=)
{
    return Boron();
}

make_asn_def(&=)
{
    return Boron();
}

make_asn_def(|=)
{
    return Boron();
}

make_asn_def(^=)
{
    return Boron();
}


make_bop_def(&&)
{
    return Boron();
}

make_bop_def(||)
{
    return Boron();
}


/*
 * 用于比较数值关系
 */

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

/*
 * 用于完成进一步复杂计算、
 */

Boron pow(Boron& a, Boron& b)
{
    return Boron();
}

Boron modpow(Boron base, Boron exponent, Boron modular)
{
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

std::vector<Boron> factorize(Boron n)
{
    return std::vector<Boron>();
}

Boron gcd(Boron& a, Boron& b)
{
    return Boron();

}

Boron lcm(Boron& a, Boron& b)
{
    return Boron();
}

Boron intSqrt(Boron& n)
{
    return Boron();
}

Boron sqrt(Boron& n, Boron& accuracy)
{
    return Boron();
}

Boron max(Boron& a, Boron& b)
{
    return Boron();
}

Boron min(Boron& a, Boron& b)
{
    return Boron();
}

/*
 * 用于向外传递数值
 */

// TODO...
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

uint32_t Boron::getUInt32()
{
    return sectionView.sectionAt(0);
}


}
