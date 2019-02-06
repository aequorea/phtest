/*
 sorter.c -- test performance of a pair of sort routines
             this one tests multiple array sizes
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned long long int ulli;

typedef struct tag_perf
{   
    ulli testtime;
    ulli reftime;
} perf;
    
extern void cppsort(int*, int*);
extern void hsort(int*, int*);

void (*sort_test)(int*, int*) = hsort;    // compare a "test" routine ...
void (*sort_ref)(int*, int*)  = cppsort;  // ... with a "reference" routine

#define VERIFY 0
#define NS     1000000000   // sample sort for a second
#define ASIZE  1000000

int A1[ASIZE];
int A2[ASIZE];

static ulli ns_time()
{
    struct timespec ts;
    
    clock_gettime(CLOCK_MONOTONIC, &ts);
    
    return (ulli) ts.tv_sec * 1000000000 + (ulli) ts.tv_nsec;
}

ulli time_sort(int *A1, int l, void (*sort)(int*, int*))
{
    for (int i = 0; i < l; i++) 
        A1[i] = rand();
    
    ulli begin = ns_time();
    sort(A1, A1+l-1);
    return (ns_time()-begin);
}

perf benchmark(int elements) 
{
    ulli testtime=0, reftime=0;    
    perf rval;
    
    while (reftime < NS)
    {
        reftime += time_sort(A1, elements, sort_ref);
        testtime += time_sort(A1, elements, sort_test);
    }
    rval.testtime = testtime;
    rval.reftime = reftime;
    return(rval);
}

double show_perf(char *s, perf p)
{
    double average = (double) p.testtime/(double) p.reftime;
    double percent = 100.0*(1.0-average)/average;
    int hybrid_ms = p.testtime/1000000;
    int cpp_ms = p.reftime/1000000;
    printf(" %s: C++ %d ms, hybrid %d ms -- %2.1lf %%\n", \
             s, cpp_ms, hybrid_ms, percent);
    return (percent);
}

void verify_something(int l, int *A1, int *A2)
{
    for (int i = 0; i < l; i++) 
        A1[i] = A2[i] = rand();
    
    sort_test(A1, A1+l-1);
    sort_ref(A2, A2+l-1);

    for (int i = 0; i < l; i++)
        if (A1[i]-A2[i] != 0)
        {
        	printf("compare error -- %d\n", i);
            return;
        }
}

int main()
{
    perf b1, b2, b3, b4, b5, b6;
    double avg = 0.0;

    srand(ns_time());
    printf("\n");
    
    b1 = benchmark(10);
    b2 = benchmark(100);
    b3 = benchmark(1000);
    b4 = benchmark(10000);
    b5 = benchmark(100000);
    b6 = benchmark(1000000);

    avg += show_perf("     10", b1);
    avg += show_perf("    100", b2);
    avg += show_perf("   1000", b3);
    avg += show_perf("  10000", b4);
    avg += show_perf(" 100000", b5);
    avg += show_perf("1000000", b6);

    avg /= 6;

    printf("\n Average --> %2.1f %%\n\n", avg);

    if (VERIFY) verify_something(10000, A1, A2);    
}
