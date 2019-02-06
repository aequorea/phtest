## phtest
#### Pointerized hybrid quicksort test
Here we compare the following pointerized hybrid quicksort with the C++ library sort on arrays of random integers. We test on linux using the GCC and G++ compilers with -O3 optimizations. We have tuned the FALLBACK parameter to suit the machine. On the Core I5 it's 65, on the Athlon 64 it's 35 and on the Raspberry pi it's 18.

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

Athlon 64 averages

          10 elements -- 4.7 %
         100 elements -- 11.9 %
        1000 elements -- 14.9 %
       10000 elements -- 16.0 %
      100000 elements -- 16.8 %
     1000000 elements -- 17.7 %
 
    Overall Average: 13.7 %

Core I5 averages
  
          10 elements -- 6.1 %
         100 elements -- 24.3 %
        1000 elements -- 16.9 %
       10000 elements -- 13.6 %
      100000 elements -- 10.8 %
     1000000 elements -- 9.5 %
 
    Overall Average: 13.5 %

Raspberry pi averages

          10 elements -- 21.3 %
         100 elements -- -4.1 %
        1000 elements -- -8.8 %
       10000 elements -- -9.7 %
      100000 elements -- -10.1 %
     1000000 elements -- -9.8 %
 
    Overall Average: -3.6 %

Overall, on the x64 machines we are 13+ % faster than the C++ library sort, but on the Raspberry pi we're a little bit slower. We note that we are faster on machines where we have shown pointerized code can run faster than array indexed code. On the Raspberry pi there is no advantage to pointerized code and we see that we are slower on that machine.

More complete data may be found [here](https://github.com/aequorea/phsort/tree/master/bm).

If you would like to try out the benchmark, make sure you have installed gcc and g++. Download the archive and run bench.py. To reduce the possibility of "linker bias" tilting the results, it builds and tests six different versions of the benchmark executable where the three different object files are linked in six different ways. The data is collected and averaged over runs of these executables.
