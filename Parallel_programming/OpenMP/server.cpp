#include <iostream>
#include <omp.h>
#include <unistd.h>

int main() {
//    #pragma omp parallel num_threads(2)
//    {
//        std::cout << "Hello world\n" << omp_get_thread_num() << "\n";
//    }
//    int sum = 0;
//    #pragma omp parallel for reduction(+:sum)
//    {
//        for (int i = 0; i < 1e8; i++) {
//            sum += 2;
//        }
//    }
//    std::cout << sum << "\n";
//
//    #pragma omp parallel
//    {
//        std::cout << 1 << "\n";
//        std::cout << 2 << "\n";
//        std::cout << 3 << "\n";
//    }
//
//    #pragma omp parallel
//    {
//        std::cout << 1 << "\n";
//        std::cout << 2 << "\n";
//        #pragma omp barrier
//        std::cout << 3 << "\n";
//    }
//
//    #pragma omp parallel
//    {
//        std::cout << 1 << "\n";
//        #pragma omp single
//        {
//            std::cout << 2 << "\n";
//        }
//        std::cout << 3 << "\n";
//    }

//    #pragma omp parallel
//    {
//        std::cout << 1 << "\n";
//        #pragma omp single nowait
//        {
//            std::cout << 2 << "\n";
//        }
//        std::cout << 3 << "\n";
//    }

//    #pragma omp parallel for ordered
//    for (int i = 0; i < 1e7; i++) {
//        std::cout << "Cycle\n";
//        #pragma omp ordered
//        {
//            std::cout << i << "\n";
//        }
//    }

//    #pragma omp parallel for ordered
//    for (int i = 0; i < 10; i++) {
//        std::cout << "executing: " << i << std::endl;
//        sleep(2);
//        #pragma omp ordered
//        {
//            std::cout << "i = " << i << std::endl;
//        }
//    }
    return 0;
}