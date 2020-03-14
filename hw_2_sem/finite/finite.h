#include"common/type_traits.h"

template<unsigned N, class Enable = void>
class Finite;

template<unsigned N>
class Finite<N, typename enable_if<(N > 0)>::type>
{
public:
    Finite(): Value_(0) {};
    explicit Finite(signed int val): Value_(val) {
        Normalize_();
    }

    Finite<N>& operator=(const Finite<N>& rhs) {
        Value_ = rhs.Value_;
        return *this;
    }
    
    Finite<N>& operator+=(const Finite<N>& rhs) {
        Value_ += rhs.Value_;
        Normalize_();
        return *this;
    }

    Finite<N>& operator-=(const Finite<N>& rhs) {
        Finite<N> res = *this;
        res -= rhs;
        return res;
    }

    Finite<N>& operator*=(const Finite<N>& rhs) {
        long long v = static_cast<long long>(Value_) * rhs.Value_;
        v %= N;
        Value_ = v;
        Normalize_();
    }

    template<unsigned NC = N>
    typename enable_if<(is_prime<N>::value && N == NC), Finite<N>&>::type
    operator/=(const Finite<N>& rhs) {
        *this *= rhs.Reverse();
        return *this;
    }

    template<unsigned NC = N>
    typename enable_if<!(is_prime<N>::value && N == NC), Finite<N>&>::type
    operator/=(const Finite<N>& rhs) = delete;

    template<unsigned NC = N>
    typename enable_if<(is_prime<N>::value && N == NC), Finite<N>>::type
    Reverse() const {
        return this->Pow(N - 2);
    } 

    template<unsigned NC = N>
    typename enable_if<!(is_prime<N>::value && N == NC), Finite<N>>::type
    Reverse() const = delete;

    Finite<N> operator+(const Finite<N>& rhs) const {
        Finite<N> res = *this;
        res += rhs;
        return res;
    }

    Finite<N> operator-(const Finite<N>& rhs) const {
        Finite<N> res = *this;
        res -= rhs;
        return res;
    }

    Finite<N> operator*(const Finite<N>& rhs) const {
        Finite<N> res = *this;
        res *= rhs;
        return res;
    }

    template<unsigned NC = N>
    typename enable_if<(is_prime<N>::value && N == NC), Finite<N>>::type
    operator/(const Finite<N>& rhs) const {
        Finite<N> res = *this;
        res *= rhs.Reverse();
        return res; 
    }

    template<unsigned NC = N>
    typename enable_if<!(is_prime<N>::value && N == NC), Finite<N>>::type
    operator/(const Finite<N>& rhs) const = delete; 
     
    Finite<N> Pow(int k) const {
        Finite<N> res(1);
        Finite<N> a = *this;

        while(k) {
            if (k & 1) {
                res *= a;
            }
            a *= a;
            k >>= 1;
        }
        
        return res;
    }

    int Representative() const {
        return Value_;
    }

    ~Finite() {};
private:
    signed int Value_;

    void Normalize_() {
        if (Value_ < -N) {
            Value_ %= N;
        }
        if (Value_ < 0) {
            Value_ += N;
        } 

        if (Value_ >= N) {
            Value_ %= N;
        }
    }
};

