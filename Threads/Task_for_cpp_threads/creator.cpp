#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

int main() {
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> uni(1, 3);
    std::uniform_real_distribution<double> unid(0.0, 1.0);
    for (int i = 0; i < 10; i++) {
        std::stringstream ss;
        ss << "in" << i << ".dat";
        std::fstream in(ss.str(), std::fstream::out);
        in << uni(rng) << "\n";
        in << unid(rng) << " " << unid(rng) << "\n";
        in.close();
    }
    return 0;
}