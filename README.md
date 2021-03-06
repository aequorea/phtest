## phtest
Pointerized hybrid quicksort test.

Here we apply the pointerization technique to speed up a hybrid quicksort routine which falls back to an insertion sort for small arrays. The FALLBACK parameter which is the size of array to fall back with is tuned to suit the machine. We compare the following pointerized hybrid quicksort with the C++ library sort on arrays of random integers.

    #define FALLBACK 35
   
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
 
Here are some results. We tested on linux with gcc using -O3 optimization. Average values shown are the average improvement of the hybrid quicksort routine over the C++ routine for the array sizes tested. 

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

    AMD E-350 Processor
    gcc (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0
    FALLBACK 30

          10: C++ 1000 ms, hybrid 872 ms -- 14.6 %
         100: C++ 1000 ms, hybrid 906 ms -- 10.3 %
        1000: C++ 1000 ms, hybrid 899 ms -- 11.2 %
       10000: C++ 1000 ms, hybrid 888 ms -- 12.7 %
      100000: C++ 1004 ms, hybrid 888 ms -- 13.1 %
     1000000: C++ 1012 ms, hybrid 903 ms -- 12.0 %

     Average --> 12.3 %

    AMD Athlon(tm) 64 Processor 3500+
    gcc (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0
    FALLBACK 35

          10: C++ 1000 ms, hybrid 985 ms -- 1.4 %
         100: C++ 1000 ms, hybrid 923 ms -- 8.2 %
        1000: C++ 1000 ms, hybrid 909 ms -- 10.0 %
       10000: C++ 1000 ms, hybrid 926 ms -- 7.9 %
      100000: C++ 1001 ms, hybrid 938 ms -- 6.8 %
     1000000: C++ 1026 ms, hybrid 980 ms -- 4.7 %

     Average --> 6.5 %

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

The x64 processors are overall faster than the C++ library routine and the Raspberry Pi is on the average a little bit slower. 

If you want to try the benchmark program yourself on linux run the python script bench.py. It compiles the program, collects some information about your environment and displays it along with the benchmark result. You'll need to make sure to install the command-line development tools.

