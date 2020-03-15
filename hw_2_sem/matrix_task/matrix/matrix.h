#pragma once

#include "biginteger/rational.h"
#include "common/type_traits.h"

#include <vector>
#include <iostream>
#include <tuple>

namespace NMatrix {
    template<unsigned N, unsigned M, typename Field>
    class Matrix;

    template<unsigned N, typename Field=Rational>
    using SquareMatrix=Matrix<N, N, Field>;

    namespace {
        template<unsigned N, typename Field>
        Matrix<N, N, Field> StrassenRec(const Matrix<N, N, Field>& lhs,
                                        const Matrix<N, N, Field>& rhs);

        template<unsigned N, typename Field>
        struct SquareMatrixSquad;

        template<unsigned N, typename Field>
        SquareMatrixSquad<N / 2, Field> GetSubmatrices(const Matrix<N, N, Field> &src);

        template<unsigned N, typename Field>
        Matrix<N * 2, N * 2, Field> FromSubmatrices(const SquareMatrixSquad<N, Field>& src);
    }

    template<unsigned M, typename Field=Rational>
    class Row {
    public:
        Row(): Values_(std::vector<Field>(M, 0)){
        }

        Row(const Row<M, Field>& rhs): Values_(rhs.Values_) {
        }

        Row& operator=(const Row<M, Field>& rhs) {
            Values_ = rhs.Values_;
            return *this;
        }

        Row& operator-=(const Row<M, Field>& rhs) {
            for (unsigned i = 0; i < M; ++i) {
                Values_[i] -= rhs.Values_[i];
            }
            return *this;
        }

        Row operator-(const Row<M, Field>& rhs) const {
            Row res = *this;
            res -= rhs;
            return res; 
        }

        Row& operator+=(const Row<M, Field>& rhs) {
            for (unsigned i = 0; i < M; ++i) {
                Values_[i] += rhs.Values_[i];
            }
            return *this;
        }

        Row operator+(const Row<M, Field>& rhs) const {
            Row res = *this;
            res += rhs;
            return res; 
        }

        Row& operator*=(const Field rhs) {
            for (unsigned i = 0; i < M; ++i) {
                Values_[i] *= rhs;
            }
            return *this;
        }

        Row operator*(const Field rhs) const {
            Row res = *this;
            res *= rhs;
            return res;
        }
        
        Row& operator/=(const Field rhs) {
            for (unsigned i = 0; i < M; ++i) {
                Values_[i] /= rhs;
            }
            return *this;
        }

        Row operator/(const Field rhs) const {
            Row res = *this;
            res /= rhs;
            return res;
        }

        bool operator==(const Row<M, Field>& rhs) const {
            return Values_ == rhs.Values_;
        }

        Field& operator[](int i) {
            return Values_[i]; 
        }

        const Field& operator[](int i) const {
            return Values_[i];
        }

        void swap(Row<M, Field>& rhs) {
            std::swap(Values_, rhs.Values_);
        }


    private:
        std::vector<Field> Values_;
    };

    template<unsigned M, typename Field>
    void swap(Row<M, Field>& lhs, Row<M, Field>& rhs) {
        lhs.swap(rhs);
    }


    template<unsigned N, unsigned M, typename Field=Rational>
    class Matrix {
    public:
        Matrix(): Values_(std::vector<Row<M, Field>>(N)) {
        }

        Matrix(std::initializer_list<Field> src): Values_(std::vector<Row<M, Field>>(N)) {
            auto it = src.begin();
            for (unsigned i = 0; i < N; ++i) {
                for (unsigned j = 0; j < M; ++j) {
                    Values_[i][j] = *it;
                    it++;
                }
            }
        }

        Matrix(const Matrix<N, M, Field>& src): Values_(src.Values_) {
        }

        bool operator==(const Matrix<N, M, Field>& rhs) const {
            return Values_ == rhs.Values_;
        }

        Matrix& operator=(const Matrix<N, M, Field>& rhs) {
            Values_ = rhs.Values_;
            return *this;
        }

        Matrix& operator=(Matrix<N, M, Field>&& rhs) {
            Values_ = std::move(rhs.Values_);
            return *this;
        }

        Row<M, Field>& operator[](int i) {
            return Values_[i];
        } 

        const Row<M, Field>& operator[](int i) const {
            return Values_[i];
        }

        Matrix& operator+=(const Matrix<N, M, Field>& rhs) {
            for (unsigned i = 0; i < N; ++i) {
                Values_[i] += rhs.Values_[i];
            }
            return *this;
        }

        Matrix operator+(const Matrix<N, M, Field>& rhs) const {
            Matrix res = *this;
            res += rhs;
            return res;
        }

        Matrix& operator-=(const Matrix<N, M, Field>& rhs) {
            for (unsigned i = 0; i < N; ++i) {
                Values_[i] -= rhs.Values_[i];
            }
            return *this;
        }

        Matrix operator-(const Matrix<N, M, Field>& rhs) const {
            Matrix res = *this;
            res -= rhs;
            return res;
        }

        Matrix& operator*=(const Field rhs) {
            for (unsigned i = 0; i < N; ++i) {
                Values_[i] *= rhs;
            }
            return *this;
        }

        Matrix operator*(const Field rhs) const {
            Matrix res = *this;
            res *= rhs;
            return res;
        }

        Matrix& operator/=(const Field rhs) {
            for (unsigned i = 0; i < N; ++i) {
                Values_[i] /= rhs;
            }
            return *this;
        }

        Matrix operator/(const Field rhs) const {
            Matrix res = *this;
            res /= rhs;
            return res;
        }

        template<unsigned K>
        Matrix<N, K, Field> operator*(const Matrix<M, K, Field>& rhs) const {
            return StrassenMultiply_(rhs);
        }

        template<unsigned R>
        Matrix<R, R, Field> ToBiggerSquare() const {
            Matrix<R, R, Field> res;
            for (unsigned i = 0; i < N; ++i) {
                for (unsigned j = 0; j < M; ++j) {
                    res[i][j] = Values_[i][j];
                }
            }
            return res;
        }

        template<unsigned NN, unsigned MM>
        Matrix<NN, MM, Field> Resized() const {
            Matrix<NN, MM, Field> res;
            for (unsigned i = 0; i < NN && i < N; ++i) {
                for (unsigned j = 0; j < MM && j < M; ++j) {
                    res[i][j] = Values_[i][j];
                }
            }
            return res;
        }

        template<unsigned K>
        Matrix<N, K, Field> SimpleMultiply_(const Matrix<M, K, Field>& rhs) const {
            Matrix<N, K, Field> res;
            for (unsigned i = 0; i < N; ++i) {
                for (unsigned j = 0; j < K; ++j) {
                    for (unsigned r = 0; r < M; ++r) {
                        res[i][j] += (*this)[i][r] * rhs[r][j];
                    }
                }
            }
            return res;
        }

        template<typename Dummy = void>
        typename enable_if<(N == M && is_same<Dummy, void>::value), Field>::type Det() const {
            Field det;
            unsigned rk;
            GaussianElimination(det, rk);
            return det;
        }

        unsigned Rank() const {
            Field det;
            unsigned rk;
            GaussianElimination(det, rk);
            return rk;
        }

        template<typename Dummy = void>
        typename enable_if<(N == M && is_same<Dummy, void>::value), Field>::type Trace() const {
            Field res = 0;
            for (unsigned i = 0; i < N; ++i) {
                res += Values_[i][i];
            }
            return res;
        }

        template<typename Dummy = void>
        typename enable_if<(N == M && is_same<Dummy, void>::value), Matrix<N, M, Field>>::type
        inverted() const {
            Matrix<N, M + M, Field> twice;
            for (unsigned i = 0; i < N; ++i) {
                for (unsigned j = 0; j < M; ++j) {
                    twice[i][j] = Values_[i][j];
                }
                twice[i][i + M] = 1;
            }

            Field det;
            unsigned rank;
            Matrix<N, M + M, Field> twiceRev = twice.GaussianElimination(det, rank);

            assert(det != 0);

            Matrix<N, M, Field> res;
            for (unsigned i = 0; i < N; ++i) {
                for (unsigned j = 0; j < M; ++j) {
                    res[i][j] = twiceRev[i][j + M];
                }
            }
            return res;
        }
        
        template<typename Dummy = void>
        typename enable_if<(N == M && is_same<Dummy, void>::value), void>::type inverte() {
            *this = this->inverted();
        }

        Matrix<N, M, Field> GaussianElimination(Field& det, unsigned& rk) const {
            Matrix res = *this;

            unsigned row = 0;
            unsigned column = 0;

            det = 1;
            rk = 0;
            
            while (row < N && column < M) {
                for (unsigned k = row; k < N; ++k) {
                    if (res[k][column] == 0) {
                        continue;
                    }
                    if (k != row) {
                        det *= (-1); 
                    }

                    swap(res[row], res[k]);
                    break;
                }
                if (res[row][column] == 0) {
                    column++;
                    det *= 0;
                    continue;
                }

                rk++;
                det *= res[row][column];
                res[row] /= res[row][column];
                for (unsigned k = 0; k < N; ++k) {
                    if (k == row) {
                        continue;
                    }
                    res[k] -= res[row] * res[k][column];
                }
                column++;
                row++;
            }

            return res;
        }

        std::vector<Field> getRow(unsigned i) const {
            std::vector<Field> res(M); 
            for (unsigned j = 0; j < M; ++j) {
                res[j] = Values_[i][j];
            }
            return res;
        }

        std::vector<Field> getColumn(unsigned j) const {
            std::vector<Field> res(N);
            for (unsigned i = 0; i < N; ++i) {
                res[i] = Values_[i][j];
            }
            return res;
        }

    private:
        std::vector<Row<M, Field>> Values_;

        template<unsigned K>
        Matrix<N, K, Field> StrassenMultiply_(const Matrix<M, K, Field>& rhs) const {
            static const unsigned SIMPLE_FROM = 64;
            if (N <= SIMPLE_FROM || M < SIMPLE_FROM || K < SIMPLE_FROM) {
                return SimpleMultiply_(rhs);
            }
            static const unsigned NN = up_to_2_pow<N>::value;
            static const unsigned MM = up_to_2_pow<M>::value;
            static const unsigned KK = up_to_2_pow<K>::value;
            
            static const unsigned MAX_NN_MM = if_<(NN > MM),
                                                   n_store<NN>,
                                                   n_store<MM>>::type::value;
            static const unsigned R = if_<(KK > MAX_NN_MM),
                                               n_store<KK>, 
                                               n_store<MAX_NN_MM>>::type::value;

            Matrix<R, R, Field> m_A = (*this).template ToBiggerSquare<R>();
            Matrix<R, R, Field> m_B = rhs.template ToBiggerSquare<R>();

            Matrix<R, R, Field> res = StrassenRec(m_A, m_B);

            return res.template Resized<N, K>();
        }        
    };

    namespace {
        template<typename Field>
        Matrix<1, 1, Field> StrassenRec(const Matrix<1, 1, Field>& lhs,
                                        const Matrix<1, 1, Field>& rhs) {
            Matrix<1, 1, Field> res;
            res[0][0] = lhs[0][0] * rhs[0][0];  
            return res;
        }

        template<unsigned N, typename Field>
        Matrix<N, N, Field> StrassenRec(const Matrix<N, N, Field>& lhs,
                                        const Matrix<N, N, Field>& rhs) {
            static const unsigned SIMPLE_FROM = 64;
            if (N <= SIMPLE_FROM) {
                return lhs * rhs;
            }

            SquareMatrixSquad<N / 2, Field> A = GetSubmatrices(lhs); 
            SquareMatrixSquad<N / 2, Field> B = GetSubmatrices(rhs); 

            Matrix<N / 2, N / 2, Field> P[7];
              
            P[0] = StrassenRec((A.Val[0][0] + A.Val[1][1]), (B.Val[0][0] + B.Val[1][1]));
            P[1] = StrassenRec((A.Val[1][0] + A.Val[1][1]), B.Val[0][0]);
            P[2] = StrassenRec(A.Val[0][0], (B.Val[0][1] - B.Val[1][1]));
            P[3] = StrassenRec(A.Val[1][1], (B.Val[1][0] - B.Val[0][0]));
            P[4] = StrassenRec((A.Val[0][0] + A.Val[0][1]), B.Val[1][1]);
            P[5] = StrassenRec((A.Val[1][0] - A.Val[0][0]), (B.Val[0][0] + B.Val[0][1]));
            P[6] = StrassenRec((A.Val[0][1] - A.Val[1][1]), (B.Val[1][0] + B.Val[1][1]));

            SquareMatrixSquad<N / 2, Field> C;
            C.Val[0][0] = P[0] + P[3] - P[4] + P[6];
            C.Val[0][1] = P[2] + P[4];
            C.Val[1][0] = P[1] + P[3];
            C.Val[1][1] = P[0] - P[1] + P[2] + P[5];

            return FromSubmatrices(C);
        }
        
        template<unsigned N, typename Field>
        struct SquareMatrixSquad {
            Matrix<N, N, Field> Val[2][2];
        };

        template<unsigned N, typename Field>
        SquareMatrixSquad<N / 2, Field> GetSubmatrices(const Matrix<N, N, Field> &src) {
            SquareMatrixSquad<N / 2, Field> res;
            for (unsigned i = 0; i < N / 2; ++i) {
                for (unsigned j = 0; j < N / 2; ++j) {
                    res.Val[0][0][i][j] = src[i][j];
                    res.Val[0][1][i][j] = src[i][j + N / 2];
                    res.Val[1][0][i][j] = src[i + N / 2][j];
                    res.Val[1][1][i][j] = src[i + N / 2][j + N / 2];
                }
            }
            return res;
        }

        template<unsigned N, typename Field>
        Matrix<N * 2, N * 2, Field> FromSubmatrices(const SquareMatrixSquad<N, Field>& src) {
            Matrix<N * 2, N * 2, Field> res;
            for (unsigned i = 0; i < N; ++i) {
                for (unsigned j = 0; j < N; ++j) {
                    res[i][j] = src.Val[0][0][i][j];
                    res[i][j + N] = src.Val[0][1][i][j];
                    res[i + N][j] = src.Val[1][0][i][j];
                    res[i + N][j + N] = src.Val[1][1][i][j];
                }
            }
            return res; 
        }
    }
}
