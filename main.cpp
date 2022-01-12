#include <iostream>
using namespace std;

unsigned long fib_seq(int n) {
    unsigned long old = 0;
    unsigned long value = 1;
    unsigned long hold;

    if(n < 1) return 0;

    for(int i = 1; i < n; i++)
    {
        hold = value;
        value += old;
        old = hold;
    }

    return value;
}

int main() {
    cout << fib_seq(45);
    return 0;
}
