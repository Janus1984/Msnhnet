#ifndef MSNHQUATERNIONS_H
#define MSNHQUATERNIONS_H

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Msnhnet/config/MsnhnetCfg.h"

namespace Msnhnet {

class MsnhNet_API QuaternionDS {
public:
    double q0;
    double q1;
    double q2;
    double q3;

    QuaternionDS() : q0(0), q1(0), q2(0), q3(0) { }

    QuaternionDS(
        const double q0, const double q1, const double q2, const double q3)
        : q0(q0), q1(q1), q2(q2), q3(q3) { }

    QuaternionDS(const QuaternionDS& q) = default;

    QuaternionDS& operator=(const QuaternionDS& q) {
        if (this != &q) {
            q0 = q.q0;
            q1 = q.q1;
            q2 = q.q2;
            q3 = q.q3;
        }
        return *this;
    }

    // move constructor
    QuaternionDS(QuaternionDS&& q) noexcept = default;

    // move assignment
    QuaternionDS& operator=(QuaternionDS&& q) noexcept = default;

    // destructor
    ~QuaternionDS() = default;

    void setVal(const double q0, const double q1, const double q2,
        const double q3) {
        this->q0 = q0;
        this->q1 = q1;
        this->q2 = q2;
        this->q3 = q3;
    }

    double mod() const {
        return sqrt((q0 * q0) + (q1 * q1) + (q2 * q2) + (q2 * q2));
    }

    QuaternionDS invert() const {
        double tmp = mod();
        return {q0 / tmp, q1 / tmp, q2 / tmp, q3 / tmp};
    }

    void print() const;

    std::string toString() const;

    std::string toHtmlStriSng() const;

    bool operator==(const QuaternionDS& q) const {
        return fabs(q0 - q.q0) < MSNH_F64_EPS &&
               fabs(q1 - q.q1) < MSNH_F64_EPS &&
               fabs(q2 - q.q2) < MSNH_F64_EPS &&
               fabs(q3 - q.q3) < MSNH_F64_EPS;
    }

    bool operator!=(const QuaternionDS& q) const {
        return !(*this == q);
    }

    friend QuaternionDS operator-(
        const QuaternionDS& A, const QuaternionDS& B) {
        return {A.q0 - B.q0, A.q1 - B.q1, A.q2 - B.q2, A.q3 - B.q3};
    }

    friend QuaternionDS operator+(
        const QuaternionDS& A, const QuaternionDS& B) {
        return {A.q0 + B.q0, A.q1 + B.q1, A.q2 + B.q2, A.q3 + B.q3};
    }

    friend QuaternionDS operator*(
        const QuaternionDS& A, const QuaternionDS& B) {
        return {
            (A.q0 * B.q0) - (A.q1 * B.q1) - (A.q2 * B.q2) - (A.q3 * B.q3),
            (A.q0 * B.q1) + (A.q1 * B.q0) + (A.q2 * B.q3) - (A.q3 * B.q2),
            (A.q0 * B.q2) - (A.q1 * B.q3) + (A.q2 * B.q0) + (A.q3 * B.q1),
            (A.q0 * B.q3) + (A.q1 * B.q2) - (A.q2 * B.q1) + (A.q3 * B.q0)};
    }

    friend QuaternionDS operator/(
        const QuaternionDS& A, const QuaternionDS& B) {
        return A * B.invert();
    }
};

class MsnhNet_API QuaternionFS {
public:
    float q0;
    float q1;
    float q2;
    float q3;

    QuaternionFS() : q0(0), q1(0), q2(0), q3(0) { }

    QuaternionFS(
        const float q0, const float q1, const float q2, const float q3)
        : q0(q0), q1(q1), q2(q2), q3(q3) { }

    QuaternionFS(const QuaternionFS& q) = default;

    QuaternionFS& operator=(const QuaternionFS& q) {
        if (this != &q) {
            q0 = q.q0;
            q1 = q.q1;
            q2 = q.q2;
            q3 = q.q3;
        }
        return *this;
    }

    // move constructor
    QuaternionFS(QuaternionFS&& q) noexcept = default;

    // move assignment
    QuaternionFS& operator=(QuaternionFS&& q) noexcept = default;

    // destructor
    ~QuaternionFS() = default;

    void setVal(
        const float q0, const float q1, const float q2, const float q3) {
        this->q0 = q0;
        this->q1 = q1;
        this->q2 = q2;
        this->q3 = q3;
    }

    float mod() const {
        return sqrtf((q0 * q0) + (q1 * q1) + (q2 * q2) + (q2 * q2));
    }

    QuaternionFS invert() const {
        float tmp = mod();
        return {q0 / tmp, q1 / tmp, q2 / tmp, q3 / tmp};
    }

    void print() const;

    std::string toString() const;

    std::string toHtmlString() const;

    bool operator==(const QuaternionFS& q) const {
        return fabsf(q0 - q.q0) < MSNH_F32_EPS &&
               fabsf(q1 - q.q1) < MSNH_F32_EPS &&
               fabsf(q2 - q.q2) < MSNH_F32_EPS &&
               fabsf(q3 - q.q3) < MSNH_F32_EPS;
    }

    bool operator!=(const QuaternionFS& q) const {
        return !(*this == q);
    }

    friend QuaternionFS operator-(
        const QuaternionFS& A, const QuaternionFS& B) {
        return {A.q0 - B.q0, A.q1 - B.q1, A.q2 - B.q2, A.q3 - B.q3};
    }

    friend QuaternionFS operator+(
        const QuaternionFS& A, const QuaternionFS& B) {
        return {A.q0 + B.q0, A.q1 + B.q1, A.q2 + B.q2, A.q3 + B.q3};
    }

    friend QuaternionFS operator*(
        const QuaternionFS& A, const QuaternionFS& B) {
        return {
            (A.q0 * B.q0) - (A.q1 * B.q1) - (A.q2 * B.q2) - (A.q3 * B.q3),
            (A.q0 * B.q1) + (A.q1 * B.q0) + (A.q2 * B.q3) - (A.q3 * B.q2),
            (A.q0 * B.q2) - (A.q1 * B.q3) + (A.q2 * B.q0) + (A.q3 * B.q1),
            (A.q0 * B.q3) + (A.q1 * B.q2) - (A.q2 * B.q1) + (A.q3 * B.q0)};
    }

    friend QuaternionFS operator/(
        const QuaternionFS& A, const QuaternionFS& B) {
        return A * B.invert();
    }
};

}  // namespace Msnhnet

#endif  // MSNHQUATERNIONS_H
