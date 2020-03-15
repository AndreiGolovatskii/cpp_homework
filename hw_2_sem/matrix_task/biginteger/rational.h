#ifndef RATIONAL_H_
#define RATIONAL_H_

#include <string>
#include <algorithm>

#include "biginteger.h"

class Rational{
 public:
  Rational();
  Rational(int64_t x);
  Rational(const BigInteger &num, const BigInteger &denom = 1);
  Rational(const Rational &copy);
  ~Rational();

  Rational &operator=(const Rational &b);
  Rational &operator+=(const Rational &b);
  Rational &operator*=(const Rational &b);
  Rational &operator-=(const Rational &b);
  Rational &operator/=(const Rational &b);
  Rational operator-() const;

  friend bool operator<(const Rational &a, const Rational &b);
  friend bool operator==(const Rational &a, const Rational &b);

  std::string toString() const;
  std::string asDecimal(size_t precision = 0) const;
  explicit operator double() const;
  friend Rational rev(const Rational &a);
 private:
  BigInteger num_, denom_;
  void norm(){
    if (denom_ < 0) {
      num_ = -num_;
      denom_ = -denom_;
    }
    if (num_ == 0) {
      denom_ = 1;
      return;
    }
    BigInteger c = gcd(abs(num_), denom_);
    num_ /= c;
    denom_ /= c;
  }
};

Rational::Rational() {
  num_ = 0;
  denom_ = 1;
}

Rational::Rational(int64_t x) {
  this->num_ = x;
  this->denom_ = 1;
  norm();
}

Rational::Rational(const BigInteger &num, const BigInteger &denom) {
  this->num_ = num;
  this->denom_ = denom;
  norm();
}

Rational::Rational(const Rational &copy) {
  if (this == &copy) {
    num_ = 0;
    denom_ = 1;
    return;
  }
  num_ = copy.num_;
  denom_ = copy.denom_;
}

Rational &Rational::operator=(const Rational &b) {
  if (this == &b) {
    return *this;
  }
  num_ = b.num_;
  denom_ = b.denom_;
  return *this;
}

Rational::~Rational() {}

Rational rev(const Rational &a) {
  return Rational(a.denom_, a.num_);
}

Rational &Rational::operator*=(const Rational &b) {
  num_ *= b.num_;
  denom_ *= b.denom_;
  norm();
  return *this;
}

Rational operator*(const Rational &a, const Rational &b) {
  Rational res = a;
  return res *= b;
}

Rational &Rational::operator/=(const Rational &b) {
  return *this *= rev(b);
}

Rational operator/(const Rational &a, const Rational &b) {
  Rational res = a;
  return res /= b;
}

Rational &Rational::operator+=(const Rational &b) {
  num_ = num_ * b.denom_ + denom_ * b.num_;
  denom_ *= b.denom_;
  norm();
  return *this;
}

Rational operator+(const Rational &a, const Rational &b) {
  Rational res = a;
  res += b;
  return res;
}

Rational &Rational::operator-=(const Rational &b) {
  num_ = num_ * b.denom_ - denom_ * b.num_;
  denom_ *= b.denom_;
  norm();
  return *this;
}

Rational operator-(const Rational &a, const Rational &b) {
  Rational res = a;
  return res -= b;
}

Rational Rational::operator-() const {
  Rational res = *this;
  res.num_ = -res.num_;
  return res;
}

bool operator<(const Rational &a, const Rational &b) {
  return a.num_ * b.denom_ < b.num_ * a.denom_;
}

bool operator>(const Rational &a, const Rational &b) {
  return b < a;
}

bool operator>=(const Rational &a, const Rational &b) {
  return !(a < b);
}

bool operator<=(const Rational &a, const Rational &b) {
  return !(a > b);
}

bool operator==(const Rational &a, const Rational &b) {
  return a.num_ == b.num_ && b.denom_ == a.denom_;
}

bool operator!=(const Rational &a, const Rational &b) {
  return !(a == b);
}

std::string Rational::toString() const {
  if (denom_ != 1) {
    return num_.toString() +  "/" + denom_.toString();
  } else {
    return num_.toString();
  }
}

std::string Rational::asDecimal(size_t percision) const {
  std::string mul = "1";
  for (size_t i = 0; i < percision; ++i) {
    mul.push_back('0');
  }
  BigInteger res = num_ * BigInteger(mul) / denom_;
  std::string sign = "";
  if (res < 0) {
    res = -res;
    sign = "-";
  }
  std::string tmp = (res).toString();

  std::reverse(tmp.begin(), tmp.end());
  tmp.resize(std::max(tmp.size(), percision + 1), '0');
  std::reverse(tmp.begin(), tmp.end());

  if (percision != 0) {
    return sign + tmp.substr(0, tmp.size() - percision) + "." + tmp.substr(tmp.size() - percision);
  } else {
    return sign + tmp.substr(0, tmp.size() - percision);
  }
}

Rational abs(const Rational &a) {
  if (a < 0) {
    return -a;
  }
  return a;
}

Rational::operator double() const {
  if (*this == 0) {
    return 0.0;
  }
  Rational tmp = *this;
  double res = 1;
  while (abs(tmp) >= 10) {
    tmp.denom_ *= 10;
    res *= 10;
  }
  while (abs(tmp) < 1) {
    tmp.num_ *= 10;
    res /= 10;
  }

  res *= std::stod(tmp.asDecimal(20));
  return res;
}

#endif  // RATIONAL_H_
