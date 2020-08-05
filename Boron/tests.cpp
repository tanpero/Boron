#include "boron.h"

int main(int argc, char* argv[])
{
    using namespace boron;
    Boron a = 20;
    Boron b = 30;
    Boron c = a * b;
    std::cout << c << std::endl;
    return 0;
}
