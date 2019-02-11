/*
   hsort.c -- hybrid quicksort with median of three pivot
*/

#define FALLBACK 35  // insertion sort for arrays smaller than this

// pointerized stack stuff

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

// pointerized insertion sort

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

// clang compiles this with no branches (pretty cool)

static int median_of_three(int pivot, int r, int l) 
{ 
    int t;
    
    if (pivot < l) { t = l; l = pivot; pivot = t; }
    if (r < l)     { r = l; }
    if (r < pivot) { pivot = r; }
    return(pivot);
}

// pointerized partition

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

// pointerized hybrid non-recursive quicksort

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

