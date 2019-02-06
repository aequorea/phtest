/*
 cppsort.cpp -- encapsulate the C++ library sort
*/

extern "C" void cppsort(int*, int*);

#include <bits/stdc++.h>

void cppsort(int *lp, int *rp)
{
    std::sort(lp, rp+1);
}

