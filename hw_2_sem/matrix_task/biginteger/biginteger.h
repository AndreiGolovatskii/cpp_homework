#ifndef BIGINTEGER_H_
#define BIGINTEGER_H_

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <utility>

#include "fast_fourier_transform.cpp"

class BigInteger{
 public:
  BigInteger();
  BigInteger(int64_t x);
  BigInteger(const std::string &s);
  BigInteger(const BigInteger &copy);

  ~BigInteger();

  std::string toString() const;

  friend BigInteger abs(const BigInteger &a);

  friend bool operator<(const BigInteger &a, const BigInteger &b);
  friend bool operator>(const BigInteger &a, const BigInteger &b);
  friend bool operator==(const BigInteger &a, const BigInteger &b);
  friend bool operator<=(const BigInteger &a, const BigInteger &b);
  friend bool operator>=(const BigInteger &a, const BigInteger &b);
  friend bool operator!=(const BigInteger &a, const BigInteger &b);

  explicit operator bool();

  BigInteger &operator+=(const BigInteger &b);
  BigInteger &operator-=(const BigInteger &b);
  BigInteger &operator*=(const BigInteger &b);
  BigInteger &operator/=(const BigInteger &b);
  BigInteger &operator%=(const BigInteger &b);
  BigInteger &operator=(const BigInteger &b);

  const BigInteger operator-() const;
  const BigInteger operator+() const;

 private:
  static const int64_t BASE_ = 100000;
  static const size_t BASE_SIZE_ = 5;
  std::vector<int64_t> values_;
  bool isPositive_;

  void eraseLeadingZeros();
  void uniSum(const BigInteger &b, bool f);
  void sum(const BigInteger &b);
  void diff(const BigInteger &b);
  friend int64_t shortDiv(const BigInteger &a, const BigInteger &b);
  friend bool modLess(const BigInteger &a, const BigInteger &b);
};

BigInteger::BigInteger() {
  isPositive_ = true;
}

BigInteger::BigInteger(int64_t x) {
  if (x < 0) {
    isPositive_ = 0;
    x *= -1;
  } else {
    isPositive_ = 1;
  }
  while (x) {
    values_.push_back(x % BASE_);
    x /= BASE_;
  }
}

BigInteger::BigInteger(const std::string &s) {
  int sum = 0, pow = 1, c = 0;
  isPositive_ = 1;
  for (size_t i = s.size(); i > 0; --i) {
    if (s[i - 1] == '-') {
      isPositive_ = 0;
      break;
    }
    sum += (s[i - 1] - '0') * pow;
    pow *= 10;
    c++;

    if (c == BASE_SIZE_) {
      values_.push_back(sum);
      pow = 1;
      sum = 0;
      c = 0;
    }
  }

  values_.push_back(sum);
  eraseLeadingZeros();
}

BigInteger::BigInteger(const BigInteger &copy) {
  if (*this == copy) {
    isPositive_ = true;
    return;
  }
  values_ = copy.values_;
  isPositive_ = copy.isPositive_;
}

std::string BigInteger::toString() const {
  std::string result;
  for (size_t i = 0; i < values_.size(); ++i) {
    std::string tmp = std::to_string(values_[i]);

    std::reverse(tmp.begin(), tmp.end());

    while (i + 1 != values_.size() && tmp.size() < BASE_SIZE_) {
      tmp.push_back('0');
    }
    result += tmp;
  }
  if (isPositive_ == 0) {
    result.push_back('-');
  }
  std::reverse(result.begin(), result.end());
  if (result.size() == 0) result = "0";
  return result;
}

BigInteger::~BigInteger() {}

BigInteger &BigInteger::operator=(const BigInteger &source)  {
  if (*this == source) {
    return *this;
  }
  values_ = source.values_;
  isPositive_ = source.isPositive_;
  return *this;
}

void BigInteger::eraseLeadingZeros() {
  while (values_.size() && values_.back() == 0) {
    values_.pop_back();
  }
  if (values_.size() == 0) {
    isPositive_ = 1;
  }
}

bool modLess(const BigInteger &a, const BigInteger &b) {
  if (a.values_.size() != b.values_.size()) {
    return a.values_.size() < b.values_.size();
  }
  for (size_t i = a.values_.size(); i > 0; --i) {
    if (a.values_[i - 1] != b.values_[i - 1]) {
      return a.values_[i - 1] < b.values_[i - 1];
    }
  }
  return 0;
}

bool operator<(const BigInteger &a, const BigInteger &b) {
  if (a.isPositive_ != b.isPositive_) {
    return b.isPositive_;
  }
  if (a.isPositive_) {
    return modLess(a, b);
  } else {
    return modLess(b, a);
  }
}

bool operator>(const BigInteger &a, const BigInteger &b) {
  return b < a;
}

bool operator!=(const BigInteger &a, const BigInteger &b) {
  return a < b || b < a;
}

bool operator==(const BigInteger &a, const BigInteger &b) {
  return !(a != b);
}

bool operator<=(const BigInteger &a, const BigInteger &b) {
  return !(a > b);
}

bool operator>=(const BigInteger &a, const BigInteger &b) {
  return !(a < b);
}

const BigInteger BigInteger::operator-() const {
  BigInteger result = *this;
  if (result.values_.size()) {
    result.isPositive_ ^= 1;
  }
  return result;
}

const BigInteger BigInteger::operator+() const {
  BigInteger result = *this;
  return result;
}

void BigInteger::sum(const BigInteger &b) {
  values_.resize(std::max(values_.size(), b.values_.size()) + 1, 0);
  for (size_t i = 0; i < b.values_.size(); ++i) {
    values_[i] += b.values_[i];
    if (values_[i] >= BASE_) {
      values_[i + 1]++;
      values_[i] -= BASE_;
    }
  }
  for (size_t i = b.values_.size(); i < values_.size(); ++i) {
    if (values_[i] >= BASE_) {
      values_[i + 1]++;
      values_[i] -= BASE_;
    }
  }

  eraseLeadingZeros();
}

void BigInteger::diff(const BigInteger &b) {
  values_.resize(std::max(values_.size(), b.values_.size()) + 1, 0);
  for (size_t i = 0; i < b.values_.size(); ++i) {
    values_[i] -= b.values_[i];
    if (values_[i] < 0) {
      values_[i + 1] -= 1;
      values_[i] += BASE_;
    }
  }
  for (size_t i = b.values_.size(); i < values_.size(); ++i) {
    if (values_[i] < 0) {
      values_[i + 1]--;
      values_[i] += BASE_;
    }
  }
  eraseLeadingZeros();
}

void BigInteger::uniSum(const BigInteger &b, bool f) {
  if (f) {
    sum(b);
  } else {
    if (modLess(*this, b)) {
      BigInteger s = *this;
      *this = b;
      diff(s);
      this->isPositive_ = s.isPositive_ ^ 1;
      eraseLeadingZeros();
    } else {
      diff(b);
    }
  }
}

BigInteger &BigInteger::operator+=(const BigInteger &b) {
  uniSum(b, isPositive_ == b.isPositive_);
  return *this;
}

BigInteger &BigInteger::operator-=(const BigInteger &b) {
  uniSum(b, isPositive_ != b.isPositive_);

  return *this;
}

const BigInteger operator-(const BigInteger &a, const BigInteger &b) {
  BigInteger res = a;
  return res -= b;
}


BigInteger operator+(const BigInteger &a, const BigInteger &b) {
  BigInteger res = a;
  return res += b;
}

BigInteger::operator bool() {
  return this->values_.size();
}

BigInteger &operator++(BigInteger &a) {
  return a += 1;
}

BigInteger &operator--(BigInteger &a) {
  return a -= 1;
}

BigInteger operator++(BigInteger &a, int) {
  BigInteger res = a;
  a += 1;
  return res;
}

BigInteger operator--(BigInteger &a, int) {
  BigInteger res = a;
  a -= 1;
  return res;
}

BigInteger &BigInteger::operator*=(const BigInteger &b) {
  std::vector<int64_t> fa = multiply_polynomials(this->values_, b.values_);

  int64_t o = 0;
  for (size_t i = 0; i < fa.size(); ++i) {
    fa[i] += o;
    o = 0;
    if (fa[i] >= BASE_) {
      o = fa[i] / BASE_;
      fa[i] %= BASE_;
    }
  }

  this->values_ = std::move(fa);
  this->isPositive_ ^= !b.isPositive_;
  eraseLeadingZeros();

  return *this;
}

const BigInteger operator*(const BigInteger &a, const BigInteger &b) {
  BigInteger res = a;
  return res *= b;
}

const BigInteger operator/(const BigInteger &a, const BigInteger &b) {
  BigInteger res = a;
  return res /= b;
}

int64_t shortDiv(const BigInteger &a, const BigInteger &b) {
  int64_t r = 2 * a.BASE_, l = -1;
  while (r - l > 1) {
    int64_t mid = (r + l) / 2;
    if (b * mid <= a) {
      l = mid;
    } else {
      r = mid;
    }
  }
  return l;
}

BigInteger abs(const BigInteger &a) {
  BigInteger res = a;
  res.isPositive_ = 1;
  return res;
}

BigInteger &BigInteger::operator/=(const BigInteger &b) {
  BigInteger res;
  res.isPositive_ = this->isPositive_ == b.isPositive_;

  BigInteger divider = abs(b);
  BigInteger divident = abs(*this);
  BigInteger buff;
  int it = static_cast<int>(divident.values_.size());
  while (it > 0) {
    if (it > 0 && modLess(buff, divider)) {
      std::reverse(buff.values_.begin(), buff.values_.end());
      buff.values_.push_back(divident.values_[--it]);
      std::reverse(buff.values_.begin(), buff.values_.end());
    }
    buff.eraseLeadingZeros();
    if (!modLess(buff, divider)) {
      int64_t tmp = shortDiv(buff, divider);
      res.values_.push_back(tmp);
      buff -= divider * tmp;
    } else {
      res.values_.push_back(0);
    }
  }
  std::reverse(res.values_.begin(), res.values_.end());
  res.eraseLeadingZeros();
  // if(this->isPositive_ != b.isPositive_ && res * b != *this) { //math division
  //   res -= 1;
  // }
  *this = res;
  return *this;
}

BigInteger &BigInteger::operator%=(const BigInteger &b) {
  return *this -= *this / b * b;
}

const BigInteger operator%(const BigInteger &a, const BigInteger &b) {
  BigInteger res = a;
  return res %= b;
}

std::istream &operator>>(std::istream &in, BigInteger &a) {
  std::string s;
  in >> s;
  a = BigInteger(s);
  return in;
}

std::ostream &operator<<(std::ostream &out, const BigInteger &a) {
  out << a.toString();
  return out;
}

BigInteger gcd(BigInteger a, BigInteger b) {
  while (b) {
    a %= b;
    std::swap(a, b);
  }
  return a;
}

#endif  //  BIGINTEGER_H_
