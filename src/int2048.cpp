#include "int2048.h"
#include <algorithm>
#include <cctype>
#include <cmath>

namespace sjtu {

const long long BASE = 1000000000; // 10^9

void int2048::normalize() {
    while (!digits.empty() && digits.back() == 0) {
        digits.pop_back();
    }
    if (digits.empty()) {
        digits.push_back(0);
        is_positive = true;
    }
}

int int2048::compare_abs(const int2048 &a, const int2048 &b) {
    if (a.digits.size() != b.digits.size()) {
        return a.digits.size() > b.digits.size() ? 1 : -1;
    }
    for (int i = a.digits.size() - 1; i >= 0; --i) {
        if (a.digits[i] != b.digits[i]) {
            return a.digits[i] > b.digits[i] ? 1 : -1;
        }
    }
    return 0;
}

int2048 int2048::add_abs(const int2048 &a, const int2048 &b) {
    int2048 res;
    res.is_positive = true;
    long long carry = 0;
    int i = 0;
    while (i < a.digits.size() || i < b.digits.size() || carry) {
        long long sum = carry;
        if (i < a.digits.size()) sum += a.digits[i];
        if (i < b.digits.size()) sum += b.digits[i];
        res.digits.push_back(sum % BASE);
        carry = sum / BASE;
        i++;
    }
    res.normalize();
    return res;
}

int2048 int2048::subtract_abs(const int2048 &a, const int2048 &b) {
    // a >= b
    int2048 res;
    res.is_positive = true;
    long long borrow = 0;
    int i = 0;
    while (i < a.digits.size() || borrow) {
        long long diff = (i < a.digits.size() ? a.digits[i] : 0) - borrow;
        if (i < b.digits.size()) {
            diff -= b.digits[i];
        }
        if (diff < 0) {
            diff += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        res.digits.push_back(diff);
        i++;
    }
    res.normalize();
    return res;
}

int2048 int2048::multiply_abs(const int2048 &a, const int2048 &b) {
    int2048 res;
    res.is_positive = true;
    res.digits.resize(a.digits.size() + b.digits.size(), 0);
    for (int i = 0; i < a.digits.size(); i++) {
        long long carry = 0;
        for (int j = 0; j < b.digits.size() || carry; j++) {
            long long product = res.digits[i + j] + carry;
            if (j < b.digits.size()) {
                product += a.digits[i] * b.digits[j];
            }
            res.digits[i + j] = product % BASE;
            carry = product / BASE;
        }
    }
    res.normalize();
    return res;
}

std::pair<int2048, int2048> int2048::divide_abs(const int2048 &dividend, const int2048 &divisor) {
    int2048 quotient;
    quotient.is_positive = true;
    int2048 remainder = dividend;
    remainder.is_positive = true;

    if (compare_abs(dividend, divisor) < 0) {
        quotient.digits.push_back(0);
        return {quotient, remainder};
    }

    int shift = dividend.digits.size() - divisor.digits.size();
    quotient.digits.resize(shift + 1, 0);

    for (int i = shift; i >= 0; i--) {
        int2048 temp_divisor = divisor;
        temp_divisor.digits.insert(temp_divisor.digits.begin(), i, 0);
        temp_divisor.normalize();

        long long low = 0, high = BASE - 1;
        long long q = 0;
        while (low <= high) {
            long long mid = (low + high) / 2;
            int2048 product;
            product.is_positive = true;
            long long carry = 0;
            for (int j = 0; j < temp_divisor.digits.size() || carry; j++) {
                long long p = carry;
                if (j < temp_divisor.digits.size()) {
                    p += temp_divisor.digits[j] * mid;
                }
                product.digits.push_back(p % BASE);
                carry = p / BASE;
            }
            product.normalize();
            if (compare_abs(product, remainder) <= 0) {
                q = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        if (q != 0) {
            quotient.digits[i] = q;
            int2048 product;
            product.is_positive = true;
            long long carry = 0;
            for (int j = 0; j < temp_divisor.digits.size() || carry; j++) {
                long long p = carry;
                if (j < temp_divisor.digits.size()) {
                    p += temp_divisor.digits[j] * q;
                }
                product.digits.push_back(p % BASE);
                carry = p / BASE;
            }
            product.normalize();
            remainder = subtract_abs(remainder, product);
        }
    }

    quotient.normalize();
    remainder.normalize();
    return {quotient, remainder};
}

int2048::int2048() : is_positive(true) {
    digits.push_back(0);
}

int2048::int2048(long long n) {
    if (n == 0) {
        is_positive = true;
        digits.push_back(0);
        return;
    }
    is_positive = (n > 0);
    n = llabs(n);
    while (n > 0) {
        digits.push_back(n % BASE);
        n /= BASE;
    }
}

int2048::int2048(const std::string &s) {
    read(s);
}

int2048::int2048(const int2048 &other) : is_positive(other.is_positive), digits(other.digits) {}

void int2048::read(const std::string &s) {
    is_positive = true;
    digits.clear();
    if (s.empty()) {
        digits.push_back(0);
        return;
    }
    int start = 0;
    if (s[0] == '-') {
        is_positive = false;
        start = 1;
    } else if (s[0] == '+') {
        start = 1;
    }
    while (start < s.size() && s[start] == '0') {
        start++;
    }
    if (start == s.size()) {
        digits.push_back(0);
        is_positive = true;
        return;
    }
    int len = s.size() - start;
    int num_groups = (len + 8) / 9;
    digits.resize(num_groups);
    int idx = 0;
    for (int i = s.size() - 1; i >= start; i -= 9) {
        long long group = 0;
        int group_start = std::max(start, i - 8);
        for (int j = group_start; j <= i; j++) {
            group = group * 10 + (s[j] - '0');
        }
        digits[idx++] = group;
    }
    normalize();
}

void int2048::print() {
    if (!is_positive && !(digits.size() == 1 && digits[0] == 0)) {
        std::cout << '-';
    }
    if (digits.empty()) {
        std::cout << 0;
        return;
    }
    std::cout << digits.back();
    char prev = std::cout.fill('0');
    for (int i = digits.size() - 2; i >= 0; i--) {
        std::cout.width(9);
        std::cout << digits[i];
    }
    std::cout.fill(prev);
}

int2048 &int2048::add(const int2048 &other) {
    *this = *this + other;
    return *this;
}

int2048 add(int2048 a, const int2048 &b) {
    return a + b;
}

int2048 &int2048::minus(const int2048 &other) {
    *this = *this - other;
    return *this;
}

int2048 minus(int2048 a, const int2048 &b) {
    return a - b;
}

int2048 int2048::operator+() const {
    return *this;
}

int2048 int2048::operator-() const {
    int2048 res(*this);
    if (!(res.digits.size() == 1 && res.digits[0] == 0)) {
        res.is_positive = !res.is_positive;
    }
    return res;
}

int2048 &int2048::operator=(const int2048 &other) {
    if (this != &other) {
        is_positive = other.is_positive;
        digits = other.digits;
    }
    return *this;
}

int2048 &int2048::operator+=(const int2048 &other) {
    *this = *this + other;
    return *this;
}

int2048 operator+(int2048 a, const int2048 &b) {
    int2048 res;
    if (a.is_positive == b.is_positive) {
        res = int2048::add_abs(a, b);
        res.is_positive = a.is_positive;
    } else {
        int cmp = int2048::compare_abs(a, b);
        if (cmp == 0) {
            res = int2048(0);
        } else if (cmp > 0) {
            res = int2048::subtract_abs(a, b);
            res.is_positive = a.is_positive;
        } else {
            res = int2048::subtract_abs(b, a);
            res.is_positive = b.is_positive;
        }
    }
    res.normalize();
    return res;
}

int2048 &int2048::operator-=(const int2048 &other) {
    *this = *this - other;
    return *this;
}

int2048 operator-(int2048 a, const int2048 &b) {
    return a + (-b);
}

int2048 &int2048::operator*=(const int2048 &other) {
    *this = *this * other;
    return *this;
}

int2048 operator*(int2048 a, const int2048 &b) {
    int2048 res = int2048::multiply_abs(a, b);
    if (res.digits.size() == 1 && res.digits[0] == 0) {
        res.is_positive = true;
    } else {
        res.is_positive = (a.is_positive == b.is_positive);
    }
    res.normalize();
    return res;
}

int2048 &int2048::operator/=(const int2048 &other) {
    *this = *this / other;
    return *this;
}

int2048 operator/(int2048 dividend, const int2048 &divisor) {
    auto [quotient, remainder] = int2048::divide_abs(dividend, divisor);
    int2048 res = quotient;
    if (res.digits.size() == 1 && res.digits[0] == 0) {
        res.is_positive = true;
    } else {
        res.is_positive = (dividend.is_positive == divisor.is_positive);
    }
    if (int2048::compare_abs(remainder, int2048(0)) != 0) {
        if (dividend.is_positive != divisor.is_positive) {
            res = int2048::subtract_abs(res, int2048(1));
            if (!(res.digits.size() == 1 && res.digits[0] == 0)) {
                res.is_positive = !res.is_positive;
            }
        }
    }
    res.normalize();
    return res;
}

int2048 &int2048::operator%=(const int2048 &other) {
    *this = *this % other;
    return *this;
}

int2048 operator%(int2048 dividend, const int2048 &divisor) {
    int2048 quotient = dividend / divisor;
    return dividend - quotient * divisor;
}

std::istream &operator>>(std::istream &is, int2048 &num) {
    std::string s;
    is >> s;
    num.read(s);
    return is;
}

std::ostream &operator<<(std::ostream &os, const int2048 &num) {
    if (!num.is_positive && !(num.digits.size() == 1 && num.digits[0] == 0)) {
        os << '-';
    }
    if (num.digits.empty()) {
        os << 0;
        return os;
    }
    os << num.digits.back();
    char prev = os.fill('0');
    for (int i = num.digits.size() - 2; i >= 0; i--) {
        os.width(9);
        os << num.digits[i];
    }
    os.fill(prev);
    return os;
}

bool operator==(const int2048 &a, const int2048 &b) {
    if (a.is_positive != b.is_positive) {
        return false;
    }
    return int2048::compare_abs(a, b) == 0;
}

bool operator!=(const int2048 &a, const int2048 &b) {
    return !(a == b);
}

bool operator<(const int2048 &a, const int2048 &b) {
    if (a.is_positive != b.is_positive) {
        return !a.is_positive;
    }
    if (a.is_positive) {
        return int2048::compare_abs(a, b) < 0;
    } else {
        return int2048::compare_abs(a, b) > 0;
    }
}

bool operator>(const int2048 &a, const int2048 &b) {
    return b < a;
}

bool operator<=(const int2048 &a, const int2048 &b) {
    return !(a > b);
}

bool operator>=(const int2048 &a, const int2048 &b) {
    return !(a < b);
}

} // namespace sjtu
