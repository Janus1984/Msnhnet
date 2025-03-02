#ifndef MSNHVECTORS_H
#define MSNHVECTORS_H

#include <iomanip>
#include <iostream>
#include <sstream>

#include "Msnhnet/config/MsnhnetCfg.h"

namespace Msnhnet {

template <int N, typename T>
class VectorS {
public:
    VectorS() = default;

    explicit VectorS(const std::vector<T>& val) {
        if (val.size() != N) {
            throw Exception(1,
                "[VectorS]: set val num must equal data num! \n", __FILE__,
                __LINE__, __FUNCTION__);
        }

        for (int i = 0; i < N; ++i) {
            this->_value[i] = val[i];
        }
    }

    VectorS(const VectorS& vec) {
        for (int i = 0; i < N; ++i) {
            this->_value[i] = vec[i];
        }
    }

    VectorS& operator=(const VectorS& vec) {
        if (this == &vec) {
            return *this;
        }
        for (int i = 0; i < N; ++i) {
            this->_value[i] = vec[i];
        }
        return *this;
    }

    VectorS& operator=(const std::vector<T>& val) {
        for (int i = 0; i < N; ++i) {
            this->_value[i] = val[i];
        }
        return *this;
    }

    void fill(const T& value) {
        for (int i = 0; i < N; ++i) {
            this->_value[i] = value;
        }
    }

    void print() {
        std::cout << "{ VectorS: " << N << '\n';
        if (isF32Vec()) {
            for (int i = 0; i < N; ++i) {
                std::cout << std::setiosflags(std::ios::left)
                          << std::setprecision(6) << std::setw(6)
                          << _value[i] << " ";
            }
        } else if (isF64Vec()) {
            for (int i = 0; i < N; ++i) {
                std::cout << std::setiosflags(std::ios::left)
                          << std::setprecision(12) << std::setw(12)
                          << _value[i] << " ";
            }
        } else {
            for (int i = 0; i < N; ++i) {
                std::cout << _value[i] << " ";
            }
        }

        std::cout << ";\n}" << std::endl;
    }

    std::string toString() const {
        std::stringstream buf;

        buf << "{ VectorS: " << N << '\n';
        if (isF32Vec()) {
            for (int i = 0; i < N; ++i) {
                buf << std::setiosflags(std::ios::left)
                    << std::setprecision(6) << std::setw(6) << _value[i]
                    << " ";
            }
        } else if (isF64Vec()) {
            for (int i = 0; i < N; ++i) {
                buf << std::setiosflags(std::ios::left)
                    << std::setprecision(12) << std::setw(12) << _value[i]
                    << " ";
            }
        } else {
            for (int i = 0; i < N; ++i) {
                buf << _value[i] << " ";
            }
        }

        buf << ";\n}" << std::endl;

        return buf.str();
    }

    std::string toHtmlString() const {
        std::stringstream buf;

        buf << "{ VectorS: " << N << "<br/>";
        if (isF32Vec()) {
            for (int i = 0; i < N; ++i) {
                buf << std::setiosflags(std::ios::left)
                    << std::setprecision(6) << std::setw(6) << _value[i]
                    << " ";
            }
        } else if (isF64Vec()) {
            for (int i = 0; i < N; ++i) {
                buf << std::setiosflags(std::ios::left)
                    << std::setprecision(12) << std::setw(12) << _value[i]
                    << " ";
            }
        } else {
            for (int i = 0; i < N; ++i) {
                buf << _value[i] << " ";
            }
        }

        buf << ";\n}" << "<br/>";
        return buf.str();
    }

    void setVal(const std::vector<T>& val) {
        if (val.size() != N) {
            throw Exception(1,
                "[VectorS]: set val num must equal data num! \n", __FILE__,
                __LINE__, __FUNCTION__);
        }

        for (int i = 0; i < N; ++i) {
            this->_value[i] = val[i];
        }
    }

    void setVal(const int& index, const T& val) {
        if (index > (N - 1)) {
            throw Exception(1, "[VectorS]: index out of memory! \n",
                __FILE__, __LINE__, __FUNCTION__);
        }

        this->_value[index] = val;
    }

    void zero() {
        for (int i = 0; i < N; ++i) {
            this->_value[i] = 0;
        }
    }

    void reverseSign() {
        for (int i = 0; i < N; ++i) {
            this->_value[i] = 0 - this->_value[i];
        }
    }

    bool isFuzzyNull() const {
        if (isF32Vec())

        {
            for (int i = 0; i < N; ++i) {
                if (fabsf(this->_value[i]) > MSNH_F32_EPS) {
                    return false;
                }
            }
            return true;
        }
        if (isF64Vec()) {
            for (int i = 0; i < N; ++i) {
                if (fabs(this->_value[i]) > MSNH_F64_EPS) {
                    return false;
                }
            }
            return true;
        }
        for (int i = 0; i < N; ++i) {
            if (this->_value[i] > 0) {
                return false;
            }
        }
        return true;
    }

    bool isNan() const {
        for (int i = 0; i < N; ++i) {
            if (std::isnan(static_cast<double>(this->_value[i]))) {
                return true;
            }
        }
        return false;
    }

    bool isF32Vec() const {
        return std::is_same_v<T, float>;
    }

    bool isF64Vec() const {
        return std::is_same_v<T, double>;
    }

    VectorS normalized() const {
        if (!(isF32Vec() || isF64Vec())) {
            throw Exception(1,
                "[VectorS] normalize only f32 and f64 is supported!",
                __FILE__, __LINE__, __FUNCTION__);
        }

        T len = 0;

        VectorS vec;

        for (int i = 0; i < N; ++i) {
            len += this->_value[i] * this->_value[i];
        }

        if (isF32Vec()) {
            if (fabsf(len - 1.0F) < MSNH_F32_EPS) {
                return *this;
            }

            if (fabsf(len) < MSNH_F32_EPS) {
                return vec;
            }

            len = sqrtf(len);
        } else if (isF64Vec()) {
            if (std::abs(len - 1.0) < MSNH_F64_EPS) {
                return *this;
            }

            if (std::abs(len) < MSNH_F64_EPS) {
                return vec;
            }

            len = sqrt(len);
        }

        for (int i = 0; i < N; ++i) {
            vec[i] = this->_value[i] / len;
        }

        return vec;
    }

    void normalize() {
        if (!(isF32Vec() || isF64Vec())) {
            throw Exception(1,
                "[VectorS] normalize only f32 and f64 is supported!",
                __FILE__, __LINE__, __FUNCTION__);
        }

        T len = 0;

        for (int i = 0; i < N; ++i) {
            len += this->_value[i] * this->_value[i];
        }

        if (this->isF32Vec()) {
            if (fabsf(len - 1.0F) < MSNH_F32_EPS ||
                fabsf(len) < MSNH_F32_EPS) {
                return;
            }

            len = sqrtf(len);
        } else {
            if (std::abs(len - 1.0) < MSNH_F64_EPS ||
                std::abs(len) < MSNH_F64_EPS) {
                return;
            }
            len = sqrt(len);
        }

        for (int i = 0; i < N; ++i) {
            this->_value[i] = this->_value[i] / len;
        }
    }

    double length() const {
        double len = 0;
        for (int i = 0; i < N; ++i) {
            len += this->_value[i] * this->_value[i];
        }
        return sqrt(len);
    }

    double lengthSquared() const {
        double len = 0;
        for (int i = 0; i < N; ++i) {
            len += this->_value[i] * this->_value[i];
        }
        return len;
    }

    T operator[](const int index) const {
        if (index > (N - 1)) {
            throw Exception(1, "[VectorS]: index out of memory! \n",
                __FILE__, __LINE__, __FUNCTION__);
        }
        return _value[index];
    }

    T& operator[](const int index) {
        if (index > (N - 1)) {
            throw Exception(1, "[VectorS]: index out of memory! \n",
                __FILE__, __LINE__, __FUNCTION__);
        }
        return _value[index];
    }

    friend VectorS operator+(const VectorS& A, const VectorS& B) {
        VectorS tmp;
        for (int i = 0; i < N; ++i) {
            tmp[i] = A[i] + B[i];
        }
        return tmp;
    }

    friend VectorS operator+(T A, const VectorS& B) {
        VectorS tmp;
        for (int i = 0; i < N; ++i) {
            tmp[i] = A + B[i];
        }
        return tmp;
    }

    friend VectorS operator+(const VectorS& A, T B) {
        VectorS tmp;
        for (int i = 0; i < N; ++i) {
            tmp[i] = A[i] + B;
        }
        return tmp;
    }

    friend VectorS operator-(const VectorS& A, const VectorS& B) {
        VectorS tmp;
        for (int i = 0; i < N; ++i) {
            tmp[i] = A[i] - B[i];
        }
        return tmp;
    }

    friend VectorS operator-(T A, const VectorS& B) {
        VectorS tmp;
        for (int i = 0; i < N; ++i) {
            tmp[i] = A - B[i];
        }
        return tmp;
    }

    friend VectorS operator-(const VectorS& A, T B) {
        VectorS tmp;
        for (int i = 0; i < N; ++i) {
            tmp[i] = A[i] - B;
        }
        return tmp;
    }

    friend VectorS operator-(const VectorS& A) {
        VectorS tmp;
        for (int i = 0; i < N; ++i) {
            tmp[i] = 0 - A[i];
        }
        return tmp;
    }

    friend VectorS operator*(const VectorS& A, const VectorS& B) {
        VectorS tmp;
        for (int i = 0; i < N; ++i) {
            tmp[i] = A[i] * B[i];
        }
        return tmp;
    }

    friend VectorS operator*(T A, const VectorS& B) {
        VectorS tmp;
        for (int i = 0; i < N; ++i) {
            tmp[i] = A * B[i];
        }
        return tmp;
    }

    friend VectorS operator*(const VectorS& A, T B) {
        VectorS tmp;
        for (int i = 0; i < N; ++i) {
            tmp[i] = A[i] * B;
        }
        return tmp;
    }

    friend VectorS operator/(const VectorS& A, T B) {
        VectorS tmp;
        for (int i = 0; i < N; ++i) {
            tmp[i] = A[i] / B;
        }
        return tmp;
    }

    friend VectorS operator/(const VectorS& A, const VectorS& B) {
        VectorS tmp;
        for (int i = 0; i < N; ++i) {
            tmp[i] = A[i] / B[i];
        }
        return tmp;
    }

    friend bool operator==(const VectorS& A, const VectorS& B) {
        if (A.isF32Vec()) {
            for (int i = 0; i < N; ++i) {
                if (fabsf(A[i] - B[i]) > MSNH_F32_EPS) {
                    return false;
                }
            }
        }
        if (A.isF64Vec()) {
            for (int i = 0; i < N; ++i) {
                if (fabsf(A[i] - B[i]) > MSNH_F64_EPS) {
                    return false;
                }
            }
        }
        for (int i = 0; i < N; ++i) {
            if (A[i] != B[i]) {
                return false;
            }
        }

        return true;
    }

    friend bool operator!=(const VectorS& A, const VectorS& B) {
        if (std::is_same_v<T, float>) {
            for (int i = 0; i < N; ++i) {
                if (fabsf(A[i] - B[i]) > MSNH_F32_EPS) {
                    return true;
                }
            }
        }
        if (std::is_same_v<T, double>) {
            for (int i = 0; i < N; ++i) {
                if (fabsf(A[i] - B[i]) > MSNH_F64_EPS) {
                    return true;
                }
            }
        } else {
            for (int i = 0; i < N; ++i) {
                if (A[i] != B[i]) {
                    return true;
                }
            }
        }
        return false;
    }

    VectorS& operator+=(const VectorS& A) {
        for (int i = 0; i < N; ++i) {
            this->_value[i] += A[i];
        }
        return *this;
    }

    VectorS& operator+=(T A) {
        for (int i = 0; i < N; ++i) {
            this->_value[i] += A;
        }
        return *this;
    }

    VectorS& operator-=(const VectorS& A) {
        for (int i = 0; i < N; ++i) {
            this->_value[i] -= A[i];
        }
        return *this;
    }

    VectorS& operator-=(T A) {
        for (int i = 0; i < N; ++i) {
            this->_value[i] -= A;
        }
        return *this;
    }

    VectorS& operator*=(const VectorS& A) {
        for (int i = 0; i < N; ++i) {
            this->_value[i] *= A[i];
        }
        return *this;
    }

    VectorS& operator*=(T A) {
        for (int i = 0; i < N; ++i) {
            this->_value[i] *= A;
        }
        return *this;
    }

    VectorS& operator/=(T A) {
        for (int i = 0; i < N; ++i) {
            this->_value[i] /= A;
        }
        return *this;
    }

private:
    std::array<T, N> _value;
};

template <int maxN, typename T>
class VectorXS {
public:
    int mN = 0;
    std::array<T, maxN> mValue;

    explicit VectorXS(const int n) {
        assert(n <= maxN);
        mN = n;
        fill(0);
    }

    VectorXS() = default;

    explicit VectorXS(const std::vector<T>& vec) {
        assert(!vec.empty());
        assert((int)vec.size() <= maxN);
        mN = (int)vec.size();

        for (int i = 0; i < mN / 4; ++i) {
            mValue[(i << 2) + 0] = vec[(i << 2) + 0];
            mValue[(i << 2) + 1] = vec[(i << 2) + 1];
            mValue[(i << 2) + 2] = vec[(i << 2) + 2];
            mValue[(i << 2) + 3] = vec[(i << 2) + 3];
        }

        for (int i = 4 * (mN / 4); i < mN; ++i) {
            mValue[i] = vec[i];
        }
    }

    VectorXS(const VectorXS& vec) : mN(vec.mN) {
        for (int i = 0; i < mN / 4; ++i) {
            mValue[(i << 2) + 0] = vec.mValue[(i << 2) + 0];
            mValue[(i << 2) + 1] = vec.mValue[(i << 2) + 1];
            mValue[(i << 2) + 2] = vec.mValue[(i << 2) + 2];
            mValue[(i << 2) + 3] = vec.mValue[(i << 2) + 3];
        }

        for (int i = 4 * (mN / 4); i < mN; ++i) {
            mValue[i] = vec.mValue[i];
        }
    }

    VectorXS& operator=(const VectorXS& vec) {
        if (this != &vec) {
            this->mN = vec.mN;
            for (int i = 0; i < mN / 4; ++i) {
                mValue[(i << 2) + 0] = vec.mValue[(i << 2) + 0];
                mValue[(i << 2) + 1] = vec.mValue[(i << 2) + 1];
                mValue[(i << 2) + 2] = vec.mValue[(i << 2) + 2];
                mValue[(i << 2) + 3] = vec.mValue[(i << 2) + 3];
            }

            for (int i = 4 * (mN / 4); i < mN; ++i) {
                mValue[i] = vec.mValue[i];
            }
        }

        return *this;
    }

    VectorXS& operator=(const std::vector<T>& vec) {
        assert(!vec.empty());
        assert((int)vec.size() <= maxN);
        mN = (int)vec.size();

        for (int i = 0; i < mN / 4; ++i) {
            mValue[(i << 2) + 0] = vec[(i << 2) + 0];
            mValue[(i << 2) + 1] = vec[(i << 2) + 1];
            mValue[(i << 2) + 2] = vec[(i << 2) + 2];
            mValue[(i << 2) + 3] = vec[(i << 2) + 3];
        }

        for (int i = 4 * (mN / 4); i < mN; ++i) {
            mValue[i] = vec[i];
        }
        return *this;
    }

    void fill(const T& value) {
        for (int i = 0; i < mN / 4; ++i) {
            mValue[(i << 2) + 0] = value;
            mValue[(i << 2) + 1] = value;
            mValue[(i << 2) + 2] = value;
            mValue[(i << 2) + 3] = value;
        }

        for (int i = 4 * (mN / 4); i < mN; ++i) {
            mValue[i] = value;
        }
    }

    void print() {
        std::cout << "{ VectorXS: " << mN << '\n';
        if (isF32Vec()) {
            for (int i = 0; i < mN; ++i) {
                std::cout << std::setiosflags(std::ios::left)
                          << std::setprecision(6) << std::setw(6)
                          << mValue[i] << " ";
            }
        } else if (isF64Vec()) {
            for (int i = 0; i < mN; ++i) {
                std::cout << std::setiosflags(std::ios::left)
                          << std::setprecision(12) << std::setw(12)
                          << mValue[i] << " ";
            }
        } else {
            for (int i = 0; i < mN; ++i) {
                std::cout << mValue[i] << " ";
            }
        }

        std::cout << ";\n}" << std::endl;
    }

    std::string toString() const {
        std::stringstream buf;

        buf << "{ VectorXS: " << mN << '\n';
        if (isF32Vec()) {
            for (int i = 0; i < mN; ++i) {
                buf << std::setiosflags(std::ios::left)
                    << std::setprecision(6) << std::setw(6) << mValue[i]
                    << " ";
            }
        } else if (isF64Vec()) {
            for (int i = 0; i < mN; ++i) {
                buf << std::setiosflags(std::ios::left)
                    << std::setprecision(12) << std::setw(12) << mValue[i]
                    << " ";
            }
        } else {
            for (int i = 0; i < mN; ++i) {
                buf << mValue[i] << " ";
            }
        }

        buf << ";\n}" << std::endl;

        return buf.str();
    }

    std::string toHtmlString() const {
        std::stringstream buf;

        buf << "{ VectorXS: " << mN << "<br/>";
        if (isF32Vec()) {
            for (int i = 0; i < mN; ++i) {
                buf << std::setiosflags(std::ios::left)
                    << std::setprecision(6) << std::setw(6) << mValue[i]
                    << " ";
            }
        } else if (isF64Vec()) {
            for (int i = 0; i < mN; ++i) {
                buf << std::setiosflags(std::ios::left)
                    << std::setprecision(12) << std::setw(12) << mValue[i]
                    << " ";
            }
        } else {
            for (int i = 0; i < mN; ++i) {
                buf << mValue[i] << " ";
            }
        }

        buf << ";\n}" << "<br/>";

        return buf.str();
    }

    void setVal(const std::vector<T>& val) {
        assert(val.size() == mN);

        for (int i = 0; i < mN / 4; ++i) {
            mValue[(i << 2) + 0] = val[(i << 2) + 0];
            mValue[(i << 2) + 1] = val[(i << 2) + 1];
            mValue[(i << 2) + 2] = val[(i << 2) + 2];
            mValue[(i << 2) + 3] = val[(i << 2) + 3];
        }

        for (int i = 4 * (mN / 4); i < mN; ++i) {
            mValue[i] = val[i];
        }
    }

    void setVal(const int index, const T& val) {
        assert(index < mN);
        this->mValue[index] = val;
    }

    void zero() {
        for (int i = 0; i < mN / 4; ++i) {
            mValue[(i << 2) + 0] = 0;
            mValue[(i << 2) + 1] = 0;
            mValue[(i << 2) + 2] = 0;
            mValue[(i << 2) + 3] = 0;
        }

        for (int i = 4 * (mN / 4); i < mN; ++i) {
            mValue[i] = 0;
        }
    }

    void reverseSign() {
        for (int i = 0; i < mN / 4; ++i) {
            mValue[(i << 2) + 0] = -mValue[(i << 2) + 0];
            mValue[(i << 2) + 1] = -mValue[(i << 2) + 1];
            mValue[(i << 2) + 2] = -mValue[(i << 2) + 2];
            mValue[(i << 2) + 3] = -mValue[(i << 2) + 3];
        }

        for (int i = 4 * (mN / 4); i < mN; ++i) {
            mValue[i] = -mValue[i];
        }
    }

    bool isFuzzyNull() const {
        if (isF32Vec()) {
            for (int i = 0; i < mN; ++i) {
                if (fabsf(this->mValue[i]) > MSNH_F32_EPS) {
                    return false;
                }
            }
            return true;
        }
        if (isF64Vec()) {
            for (int i = 0; i < mN; ++i) {
                if (fabs(this->mValue[i]) > MSNH_F64_EPS) {
                    return false;
                }
            }
            return true;
        }

        for (int i = 0; i < mN; ++i) {
            if (this->mValue[i] > 0) {
                return false;
            }
        }
        return true;
    }

    bool isFuzzyNull(double eps) const {
        for (int i = 0; i < mN; ++i) {
            if (fabs(this->mValue[i]) > eps) {
                return false;
            }
        }
        return true;
    }

    bool isNan() const {
        for (int i = 0; i < mN; ++i) {
            if (std::isnan(static_cast<double>(this->mValue[i]))) {
                return true;
            }
        }
        return false;
    }

    bool isF32Vec() const {
        return std::is_same_v<T, float>;
    }

    bool isF64Vec() const {
        return std::is_same_v<T, double>;
    }

    VectorXS normalized() const {
        assert(isF32Vec() || isF64Vec());

        T len = 0;

        VectorXS vec(mN);

        for (int i = 0; i < mN / 4; ++i) {
            len += this->mValue[(i << 2) + 0] * this->mValue[(i << 2) + 0];
            len += this->mValue[(i << 2) + 1] * this->mValue[(i << 2) + 1];
            len += this->mValue[(i << 2) + 2] * this->mValue[(i << 2) + 2];
            len += this->mValue[(i << 2) + 3] * this->mValue[(i << 2) + 3];
        }

        for (int i = 4 * (mN / 4); i < mN; ++i) {
            len += this->mValue[i] * this->mValue[i];
        }

        if (isF32Vec()) {
            if (fabsf(len - 1.0F) < MSNH_F32_EPS) {
                return *this;
            }

            if (fabsf(len) < MSNH_F32_EPS) {
                return vec;
            }

            len = sqrtf(len);
        } else if (isF64Vec()) {
            if (std::abs(len - 1.0) < MSNH_F64_EPS) {
                return *this;
            }

            if (std::abs(len) < MSNH_F64_EPS) {
                return vec;
            }

            len = sqrt(len);
        }

        for (int i = 0; i < mN; ++i) {
            vec[i] = this->mValue[i] / len;
        }

        return vec;
    }

    void normalize() {
        if (!(isF32Vec() || isF64Vec())) {
            throw Exception(1,
                "[VectorXS] normalize only f32 and f64 is supported!",
                __FILE__, __LINE__, __FUNCTION__);
        }

        T len = 0;

        for (int i = 0; i < mN / 4; ++i) {
            len += this->mValue[(i << 2) + 0] * this->mValue[(i << 2) + 0];
            len += this->mValue[(i << 2) + 1] * this->mValue[(i << 2) + 1];
            len += this->mValue[(i << 2) + 2] * this->mValue[(i << 2) + 2];
            len += this->mValue[(i << 2) + 3] * this->mValue[(i << 2) + 3];
        }

        for (int i = 4 * (mN / 4); i < mN; ++i) {
            len += this->mValue[i] * this->mValue[i];
        }

        if (this->isF32Vec()) {
            if (fabsf(len - 1.0F) < MSNH_F32_EPS ||
                fabsf(len) < MSNH_F32_EPS) {
                return;
            }

            len = sqrtf(len);
        } else {
            if (std::abs(len - 1.0) < MSNH_F64_EPS ||
                std::abs(len) < MSNH_F64_EPS) {
                return;
            }
            len = sqrt(len);
        }

        for (int i = 0; i < mN; ++i) {
            this->mValue[i] = this->mValue[i] / len;
        }
    }

    double length() const {
        double len = 0;
        for (int i = 0; i < mN / 4; ++i) {
            len += this->mValue[(i << 2) + 0] * this->mValue[(i << 2) + 0];
            len += this->mValue[(i << 2) + 1] * this->mValue[(i << 2) + 1];
            len += this->mValue[(i << 2) + 2] * this->mValue[(i << 2) + 2];
            len += this->mValue[(i << 2) + 3] * this->mValue[(i << 2) + 3];
        }

        for (int i = 4 * (mN / 4); i < mN; ++i) {
            len += this->mValue[i] * this->mValue[i];
        }

        return sqrt(len);
    }

    double lengthSquared() const {
        double len = 0;
        for (int i = 0; i < mN / 4; ++i) {
            len += this->mValue[(i << 2) + 0] * this->mValue[(i << 2) + 0];
            len += this->mValue[(i << 2) + 1] * this->mValue[(i << 2) + 1];
            len += this->mValue[(i << 2) + 2] * this->mValue[(i << 2) + 2];
            len += this->mValue[(i << 2) + 3] * this->mValue[(i << 2) + 3];
        }

        for (int i = 4 * (mN / 4); i < mN; ++i) {
            len += this->mValue[i] * this->mValue[i];
        }
        return len;
    }

    static T dotProduct(const VectorXS& A, const VectorXS& B) {
        assert(A.mN == B.mN);

        T finalVal = 0;

        for (int i = 0; i < A.mN / 4; ++i) {
            finalVal += A[(i << 2) + 0] * B[(i << 2) + 0];
            finalVal += A[(i << 2) + 1] * B[(i << 2) + 1];
            finalVal += A[(i << 2) + 2] * B[(i << 2) + 2];
            finalVal += A[(i << 2) + 3] * B[(i << 2) + 3];
        }

        for (int i = 4 * (A.mN / 4); i < A.mN; ++i) {
            finalVal += A[i] * B[i];
        }
        return finalVal;
    }

    T operator[](const int index) const {
        assert(index < mN);
        return mValue[index];
    }

    T& operator[](const int index) {
        assert(index < mN);
        return mValue[index];
    }

    T operator()(const int index) const {
        assert(index < mN);
        return mValue[index];
    }

    T& operator()(const int index) {
        assert(index < mN);
        return mValue[index];
    }

    friend VectorXS operator+(const VectorXS& A, const VectorXS& B) {
        assert(A.mN == B.mN);

        VectorXS tmp(A.mN);

        for (int i = 0; i < B.mN / 4; ++i) {
            tmp[(i << 2) + 0] = A[(i << 2) + 0] + B[(i << 2) + 0];
            tmp[(i << 2) + 1] = A[(i << 2) + 1] + B[(i << 2) + 1];
            tmp[(i << 2) + 2] = A[(i << 2) + 2] + B[(i << 2) + 2];
            tmp[(i << 2) + 3] = A[(i << 2) + 3] + B[(i << 2) + 3];
        }

        for (int i = 4 * (B.mN / 4); i < B.mN; ++i) {
            tmp[i] = A[i] + B[i];
        }

        return tmp;
    }

    friend VectorXS operator+(T A, const VectorXS& B) {
        VectorXS tmp(B.mN);
        for (int i = 0; i < B.mN / 4; ++i) {
            tmp[(i << 2) + 0] = A + B[(i << 2) + 0];
            tmp[(i << 2) + 1] = A + B[(i << 2) + 1];
            tmp[(i << 2) + 2] = A + B[(i << 2) + 2];
            tmp[(i << 2) + 3] = A + B[(i << 2) + 3];
        }

        for (int i = 4 * (B.mN / 4); i < B.mN; ++i) {
            tmp[i] = A + B[i];
        }
        return tmp;
    }

    friend VectorXS operator+(const VectorXS& A, T B) {
        VectorXS tmp(A.mN);
        for (int i = 0; i < B.mN / 4; ++i) {
            tmp[(i << 2) + 0] = A[(i << 2) + 0] + B;
            tmp[(i << 2) + 1] = A[(i << 2) + 1] + B;
            tmp[(i << 2) + 2] = A[(i << 2) + 2] + B;
            tmp[(i << 2) + 3] = A[(i << 2) + 3] + B;
        }

        for (int i = 4 * (B.mN / 4); i < B.mN; ++i) {
            tmp[i] = A[i] + B;
        }
        return tmp;
    }

    friend VectorXS operator-(const VectorXS& A, const VectorXS& B) {
        assert(A.mN == B.mN);

        VectorXS tmp(A.mN);

        for (int i = 0; i < A.mN / 4; ++i) {
            tmp[(i << 2) + 0] = A[(i << 2) + 0] - B[(i << 2) + 0];
            tmp[(i << 2) + 1] = A[(i << 2) + 1] - B[(i << 2) + 1];
            tmp[(i << 2) + 2] = A[(i << 2) + 2] - B[(i << 2) + 2];
            tmp[(i << 2) + 3] = A[(i << 2) + 3] - B[(i << 2) + 3];
        }

        for (int i = 4 * (A.mN / 4); i < A.mN; ++i) {
            tmp[i] = A[i] - B[i];
        }

        return tmp;
    }

    friend VectorXS operator-(T A, const VectorXS& B) {
        VectorXS tmp(B.mN);
        for (int i = 0; i < B.mN / 4; ++i) {
            tmp[(i << 2) + 0] = A - B[(i << 2) + 0];
            tmp[(i << 2) + 1] = A - B[(i << 2) + 1];
            tmp[(i << 2) + 2] = A - B[(i << 2) + 2];
            tmp[(i << 2) + 3] = A - B[(i << 2) + 3];
        }

        for (int i = 4 * (B.mN / 4); i < B.mN; ++i) {
            tmp[i] = A - B[i];
        }
        return tmp;
    }

    friend VectorXS operator-(const VectorXS& A, T B) {
        VectorXS tmp(A.mN);
        for (int i = 0; i < A.mN / 4; ++i) {
            tmp[(i << 2) + 0] = A[(i << 2) + 0] - B;
            tmp[(i << 2) + 1] = A[(i << 2) + 1] - B;
            tmp[(i << 2) + 2] = A[(i << 2) + 2] - B;
            tmp[(i << 2) + 3] = A[(i << 2) + 3] - B;
        }

        for (int i = 4 * (A.mN / 4); i < A.mN; ++i) {
            tmp[i] = A[i] - B;
        }
        return tmp;
    }

    friend VectorXS operator-(const VectorXS& A) {
        return 0 - A;
    }

    friend VectorXS operator*(const VectorXS& A, const VectorXS& B) {
        assert(A.mN == B.mN);

        VectorXS tmp(A.mN);

        for (int i = 0; i < A.mN / 4; ++i) {
            tmp[(i << 2) + 0] = A[(i << 2) + 0] * B[(i << 2) + 0];
            tmp[(i << 2) + 1] = A[(i << 2) + 1] * B[(i << 2) + 1];
            tmp[(i << 2) + 2] = A[(i << 2) + 2] * B[(i << 2) + 2];
            tmp[(i << 2) + 3] = A[(i << 2) + 3] * B[(i << 2) + 3];
        }

        for (int i = 4 * (A.mN / 4); i < A.mN; ++i) {
            tmp[i] = A[i] * B[i];
        }

        return tmp;
    }

    friend VectorXS operator*(T A, const VectorXS& B) {
        VectorXS tmp(B.mN);
        for (int i = 0; i < B.mN / 4; ++i) {
            tmp[(i << 2) + 0] = A * B[(i << 2) + 0];
            tmp[(i << 2) + 1] = A * B[(i << 2) + 1];
            tmp[(i << 2) + 2] = A * B[(i << 2) + 2];
            tmp[(i << 2) + 3] = A * B[(i << 2) + 3];
        }

        for (int i = 4 * (B.mN / 4); i < B.mN; ++i) {
            tmp[i] = A * B[i];
        }
        return tmp;
    }

    friend VectorXS operator*(const VectorXS& A, T B) {
        VectorXS tmp(A.mN);
        for (int i = 0; i < A.mN / 4; ++i) {
            tmp[(i << 2) + 0] = A[(i << 2) + 0] * B;
            tmp[(i << 2) + 1] = A[(i << 2) + 1] * B;
            tmp[(i << 2) + 2] = A[(i << 2) + 2] * B;
            tmp[(i << 2) + 3] = A[(i << 2) + 3] * B;
        }

        for (int i = 4 * (A.mN / 4); i < A.mN; ++i) {
            tmp[i] = A[i] * B;
        }
        return tmp;
    }

    friend VectorXS operator/(const VectorXS& A, T B) {
        VectorXS tmp(A.mN);
        for (int i = 0; i < A.mN / 4; ++i) {
            tmp[(i << 2) + 0] = A[(i << 2) + 0] / B;
            tmp[(i << 2) + 1] = A[(i << 2) + 1] / B;
            tmp[(i << 2) + 2] = A[(i << 2) + 2] / B;
            tmp[(i << 2) + 3] = A[(i << 2) + 3] / B;
        }

        for (int i = 4 * (A.mN / 4); i < A.mN; ++i) {
            tmp[i] = A[i] / B;
        }
        return tmp;
    }

    friend VectorXS operator/(const VectorXS& A, const VectorXS& B) {
        assert(A.mN == B.mN);

        VectorXS tmp(A.mN);

        for (int i = 0; i < A.mN / 4; ++i) {
            tmp[(i << 2) + 0] = A[(i << 2) + 0] / B[(i << 2) + 0];
            tmp[(i << 2) + 1] = A[(i << 2) + 1] / B[(i << 2) + 1];
            tmp[(i << 2) + 2] = A[(i << 2) + 2] / B[(i << 2) + 2];
            tmp[(i << 2) + 3] = A[(i << 2) + 3] / B[(i << 2) + 3];
        }

        for (int i = 4 * (A.mN / 4); i < A.mN; ++i) {
            tmp[i] = A[i] / B[i];
        }

        return tmp;
    }

    friend bool operator==(const VectorXS& A, const VectorXS& B) {
        if (A.mN != B.mN) {
            return false;
        }

        if (A.isF32Vec()) {
            for (int i = 0; i < A.mN; ++i) {
                if (fabsf(A[i] - B[i]) > MSNH_F32_EPS) {
                    return false;
                }
            }
        } else if (A.isF64Vec()) {
            for (int i = 0; i < A.mN; ++i) {
                if (fabsf(A[i] - B[i]) > MSNH_F64_EPS) {
                    return false;
                }
            }
        } else {
            for (int i = 0; i < A.mN; ++i) {
                if (A[i] != B[i]) {
                    return false;
                }
            }
        }
        return true;
    }

    friend bool operator!=(const VectorXS& A, const VectorXS& B) {
        if (A.mN != B.mN) {
            return true;
        }

        if (std::is_same_v<T, float>) {
            for (int i = 0; i < A.mN; ++i) {
                if (fabsf(A[i] - B[i]) > MSNH_F32_EPS) {
                    return true;
                }
            }
        } else if (std::is_same_v<T, double>) {
            for (int i = 0; i < A.mN; ++i) {
                if (fabsf(A[i] - B[i]) > MSNH_F64_EPS) {
                    return true;
                }
            }
        } else {
            for (int i = 0; i < A.mN; ++i) {
                if (A[i] != B[i]) {
                    return true;
                }
            }
        }
        return false;
    }

    VectorXS& operator+=(const VectorXS& A) {
        for (int i = 0; i < mN / 4; ++i) {
            this->mValue[(i << 2) + 0] += A[(i << 2) + 0];
            this->mValue[(i << 2) + 1] += A[(i << 2) + 1];
            this->mValue[(i << 2) + 2] += A[(i << 2) + 2];
            this->mValue[(i << 2) + 3] += A[(i << 2) + 3];
        }

        for (int i = 4 * (mN / 4); i < mN; ++i) {
            this->mValue[i] += A[i];
        }
        return *this;
    }

    VectorXS& operator+=(T A) {
        for (int i = 0; i < mN / 4; ++i) {
            this->mValue[(i << 2) + 0] += A;
            this->mValue[(i << 2) + 1] += A;
            this->mValue[(i << 2) + 2] += A;
            this->mValue[(i << 2) + 3] += A;
        }

        for (int i = 4 * (mN / 4); i < mN; ++i) {
            this->mValue[i] += A;
        }
        return *this;
    }

    VectorXS& operator-=(const VectorXS& A) {
        for (int i = 0; i < mN / 4; ++i) {
            this->mValue[(i << 2) + 0] -= A[(i << 2) + 0];
            this->mValue[(i << 2) + 1] -= A[(i << 2) + 1];
            this->mValue[(i << 2) + 2] -= A[(i << 2) + 2];
            this->mValue[(i << 2) + 3] -= A[(i << 2) + 3];
        }

        for (int i = 4 * (mN / 4); i < mN; ++i) {
            this->mValue[i] -= A[i];
        }
        return *this;
    }

    VectorXS& operator-=(T A) {
        for (int i = 0; i < mN / 4; ++i) {
            this->mValue[(i << 2) + 0] -= A;
            this->mValue[(i << 2) + 1] -= A;
            this->mValue[(i << 2) + 2] -= A;
            this->mValue[(i << 2) + 3] -= A;
        }

        for (int i = 4 * (mN / 4); i < mN; ++i) {
            this->mValue[i] -= A;
        }
        return *this;
    }

    VectorXS& operator*=(const VectorXS& A) {
        for (int i = 0; i < mN / 4; ++i) {
            this->mValue[(i << 2) + 0] *= A[(i << 2) + 0];
            this->mValue[(i << 2) + 1] *= A[(i << 2) + 1];
            this->mValue[(i << 2) + 2] *= A[(i << 2) + 2];
            this->mValue[(i << 2) + 3] *= A[(i << 2) + 3];
        }

        for (int i = 4 * (mN / 4); i < mN; ++i) {
            this->mValue[i] *= A[i];
        }
        return *this;
    }

    VectorXS& operator*=(T A) {
        for (int i = 0; i < mN / 4; ++i) {
            this->mValue[(i << 2) + 0] *= A;
            this->mValue[(i << 2) + 1] *= A;
            this->mValue[(i << 2) + 2] *= A;
            this->mValue[(i << 2) + 3] *= A;
        }

        for (int i = 4 * (mN / 4); i < mN; ++i) {
            this->mValue[i] *= A;
        }
        return *this;
    }

    VectorXS& operator/=(T A) {
        for (int i = 0; i < mN / 4; ++i) {
            this->mValue[(i << 2) + 0] /= A;
            this->mValue[(i << 2) + 1] /= A;
            this->mValue[(i << 2) + 2] /= A;
            this->mValue[(i << 2) + 3] /= A;
        }

        for (int i = 4 * (mN / 4); i < mN; ++i) {
            this->mValue[i] /= A;
        }
        return *this;
    }
};

using VectorXSDS = VectorXS<16, double>;
using VectorXMDS = VectorXS<128, double>;
using VectorXBDS = VectorXS<1024, double>;

using VectorXSFS = VectorXS<16, float>;
using VectorXMFS = VectorXS<128, float>;
using VectorXBFS = VectorXS<1024, float>;

using Vector2DS = VectorS<2, double>;
using Vector4DS = VectorS<4, double>;
using Vector5DS = VectorS<5, double>;
using Vector4DS = VectorS<4, double>;
using Vector6DS = VectorS<6, double>;
using Vector7DS = VectorS<7, double>;

using Vector2FS = VectorS<2, float>;
using Vector4FS = VectorS<4, float>;
using Vector5FS = VectorS<5, float>;
using Vector6FS = VectorS<6, float>;

}  // namespace Msnhnet

#endif  // __MSNH_VECTOR_S__
