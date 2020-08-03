#include "boron.h"

int main(int argc, char* argv[])
{
	using namespace boron;
	Boron a = 100;
	Boron b = 200;
	Boron c = a + b;
	std::cout << a.toString() << std::endl;
	return 0;
}
