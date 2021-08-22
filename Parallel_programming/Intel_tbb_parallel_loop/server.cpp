#include <iostream>
#include <unistd.h>
#include <tbb/tbb.h>
#include <cmath>

void Foo(float* x) {
    *x = sqrtf(*x);
}

void RunParallelLoop(float a[], size_t n) {
    tbb::parallel_for(tbb::blocked_range<size_t>(0, n),
            [a, n](const tbb::blocked_range<size_t>& r) {
        for (auto i = r.begin(); i != r.end(); i++) {
            Foo(&a[i]);
        }
    });
}

int main() {
    float a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 81};
    size_t n = sizeof(a) / sizeof(float);
    for (auto& it : a) {
        std::cout << it << " ";
    }
    std::cout << "\n";
    RunParallelLoop(a, n);
    for (auto& it : a) {
        std::cout << it << " ";
    }
    std::cout << "\n";
    return 0;
}