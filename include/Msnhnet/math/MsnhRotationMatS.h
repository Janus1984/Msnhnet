#ifndef MSNHROTATIONMAT_H
#define MSNHROTATIONMAT_H

#include <math.h>

#include "Msnhnet/math/MsnhVector3S.h"

namespace Msnhnet {

class MsnhNet_API RotationMatDS {
public:
    std::array<double, 9> val {1, 0, 0, 0, 1, 0, 0, 0, 1};

public:
    // default constructor
    RotationMatDS() = default;

    // constructor
    explicit RotationMatDS(const std::vector<double>& vec) {
        assert(vec.size() == 9);
        val[0] = vec[0];
        val[1] = vec[1];
        val[2] = vec[2];
        val[3] = vec[3];
        val[4] = vec[4];
        val[5] = vec[5];
        val[6] = vec[6];
        val[7] = vec[7];
        val[8] = vec[8];
    }

    RotationMatDS(double a11, double a12, double a13, double a21,
        double a22, double a23, double a31, double a32, double a33)
        : val {a11, a12, a13, a21, a22, a23, a31, a32, a33} { }

    RotationMatDS(
        const Vector3DS& x, const Vector3DS& y, const Vector3DS& z) {
        val[0] = x.val[0];
        val[1] = y.val[0];
        val[2] = z.val[0];
        val[3] = x.val[1];
        val[4] = y.val[1];
        val[5] = z.val[1];
        val[6] = x.val[2];
        val[7] = y.val[2];
        val[8] = z.val[2];
    }

    // copy constructor
    RotationMatDS(const RotationMatDS& vec) {
        val[0] = vec.val[0];
        val[1] = vec.val[1];
        val[2] = vec.val[2];
        val[3] = vec.val[3];
        val[4] = vec.val[4];
        val[5] = vec.val[5];
        val[6] = vec.val[6];
        val[7] = vec.val[7];
        val[8] = vec.val[8];
    }

    // copy assignment
    RotationMatDS& operator=(const RotationMatDS& vec) {
        if (this != &vec) {
            val[0] = vec.val[0];
            val[1] = vec.val[1];
            val[2] = vec.val[2];
            val[3] = vec.val[3];
            val[4] = vec.val[4];
            val[5] = vec.val[5];
            val[6] = vec.val[6];
            val[7] = vec.val[7];
            val[8] = vec.val[8];
        }
        return *this;
    }

    // move constructor
    RotationMatDS(RotationMatDS&& other) noexcept {
        val[0] = other.val[0];
        val[1] = other.val[1];
        val[2] = other.val[2];
        val[3] = other.val[3];
        val[4] = other.val[4];
        val[5] = other.val[5];
        val[6] = other.val[6];
        val[7] = other.val[7];
        val[8] = other.val[8];
    }

    // move assignment
    RotationMatDS& operator=(RotationMatDS&& other) noexcept {
        if (this != &other) {
            val[0] = other.val[0];
            val[1] = other.val[1];
            val[2] = other.val[2];
            val[3] = other.val[3];
            val[4] = other.val[4];
            val[5] = other.val[5];
            val[6] = other.val[6];
            val[7] = other.val[7];
            val[8] = other.val[8];
        }
        return *this;
    }

    // destructor
    ~RotationMatDS() = default;

    double operator()(uint8_t i, uint8_t j) const {
        int n = (i * 3) + j;
        assert(n < 9);
        return val[n];
    }

    double& operator()(uint8_t i, uint8_t j) {
        int n = (i * 3) + j;
        assert(n < 9);
        return val[n];
    }

    void setVal(const std::vector<double>& vec) {
        assert(vec.size() == 9);
        val[0] = vec[0];
        val[1] = vec[1];
        val[2] = vec[2];
        val[3] = vec[3];
        val[4] = vec[4];
        val[5] = vec[5];
        val[6] = vec[6];
        val[7] = vec[7];
        val[8] = vec[8];
    }

    void setX(const Vector3DS& x) {
        val[0] = x.val[0];
        val[3] = x.val[1];
        val[6] = x.val[2];
    }

    void setY(const Vector3DS& y) {
        val[1] = y.val[0];
        val[4] = y.val[1];
        val[7] = y.val[2];
    }

    void setZ(const Vector3DS& z) {
        val[2] = z.val[0];
        val[5] = z.val[1];
        val[8] = z.val[2];
    }

    Vector3DS getX() const {
        return {val[0], val[3], val[6]};
    }

    Vector3DS getY() const {
        return {val[1], val[4], val[7]};
    }

    Vector3DS getZ() const {
        return {val[2], val[5], val[8]};
    }

    double L1() const {
        double l1 = fabs(val[0]) + fabs(val[1]) + fabs(val[2]) +
                    fabs(val[3]) + fabs(val[4]) + fabs(val[5]) +
                    fabs(val[6]) + fabs(val[7]) + fabs(val[8]);

        return l1;
    }

    double L2() const {
        double l2 =
            (val[0] * val[0]) + (val[1] * val[1]) + (val[2] * val[2]) +
            (val[3] * val[3]) + (val[4] * val[4]) + (val[5] * val[5]) +
            (val[6] * val[6]) + (val[7] * val[7]) + (val[8] * val[8]);

        return std::sqrt(l2);
    }

    double LInf() const {
        std::vector<double> tmp;

        tmp.emplace_back(fabs(val[0]));
        tmp.emplace_back(fabs(val[1]));
        tmp.emplace_back(fabs(val[2]));
        tmp.emplace_back(fabs(val[3]));
        tmp.emplace_back(fabs(val[4]));
        tmp.emplace_back(fabs(val[5]));
        tmp.emplace_back(fabs(val[6]));
        tmp.emplace_back(fabs(val[7]));
        tmp.emplace_back(fabs(val[8]));
        return *std::max_element(tmp.begin(), tmp.end());
    }

    double getRotAngle(Vector3DS& axis, double eps) const;

    Vector3DS getRot() const {
        Vector3DS axis;
        double angle = getRotAngle(axis, 10e-6);
        return axis * angle;
    }

    friend bool operator==(const RotationMatDS& A, const RotationMatDS& B) {
        return fabs(A.val[0] - B.val[0]) < MSNH_F64_EPS &&
               fabs(A.val[1] - B.val[1]) < MSNH_F64_EPS &&
               fabs(A.val[2] - B.val[2]) < MSNH_F64_EPS &&
               fabs(A.val[3] - B.val[3]) < MSNH_F64_EPS &&
               fabs(A.val[4] - B.val[4]) < MSNH_F64_EPS &&
               fabs(A.val[5] - B.val[5]) < MSNH_F64_EPS &&
               fabs(A.val[6] - B.val[6]) < MSNH_F64_EPS &&
               fabs(A.val[7] - B.val[7]) < MSNH_F64_EPS &&
               fabs(A.val[8] - B.val[8]) < MSNH_F64_EPS;
    }

    friend bool operator!=(const RotationMatDS& A, const RotationMatDS& B) {
        return !(A == B);
    }

    bool isFuzzyNull() const {
        return fabs(val[0]) < MSNH_F64_EPS && fabs(val[1]) < MSNH_F64_EPS &&
               fabs(val[2]) < MSNH_F64_EPS && fabs(val[3]) < MSNH_F64_EPS &&
               fabs(val[4]) < MSNH_F64_EPS && fabs(val[5]) < MSNH_F64_EPS &&
               fabs(val[6]) < MSNH_F64_EPS && fabs(val[7]) < MSNH_F64_EPS &&
               fabs(val[8]) < MSNH_F64_EPS;
    }

    bool closeToEps(double eps) {
        return fabs(val[0] - eps) < MSNH_F64_EPS &&
               fabs(val[1] - eps) < MSNH_F64_EPS &&
               fabs(val[2] - eps) < MSNH_F64_EPS &&
               fabs(val[3] - eps) < MSNH_F64_EPS &&
               fabs(val[4] - eps) < MSNH_F64_EPS &&
               fabs(val[5] - eps) < MSNH_F64_EPS &&
               fabs(val[6] - eps) < MSNH_F64_EPS &&
               fabs(val[7] - eps) < MSNH_F64_EPS &&
               fabs(val[8] - eps) < MSNH_F64_EPS;
    }

    void print() const;

    std::string toString() const;

    std::string toHtmlString() const;

    friend RotationMatDS operator*(
        const RotationMatDS& A, const RotationMatDS& B) {
        RotationMatDS tmp;
        tmp.val[0] = (A.val[0] * B.val[0]) + (A.val[1] * B.val[3]) +
                     (A.val[2] * B.val[6]);
        tmp.val[1] = (A.val[0] * B.val[1]) + (A.val[1] * B.val[4]) +
                     (A.val[2] * B.val[7]);
        tmp.val[2] =
            A.val[0] * B.val[2] + A.val[1] * B.val[5] + A.val[2] * B.val[8];
        tmp.val[3] =
            A.val[3] * B.val[0] + A.val[4] * B.val[3] + A.val[5] * B.val[6];
        tmp.val[4] =
            A.val[3] * B.val[1] + A.val[4] * B.val[4] + A.val[5] * B.val[7];
        tmp.val[5] =
            A.val[3] * B.val[2] + A.val[4] * B.val[5] + A.val[5] * B.val[8];
        tmp.val[6] =
            A.val[6] * B.val[0] + A.val[7] * B.val[3] + A.val[8] * B.val[6];
        tmp.val[7] =
            A.val[6] * B.val[1] + A.val[7] * B.val[4] + A.val[8] * B.val[7];
        tmp.val[8] =
            A.val[6] * B.val[2] + A.val[7] * B.val[5] + A.val[8] * B.val[8];
        return tmp;
    }

    friend RotationMatDS operator*(const RotationMatDS& A, double b) {
        RotationMatDS tmp;
        tmp.val[0] = A.val[0] * b;
        tmp.val[1] = A.val[1] * b;
        tmp.val[2] = A.val[2] * b;
        tmp.val[3] = A.val[3] * b;
        tmp.val[4] = A.val[4] * b;
        tmp.val[5] = A.val[5] * b;
        tmp.val[6] = A.val[6] * b;
        tmp.val[7] = A.val[7] * b;
        tmp.val[8] = A.val[8] * b;
        return tmp;
    }

    friend RotationMatDS operator*(
        const double& a, const RotationMatDS& B) {
        RotationMatDS tmp;
        tmp.val[0] = B.val[0] * a;
        tmp.val[1] = B.val[1] * a;
        tmp.val[2] = B.val[2] * a;
        tmp.val[3] = B.val[3] * a;
        tmp.val[4] = B.val[4] * a;
        tmp.val[5] = B.val[5] * a;
        tmp.val[6] = B.val[6] * a;
        tmp.val[7] = B.val[7] * a;
        tmp.val[8] = B.val[8] * a;
        return tmp;
    }

    friend TranslationDS operator*(
        const RotationMatDS& A, const TranslationDS& b) {
        TranslationDS tmp;

        tmp[0] = (A(0, 0) * b[0]) + (A(0, 1) * b[1]) + (A(0, 2) * b[2]);
        tmp[1] = (A(1, 0) * b[0]) + (A(1, 1) * b[1]) + (A(1, 2) * b[2]);
        tmp[2] = (A(2, 0) * b[0]) + (A(2, 1) * b[1]) + (A(2, 2) * b[2]);

        return tmp;
    }

    friend RotationMatDS operator+(
        const RotationMatDS& A, const RotationMatDS& B) {
        RotationMatDS tmp;
        tmp.val[0] = A.val[0] + B.val[0];
        tmp.val[1] = A.val[1] + B.val[1];
        tmp.val[2] = A.val[2] + B.val[2];
        tmp.val[3] = A.val[3] + B.val[3];
        tmp.val[4] = A.val[4] + B.val[4];
        tmp.val[5] = A.val[5] + B.val[5];
        tmp.val[6] = A.val[6] + B.val[6];
        tmp.val[7] = A.val[7] + B.val[7];
        tmp.val[8] = A.val[8] + B.val[8];
        return tmp;
    }

    friend RotationMatDS operator-(
        const RotationMatDS& A, const RotationMatDS& B) {
        RotationMatDS tmp;
        tmp.val[0] = A.val[0] - B.val[0];
        tmp.val[1] = A.val[1] - B.val[1];
        tmp.val[2] = A.val[2] - B.val[2];
        tmp.val[3] = A.val[3] - B.val[3];
        tmp.val[4] = A.val[4] - B.val[4];
        tmp.val[5] = A.val[5] - B.val[5];
        tmp.val[6] = A.val[6] - B.val[6];
        tmp.val[7] = A.val[7] - B.val[7];
        tmp.val[8] = A.val[8] - B.val[8];
        return tmp;
    }

    RotationMatDS inverse() const {
        RotationMatDS tmpRot = *this;
        double tmp = 0;
        tmp = tmpRot.val[1];
        tmpRot.val[1] = tmpRot.val[3];
        tmpRot.val[3] = tmp;
        tmp = tmpRot.val[2];
        tmpRot.val[2] = tmpRot.val[6];
        tmpRot.val[6] = tmp;
        tmp = tmpRot.val[5];
        tmpRot.val[5] = tmpRot.val[7];
        tmpRot.val[7] = tmp;
        return tmpRot;
    }

    RotationMatDS transpose() const {
        RotationMatDS tmpRot = *this;
        double tmp = 0;
        tmp = tmpRot.val[1];
        tmpRot.val[1] = tmpRot.val[3];
        tmpRot.val[3] = tmp;
        tmp = tmpRot.val[2];
        tmpRot.val[2] = tmpRot.val[6];
        tmpRot.val[6] = tmp;
        tmp = tmpRot.val[5];
        tmpRot.val[5] = tmpRot.val[7];
        tmpRot.val[7] = tmp;
        return tmpRot;
    }

    bool isRealRotMat() const {
        return (transpose() * (*this) - RotationMatDS()).normal() <
               MSNH_F32_EPS;
    }

    double normal() const {
        double final =
            (val[0] * val[0]) + (val[1] * val[1]) + (val[2] * val[2]) +
            (val[3] * val[3]) + (val[4] * val[4]) + (val[5] * val[5]) +
            (val[6] * val[6]) + (val[7] * val[7]) + (val[8] * val[8]);

        return sqrt(final);
    }

    Vector3DS invMul(const Vector3DS& vec) const {
        return {(val[0] * vec.val[0]) + (val[3] * vec.val[1]) +
                    (val[6] * vec.val[2]),
            (val[1] * vec.val[0]) + (val[4] * vec.val[1]) +
                (val[7] * vec.val[2]),
            (val[2] * vec.val[0]) + (val[5] * vec.val[1]) +
                (val[8] * vec.val[2])};
    }
};

class MsnhNet_API RotationMatFS {
public:
    std::array<float, 9> val {1, 0, 0, 0, 1, 0, 0, 0, 1};

public:
    // Default constructor
    RotationMatFS() : val {1, 0, 0, 0, 1, 0, 0, 0, 1} { }

    // Constructor
    explicit RotationMatFS(const std::vector<float>& vec) {
        assert(vec.size() == 9);
        val[0] = vec[0];
        val[1] = vec[1];
        val[2] = vec[2];
        val[3] = vec[3];
        val[4] = vec[4];
        val[5] = vec[5];
        val[6] = vec[6];
        val[7] = vec[7];
        val[8] = vec[8];
    }

    RotationMatFS(float a11, float a12, float a13, float a21, float a22,
        float a23, float a31, float a32, float a33)
        : val {a11, a12, a13, a21, a22, a23, a31, a32, a33} { }

    RotationMatFS(
        const Vector3FS& x, const Vector3FS& y, const Vector3FS& z) {
        val[0] = x.val[0];
        val[1] = y.val[0];
        val[2] = z.val[0];
        val[3] = x.val[1];
        val[4] = y.val[1];
        val[5] = z.val[1];
        val[6] = x.val[2];
        val[7] = y.val[2];
        val[8] = z.val[2];
    }

    // Copy constructor
    RotationMatFS(const RotationMatFS& other) {
        val[0] = other.val[0];
        val[1] = other.val[1];
        val[2] = other.val[2];
        val[3] = other.val[3];
        val[4] = other.val[4];
        val[5] = other.val[5];
        val[6] = other.val[6];
        val[7] = other.val[7];
        val[8] = other.val[8];
    }

    // Copy assignment
    RotationMatFS& operator=(const RotationMatFS& other) {
        if (this != &other) {
            val[0] = other.val[0];
            val[1] = other.val[1];
            val[2] = other.val[2];
            val[3] = other.val[3];
            val[4] = other.val[4];
            val[5] = other.val[5];
            val[6] = other.val[6];
            val[7] = other.val[7];
            val[8] = other.val[8];
        }
        return *this;
    }

    // Move constructor
    RotationMatFS(RotationMatFS&& other) {
        val[0] = other.val[0];
        val[1] = other.val[1];
        val[2] = other.val[2];
        val[3] = other.val[3];
        val[4] = other.val[4];
        val[5] = other.val[5];
        val[6] = other.val[6];
        val[7] = other.val[7];
        val[8] = other.val[8];
    }

    // Move assignment
    RotationMatFS& operator=(RotationMatFS&& other) {
        if (this != &other) {
            val[0] = other.val[0];
            val[1] = other.val[1];
            val[2] = other.val[2];
            val[3] = other.val[3];
            val[4] = other.val[4];
            val[5] = other.val[5];
            val[6] = other.val[6];
            val[7] = other.val[7];
            val[8] = other.val[8];
        }
        return *this;
    }

    // Destructor
    ~RotationMatFS() = default;

    float operator()(const uint8_t i, const uint8_t j) const {
        int n = i * 3 + j;
        assert(n < 9);
        return val[n];
    }

    float& operator()(const uint8_t i, const uint8_t j) {
        int n = i * 3 + j;
        assert(n < 9);
        return val[n];
    }

    void setVal(const std::vector<float>& vec) {
        assert(vec.size() == 9);
        val[0] = vec[0];
        val[1] = vec[1];
        val[2] = vec[2];
        val[3] = vec[3];
        val[4] = vec[4];
        val[5] = vec[5];
        val[6] = vec[6];
        val[7] = vec[7];
        val[8] = vec[8];
    }

    void setX(const Vector3FS& x) {
        val[0] = x.val[0];
        val[3] = x.val[1];
        val[6] = x.val[2];
    }

    void setY(const Vector3FS& y) {
        val[1] = y.val[0];
        val[4] = y.val[1];
        val[7] = y.val[2];
    }

    void setZ(const Vector3FS& z) {
        val[2] = z.val[0];
        val[5] = z.val[1];
        val[8] = z.val[2];
    }

    Vector3FS getX() const {
        return {val[0], val[3], val[6]};
    }

    Vector3FS getY() const {
        return {val[1], val[4], val[7]};
    }

    Vector3FS getZ() const {
        return {val[2], val[5], val[8]};
    }

    float L1() const {
        float l1 = fabsf(val[0]) + fabsf(val[1]) + fabsf(val[2]) +
                   fabsf(val[3]) + fabsf(val[4]) + fabsf(val[5]) +
                   fabsf(val[6]) + fabsf(val[7]) + fabsf(val[8]);

        return l1;
    }

    float L2() const {
        float l2 =
            (val[0] * val[0]) + (val[1] * val[1]) + (val[2] * val[2]) +
            (val[3] * val[3]) + (val[4] * val[4]) + (val[5] * val[5]) +
            (val[6] * val[6]) + (val[7] * val[7]) + (val[8] * val[8]);

        return sqrtf(l2);
    }

    float LInf() const {
        std::vector<float> tmp;

        tmp.emplace_back(fabsf(val[0]));
        tmp.emplace_back(fabsf(val[1]));
        tmp.emplace_back(fabsf(val[2]));
        tmp.emplace_back(fabsf(val[3]));
        tmp.emplace_back(fabsf(val[4]));
        tmp.emplace_back(fabsf(val[5]));
        tmp.emplace_back(fabsf(val[6]));
        tmp.emplace_back(fabsf(val[7]));
        tmp.emplace_back(fabsf(val[8]));

        return *std::max_element(tmp.begin(), tmp.end());
    }

    friend bool operator==(const RotationMatFS& A, const RotationMatFS& B) {
        return fabsf(A.val[0] - B.val[0]) < MSNH_F32_EPS &&
               fabsf(A.val[1] - B.val[1]) < MSNH_F32_EPS &&
               fabsf(A.val[2] - B.val[2]) < MSNH_F32_EPS &&
               fabsf(A.val[3] - B.val[3]) < MSNH_F32_EPS &&
               fabsf(A.val[4] - B.val[4]) < MSNH_F32_EPS &&
               fabsf(A.val[5] - B.val[5]) < MSNH_F32_EPS &&
               fabsf(A.val[6] - B.val[6]) < MSNH_F32_EPS &&
               fabsf(A.val[7] - B.val[7]) < MSNH_F32_EPS &&
               fabsf(A.val[8] - B.val[8]) < MSNH_F32_EPS;
    }

    friend bool operator!=(const RotationMatFS& A, const RotationMatFS& B) {
        return !(A == B);
    }

    bool isFuzzyNull() const {
        return fabsf(val[0]) < MSNH_F32_EPS &&
               fabsf(val[1]) < MSNH_F32_EPS &&
               fabsf(val[2]) < MSNH_F32_EPS &&
               fabsf(val[3]) < MSNH_F32_EPS &&
               fabsf(val[4]) < MSNH_F32_EPS &&
               fabsf(val[5]) < MSNH_F32_EPS &&
               fabsf(val[6]) < MSNH_F32_EPS &&
               fabsf(val[7]) < MSNH_F32_EPS && fabsf(val[8]) < MSNH_F32_EPS;
    }

    bool closeToEps(const float eps) const {
        return fabsf(val[0] - eps) < MSNH_F32_EPS &&
               fabsf(val[1] - eps) < MSNH_F32_EPS &&
               fabsf(val[2] - eps) < MSNH_F32_EPS &&
               fabsf(val[3] - eps) < MSNH_F32_EPS &&
               fabsf(val[4] - eps) < MSNH_F32_EPS &&
               fabsf(val[5] - eps) < MSNH_F32_EPS &&
               fabsf(val[6] - eps) < MSNH_F32_EPS &&
               fabsf(val[7] - eps) < MSNH_F32_EPS &&
               fabsf(val[8] - eps) < MSNH_F32_EPS;
    }

    void print() const;

    std::string toString() const;

    std::string toHtmlString() const;

    friend RotationMatFS operator*(
        const RotationMatFS& A, const RotationMatFS& B) {
        RotationMatFS tmp;
        tmp.val[0] =
            A.val[0] * B.val[0] + A.val[1] * B.val[3] + A.val[2] * B.val[6];
        tmp.val[1] =
            A.val[0] * B.val[1] + A.val[1] * B.val[4] + A.val[2] * B.val[7];
        tmp.val[2] =
            A.val[0] * B.val[2] + A.val[1] * B.val[5] + A.val[2] * B.val[8];
        tmp.val[3] =
            A.val[3] * B.val[0] + A.val[4] * B.val[3] + A.val[5] * B.val[6];
        tmp.val[4] =
            A.val[3] * B.val[1] + A.val[4] * B.val[4] + A.val[5] * B.val[7];
        tmp.val[5] =
            A.val[3] * B.val[2] + A.val[4] * B.val[5] + A.val[5] * B.val[8];
        tmp.val[6] =
            A.val[6] * B.val[0] + A.val[7] * B.val[3] + A.val[8] * B.val[6];
        tmp.val[7] =
            A.val[6] * B.val[1] + A.val[7] * B.val[4] + A.val[8] * B.val[7];
        tmp.val[8] =
            A.val[6] * B.val[2] + A.val[7] * B.val[5] + A.val[8] * B.val[8];
        return tmp;
    }

    friend RotationMatFS operator*(const RotationMatFS& A, const float& b) {
        RotationMatFS tmp;
        tmp.val[0] = A.val[0] * b;
        tmp.val[1] = A.val[1] * b;
        tmp.val[2] = A.val[2] * b;
        tmp.val[3] = A.val[3] * b;
        tmp.val[4] = A.val[4] * b;
        tmp.val[5] = A.val[5] * b;
        tmp.val[6] = A.val[6] * b;
        tmp.val[7] = A.val[7] * b;
        tmp.val[8] = A.val[8] * b;
        return tmp;
    }

    friend RotationMatFS operator*(const float a, const RotationMatFS& B) {
        RotationMatFS tmp;
        tmp.val[0] = B.val[0] * a;
        tmp.val[1] = B.val[1] * a;
        tmp.val[2] = B.val[2] * a;
        tmp.val[3] = B.val[3] * a;
        tmp.val[4] = B.val[4] * a;
        tmp.val[5] = B.val[5] * a;
        tmp.val[6] = B.val[6] * a;
        tmp.val[7] = B.val[7] * a;
        tmp.val[8] = B.val[8] * a;
        return tmp;
    }

    friend TranslationFS operator*(
        const RotationMatFS& A, const TranslationFS& b) {
        TranslationFS tmp;

        tmp[0] = A(0, 0) * b[0] + A(0, 1) * b[1] + A(0, 2) * b[2];
        tmp[1] = A(1, 0) * b[0] + A(1, 1) * b[1] + A(1, 2) * b[2];
        tmp[2] = A(2, 0) * b[0] + A(2, 1) * b[1] + A(2, 2) * b[2];

        return tmp;
    }

    friend RotationMatFS operator+(
        const RotationMatFS& A, const RotationMatFS& B) {
        RotationMatFS tmp;
        tmp.val[0] = A.val[0] + B.val[0];
        tmp.val[1] = A.val[1] + B.val[1];
        tmp.val[2] = A.val[2] + B.val[2];
        tmp.val[3] = A.val[3] + B.val[3];
        tmp.val[4] = A.val[4] + B.val[4];
        tmp.val[5] = A.val[5] + B.val[5];
        tmp.val[6] = A.val[6] + B.val[6];
        tmp.val[7] = A.val[7] + B.val[7];
        tmp.val[8] = A.val[8] + B.val[8];
        return tmp;
    }

    friend RotationMatFS operator-(
        const RotationMatFS& A, const RotationMatFS& B) {
        RotationMatFS tmp;
        tmp.val[0] = A.val[0] - B.val[0];
        tmp.val[1] = A.val[1] - B.val[1];
        tmp.val[2] = A.val[2] - B.val[2];
        tmp.val[3] = A.val[3] - B.val[3];
        tmp.val[4] = A.val[4] - B.val[4];
        tmp.val[5] = A.val[5] - B.val[5];
        tmp.val[6] = A.val[6] - B.val[6];
        tmp.val[7] = A.val[7] - B.val[7];
        tmp.val[8] = A.val[8] - B.val[8];
        return tmp;
    }

    RotationMatFS inverse() const {
        RotationMatFS tmpRot = *this;
        float tmp = 0;
        tmp = tmpRot.val[1];
        tmpRot.val[1] = tmpRot.val[3];
        tmpRot.val[3] = tmp;
        tmp = tmpRot.val[2];
        tmpRot.val[2] = tmpRot.val[6];
        tmpRot.val[6] = tmp;
        tmp = tmpRot.val[5];
        tmpRot.val[5] = tmpRot.val[7];
        tmpRot.val[7] = tmp;
        return tmpRot;
    }

    RotationMatFS transpose() const {
        RotationMatFS tmpRot = *this;
        float tmp = 0;
        tmp = tmpRot.val[1];
        tmpRot.val[1] = tmpRot.val[3];
        tmpRot.val[3] = tmp;
        tmp = tmpRot.val[2];
        tmpRot.val[2] = tmpRot.val[6];
        tmpRot.val[6] = tmp;
        tmp = tmpRot.val[5];
        tmpRot.val[5] = tmpRot.val[7];
        tmpRot.val[7] = tmp;
        return tmpRot;
    }

    bool isRealRotMat() const {
        return (transpose() * (*this) - RotationMatFS()).normal() <
               MSNH_F32_EPS;
    }

    float normal() const {
        float final =
            (val[0] * val[0]) + (val[1] * val[1]) + (val[2] * val[2]) +
            (val[3] * val[3]) + (val[4] * val[4]) + (val[5] * val[5]) +
            (val[6] * val[6]) + (val[7] * val[7]) + (val[8] * val[8]);

        return sqrtf(final);
    }

    Vector3FS invMul(const Vector3FS& vec) const {
        return {(val[0] * vec.val[0]) + (val[3] * vec.val[1]) +
                    (val[6] * vec.val[2]),
            (val[1] * vec.val[0]) + (val[4] * vec.val[1]) +
                (val[7] * vec.val[2]),
            (val[2] * vec.val[0]) + (val[5] * vec.val[1]) +
                (val[8] * vec.val[2])};
    }
};

}  // namespace msnh
#endif  // MSNHROTATIONMATS_H
