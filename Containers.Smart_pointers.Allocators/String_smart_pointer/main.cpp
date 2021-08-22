#include <iostream>
#include <string>

class StringPointer {
public:
    StringPointer(std::string *Pointer) : ptr(Pointer) {}
    ~StringPointer() {
        if (own_string) {
            delete ptr;
        }
    }
    std::string* operator->() {
        if (!ptr) {
            ptr = new std::string;
            own_string = true;
        }
        return ptr;
    }
    operator std::string*() {
        if (!ptr) {
            ptr = new std::string;
            own_string = true;
        }
        return ptr;
    }

private:
    std::string* ptr;
    bool own_string{false};
};

int main() {
    std::string s1 = "Hello, world!";

    StringPointer sp1(&s1);
    StringPointer sp2(NULL);

    std::cout << sp1->length() << std::endl;
    std::cout << *sp1 << std::endl;
    std::cout << sp2->length() << std::endl;
    std::cout << *sp2 << std::endl;
    return 0;
}
