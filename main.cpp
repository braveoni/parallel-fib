#include <iostream>
#include <omp.h>

#define ll unsigned long long

using namespace std;

ll fib(int n) {
    if (n < 2) return n;
    return fib(n - 1) + fib(n - 2);
}

ll fib_seq(int n) {
    ll old = 0;
    ll value = 1;
    ll hold;

    if(n < 1) return 0;

    for(ll i = 1; i < n; i++)
    {
        hold = value;
        value += old;
        old = hold;
    }

    return value;
}

ll fib_omp_sections(int n) {
    ll x, y;
    #pragma omp parallel
    {
        #pragma omp parallel sections shared(x, y)
        {
            #pragma omp section
            {
                x = fib(n - 1);
            }
            #pragma omp section
            {
                y = fib(n - 2);
            }
        }
    }

    return x + y;
}

ll fib_omp_tasks(int n) {
    ll x, y;
    if (n < 2) return n;
    #pragma omp parallel shared(n)
    {
        #pragma omp single
        {
            #pragma omp task shared(x)
            x = fib_omp_tasks(n - 1);

            #pragma omp task shared(y)
            y = fib_omp_tasks(n - 2);

            #pragma omp taskwait
        }
    }

    return x + y;
}


void run(const string& name, ll f(int)) {
    double time = omp_get_wtime();
    ll res = f(50);
    printf("%s Time: %f Result: %llu\n", name.c_str() , omp_get_wtime() - time, res);
}


int main() {
    omp_set_num_threads(4);

    run("Recursive", fib);
    run("Sequence ", fib_seq);
    run("Sections ", fib_omp_sections);
    run("Tasks    ", fib_omp_tasks);

    return 0;
}