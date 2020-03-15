#include "matrix/matrix.h"
#include "finite/finite.h"
#include "biginteger/rational.h"

#include <gtest/gtest.h>

#include <random>
#include <iostream>
#include <algorithm>

using namespace NMatrix;

TEST(MATRIX, COMMON) {
    Matrix<3, 3> a;
    a[1][1] = -1;

    Matrix<1, 7> b;
    //a * b; compile error
    //b.Det(); compile error
    //b.Trace() compile error
    a += a;
    b += b;
    b -= Matrix<1, 7>();
    
    int x = 10;
    x -= x;

    b + b;
    a + a;
    a - a;


    a *= 1;
    a *= 2;
    a /= 2;

    Matrix<7, 1> c;
    b * c;//ok

}

TEST(MATRIX, MUL_1) {
    Matrix<3, 3, int> a{3, -1, 2,
                   4, 2, 0,
                   -5, 6, 1};
    Matrix<3, 1, int> b{8,
                   7, 2};

    auto r = a * b;
    Matrix<3, 1, int> r_correct{21,
                           46,
                           4};

    ASSERT_EQ(r, r_correct);
}

template<unsigned N, unsigned M, typename Field>
Matrix<N, M, Field> RandomMatrix(std::mt19937& rnd) {
    Matrix<N, M, Field> a;
    for (unsigned i = 0; i < N; ++i) {
        for (unsigned j = 0; j < M; ++j) {
            a[i][j] = static_cast<Field>(rnd());
        }
    }
    return a;
}

template<unsigned N, unsigned M>
Matrix<N, M, double> RandomDoubleMatrix(std::mt19937& rnd) {
    Matrix<N, M, double> a;
    for (unsigned i = 0; i < N; ++i) {
        for (unsigned j = 0; j < M; ++j) {
            a[i][j] = static_cast<double>(rnd()) / (1 + std::abs(static_cast<int>(rnd())));
        }
    }
    return a;
}

TEST(MATRIX, MUL_2) {
    std::mt19937 rnd(1);
    
    static const int N = 130;
    static const int M = 111;
    static const int K = 227;

    Matrix<N, M, double> a = RandomDoubleMatrix<N, M>(rnd);
    Matrix<M, K, double> b = RandomDoubleMatrix<M, K>(rnd);
    
    Matrix<N, K, double> res = a * b;
    Matrix<N, K, double> res_correct = a.SimpleMultiply_(b);
    for (unsigned i = 0; i < N; ++i) {
        for (unsigned j = 0; j < K; ++j) {
            ASSERT_NEAR(res[i][j], res_correct[i][j], 1e-6);
        }
    }
}

TEST(MATRIX, MUL_3) {
    std::mt19937 rnd(1);
    
    static const int N = 11;
    static const int M = 23;
    static const int K = 19;

    Matrix<N, M, BigInteger> a = RandomMatrix<N, M, BigInteger>(rnd);
    Matrix<M, K, BigInteger> b = RandomMatrix<M, K, BigInteger>(rnd);
    
    Matrix<N, K, BigInteger> res = a * b;
    Matrix<N, K, BigInteger> res_correct = a.SimpleMultiply_(b);
    for (unsigned i = 0; i < N; ++i) {
        for (unsigned j = 0; j < K; ++j) {
            ASSERT_EQ(res[i][j], res_correct[i][j]);
        }
    }
}

TEST(MATRIX, TEST5) {
    std::mt19937 rnd(1);

    static const int N = 111;
    static const int M = 113;
    static const int K = 211;

    //using Field = Finite<13>;
    using Field = Finite<10>;

    Matrix<N, M, Field> a = RandomMatrix<N, M, Field>(rnd);
    Matrix<M, K, Field> b = RandomMatrix<M, K, Field>(rnd);
    
    Matrix<N, K, Field> res = a * b;
    Matrix<N, K, Field> res_correct = a.SimpleMultiply_(b);
    for (unsigned i = 0; i < N; ++i) {
        for (unsigned j = 0; j < K; ++j) {
            ASSERT_EQ(res[i][j], res_correct[i][j]);
        }
    }
}

TEST(MATRIX, DET_1) {
    Matrix<2, 2, double> m{1, 3,
                           7, 9};
    ASSERT_NEAR(m.Det(), -12.0, 1e-6);

    Matrix<2, 2, double> n{-3, 3,
                           6, -8};
    ASSERT_NEAR(n.Det(), 6.0, 1e-6);
} 

TEST(MATRIX, DET_2) {
    Matrix<3, 3, double> n{-1, 2, -5,
                           4, -1, 3,
                           3, 0, -6};
    ASSERT_NEAR(n.Det(), 45.0, 1e-6);
} 

TEST(MATRIX, RANK_1) {
    Matrix<3, 3, double> n{-1, 2, -5,
                           4, -1, 3,
                           3, 0, -6};
    ASSERT_EQ(n.Rank(), 3);

    Matrix<1, 7, Finite<11>> m{1, 2, 3, 4, 5, 6, 7};
    ASSERT_EQ(m.Rank(), 1);

    Matrix<2, 7, Finite<11>> h{1, 2, 3, 4, 5, 6, 7,
                               1, 2, 3, 4 ,5, 6, 7};
    ASSERT_EQ(h.Rank(), 1);
}

TEST(MATRIX, TRACE_1) {
    Matrix<3, 3, double> n{-1, 2, -5,
                           4, -1, 3,
                           3, 0, -6};
    ASSERT_EQ(n.Trace(), -8);
}

TEST(MATRIX, INVERSE_1) {
    Matrix<3, 3, double> a{-1, 2, -5,
                           4, -1, 3,
                           3, 0, -6};
    Matrix<3, 3, double> res{2.0 / 15, 4.0 / 15, 1.0 / 45,
                             11.0 / 15, 7.0 / 15, -17.0 / 45,
                             1.0 / 15, 2.0 / 15, -7.0 / 45};

    Matrix<3, 3, double> pb = a.inverted();
    a.inverte();

    for (unsigned i = 0; i < 3; ++i) {
        for (unsigned j = 0; j < 3; ++j) {
            ASSERT_NEAR(res[i][j], a[i][j], 1e-6);
            ASSERT_NEAR(res[i][j], pb[i][j], 1e-6);
        }
    }

    /*
    Matrix<3, 4> b;
    b.inverte(); not compile
    b.inverted()
    */ 
}
