## phtest
#### Pointerized hybrid quicksort test
Here we apply the pointerization technique to speed up a hybrid quicksort routine. Our strategy to the best of our knowledge was to use the same kinds of algorithms that the C++ library sort uses for random integers, except we pointerize our versions and we tune the fallback parameter where the quicksort falls back to an insertion sort to suit the machine. We compare the following pointerized hybrid quicksort with the C++ library sort on arrays of random integers.

    #define FALLBACK 35
    #define MAXSTACK 1000000*4

    static int *stack[MAXSTACK+4], **sp;

    static void push(int *v)
        { *sp++ = v; }
    
    static int *pop()
        { return *--sp; }
    
    static void stackinit()
        { sp = stack; }
    
    static int stackempty()
        { return (sp==stack); }

    static void isort(int *lp, int *rp)
    {
        int *ip, *jp, t;
    
        for (ip=lp+1; ip<=rp; ip++)
        {
            t = *ip;
            for (jp=ip; jp>lp && *(jp-1) > t; jp--)
                *jp = *(jp-1);
            *jp = t;
        }
    }

    static int median_of_three(int pivot, int r, int l) 
    { 
        int t;
    
        if (pivot < l) { t = l; l = pivot; pivot = t; }
        if (r < l)     { r = l; }
        if (r < pivot) { pivot = r; }
        return(pivot);
    }

    static int *ip, *jp;
    static void partition(int *lp, int *rp)
    {
        int t, pivot;
    
        ip = lp-1, jp = rp+1;
    
        pivot = median_of_three(*lp, *(lp+(rp-lp)/2), *rp); 

        while(1)
        {
            while (*++ip < pivot) ;
            while (pivot < *--jp) ;
            if (ip >= jp) break;

            t = *ip; *ip = *jp; *jp = t;
        }
    }

    void hsort(int *lp, int *rp)
    {
        stackinit();
        while (1)
        {
            if (rp-lp <= FALLBACK)
                isort(lp, rp);
            else
            {
                partition(lp, rp);
                push(lp);   push(ip-1);
                push(jp+1); push(rp);
            }
            if (stackempty()) break;
            rp = pop(); lp = pop();
        }
    }

Here are some results. We tested on linux with the compiled code optimized with the -O3 parameter. The average values shown are the average improvement of the hybrid quicksort routine over the C++ routine for the array sizes tested. 


    Intel(R) Core(TM) i5-4690 CPU @ 3.50GHz
    gcc (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0
    FALLBACK 65

          10: C++ 1000 ms, hybrid 907 ms -- 10.2 %
         100: C++ 1000 ms, hybrid 798 ms -- 25.3 %
        1000: C++ 1000 ms, hybrid 837 ms -- 19.5 %
       10000: C++ 1000 ms, hybrid 856 ms -- 16.7 %
      100000: C++ 1002 ms, hybrid 878 ms -- 14.1 %
     1000000: C++ 1021 ms, hybrid 900 ms -- 13.5 %

    Average --> 16.5 %


    Intel(R) Pentium(R) CPU  J2900  @ 2.41GHz
    gcc (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0
    FALLBACK 35

          10: C++ 1000 ms, hybrid 790 ms -- 26.4 %
         100: C++ 1000 ms, hybrid 893 ms -- 11.9 %
        1000: C++ 1000 ms, hybrid 942 ms -- 6.1 %
       10000: C++ 1000 ms, hybrid 957 ms -- 4.5 %
      100000: C++ 1000 ms, hybrid 963 ms -- 3.8 %
     1000000: C++ 1073 ms, hybrid 1037 ms -- 3.5 %

     Average --> 9.4 %


      ARMv7 Processor rev 4 (v7l)
    gcc (Raspbian 6.3.0-18+rpi1+deb9u1) 6.3.0 20170516
    FALLBACK 18

          10: C++ 1000 ms, hybrid 823 ms -- 21.5 %
         100: C++ 1000 ms, hybrid 1029 ms -- -2.8 %
        1000: C++ 1000 ms, hybrid 1066 ms -- -6.2 %
       10000: C++ 1000 ms, hybrid 1070 ms -- -6.6 %
      100000: C++ 1005 ms, hybrid 1077 ms -- -6.6 %
     1000000: C++ 1063 ms, hybrid 1142 ms -- -7.0 %

     Average --> -1.3 %

The x64 processors are overall faster than the C++ library routine and the Raspberry Pi is on the average a little bit slower. We note that the relative performance of the processors in this test correlates with their performance on pointerized code in the [insertion sort benchmark](https://github.com/aequorea/pitest). This suggests there may be a relationship between the two. Evidently there is more to the picture than the fast insertion sort that this routine falls back to since on the Raspberry Pi we see the insertion sort is working very well on the smallest arrays but the performance drops off on the larger ones.

If you want to try the benchmark program yourself run the python script bench.py. It compiles the program, collects some information about your environment and displays it along with the benchmark result.
