#ifndef FAST_FOURIER_TRANSFORM
#define FAST_FOURIER_TANSSFORM

#include <assert.h>
#include <complex>
#include <vector>
#include <algorithm>

std::vector<int> revBit(int log_n) {
  int n = 1 << log_n;

  std::vector<int> rev(n, 0);
  for (int i = 0; i < n; ++i) {
    rev[i] =(rev[i >> 1] >> 1) + ((i & 1) << (log_n - 1));
  }
  return rev;
}


std::vector<std::complex<double>> roots(int log_n) {
  int n = 1 << log_n;

  std::vector<std::complex<double>> root(n, 0);
  for (int k = 1; k < n; k *= 2) {
    std::complex<double> tmp(std::cos(M_PI / k), std::sin(M_PI / k));

    root[k] = {1, 0};
    for (int i = 1; i < k; ++i) {
      root[k + i] = root[(k + i) >> 1];
      if ((i & 1)) root[k + i] *= tmp;
    }
  }
  return root;
}

std::vector<std::complex<double>> fft(const std::vector<std::complex<double>> &in,
                                      const std::vector<int> &rev,
                                      const std::vector<std::complex<double>> &root) {
  int n = static_cast<int>(in.size());
  std::vector<std::complex<double>> result(n);

  for (int i = 0; i < n; ++i) {
    result[rev[i]] = in[i];
  }

  for (int k = 1; k < n; k *= 2) {
    for (int i = 0; i < n; i += 2 * k) {
      for (int j = 0; j < k; ++j) {
        std::complex<double> tmp = root[k + j] * result[i + j + k];
        result[i + j + k] = result[i + j] - tmp;
        result[i + j] = result[i + j] + tmp;
      }
    }
  }
  return result;
}


std::vector<std::complex<double>> fft_inverse(const std::vector<std::complex<double>> &in,
                                              const std::vector<int> &rev,
                                              const std::vector<std::complex<double>> &root) {
  std::vector<std::complex<double>> result = fft(in, rev, root);
  std::reverse(result.begin() + 1, result.end());
  for (size_t i = 0; i < result.size(); ++i) {
    result[i] /= static_cast<int>(result.size());
  }
  return result;
}

std::vector<std::complex<double>> toComplex(const std::vector<int64_t> &a, int log_n) {
  std::vector<std::complex<double>> result(1 << log_n, 0);
  for (size_t i = 0; i < a.size(); ++i) {
    result[i] = static_cast<std::complex<double>>(static_cast<double>(a[i]));
  }
  return result;
}

std::vector<int64_t> toLongLong(const std::vector<std::complex<double>> &a) {
  std::vector<int64_t> result(a.size(), 0);
  for (size_t i = 0; i < a.size(); ++i) {
    result[i] = static_cast<int64_t>(round(a[i].real()));
  }
  return result;
}
/* TODO: fix that
std::vector<std::complex<double>> twoInOne(const std::vector<int64_t> &a,
                                           const std::vector<int64_t> &b,
                                           int log_n) {
  std::vector<std::complex<double>> c(1 << log_n, std::complex<double>(0.0, 0.0));
  for (size_t i = 0; i < a.size(); ++i) {
    c[i].real(a[i]);
  }
  for (size_t i = 0; i < b.size(); ++i) {
    c[i].imag(b[i]);
  }
  return c;
}

std::pair<std::vector<std::complex<double>>,
          std::vector<std::complex<double>>> separate(const std::vector<std::complex<double>> &res) {
  std::vector<std::complex<double>> a(res.size()), b(res.size());
  for (size_t i = 0; i < res.size(); ++i) {
    a[i] = 0.5 * (res[i] + std::conj(res[res.size() - i - 1]));
    b[i] = 0.5 * (-res[i] + std::conj(res[res.size() - i - 1])) * std::complex<double>(0, 1);
  }
  return make_pair(a, b);
}
*/

std::vector<int64_t> multiply_polynomials(const std::vector<int64_t> &a, const std::vector<int64_t> &b) {
  int log_n = 0;
  while ((1 << log_n) < 2 * static_cast<int>(std::max(a.size(), b.size()))) {
    ++log_n;
  }

  std::vector<int> rev = revBit(log_n);
  std::vector<std::complex<double>> root = roots(log_n);

  std::vector<std::complex<double>> ra = fft(toComplex(a, log_n), rev, root);
  std::vector<std::complex<double>> rb = fft(toComplex(b, log_n), rev, root);

  for (size_t i = 0; i < ra.size(); ++i) {
    ra[i] *= rb[i];
  }

  std::vector<std::complex<double>> result = fft_inverse(ra, rev, root);

  return toLongLong(result);
}

#endif
