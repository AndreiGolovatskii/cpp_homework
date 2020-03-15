#pragma once

struct false_type
{
    typedef false_type type;
    static const bool value = false;
};

struct true_type {
    typedef true_type type;
    static const bool value = true;
};

template<bool condition, class T, class U>
struct if_ 
{
    typedef U type;
};

template<class T, class U>
struct if_<true, T, U> { typedef T type;
};

template<int N, int c>
struct is_prime_rec 
{
    typedef typename if_<((N % c) == 0),
                         false_type,
                         typename is_prime_rec<N, c - 1>::type>::type type;
};

template<int N>
struct is_prime_rec<N, 1>
{
    typedef true_type type;
};

template<int N>
struct is_prime_rec<1, N>
{ 
    typedef false_type type;
};

template<int N>
struct is_prime 
{
    static const bool value = is_prime_rec<N, N / 2>::type::value;
};

template<bool B, class T = void>
struct enable_if {};

template<class T>
struct enable_if<true, T> {
    typedef T type;
};

template<unsigned N>
struct n_store {
    static const unsigned value = N;
};

template<unsigned N>
struct up_to_2_pow {
    static const unsigned value = if_<(N & (N - 1)) == 0,
                                       n_store<N>,
                                       up_to_2_pow<N + 1>>::type::value;
                                        
};

template<typename A, typename B>
struct is_same: false_type {
};

template<typename A>
struct is_same<A, A>: true_type {
};

