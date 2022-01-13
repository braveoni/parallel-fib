#include <iostream>
#include <omp.h>
using namespace std;

#define ull unsigned long long
#define N 15

ull value[N+1];
bool done[N+1];
omp_lock_t lock[N+1];

const ull e = 10e10 + 7;

ull fib(int n) {
    ull i, j;

    omp_set_lock( &(lock[n]) );

    if (!done[n]) {
        #pragma omp parallel shared(value, done)
        {
            #pragma omp task shared(i) firstprivate(n)
            i = fib(n - 1);

            #pragma omp task shared(j) firstprivate(n)
            j = fib(n - 2);

            #pragma omp taskwait
            value[n] = (i + j) % e;
            done[n] = true;
        }
    }

    omp_unset_lock( &(lock[n]) );
    return value[n];
}

int main() {
    value[0] = 0;
    value[1] = 1;

    for (int i = 0; i <= N; i++) done[i] = false;

    done[0] = true;
    done[1] = true;

    for (int i=0; i <= N; i++) omp_init_lock(&(lock[i]));
    cout << fib(N) << '\n';
    for (int i=0; i <= N ; i++) omp_destroy_lock(&(lock[i]));
}