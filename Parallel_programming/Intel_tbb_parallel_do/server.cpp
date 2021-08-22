#include <iostream>
#include <unistd.h>
#include <tbb/tbb.h>
#include <cmath>
#include <utility>

void Foo(float& x) {
    x = sqrtf(x);
}

void Bar(int& x) {
    ++x;
}

template<typename T>
class ApplyFunc {
public:
    void operator()(T& x) const {
        func_(x);
    }

    explicit ApplyFunc(std::function<void(T&)> func) : func_(std::move(func)) {};

private:
    std::function<void(T& x)> func_;
};

template<typename T>
void ParallelApplyToList(std::list<T>& list, std::function<void(T&)> func) {
    ApplyFunc<T> af(func);
    tbb::parallel_do(list.begin(), list.end(), af);
}

int main() {
    {
        std::list<float> a = {7, 2, 4.5, 6.7};
        for (const auto &it : a) {
            std::cout << it << " ";
        }
        std::cout << "\n";
        ParallelApplyToList<float>(a, Foo);
        for (const auto &it : a) {
            std::cout << it << " ";
        }
        std::cout << "\n";
    }
    {
        std::list<int> a = {7, 2, 4, 6};
        for (const auto &it : a) {
            std::cout << it << " ";
        }
        std::cout << "\n";
        ParallelApplyToList<int>(a, Bar);
        for (const auto &it : a) {
            std::cout << it << " ";
        }
        std::cout << "\n";
    }
    return 0;
}