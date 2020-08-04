#include "boron.h"

int main(int argc, char* argv[])
{
    using namespace boron;
    Boron a = 0xffff;
    Boron b = 0xffff;
    Boron c = a + b;
    std::cout << c.toString() << std::endl;
    return 0;
}
