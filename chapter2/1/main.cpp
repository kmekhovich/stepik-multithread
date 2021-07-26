#include <iostream>
#include <string>
#include <map>
#include <vector>

void ProcessComponent(const std::string::iterator& begin,
                      const std::string::iterator& end,
                      std::map<int, int>* polynom,
                      bool negate) {
    auto it = begin;
    int coff = 0;
    if (*it == 'x') {
        coff = 1;
    } else {
        while (it != end && *it >= '0' && *it <= '9') {
            coff = coff * 10 + *it - '0';
            ++it;
        }
    }
    it = end - 1;
    std::vector<int> reverse_power;
    if (*it == 'x') {
        reverse_power.push_back(1);
    } else {
        while (it != begin && *it >= '0' && *it <= '9') {
            reverse_power.push_back(*it - '0');
            --it;
        }
    }
    int power = 0;
    for (auto iter = reverse_power.rbegin(); iter != reverse_power.rend(); iter++) {
        power = power * 10 + *iter;
    }
    if (negate) {
        coff *= -1;
    }
    (*polynom)[power] += coff;
}

std::string derivative(std::string polynomial) {
    std::map<int, int> polynom;
    std::vector<bool> negate;
    if (!polynomial.empty() && polynomial[0] == '-') {
        negate.push_back(true);
        polynomial.erase(0, 1);
    } else if (!polynomial.empty() && polynomial[0] == '+') {
        negate.push_back(false);
        polynomial.erase(0, 1);
    } else if(!polynomial.empty()) {
        negate.push_back(false);
    }
    for (char& i : polynomial) {
        if (i == '+') {
            negate.push_back(false);
        } else if (i == '-') {
            negate.push_back(true);
            i = '+';
        }
    }
    int last_begin = 0;
    auto negate_iter = negate.begin();
    for (int i = 0; i < polynomial.size(); i++) {
        if (polynomial[i] == '+') {
            ProcessComponent(polynomial.begin() + last_begin,
                             polynomial.begin() + i, &polynom,
                             *(negate_iter++));
            last_begin = i + 1;
        }
    }
    ProcessComponent(polynomial.begin() + last_begin,
                     polynomial.end(), &polynom,
                     *(negate_iter++));

    std::string answer;
    for (auto iter = polynom.rbegin(); iter != polynom.rend(); iter++) {
        int new_coff = iter->first * iter->second;
        int new_power = iter->first - 1;
        if (new_power == -1) {
            continue;
        }
        if (new_coff == 0) {
            continue;
        }
        if (!answer.empty()) {
            if (new_coff > 0) {
                answer += "+";
            }
        }
        bool wrote_coff = false;
        if (new_coff != 1 || new_power == 0) {
            answer += std::to_string(new_coff);
            wrote_coff = true;
        }
        if (new_power >= 1) {
            if (wrote_coff) {
                answer += "*";
            }
            answer += "x";
        }
        if (new_power > 1) {
            answer += "^";
            answer += std::to_string(new_power);
        }
    }

    return answer;
}

int main() {
    std::string s;
    std::cin >> s;
    std::cout << derivative(s);
    return 0;
}
