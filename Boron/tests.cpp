#include "boron.h"

int main(int argc, char* argv[])
{
	using namespace boron;
	Boron a = 0b10011101100111011010011011011001110110011101L;
	for (size_t i = 0; i < a.digits(); i++)
	{
		std::cout << "In 0b1001 11011001 11011010 01101101 10011101 10011101, bit at " << i << " is " << a.bitAt(i) << std::endl;
	}
	return 0;
}
