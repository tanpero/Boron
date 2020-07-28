#include "boron.h"

int main(int argc, char* argv[])
{
	using namespace boron;
	Boron a(0xffffffff);
	Boron b(200);
	std::cout << a.toString() << " + " << b.toString() << " = " << (a + b).toString() << std::endl;
	return 0;
}
