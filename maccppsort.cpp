/*
 cppsort.cpp -- encapsulate the C++ library sort
*/

extern "C" void cppsort(int*, int*);

#include <algorithm>

void cppsort(int *lp, int *rp)
{
    std::sort(lp, rp+1);
}

