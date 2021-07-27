#include <iostream>
#include <map>
#include <random>
#include <cstring>

constexpr unsigned int sz = 1048576;

class SmallAllocator {
private:
    char Memory[sz];
    std::map<unsigned int, unsigned int> mem;
    std::mt19937 rng;
public:
    void* Alloc(unsigned int Size) {
        std::uniform_int_distribution<unsigned int> uid(0, sz - 1u - Size);
        unsigned int success_begin;
        while (true) {
            unsigned int begin = uid(rng);
            unsigned int end = begin + Size;
            bool bad = false;
            for (const auto& it : mem) {
                unsigned int have_beg = it.first;
                unsigned int have_end = it.second;
                if (have_end <= begin) {
                    continue;
                }
                if (end <= have_beg) {
                    continue;
                }
                bad = true;
                break;
            }
            if (!bad) {
                success_begin = begin;
                break;
            }
        }
        mem[success_begin] = Size;
        return Memory + success_begin;
    };

    void* ReAlloc(void* Pointer, unsigned int Size) {
        unsigned int begin = (unsigned long long)Pointer - (unsigned long long)&Memory;
        unsigned int was_size = mem[begin];
        void* new_ptr = Alloc(Size);
        memcpy(new_ptr, Pointer, was_size);
        Free(Pointer);
        return new_ptr;
    };

    void Free(void *Pointer) {
        unsigned int begin = (unsigned long long)Pointer - (unsigned long long)&Memory;
        mem.erase(begin);
    };
};

int main() {
    SmallAllocator A1;
    int * A1_P1 = (int *) A1.Alloc(sizeof(int));
    A1_P1 = (int *) A1.ReAlloc(A1_P1, 2 * sizeof(int));
    A1.Free(A1_P1);
    SmallAllocator A2;
    int * A2_P1 = (int *) A2.Alloc(10 * sizeof(int));
    for(unsigned int i = 0; i < 10; i++) A2_P1[i] = i;
    for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 1" << std::endl;
    int * A2_P2 = (int *) A2.Alloc(10 * sizeof(int));
    for(unsigned int i = 0; i < 10; i++) A2_P2[i] = -1;
    for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 2" << std::endl;
    for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 3" << std::endl;
    A2_P1 = (int *) A2.ReAlloc(A2_P1, 20 * sizeof(int));
    for(unsigned int i = 10; i < 20; i++) A2_P1[i] = i;
    for(unsigned int i = 0; i < 20; i++) if(A2_P1[i] != i) std::cout << "ERROR 4" << std::endl;
    for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 5" << std::endl;
    A2_P1 = (int *) A2.ReAlloc(A2_P1, 5 * sizeof(int));
    for(unsigned int i = 0; i < 5; i++) if(A2_P1[i] != i) std::cout << "ERROR 6" << std::endl;
    for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 7" << std::endl;
    A2.Free(A2_P1);
    A2.Free(A2_P2);
    return 0;
}
