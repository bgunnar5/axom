//
// ctest.cpp
//

#include "tutorial.hpp"


int main(int argc, char *argv[])
{
    using namespace tutorial;

    Function1();

    Class1 *cptr = new Class1();

    cptr->Method1();

    // Arguments
    // Integer and Real
    double rv2 = Function2(1.5, 2);

    Function6("name");
    Function6(1);

    int rv8a = Function8<int>();
    double rv8b = Function8<double>();
}
