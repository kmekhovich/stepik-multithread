#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <random>
#include <mutex>

class Calculator {
public:
    explicit Calculator(int modulas) : modulas_(modulas) {};

    static void Run() {
        std::vector<std::thread> v;
        v.reserve(kCountThreads);
        for (int i = 0; i < kCountThreads; i++) {
            v.emplace_back(Calculator{i});
        }
        for (auto& it : v) {
            it.join();
        }

        std::fstream out("out.dat", std::fstream::out);
        out << result;
        out.close();
    }

    void operator()() {
        while (modulas_ < kCountFiles) {
            std::stringstream ss;
            ss << "in" << modulas_ << ".dat";
            std::fstream in(ss.str());
            int type;
            in >> type;
            double a, b;
            in >> a >> b;
            double now_result{0};
            switch (type) {
                case 1:
                    now_result = a + b;
                    break;
                case 2:
                    now_result = a * b;
                    break;
                case 3:
                    now_result = a * a + b * b;
                    break;
                default:
                    break;
            }
            in.close();
            m.lock();
            result += now_result;
            std::cout << "added " << now_result << " from #" << modulas_ << " file\n";
            m.unlock();
            modulas_ += kCountThreads;
        }
    }

private:
    int modulas_;
    static const int kCountThreads = 4;
    static const int kCountFiles = 10;
    static std::mutex m;
    static double result;
};

std::mutex Calculator::m;
double Calculator::result = 0;

int main() {
    Calculator::Run();
    return 0;
}