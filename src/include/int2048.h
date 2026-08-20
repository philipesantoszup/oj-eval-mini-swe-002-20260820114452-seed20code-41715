#pragma once
#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <utility>

namespace sjtu {
class int2048 {
private:
    bool is_positive;
    std::vector<long long> digits;

    void normalize();
    static int compare_abs(const int2048 &, const int2048 &);
    static int2048 add_abs(const int2048 &, const int2048 &);
    static int2048 subtract_abs(const int2048 &, const int2048 &);
    static int2048 multiply_abs_naive(const int2048 &, const int2048 &);
    static int2048 multiply_abs_fft(const int2048 &, const int2048 &);
    static std::pair<int2048, int2048> divide_abs(const int2048 &, const int2048 &);

public:
  int2048();
  int2048(long long);
  int2048(const std::string &);
  int2048(const int2048 &);

  void read(const std::string &);
  void print();

  int2048 &add(const int2048 &);
  friend int2048 add(int2048, const int2048 &);

  int2048 &minus(const int2048 &);
  friend int2048 minus(int2048, const int2048 &);

  int2048 operator+() const;
  int2048 operator-() const;

  int2048 &operator=(const int2048 &);

  int2048 &operator+=(const int2048 &);
  friend int2048 operator+(int2048, const int2048 &);

  int2048 &operator-=(const int2048 &);
  friend int2048 operator-(int2048, const int2048 &);

  int2048 &operator*=(const int2048 &);
  friend int2048 operator*(int2048, const int2048 &);

  int2048 &operator/=(const int2048 &);
  friend int2048 operator/(int2048, const int2048 &);

  int2048 &operator%=(const int2048 &);
  friend int2048 operator%(int2048, const int2048 &);

  friend std::istream &operator>>(std::istream &, int2048 &);
  friend std::ostream &operator<<(std::ostream &, const int2048 &);

  friend bool operator==(const int2048 &, const int2048 &);
  friend bool operator!=(const int2048 &, const int2048 &);
  friend bool operator<(const int2048 &, const int2048 &);
  friend bool operator>(const int2048 &, const int2048 &);
  friend bool operator<=(const int2048 &, const int2048 &);
  friend bool operator>=(const int2048 &, const int2048 &);
};
} // namespace sjtu

#endif
