## phtest
#### Pointerized hybrid quicksort test
Here we compare the following pointerized hybrid quicksort with the C++ library sort on arrays of random integers. We test on linux using the GCC and G++ compilers with -O3 optimizations. We have tuned the FALLBACK parameter to suit the machine. On the Core I5 it's 65, on the Athlon 64 it's 35 on the Pentium J2900 it's 35 and on the Raspberry pi it's 18.

    #define FALLBACK 35     // fallback to insertion sort
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

Here are some results.

    Intel(R) Pentium(R) CPU  J2900  @ 2.41GHz
    gcc (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0

          10: C++ 1000 ms, hybrid 793 ms -- 26.0 %
         100: C++ 1000 ms, hybrid 893 ms -- 11.9 %
        1000: C++ 1000 ms, hybrid 942 ms -- 6.1 %
       10000: C++ 1000 ms, hybrid 957 ms -- 4.5 %
      100000: C++ 1007 ms, hybrid 971 ms -- 3.7 %
     1000000: C++ 1071 ms, hybrid 1041 ms -- 2.9 %

     Average --> 9.2 %

The average value shown is the average improvement of the hybrid quicksort routine over the C++ routine for the array sizes tested.
