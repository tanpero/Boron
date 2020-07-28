#include "boron.h"

int main(int argc, char* argv[])
{
	using namespace boron;
	Boron a = 0b100011101;
	for (size_t i = 0; i < length_of_bits(a.getData()[0]); i++)
	{
		std::cout << "In 0b10011101, bit at " << i << " is " << a.bitAt(i) << std::endl;
	}
	return 0;
}
