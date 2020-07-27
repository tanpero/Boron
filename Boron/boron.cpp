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
              
Boron::Boron(signed char n)
{
	sign = n < 0;
	data = { static_cast<unsigned>(std::abs(n)) };
}

Boron::Boron(unsigned char n)
{
	sign = 0;
	data = { static_cast<unsigned>(n) };
}

Boron::Boron(signed short n)
{
	sign = n < 0;
	data = { static_cast<unsigned>(std::abs(n)) };
}

Boron::Boron(unsigned short n)
{
	sign = 0;
	data = { static_cast<unsigned>(n) };
}

Boron::Boron(signed int n)
{
	sign = n < 0;
	data = { (unsigned)std::abs(n) };
}

Boron::Boron(unsigned int n)
{
	sign = 0;
	data = { n };
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

}
