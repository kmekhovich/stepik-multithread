#include <iostream>
#include <tbb/tbb.h>
#include <fstream>


template<typename T>
void ParallelMergeSort(std::vector<T>& array, int start = 0, int end = -1) {
    if (end == -1) {
        end = array.size() - 1;
    }
    int sz = end - start + 1;
    if (sz <= 1) {
        return;
    } else if (sz == 2) {
        if (array[start] > array[end]) {
            std::swap(array[start], array[end]);
        }
    } else {
        int mid = start + sz / 2;
        tbb::parallel_invoke([&] { ParallelMergeSort(array, start, mid); },
                [&] { ParallelMergeSort(array, mid + 1, end); });
        // merging (start..mid) with (mid+1..end) to (start..end)
        std::vector<T> buffer;
        std::merge(array.begin() + start, array.begin() + mid + 1,
                   array.begin() + mid + 1, array.begin() + end + 1,
                   std::back_inserter(buffer));
        auto it = buffer.begin();
        for (int i = start; i <= end; i++, it++) {
            array[i] = *it;
        }
    }
}

int main() {
    std::fstream in("data.txt");
    std::vector<int> a;
    while (!in.eof()) {
        int last;
        in >> last;
        a.push_back(last);
    }
    a.pop_back();
    ParallelMergeSort(a);
    for (auto& it : a) {
        std::cout << it << " ";
    }

    return 0;
}