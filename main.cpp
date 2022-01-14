#include <iostream>
#include <omp.h>
using namespace std;

#define ull unsigned long long
#define N 100000

ull value[N+1];
bool done[N+1];
omp_lock_t lock[N+1];

const ull e = 10e10 + 7;

ull fib_seq(ull n) {
    ull old = 0;
    ull result = 1;
    ull hold;

    if (n < 1) return 0;
    for (int i = 0; i < n; ++i) {
        hold = result;
        result = (result + old) % e;
        old = hold;
    }

    return result;
}

ull fib_omp(ull n) {
    if (n < 2) return n;

    ull r1, r2;

#pragma omp task shared(r1)
    {
        r1 = fib_omp(n - 1);
    }

#pragma omp task shared(r2)
    {
        r2 = fib_omp(n - 2);
    }

#pragma omp taskwait
    return r1 + r2;
}

ull fib_omp_bor(ull n) {
    ull i, j;

    omp_set_lock( &(lock[n]) );

    if (!done[n]) {
        #pragma omp parallel
        {
            #pragma omp task shared(i) firstprivate(n)
            i = fib_omp_bor(n - 1);

            #pragma omp task shared(j) firstprivate(n)
            j = fib_omp_bor(n - 2);

            #pragma omp taskwait
            value[n] = (i + j) % e;
            done[n] = true;
        }
    }

    omp_unset_lock( &(lock[n]) );
    return value[n];
}

void run(ull f(ull)) {
    double time = omp_get_wtime();
    ull res = f(N);
    printf("Time: %f Result: %llu \n", omp_get_wtime() - time, res);
}

int main() {

    run(fib_omp);

    value[0] = 1;
    value[1] = 1;

    for (int i = 0; i <= N; i++) done[i] = false;

    done[0] = true;
    done[1] = true;

    for (int i=0; i <= N; i++) omp_init_lock(&(lock[i]));
    run(fib_omp_bor);
    for (int i=0; i <= N ; i++) omp_destroy_lock(&(lock[i]));
}