#include <stdexcept>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <format>

#include "../include/WideInt.h"


WideInt::WideInt(const std::string &num) {
    bool is_zero = true;

    // Actually `start` and `end` show index of the first and the last
    // non-zero digit in number respectively
    int start, end;
    int point = num.size();

    sign = num[0] == '-';

    for (int i = sign; i < num.size(); ++i) {
        char c = num[i];

        if (c > '0' && c <= '9') {
            if (is_zero) {
                start = i;
                is_zero = false;
            }
            end = i;
        } else if (c == '.') {
            point = i;
        } else if (c != '0') {
            throw std::runtime_error(
                    std::format("Unexpected symbol '{}' at position {}", c, i));
        }
    }

    if (is_zero) {
        exp = 0;
        return;
    }

    // We move a point until number turns into the smallest integer
    // TODO: I believe it can be done much simpler
    if (point > end) {
        exp = -((end - point + 1) / PART_SIZE);
        point += -exp * PART_SIZE;
    } else {
        exp = -(((end - point) + (PART_SIZE - 1)) / PART_SIZE);
        point += -exp * PART_SIZE + 1;
    }
    base offset = std::pow(10, point - end - 1);
    int j = point - end - 1;

    // FIXME: `point - start` may count point symbol
    parts = std::vector<base>(((point - start) + (PART_SIZE - 1)) / PART_SIZE, 0);

    // TODO: Point symbol makes impossible to use `i` as digit counter
    //       and adds an extra `if` in loop. Workaround needed
    for (int i = end; i >= start; --i) {
        if (num[i] == '.')
            continue;

        parts[j / PART_SIZE] += (num[i] - '0') * offset;
        offset *= 10;
        if (offset >= PART_MAX)
            offset = 1;
        ++j;
    }

    int test = 0;
}

void WideInt::print() {
    using namespace std;

    if (sign) {
        cout << '-';
    }

    for (int i = 0; i < parts.size(); ++i) {
        cout << setfill('0') << setw(9) << parts[i];
    }

    cout << endl;
};

int8_t WideInt::compare(const WideInt &other) {
    if (sign < other.sign) {
        return 1;
    } else if (sign > other.sign) {
        return -1;
    }

    int8_t abs_cmp = 0;
}

WideInt WideInt::operator-() {
    WideInt w(*this);
    w.sign = !w.sign;
    return w;
}

int main(void) {
    WideInt a = 0.01_w;
    a.print();
}
