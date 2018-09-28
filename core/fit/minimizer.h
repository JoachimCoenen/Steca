//  ***********************************************************************************************
//
//  Steca: stress and texture calculator
//
//! @file      core/fit/minimizer.h
//! @brief     Defines a minimizer
//!
//! @homepage  https://github.com/scgmlz/Steca
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016-2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, MAINTAINER)
//
//  ***********************************************************************************************

#ifndef MINIMIZER_H
#define MINIMIZER_H
#define NDEBUG
#include <functional>
#include <iostream>
#include <vector>
#include <cmath>
#include <cfloat>
#include <QtGlobal>
#define NDEBUG
#include <assert.h>
//#include <openmpi/shmem-compat.h>

#define PUT_VAR(var) << #var ": " << var
#define ADD_VAR(var) << ", " PUT_VAR(var)
#define LOG_BEGIN    std::cout
#define LOG_END      << std::endl;

#define LOG_VAR1(var1) LOG_BEGIN PUT_VAR(var1) LOG_END
#define LOG_VAR2(var1, var2) LOG_BEGIN PUT_VAR(var1) ADD_VAR(var2) LOG_END
#define LOG_VAR3(var1, var2, var3) LOG_BEGIN PUT_VAR(var1) ADD_VAR(var2) ADD_VAR(var3) LOG_END

//#define THROW_STR(a) throw std::string(a)
#define THROW_STR(a) 1+1

//#define FloatType float

using FloatType = double;
using FloatOType = float;
//#define FloatType float
#define F(a) a

#define MATRIX_ERR_INVERT_NONSQUARE  "Cannot invert non-square matrix"
#define MATRIX_ERR_INVERT_DETERMINANT  "Determinant is zero; cannot invert"
#define MATRIX_ERR_MULTIPLY_DIMENSION  "Cannot mutliply matrices; height_1 must equal width_2"
#define MATRIX_ERR_COPY_DIMENSION  "Cannot copy matrix as dimensions are not the same"
#define MATRIX_ERR_COPYTRANSPOSE_DIMENSION  "Cannot copy matrix transpose as dimensions are not the same"
#define MATRIX_ERR_ADD_DIMENSION  "Cannot add matrices as the dimensions are not the same"
#define MATRIX_ERR_DETERMINANT_NONSQUARE  "Cannot compute the determinant of a non-square matrix"

#define DEFINE_FOR_FLOAT(a) inline float a(float x) { return a##f(x); }

DEFINE_FOR_FLOAT(sqrt)
DEFINE_FOR_FLOAT(fabs)

inline int powNeg1(uint i) { return (i % 2) == 0 ? 1 : -1; }

template<typename T>
std::ostream &operator<<(std::ostream &lhs, const std::vector<T> &rhs) {
    for (const auto &val : rhs) {
        lhs << val << "\t";
    }
    return lhs;
}
template<typename T>
std::ostream &operator<<(std::ostream &lhs, const std::vector<std::vector<T>> &rhs) {
    for (const auto &val : rhs) {
        lhs << val << "\n";
    }
    return lhs;
}

//static uint64_t fsumCounter = 0;

template<typename F>
FloatType fsum(F fun) {
    FloatType sum = F(0.0);
    volatile FloatType c = F(0.0); // A running compensation for lost low-order bits.

    fun([&](auto val){
        FloatType y = val - c;    // So far, so good: c is zero.
        volatile FloatType t = sum + y; // Beware overly-aggressive optimizing compilers!
        c = (t - sum) - y;       // (t - sum) cancels the high-order part of y; subtracting y recovers negative (low part of y)
        sum = t;                 // Algebraically, c should always be zero. Beware overly-aggressive optimizing compilers!})
    });
    return sum;
}

class DeterminantIsZeroException { };


class MatrixNullOperation;
class VectorNullOperation;
template<typename MatOrMatOpL, typename MatOrMatOpR>
class MatrixAddition;
template<typename MatOrMatOpL, typename MatOrMatOpR>
class MatrixSubtraction;
template<typename MatOrMatOpL, typename MatOrMatOpR>
class MatrixMultiplication;
template<typename MatOrMatOpL>
class MatrixMultiplication<MatOrMatOpL, FloatType>;
template<typename MatOrMatOpL>
class MatrixTranspose;
class MatrixInversion;
template<typename MatOrMatOpL>
class MatrixDiagonal;
class IType;

#define ASSERT_EQUAL_WIDTH(matOrMatOpL, matOrMatOpR)    \
    assert(matOrMatOpL.width() == matOrMatOpR.width())

#define ASSERT_EQUAL_HEIGHT(matOrMatOpL, matOrMatOpR)   \
    assert(matOrMatOpL.height() == matOrMatOpR.height())

#define ASSERT_EQUAL_SIZE(matOrMatOpL, matOrMatOpR)     \
    ASSERT_EQUAL_WIDTH(matOrMatOpL, matOrMatOpR);       \
    ASSERT_EQUAL_HEIGHT(matOrMatOpL, matOrMatOpR)

#define ASSERT_IS_MATRIX_TYPE(MatOrMatOp)                     \
static_assert ((std::is_base_of<MatrixOperation, MatOrMatOp>() \
               || std::is_constructible<MatOrMatOp, Vector>() \
               || std::is_constructible<MatOrMatOp, Matrix>()\
               )&& !std::is_convertible<MatOrMatOp, std::nullptr_t>(),\
               "A variable of this Type cannot be assigned to a Matrix.");

#define ADD_BASIC_BINARY_MATRIX_OPERATIONS(Type)            \
    template<typename T>                                    \
    auto operator+(const T &rhs) const {                    \
        return MatrixAddition<Type, T>(*this, rhs);         \
    }                                                       \
    template<typename T>                                    \
    auto operator-(const T &rhs) const {                    \
        return MatrixSubtraction<Type, T>(*this, rhs);      \
    }                                                       \
    template<typename T>                                    \
    auto operator*(const T &rhs) const {                    \
        return MatrixMultiplication<Type, T>(*this, rhs);   \
    }

class Matrix;
class Vector;

template<typename MatOrMatOp>
struct MatrixView {
    typedef const MatOrMatOp &type;
};

template<>
struct MatrixView<Matrix> {
    typedef const Matrix &type;
};

template<>
struct MatrixView<Vector> {
    typedef const Vector &type;
};

template<typename MatOrMatOp>
const MatOrMatOp &copyMatrixView(const MatOrMatOp &mat);
const Matrix &copyMatrixView(const Matrix &mat);
const Vector &copyMatrixView(const Vector &mat);

struct MatrixOperation { };

template<typename MatOrMatOp>
struct MatrixUnaryOp : public MatrixOperation {
public:
    MatrixUnaryOp(const MatOrMatOp &lhs)
        : MatrixOperation(),
          lhs_(copyMatrixView(lhs))
    { }

protected:
    const typename MatrixView<MatOrMatOp>::type lhs_;
};

template<typename MatOrMatOpL, typename MatOrMatOpR>
struct MatrixBinaryOp : public MatrixOperation  {
public:
    MatrixBinaryOp(const MatOrMatOpL &lhs, const MatOrMatOpR &rhs)
        :
          lhs_(copyMatrixView(lhs)),
          rhs_(copyMatrixView(rhs))
    { }

    static constexpr bool isMatrix = true;
protected:
    const typename MatrixView<MatOrMatOpL>::type lhs_;
    const typename MatrixView<MatOrMatOpR>::type rhs_;
};


class Matrix {
private:
    std::vector<FloatType> _matrix;
    uint _width;  // size of inner Vectors
    uint _height; // size of outer Vector

    inline size_t getIndex(uint x, uint y) const {
        return y*_width + x;
    }
public:
    Matrix(uint width, uint height) :_width(width), _height(height) {
        resize(_width, _height);
    }

    Matrix():_width(1),_height(1) {
        resize(1, 1);
    }

    template<typename MatOrMatOp>
    Matrix(const MatOrMatOp &rhs) {
        ASSERT_IS_MATRIX_TYPE(MatOrMatOp);

        resize(rhs.width(), rhs.height());
        for (uint y = 0; y < height(); y++) {
            for (uint x = 0; x < width(); x++) {
                get(x, y) = rhs.get(x, y);
            }
        }
    }

    //inline auto &get(uint y) {
    //    return _matrix[y];
    //}

    //inline const  auto &get(uint y) const {
    //    return _matrix[y];
    //}

    inline auto &get() {
        return _matrix;
    }

    inline const auto &get() const {
        return _matrix;
    }

    inline FloatType &get(uint x, uint y) {
        return _matrix[getIndex(x, y)];
    }

    inline const  FloatType &get(uint x, uint y) const{
        return _matrix[getIndex(x, y)];
    }

    Matrix &reset(FloatType value = F(0.0)) {
        for (auto &cell : _matrix) {
            cell = value;
        }

        return *this;
    }

    inline uint width() const {
        return _width;
    }

    inline uint height() const {
        return _height;
    }

    void resize(uint width, uint height) {
        _matrix.resize(height * width);
        _height = height;
        _width = width;
    }

    inline Matrix & operator+=(FloatType val) {
        const auto end = _matrix.end();
        ////#pragma omp simd
        for (auto iter = _matrix.begin(); iter < end; ++iter) {
            *iter += val;
        }
        return *this;
    }
    inline Matrix &operator-=(FloatType val) {
        return (*this) += -val;
    }

    template<typename MatOrMatOp>
    inline Matrix &operator+=(const MatOrMatOp &other) {
        ASSERT_IS_MATRIX_TYPE(MatOrMatOp);

//        if (height() != other.height() || width() != other.width()) {
//            std::cout << MATRIX_ERR_ADD_DIMENSION << std::endl;
//           THROW_STR(MATRIX_ERR_ADD_DIMENSION);
//        }
        //#pragma omp simd collapse(2)
        for (uint i = 0; i < _height; i++) {
            for (uint j = 0; j < _width; j++) {
                get(j, i) += other.get(j, i);
            }
        }
        return *this;
    }

    template<typename MatOrMatOp>
    inline Matrix &operator-=(const MatOrMatOp &other);

    inline Matrix &operator*=(FloatType val) {
        const auto end = _matrix.end();
        //#pragma omp simd
        for (auto iter = _matrix.begin(); iter < end; ++iter) {
            *iter *= val;
        }
        return *this;
    }


    ADD_BASIC_BINARY_MATRIX_OPERATIONS(Matrix)


    template<typename MatOrMatOp>
    inline Matrix &operator=(const MatOrMatOp &rhs) {
        ASSERT_IS_MATRIX_TYPE(MatOrMatOp);

        resize(rhs.width(), rhs.height());
        for (uint y = 0; y < height(); y++) {
            for (uint x = 0; x < width(); x++) {
                get(x, y) = rhs.get(x, y);
            }
        }
        return *this;
    }
};

class Vector {
private:
    std::vector<FloatType> _vector;
public:
    Vector(uint size) :_vector(size) { }

    Vector(const std::vector<FloatType> &data) :_vector(data) { }

    Vector(const std::vector<FloatOType> &data) :_vector(data.begin(), data.end()) { }

    Vector() :_vector(1) { }

    template<typename MatOrMatOp>
    Vector(const MatOrMatOp &rhs) {
        ASSERT_IS_MATRIX_TYPE(MatOrMatOp);

        assert(rhs.width() == 1);
        resize(rhs.width(), rhs.height());
        for (uint y = 0; y < height(); y++) {
            for (uint x = 0; x < width(); x++) {
                get(x, y) = rhs.get(x, y);
            }
        }
    }

    inline auto &get(uint y) {
        return _vector[y];
    }

    inline const  auto &get(uint y) const {
        return _vector[y];
    }

    inline auto &get() {
        return _vector;
    }

    inline FloatType &get(uint x, uint y) {
        assert(x == 0);
        return get(y);
    }

    inline const  FloatType &get(uint x, uint y) const{
        assert(x == 0);
        return get(y);
    }

    Vector &reset(FloatType value = F(0.0)) {
        for (auto &cell : _vector) {
            cell = value;
        }
        return *this;
    }

    inline uint width() const {
        return 1;
    }

    inline uint height() const {
        return uint(_vector.size());
    }

    inline void resize(uint width, uint height) {
        assert(width == 1);
        resize(height);
    }

    inline void resize(uint height) {
        assert(height >= 1);
        _vector.resize(height);
    }

    inline FloatType getDeterminant() const {
        assert(height() == 1);
        return get(0, 0);
    }

    inline Vector & operator+=(FloatType val) {
        const auto end = _vector.end();
        //#pragma omp simd
        for (auto iter = _vector.begin(); iter < end; ++iter) {
            *iter += val;
        }
        return *this;
    }
    inline Vector &operator-=(FloatType val) {
        return (*this) += -val;
    }

    template<typename MatOrMatOp>
    inline Vector &operator+=(const MatOrMatOp &other) {
        ASSERT_IS_MATRIX_TYPE(MatOrMatOp);

//        if (height() != other.height() || width() != other.width()) {
//            std::cout << MATRIX_ERR_ADD_DIMENSION << std::endl;
//           THROW_STR(MATRIX_ERR_ADD_DIMENSION);
//        }
        //#pragma omp simd collapse(2)
        for (uint i = 0; i < height(); i++) {
            for (uint j = 0; j < width(); j++) {
                get(j, i) += other.get(j, i);
            }
        }
        return *this;
    }

    template<typename MatOrMatOp>
    inline Vector &operator-=(const MatOrMatOp &other);

    inline Vector &operator*=(FloatType val) {
        const auto end = _vector.end();
        //#pragma omp simd
        for (auto iter = _vector.begin(); iter < end; ++iter) {
            *iter *= val;
        }
        return *this;
    }

    ADD_BASIC_BINARY_MATRIX_OPERATIONS(Vector)

    template<typename MatOrMatOp>
    inline Vector &operator=(const MatOrMatOp &rhs) {
        ASSERT_IS_MATRIX_TYPE(MatOrMatOp);

        resize(rhs.width(), rhs.height());
        //#pragma omp simd
        for (uint y = 0; y < height(); y++) {
            get(0, y) = rhs.get(0, y);
        }
        return *this;
    }

    inline Vector &operator=(const std::vector<FloatType> &rhs) {
        resize(1, uint(rhs.size()));
        //#pragma omp simd
        for (uint y = 0; y < height(); y++) {
            get(0, y) = rhs[y];
        }
        return *this;
}
};

class VectorView : MatrixOperation {
private:
    std::vector<FloatType> &_vector;
public:
    VectorView(Vector &other) :_vector(other.get()) { }
    VectorView(std::vector<FloatType> &data) :_vector(data) { }

    inline auto &get(uint y) {
        return _vector[y];
    }

    inline const  auto &get(uint y) const {
        return _vector[y];
    }

    inline auto &get() {
        return _vector;
    }

    inline FloatType &get(uint x, uint y) {
        assert(x == 0);
        return get(y);
    }

    inline const  FloatType &get(uint x, uint y) const{
        assert(x == 0);
        return get(y);
    }

    VectorView &reset(FloatType value = F(0.0)) {
        for (auto &cell : _vector) {
            cell = value;
        }
        return *this;
    }

    inline uint width() const {
        return 1;
    }

    inline uint height() const {
        return uint(_vector.size());
    }

    inline void resize(uint width, uint height) {
        assert(width == 1);
        resize(height);
    }

    inline void resize(uint height) {
        assert(height >= 1);
        _vector.resize(height);
    }

    inline FloatType getDeterminant() const {
        assert(height() == 1);
        return get(0, 0);
    }

    inline VectorView & operator+=(FloatType val) {
        for (auto &cell : _vector) {
            cell += val;
        }
        return *this;
    }
    inline VectorView &operator-=(FloatType val) {
        return (*this) += -val;
    }

    template<typename MatOrMatOp>
    inline VectorView &operator+=(const MatOrMatOp &other) {
        ASSERT_IS_MATRIX_TYPE(MatOrMatOp);

//        if (height() != other.height() || width() != other.width()) {
//            std::cout << MATRIX_ERR_ADD_DIMENSION << std::endl;
//           THROW_STR(MATRIX_ERR_ADD_DIMENSION);
//        }
        for (uint i = 0; i < height(); i++) {
            for (uint j = 0; j < width(); j++) {
                get(j, i) += other.get(j, i);
            }
        }
        return *this;
    }

    template<typename MatOrMatOp>
    inline VectorView &operator-=(const MatOrMatOp &other);

    inline VectorView &operator*=(FloatType val) {
        for (auto &cell : _vector) {
            cell *= val;
        }
        return *this;
    }

    ADD_BASIC_BINARY_MATRIX_OPERATIONS(VectorView)

    template<typename MatOrMatOp>
    inline VectorView &operator=(const MatOrMatOp &rhs) {
        ASSERT_IS_MATRIX_TYPE(MatOrMatOp);

        resize(rhs.width(), rhs.height());
        for (uint y = 0; y < height(); y++) {
                get(0, y) = rhs.get(0, y);
        }
        return *this;
    }

    inline VectorView &operator=(const std::vector<FloatType> &rhs) {

        resize(1, uint(rhs.size()));
        for (uint y = 0; y < height(); y++) {
                get(0, y) = rhs[y];
        }
        return *this;
}
};


class MatrixNullOperation : protected MatrixOperation {
public:
    MatrixNullOperation(const Matrix &lhs)
        : MatrixOperation(), lhs_(lhs)
    { }

    inline uint width() const { return this->lhs_.width(); }
    inline uint height() const { return this->lhs_.height(); }

    inline FloatType get(uint x, uint y) const {
        return this->lhs_.get(x, y);
    }

    ADD_BASIC_BINARY_MATRIX_OPERATIONS(MatrixNullOperation)
private:
    const Matrix &lhs_;
};

class VectorNullOperation : protected MatrixOperation {
public:
    VectorNullOperation(const Vector &lhs)
        : MatrixOperation(), lhs_(lhs)
    { }

    inline uint width() const { return this->lhs_.width(); }
    inline uint height() const { return this->lhs_.height(); }

    inline FloatType get(uint x, uint y) const {
        return this->lhs_.get(x, y);
    }

    ADD_BASIC_BINARY_MATRIX_OPERATIONS(VectorNullOperation)
private:
    const Vector &lhs_;
};

template<typename MatOrMatOp>
inline const MatOrMatOp &copyMatrixView(const MatOrMatOp &mat) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOp);
    return mat;
}
inline const Matrix &copyMatrixView(const Matrix &mat) {
    return mat;
}
inline const Vector &copyMatrixView(const Vector &mat) {
    return mat;
}


template<typename MatOrMatOpL, typename MatOrMatOpR>
class MatrixAddition : protected MatrixBinaryOp<MatOrMatOpL, MatOrMatOpR> {
public:
    MatrixAddition(const MatOrMatOpL &lhs, const MatOrMatOpR &rhs)
        : MatrixBinaryOp<MatOrMatOpL, MatOrMatOpR>(lhs, rhs)
    {
        ASSERT_EQUAL_SIZE(lhs, rhs);
    }

    inline uint width() const { return this->lhs_.width(); }
    inline uint height() const { return this->lhs_.height(); }

    inline FloatType get(uint x, uint y) const {
        return this->lhs_.get(x, y) + this->rhs_.get(x, y);
    }

    ADD_BASIC_BINARY_MATRIX_OPERATIONS(MatrixAddition)
};

template<typename MatOrMatOpL>
class MatrixAddition<MatOrMatOpL, FloatType> : protected MatrixUnaryOp<MatOrMatOpL> {
public:
    MatrixAddition(const MatOrMatOpL &lhs, const FloatType &rhs)
        : MatrixUnaryOp<MatOrMatOpL>(lhs), rhs_(rhs)
    { }

    inline uint width() const { return this->lhs_.width(); }
    inline uint height() const { return this->lhs_.height(); }

    inline FloatType get(uint x, uint y) const {
        return this->lhs_.get(x, y) + this->rhs_;
    }

    ADD_BASIC_BINARY_MATRIX_OPERATIONS(MatrixAddition)

protected:
    const FloatType rhs_;
};

template<typename MatOrMatOpL, typename MatOrMatOpR>
class MatrixSubtraction : protected MatrixBinaryOp<MatOrMatOpL, MatOrMatOpR> {
public:
    MatrixSubtraction(const MatOrMatOpL &lhs, const MatOrMatOpR &rhs)
        : MatrixBinaryOp<MatOrMatOpL, MatOrMatOpR>(lhs, rhs)
    {
        ASSERT_EQUAL_SIZE(lhs, rhs);
    }

    inline uint width() const { return this->lhs_.width(); }
    inline uint height() const { return this->lhs_.height(); }

    inline FloatType get(uint x, uint y) const {
        return this->lhs_.get(x, y) - this->rhs_.get(x, y);
    }

    ADD_BASIC_BINARY_MATRIX_OPERATIONS(MatrixSubtraction)
};

template<typename MatOrMatOpL, typename MatOrMatOpR>
class MatrixMultiplication : protected MatrixBinaryOp<MatOrMatOpL, MatOrMatOpR> {
public:
    MatrixMultiplication(const MatOrMatOpL &lhs, const MatOrMatOpR &rhs)
        : MatrixBinaryOp<MatOrMatOpL, MatOrMatOpR>(lhs, rhs)
    {
        ASSERT_EQUAL_WIDTH(lhs, transpose(rhs));
    }

    inline uint width() const { return this->rhs_.width(); }
    inline uint height() const { return this->lhs_.height(); }

    //#pragma omp declare simd
    inline FloatType get(uint x, uint y) const {
        //set this->_matrix to the matrix created from multilplying axb

        FloatType sum = F(0.0);
        //#pragma omp simd reduction( +: sum )
        for (uint k = 0; k < this->lhs_.width(); k++) {
            sum += this->lhs_.get(k, y) * this->rhs_.get(x, k);
        }
        return sum;
    }

    ADD_BASIC_BINARY_MATRIX_OPERATIONS(MatrixMultiplication)
};

template<typename MatOrMatOpL>
class MatrixMultiplication<MatOrMatOpL, FloatType> : protected MatrixUnaryOp<MatOrMatOpL> {
public:
    MatrixMultiplication(const MatOrMatOpL &lhs, const FloatType &rhs)
        : MatrixUnaryOp<MatOrMatOpL>(lhs), rhs_(rhs)
    { }

    inline uint width() const { return this->lhs_.width(); }
    inline uint height() const { return this->lhs_.height(); }

    inline FloatType get(uint x, uint y) const {
        return this->lhs_.get(x, y) * this->rhs_;
    }

    ADD_BASIC_BINARY_MATRIX_OPERATIONS(MatrixMultiplication)

protected:
    const FloatType rhs_;
};

template<typename MatOrMatOpL>
class MatrixTranspose : protected MatrixUnaryOp<MatOrMatOpL> {
public:
    MatrixTranspose(const MatOrMatOpL &lhs)
        : MatrixUnaryOp<MatOrMatOpL>(lhs)
    { }

    inline uint width() const { return this->lhs_.height(); }
    inline uint height() const { return this->lhs_.width(); }

    inline FloatType get(uint x, uint y) const {
        return this->lhs_.get(y, x);
    }

    ADD_BASIC_BINARY_MATRIX_OPERATIONS(MatrixTranspose)
};

template<typename MatOrMatOpL>
class MatrixDiagonal : protected MatrixUnaryOp<MatOrMatOpL> {
public:
    MatrixDiagonal(const MatOrMatOpL &lhs)
        : MatrixUnaryOp<MatOrMatOpL>(lhs)
    { assert(lhs.width() == lhs.height()); }

    inline uint width() const { return this->lhs_.width(); }
    inline uint height() const { return this->lhs_.height(); }

    inline FloatType get(uint x, uint y) const {
        return x==y ? this->lhs_.get(x, x) : F(0.0);
    }

    ADD_BASIC_BINARY_MATRIX_OPERATIONS(MatrixDiagonal)
};


//template<typename MatOrMatOpL, typename MatOrMatOpR, typename TI, typename R, R(op)(TI, TI)>
template<typename MatOrMatOpL, typename MatOrMatOpR, typename Op, Op op>
class MatrixElementwiseBiOp : protected MatrixBinaryOp<MatOrMatOpL, MatOrMatOpR> {
public:
    MatrixElementwiseBiOp(const MatOrMatOpL &lhs, const MatOrMatOpR &rhs)
        : MatrixBinaryOp<MatOrMatOpL, MatOrMatOpR>(lhs, rhs)
    {
        ASSERT_EQUAL_SIZE(lhs, rhs);
    }

    inline uint width() const { return this->lhs_.width(); }
    inline uint height() const { return this->lhs_.height(); }

    inline FloatType get(uint x, uint y) const {
        return op(this->lhs_.get(x, y), this->rhs_.get(x, y));
    }

    ADD_BASIC_BINARY_MATRIX_OPERATIONS(MatrixElementwiseBiOp)
};

template<typename MatOrMatOpL, typename MatOrMatOpR, typename Op>
class MatrixElementwiseBiOp2 : protected MatrixBinaryOp<MatOrMatOpL, MatOrMatOpR> {
public:
    MatrixElementwiseBiOp2(const MatOrMatOpL &lhs, const MatOrMatOpR &rhs)
        : MatrixBinaryOp<MatOrMatOpL, MatOrMatOpR>(lhs, rhs)
    {
        ASSERT_EQUAL_SIZE(lhs, rhs);
    }

    inline uint width() const { return this->lhs_.width(); }
    inline uint height() const { return this->lhs_.height(); }

    inline FloatType get(uint x, uint y) const {
        return Op()(this->lhs_.get(x, y), this->rhs_.get(x, y));
    }

    ADD_BASIC_BINARY_MATRIX_OPERATIONS(MatrixElementwiseBiOp2)
};

template<typename MatOrMatOpL, typename Op, Op op>
class MatrixElementwiseUnaryOp : protected MatrixUnaryOp<MatOrMatOpL> {
public:
    MatrixElementwiseUnaryOp(const MatOrMatOpL &lhs)
        : MatrixUnaryOp<MatOrMatOpL>(lhs)
    { }

    inline uint width() const { return this->lhs_.width(); }
    inline uint height() const { return this->lhs_.height(); }

    inline FloatType get(uint x, uint y) const {
        return op(this->lhs_.get(x, y));
    }

    ADD_BASIC_BINARY_MATRIX_OPERATIONS(MatrixElementwiseUnaryOp)
};

template<typename MatOrMatOpL, typename Op>
class MatrixElementwiseUnaryOp2 : protected MatrixUnaryOp<MatOrMatOpL> {
public:
    MatrixElementwiseUnaryOp2(const MatOrMatOpL &lhs)
        : MatrixUnaryOp<MatOrMatOpL>(lhs)
    { }

    inline uint width() const { return this->lhs_.width(); }
    inline uint height() const { return this->lhs_.height(); }

    inline FloatType get(uint x, uint y) const {
        return Op(this->lhs_.get(x, y));
    }

    ADD_BASIC_BINARY_MATRIX_OPERATIONS(MatrixElementwiseUnaryOp2)
};

template<typename Op>
class MatrixElementwiseOp : protected MatrixOperation {
public:
    template<typename MatOrMatOpL>
    explicit MatrixElementwiseOp(const MatOrMatOpL &size, Op op)
        : _width(size.width()), _height(size.height()), _op(op)
    {
        ASSERT_IS_MATRIX_TYPE(MatOrMatOpL);
    }

    inline uint width() const { return _width; }
    inline uint height() const { return _height; }

    inline FloatType get(uint x, uint y) const {
        return _op(x, y);
    }

    ADD_BASIC_BINARY_MATRIX_OPERATIONS(MatrixElementwiseOp)
private:
    uint _width;  // size of inner Vectors
    uint _height; // size of outer Vector
    Op _op;

};

template<typename T>
using MinMaxFunc = const T&(const T&, const T&);

template<typename MatOrMatOpL, typename MatOrMatOpR>
using MatrixElementwiseMin = MatrixElementwiseBiOp<MatOrMatOpL, MatOrMatOpR, MinMaxFunc<FloatType>, std::min<FloatType>>;

template<typename MatOrMatOpL, typename MatOrMatOpR>
using MatrixElementwiseMax = MatrixElementwiseBiOp<MatOrMatOpL, MatOrMatOpR, MinMaxFunc<FloatType>, std::max<FloatType>>;

class IType {
private:
    uint _width;  // size of inner Vectors
    uint _height; // size of outer Vector

public:
    IType(uint width, uint height) :_width(width), _height(height) { assert(width == height); }

    IType():_width(1),_height(1) { }

    template<typename MatOrMatOp>
    IType(const MatOrMatOp &rhs) {
            ASSERT_IS_MATRIX_TYPE(MatOrMatOp);
            resize(rhs.width(), rhs.height());
    }

    inline FloatType get(uint x, uint y) const{
        return x==y ? F(1.0) : F(0.0);
    }

    inline uint width() const {
        return _width;
    }

    inline uint height() const {
        return _height;
    }

    inline void resize(uint width, uint height) {
        assert(width == height);
        _height = height;
        _width = width;
    }

    Matrix transposed() const {
        return IType(*this);
    }

    inline FloatType sqrMagnitude() const {
        return _height;
    }

    inline FloatType maxAbsElement() const {
        return F(1.0);
    }

    ADD_BASIC_BINARY_MATRIX_OPERATIONS(IType)

};
using I = IType;
//static const IType I;

template<typename MatOrMatOp>
inline Matrix &Matrix::operator-=(const MatOrMatOp &other) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOp);

    return(*this) += other * F(-1.0);
}

template<typename MatOrMatOp>
inline Vector &Vector::operator-=(const MatOrMatOp &other) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOp);

    return(*this) += other * F(-1.0);
}

template<typename MatOrMatOp>
inline auto transpose(const MatOrMatOp &lhs) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOp);
    return MatrixTranspose<MatOrMatOp>(lhs);
}

template<typename MatOrMatOp>
inline auto submatrix(const MatOrMatOp &matrix, uint x, uint y) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOp);
    //return an (height-1)x(width-1) matrix
    //FloatType
    Matrix ret(matrix.width() - 1, matrix.height() - 1);

    uint yIndex = 0;
    for (uint i = 0; i < matrix.height(); i++) {
        if (i != y) {
            uint xIndex = 0;
            for (uint j = 0; j < matrix.width(); j++) {
                if (j != x) {
                    ret.get(xIndex, yIndex) = matrix.get(j, i);
                    xIndex++;
                }
            }
            yIndex++;
        }
    }
    return ret;
}

template<typename MatOrMatOp>
inline FloatType determinant(const MatOrMatOp &matrix) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOp);
    //recursive method to get the determinant of nxn matrix

    if (matrix.width() == 3) { // _width == _height !!!
        const auto & m = matrix;
        FloatType sum = F(0.0);
        sum += +matrix.get(0, 0) * (m.get(1, 1) * m.get(2, 2) - m.get(1, 2) * m.get(2, 1));
        sum += -matrix.get(0, 1) * (m.get(0, 1) * m.get(2, 2) - m.get(0, 2) * m.get(2, 1));
        sum += +matrix.get(0, 2) * (m.get(0, 1) * m.get(1, 2) - m.get(0, 2) * m.get(1, 1));
        return sum;
    }
    if (matrix.width() == 0) { // _width == _height !!!
        return 1;
    }
    if (matrix.width() == 1) { // _width == _height !!!
        return matrix.get(0, 0);
    }
    if (matrix.width() == 2) { // _width == _height !!!
        return matrix.get(0, 0) * matrix.get(1, 1) - matrix.get(0, 1) * matrix.get(1, 0);
    }
    int sign = 1;
    //Matrix submatrix;
    FloatType sum = F(0.0);
    for (uint i = 0; i < matrix.width(); i++) {// _width == _height !!!
        sum += powNeg1(i) * matrix.get(0, i) * determinant(submatrix(matrix, i, 0));
    }
    return sum;
}

template<typename MatOrMatOp>
inline FloatType determinant(const MatOrMatOp &matrix, uint x, uint y) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOp);
    //recursive method to get the determinant of nxn matrix
/*
    if (matrix.width() == 4) { // _width == _height !!!
        const auto & m = matrix;
        FloatType sum = F(0.0);
        //#pragma omp simd
        {
        sum += +matrix.get(0>=x?0+1:0, 0>=y?0+1:0) * (m.get(1>=x?1+1:1, 1>=y?1+1:1) * m.get(2>=x?2+1:2, 2>=y?2+1:2) - m.get(1>=x?1+1:1, 2>=y?2+1:2) * m.get(2>=x?2+1:2, 1>=y?1+1:1));
        sum += -matrix.get(0>=x?0+1:0, 1>=y?1+1:1) * (m.get(0>=x?0+1:0, 1>=y?1+1:1) * m.get(2>=x?2+1:2, 2>=y?2+1:2) - m.get(0>=x?0+1:0, 2>=y?2+1:2) * m.get(2>=x?2+1:2, 1>=y?1+1:1));
        sum += +matrix.get(0>=x?0+1:0, 2>=y?2+1:2) * (m.get(0>=x?0+1:0, 1>=y?1+1:1) * m.get(1>=x?1+1:1, 2>=y?2+1:2) - m.get(0>=x?0+1:0, 2>=y?2+1:2) * m.get(1>=x?1+1:1, 1>=y?1+1:1));
        }
        return sum;
    }*/
    if (matrix.width() == 1) { // _width == _height !!!
        return 1;
    }
    if (matrix.width() == 2) { // _width == _height !!!
        return matrix.get(0>=x?0+1:0, 0>=y?0+1:0);
    }
    if (matrix.width() == 3) { // _width == _height !!!
        return matrix.get(0>=x?0+1:0, 0>=y?0+1:0) * matrix.get(1>=x?1+1:1, 1>=y?1+1:1) - matrix.get(0>=x?0+1:0, 1>=y?1+1:1) * matrix.get(1>=x?1+1:1, 0>=y?0+1:0);
    }
    int sign = 1;
    //Matrix submatrix;
    Matrix mat = submatrix(matrix, x, y);
    FloatType sum = F(0.0);
    for (uint i = 0; i < mat.width(); i++) {// _width == _height !!!
        sum += powNeg1(i) * mat.get(0, i) * determinant(submatrix(mat, i, 0));
    }
    return sum;
}

template<typename MatOrMatOp>
inline Matrix invert(const MatOrMatOp &lhs) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOp);
    //invert this matrix
    // to get inverse, get cofactor matrix, divide by determinant and transpose
    Matrix other = lhs;

    FloatType determinant_ = determinant(other);
//    if (other.width() != other.height()) {
//        std::cout << MATRIX_ERR_INVERT_NONSQUARE << std::endl;
//       THROW_STR(MATRIX_ERR_INVERT_NONSQUARE);
//    }
    if (fabs(determinant_) <= std::numeric_limits<FloatType>::epsilon()) {
        std::cout << MATRIX_ERR_INVERT_DETERMINANT << std::endl;
       throw DeterminantIsZeroException();
    }
    Matrix ret(other.width(), other.height());

    for (uint i = 0; i < ret.height(); i++) {
        for (uint j = 0; j < ret.width(); j++) {
            //swapping i and j for array indexes as need to take transpose
            ret.get(i, j) =  powNeg1(i+j) / determinant_ * determinant(other, j, i);
        }
    }
    return ret;
}

template<typename MatOrMatOp>
inline auto diagonal(const MatOrMatOp &lhs) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOp);
    return MatrixDiagonal<MatOrMatOp>(lhs);
}

template<typename MatOrMatOpL, typename MatOrMatOpR>
inline FloatType dot(const MatOrMatOpL &lhs, const MatOrMatOpR &rhs) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpL);
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpR);
    assert(rhs.width() == 1 && lhs.width() == 1);
    FloatType sum = 0;
    //#pragma omp simd reduction( +: sum )
    for (uint k = 0; k < lhs.height(); k++) {
        sum += lhs.get(0, k) * rhs.get(0, k);
    }
    return sum;
}

template<typename MatOrMatOp>
inline FloatType sqrMagnitude(const MatOrMatOp &matrix) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOp);

    return dot(matrix, matrix);
}

inline FloatType sqrMagnitude(const std::vector<FloatType> &vector) {
    FloatType sum = F(0.0);
    //#pragma omp simd reduction( +: sum )
    for (uint i = 0; i < vector.size(); ++i) {
        auto val  = vector[i];
        sum += val * val;
    }
    return sum;
}

template<typename MatOrMatOp>
inline FloatType maxAbsComponent(const MatOrMatOp &matrix) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOp);
    FloatType result = -1;
   //#pragma omp simd reduction( max: result ) collapse(2)
    for (uint y = 0; y < matrix.height(); y++) {
        for (uint x = 0; x < matrix.width(); x++) {
            result = std::max(result, fabs(matrix.get(x, y)));
        }
    }
    return result;
}

template<typename MatOrMatOp>
inline FloatType maxComponent(const MatOrMatOp &matrix) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOp);
    FloatType result = -1;
   //#pragma omp simd reduction( max: result ) collapse(2)
    for (uint y = 0; y < matrix.height(); y++) {
        for (uint x = 0; x < matrix.width(); x++) {
            result = std::max(result, matrix.get(x, y));
        }
    }
    return result;
}

class QuasiNewtonIsLandLockedException { };

template<typename MatOrMatOpL>
inline auto elementwiseAbs(const MatOrMatOpL &lhs) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpL);
    return MatrixElementwiseUnaryOp<MatOrMatOpL, double(double), std::abs>(lhs);
}

template<typename MatOrMatOpL, typename MatOrMatOpR>
inline auto scale(const MatOrMatOpL &lhs, const MatOrMatOpR &rhs) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpL);
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpR);
    return MatrixElementwiseBiOp2<MatOrMatOpL, MatOrMatOpR, std::multiplies<FloatType>>(lhs, rhs);
}

template<typename MatOrMatOpL, typename MatOrMatOpR>
inline auto scaleInv(const MatOrMatOpL &lhs, const MatOrMatOpR &rhs) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpL);
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpR);
    return MatrixElementwiseBiOp2<MatOrMatOpL, MatOrMatOpR, std::divides<FloatType>>(lhs, rhs);
}

template<typename MatOrMatOpL, typename MatOrMatOpR>
inline auto elementwiseMin(const MatOrMatOpL &lhs, const MatOrMatOpR &rhs) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpL);
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpR);
    return MatrixElementwiseMin<MatOrMatOpL, MatOrMatOpR>(lhs, rhs);
}

template<typename MatOrMatOpL, typename MatOrMatOpR>
inline auto elementwiseMax(const MatOrMatOpL &lhs, const MatOrMatOpR &rhs) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpL);
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpR);
    return MatrixElementwiseMax<MatOrMatOpL, MatOrMatOpR>(lhs, rhs);
}

template<typename MatOrMatOpL, typename Op>
inline auto elementwiseOp(const MatOrMatOpL &size, Op op) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpL);
    return MatrixElementwiseOp<Op>(size, op);
}

template<typename MatOrMatOpVec, typename MatOrMatOpLb, typename MatOrMatOpUb>
static auto boxProject(const MatOrMatOpVec &vec, const MatOrMatOpLb &lb, const MatOrMatOpUb &ub) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpVec);
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpLb);
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpUb);
    ASSERT_EQUAL_SIZE(vec, lb);
    ASSERT_EQUAL_SIZE(vec, ub);

    return elementwiseMin(elementwiseMax(vec, lb), ub);
}

template<typename MatOrMatOpVec, typename MatOrMatOpLb>
static auto boxProject(const MatOrMatOpVec &vec, const MatOrMatOpLb &lb, std::nullptr_t) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpVec);
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpLb);
    ASSERT_EQUAL_SIZE(vec, lb);

    return elementwiseMax(vec, lb);
}

template<typename MatOrMatOpVec, typename MatOrMatOpUb>
static auto boxProject(const MatOrMatOpVec &vec, std::nullptr_t, const MatOrMatOpUb &ub) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpVec);
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpUb);
    ASSERT_EQUAL_SIZE(vec, ub);

    return elementwiseMin(vec, ub);
}

template<typename MatOrMatOpVec>
static auto boxProject(const MatOrMatOpVec &vec, std::nullptr_t, std::nullptr_t) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpVec);

    return vec;
}


template<typename MatOrMatOpVec, typename MatOrMatOpPos, typename MatOrMatOpLb, typename MatOrMatOpUb>
static auto boxProject2(const MatOrMatOpVec &vec, const MatOrMatOpPos &pos, const MatOrMatOpLb &lb, const MatOrMatOpUb &ub) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpVec);
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpPos);
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpLb);
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpUb);
    ASSERT_EQUAL_SIZE(vec, pos);
    ASSERT_EQUAL_SIZE(vec, lb);
    ASSERT_EQUAL_SIZE(vec, ub);

    const auto bor = elementwiseOp(vec, [&](uint x, uint y){ return vec.get(x,y) > 0 ? ub.get(x, y) : lb.get(x, y); });
    const auto dist = elementwiseAbs(scaleInv(vec, bor - pos));
    FloatType scale =  std::max(1.0, maxComponent((scaleInv(vec, bor - pos))));
    return vec * (F(1.0) / scale);
}

template<typename MatOrMatOpVec, typename MatOrMatOpPos, typename MatOrMatOpLb>
static auto boxProject2(const MatOrMatOpVec &vec, const MatOrMatOpPos &pos, const MatOrMatOpLb &lb, std::nullptr_t) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpVec);
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpPos);
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpLb);
    ASSERT_EQUAL_SIZE(vec, pos);
    ASSERT_EQUAL_SIZE(vec, lb);

    const Vector bor = elementwiseOp(vec, [&](uint x, uint y){ return vec.get(x,y) > 0 ? vec.get(x,y)+pos.get(x,y) : lb.get(x, y); });
    const Vector dist = (scaleInv(vec, bor - pos));
    FloatType scale =  std::max(1.0, maxComponent((scaleInv(vec, bor - pos))));
    return vec * (F(1.0) / scale);
}

template<typename MatOrMatOpVec, typename MatOrMatOpPos, typename MatOrMatOpUb>
static auto boxProject2(const MatOrMatOpVec &vec, const MatOrMatOpPos &pos, std::nullptr_t, const MatOrMatOpUb &ub) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpVec);
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpPos);
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpUb);
    ASSERT_EQUAL_SIZE(vec, pos);
    ASSERT_EQUAL_SIZE(vec, ub);

    const auto bor = elementwiseOp(vec, [&](uint x, uint y){ return vec.get(x,y) > 0 ? ub.get(x, y) : vec.get(x,y)+pos.get(x,y); });
    const auto dist = elementwiseAbs(scaleInv(vec, bor - pos));
    FloatType scale =  std::max(1.0, maxComponent((scaleInv(vec, bor - pos))));
    return vec * (F(1.0) / scale);
}

template<typename MatOrMatOpVec, typename MatOrMatOpPos>
static auto boxProject2(const MatOrMatOpVec &vec, const MatOrMatOpPos &pos, std::nullptr_t, std::nullptr_t) {
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpVec);
    ASSERT_IS_MATRIX_TYPE(MatOrMatOpPos);
    ASSERT_EQUAL_SIZE(vec, pos);

    return vec;
}


//typedef std::function<void(const std::vector<double>&, std::vector<double>&)> Func;
//typedef std::function<void(const std::vector<double>&, std::vector<std::vector<double>>&)> Jacobian;

inline FloatType pow2(FloatType x) { return x*x; }
inline FloatType pow3(FloatType x) { return x*x*x; }

template <typename Func, typename Jacobian, typename LB, typename UB>
std::vector<double> levenbergMarquardtInternal(
    const std::vector<double> &dataIn,
    const std::vector<double> &initGuessIn, const LB &lowerBound, const UB &upperBound,
    Func f, Jacobian j, Matrix &covar, std::array<FloatType, 3> epsilons = { 1E-17, 1E-17, 1E-17 },
    bool doLog = true
) {
    const FloatType rho = F(1e-8);
    const FloatType lambda = F(1.5);
    const uint maxIterations = 1000;
    //fsumCounter = 0;

    const VectorView data(const_cast<std::vector<double>&>(dataIn) );
    const VectorView initGuess(const_cast<std::vector<double>&>(initGuessIn) );

    Vector beta = boxProject(initGuess, lowerBound, upperBound);
    uint64_t v = 2;
    Matrix J(beta.height(), data.height());
    j(beta.get().data(), J.get().data());
    //Matrix JT = transpose(J);
    Matrix JTJ = transpose(J) * J;
    Matrix B = I(JTJ);

    Vector e(data.height());
    f(beta.get().data(), e.get().data());
    e = data - e;
    FloatType eSqrMag = sqrMagnitude(e);

    Vector JTe = transpose(J) * e;
    Matrix oldJ;

    Vector delta;

    //bool found = maxAbsComponent(JTe) <= epsilons[0];
    FloatType mu = F(1e-3);//rho * maxAbsComponent(diagonal(JTJ));

    uint iterations = maxIterations;
    try {
        while (true) {
            // Levenberg-Marquardt:
            uint16_t count = 0;
            while (true) {
                iterations--;
                Matrix diagJTJ = diagonal(JTJ);
                double diagJTJMag = sqrt(sqrMagnitude(diagJTJ)) / FloatType(diagJTJ.height());
                Matrix diagJTJmu = (elementwiseAbs(diagJTJ)*(1.0/diagJTJMag) + I(JTJ) ) * mu;
                //Solve (JTJ + I*mu)h = -JTe;
                delta = invert(JTJ + (diagJTJmu)) * JTe;

                Vector newBeta = boxProject(beta + delta, lowerBound, upperBound);
                delta = newBeta - beta;
                Vector newE(data.height());
                f(newBeta.get().data(), newE.get().data());
                newE = data - newE;
                FloatType newESqrMag = sqrMagnitude(newE);

                const FloatType deltaSqrMag = sqrMagnitude(delta);
                const FloatType betaSqrMag = sqrMagnitude(beta);
                Vector deltaScaled = scaleInv(delta, elementwiseAbs(beta) + epsilons[1]);
                //if (iterations <= 0 || sqrt(deltaSqrMag) <= (epsilons[1] * (sqrt(betaSqrMag) + epsilons[1]))) {
                //if (iterations <= 0 || deltaSqrMag <= pow2(epsilons[1] * (sqrt(betaSqrMag) + epsilons[1]))) {
                //if (iterations <= 0 || sqrMagnitude(scaleInv(delta, elementwiseAbs(beta) + epsilons[1])) <= (epsilons[1])) {
                if (iterations <= 0 || sqrMagnitude(deltaScaled) <= pow2(epsilons[1])) {
                    if (doLog)
                        std::cout << "exit exit exit 1" << std::endl;
                    goto breakOuterLoop;
                }

                if (newESqrMag > eSqrMag) { //step NOT acceptable
                    mu *= v;
                    auto v2 = v * 2;
                    if (v2 < v) {
                        mu=F(0.1);
                        v2 = 2;
                    }
                    v = v2;
                    count = 0;
                    continue; // inner loop
                }

                FloatType sigma = (eSqrMag - newESqrMag) / dot(delta, (delta * mu + JTe) * F(0.5));
                mu = mu * std::max(F(1.0)/F(3.0), F(1.0) - pow3(F(2.0) * sigma - F(1.0)));
                v = 2;

                // how many steps can we do at once?:
                while (true) {
                    delta *= lambda;
                    Vector newestBeta = boxProject(newBeta + delta, lowerBound, upperBound);
                    delta = newestBeta - newBeta;
                    Vector newestE(data.height());
                    f(newestBeta.get().data(), newestE.get().data());
                    newestE = data - newestE;
                    FloatType newestESqrMag = sqrMagnitude(newestE);
                    if (newestESqrMag < newESqrMag) {
                        newBeta = newestBeta;
                        newE = newestE;
                        newESqrMag = newestESqrMag;
                    } else {
                        break;
                    }
                }
                delta = newBeta - beta;

                beta = newBeta;
                e = newE;
                eSqrMag = newESqrMag;

                oldJ = J;
                j(beta.get().data(), J.get().data());
                JTJ = transpose(J) * J;
                JTe = transpose(J) * e;
                if (maxAbsComponent(JTe) <= epsilons[0]) {
                    if (doLog)
                        std::cout << "exit exit exit 2" << std::endl;
                    goto breakOuterLoop;
                }

                if (maxAbsComponent(JTe) < F(0.02) * eSqrMag) {
                    // it might be time to switch method...
                    count++;
                    if(doLog)
                        LOG_VAR1(count)
                    if (count == 3) {
                        break; // inner loop and proceed with Quasi-Newton.
                    }
                } else {
                    count = 0;
                }
            }


            // Quasi-Newton:
            // update B:
            //Vector y = newJTJ * delta + transpose(newJ - J) * newE;
            //Vector y = newJTJ * delta + (newJT*newE - JT*newE);
            Vector y = JTJ * delta + transpose(J - oldJ) * e;
            FloatType deltaTy = dot(delta, y);
            if (deltaTy > 0) {
                Vector v = B * delta;
                B += (y * (F(1.0)/deltaTy)) * transpose(y) - (v * (F(1.0)/dot(delta, v))) * transpose(v);
            }

            FloatType deltaCap = std::max(F(1.5)*epsilons[1] * (sqrt(sqrMagnitude(beta))+epsilons[1]), F(1.5)*sqrt(sqrMagnitude(delta)));
            while (true) {
                iterations--;

                //? xnew := x;
                //Solve B * delta = -JTe;
                delta = invert(B) * JTe;
                Vector newBeta = boxProject(beta + delta, lowerBound, upperBound);
                delta = newBeta - beta;
                const FloatType sqrMagDelta = sqrMagnitude(delta);
                if (iterations <= 0 || sqrMagDelta <= pow2(epsilons[1] * (sqrMagnitude(beta) + epsilons[1]))) {
                    if (doLog)
                        std::cout << "exit exit exit 3" << std::endl;
                    goto breakOuterLoop;
                }

                if (sqrMagDelta > pow2(deltaCap))
                    delta *= (deltaCap / sqrt(sqrMagDelta));
                newBeta = boxProject(beta + delta, lowerBound, upperBound);
                delta = newBeta - beta;

                Vector newE(data.height());
                f(newBeta.get().data(), newE.get().data());
                newE = data - newE;
                FloatType newESqrMag = sqrMagnitude(newE);

                // how many steps can we do at once?:
                while (true) {
                    delta *= lambda;
                    Vector newestBeta = boxProject(newBeta + delta, lowerBound, upperBound);
                    delta = newestBeta - newBeta;
                    Vector newestE(data.height());
                    f(newestBeta.get().data(), newestE.get().data());
                    newestE = data - newestE;
                    FloatType newestESqrMag = sqrMagnitude(newestE);
                    if (newestESqrMag < newESqrMag) {
                        newBeta = newestBeta;
                        newE = newestE;
                        newESqrMag = newestESqrMag;
                    } else {
                        break;
                    }
                }
                delta = newBeta - beta;


                Matrix newJ(beta.height(), data.height());
                j(newBeta.get().data(), newJ.get().data());
                const Matrix newJTe = transpose(newJ) * newE;

                // Update deltaCap:
                FloatType sigma = (eSqrMag - newESqrMag) / dot(delta, (delta * mu + JTe) * F(0.5));
                if (sigma < F(0.25))
                    deltaCap = deltaCap / F(2.0);
                else if (sigma > F(0.75))
                    deltaCap = std::max(deltaCap, F(3.0)*sqrt(sqrMagnitude(delta)));

                if (maxAbsComponent(newJTe) <= epsilons[0]) {
                    if (doLog)
                        std::cout << "exit exit exit 4" << std::endl;
                    goto breakOuterLoop;
                }


                // update B:
                Matrix newJTJ = transpose(newJ) * newJ;
                Vector y = newJTJ * delta + transpose(newJ - J) * newE;
                //Vector y = newJTJ * delta + (newJT*newE - JT*newE);
                //Vector y = newJTJ * delta + (newJTe - JTe);
                FloatType deltaTy = dot(delta, y);
                if (deltaTy > 0) {
                    Vector v = B * delta;
                    B += (y * (F(1.0)/deltaTy)) * transpose(y) - (v * (F(1.0)/dot(delta, v))) * transpose(v);
                }


                const FloatType maxOldJTe = maxAbsComponent(JTe);
                static const FloatType deltaConst = sqrt(DBL_EPSILON);
                if ((newESqrMag < eSqrMag) /*|| (newESqrMag <= eSqrMag*(1+deltaConst)  &&  maxAbsComponent(newJTe) < maxAbsComponent(JTe))*/) {
                    // newBeta is better (no pun intended...)

                    beta = newBeta;
                    e = newE;
                    eSqrMag = newESqrMag;
                    oldJ = J;
                    J = newJ;
                    JTJ = newJTJ;
                    JTe = newJTe;
                }
                if (maxAbsComponent(JTe) >= maxOldJTe)
                    break; // inner loop and proceed with Levenberg-Marquardt.
            }

        }

        //std::cout << "exit -  " << iterations << std::endl;
        //std::cout << "stp " << iterations << std::endl;
    } catch (DeterminantIsZeroException) {
        // could not solve linear equations!!!
        std::cout << "Determinant was zero!" << std::endl;
    }

breakOuterLoop:
    if (doLog)
        std::cout << "stp " << " | " << iterations << std::endl;
    /* covariance matrix */
    covar.resize(beta.height(), beta.height());
    try {
        covar = invert(JTJ);
        covar *= eSqrMag/FloatType(data.height()-beta.height());
    } catch (DeterminantIsZeroException) {
        // could not solve linear equations!!!
        std::cout << "Bad _matrixJTJ, cannot calculate covariance!" << std::endl;
    }

    //std::cout << "fsumCounter: " << fsumCounter << std::endl;
    std::vector<double> result(beta.get().begin(), beta.get().end());

    return result;




}

template <typename Func, typename Jacobian>
std::vector<double> levenbergMarquardt(
    const std::vector<double> &dataIn,
    const std::vector<double> &initGuess,
    Func f, Jacobian j, Matrix &covar, std::array<FloatType, 3> epsilons = { 1E-17, 1E-17, 1E-17 },
    bool doLog = true
) {
    return levenbergMarquardtInternal(dataIn, initGuess, nullptr, nullptr, f, j, covar, epsilons, doLog);
}

template <typename Func, typename Jacobian>
std::vector<double> levenbergMarquardtLowerBound(
    const std::vector<double> &dataIn,
    const std::vector<double> &initGuess, const std::vector<double> &lowerBound,
    Func f, Jacobian j, Matrix &covar, std::array<FloatType, 3> epsilons = { 1E-17, 1E-17, 1E-17 },
    bool doLog = true
) {
    return levenbergMarquardtInternal(dataIn, initGuess, VectorView(const_cast<std::vector<double>&>(lowerBound)), nullptr, f, j, covar, epsilons, doLog);
}

template <typename Func, typename Jacobian>
std::vector<double> levenbergMarquardtUpperBound(
    const std::vector<double> &dataIn,
    const std::vector<double> &initGuess, const std::vector<double> &upperBound,
    Func f, Jacobian j, Matrix &covar, std::array<FloatType, 3> epsilons = { 1E-17, 1E-17, 1E-17 },
    bool doLog = true
) {
    return levenbergMarquardtInternal(dataIn, initGuess, nullptr, VectorView(const_cast<std::vector<double>&>(upperBound)), f, j, covar, epsilons, doLog);
}

template <typename Func, typename Jacobian>
std::vector<double> levenbergMarquardtBounded(
    const std::vector<double> &dataIn,
    const std::vector<double> &initGuess, const std::vector<double> &lowerBound, const std::vector<double> &upperBound,
    Func f, Jacobian j, Matrix &covar, std::array<FloatType, 3> epsilons = { 1E-17, 1E-17, 1E-17 },
    bool doLog = true
) {
    return levenbergMarquardtInternal(dataIn, initGuess, VectorView(const_cast<std::vector<double>&>(lowerBound)), VectorView(const_cast<std::vector<double>&>(upperBound)), f, j, covar, epsilons, doLog);
}
#undef F
#undef THROW_STR

#endif // MINIMIZER_H
