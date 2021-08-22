#include <iostream>
#include <unistd.h>
#include <tbb/tbb.h>
#include <cmath>

float Foo(float x) {
    return sqrtf(x);
}

class SumClass {
public:
    void operator()(const tbb::blocked_range<size_t>& r) {
        const float* a = a_;
        float sum = sum_;
        for (auto i = r.begin(); i != r.end(); i++) {
            sum += Foo(a[i]);
        }
        sum_ = sum;
    }

    SumClass(const SumClass& rhs, tbb::split) {
        a_ = rhs.a_;
        sum_ = 0;
    }

    void join(const SumClass& rhs) {
        sum_ += rhs.sum_;
    }

    explicit SumClass(const float* a) : a_(a), sum_(0) {};

    float GetSum() const {
        return sum_;
    }

private:
    const float* a_;
    float sum_;
};

float RunParallelSum(const float* a, size_t n) {
    SumClass sc(a);
    tbb::parallel_reduce(tbb::blocked_range<size_t>(0, n), sc);
    return sc.GetSum();
}

float RunSimpleSum(const float* a, size_t n) {
    float ans = 0;
    for (size_t i = 0; i < n; i++) {
        ans += Foo(a[i]);
    }
    return ans;
}

int main() {
    float a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 81};
    size_t n = sizeof(a) / sizeof(float);
    for (auto& it : a) {
        std::cout << it << " ";
    }
    std::cout << "\n";
    std::cout << RunParallelSum(a, n) << " " << RunSimpleSum(a, n) << "\n";
    return 0;
}