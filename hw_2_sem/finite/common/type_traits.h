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
