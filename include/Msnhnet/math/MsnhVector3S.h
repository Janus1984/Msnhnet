#ifndef VECTOR3S_H
#define VECTOR3S_H

#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "Msnhnet/config/MsnhnetCfg.h"

namespace Msnhnet {

class MsnhNet_API Vector3DS {
public:
    std::array<double, 3> val {0, 0, 0};

public:
    // default constructor
    Vector3DS() : val {0, 0, 0} { }

    // constructor
    Vector3DS(double x, double y, double z) : val {x, y, z} { }

    explicit Vector3DS(const std::array<double, 3>& vec) : val(vec) { }

    explicit Vector3DS(const std::vector<double>& vec) {
        assert(vec.size() == 3);

        val[0] = vec[0];
        val[1] = vec[1];
        val[2] = vec[2];
    }

    // copy constructor
    Vector3DS(const Vector3DS& vec) = default;

    Vector3DS& operator=(const Vector3DS& vec) {
        if (this == &vec) {
            return *this;
        }
        val[0] = vec.val[0];
        val[1] = vec.val[1];
        val[2] = vec.val[2];
        return *this;
    }

    // move constructor
    Vector3DS(Vector3DS&& other) noexcept {
        if (this == &other) {
            return;
        }
        val[0] = other.val[0];
        val[1] = other.val[1];
        val[2] = other.val[2];
    }

    // move assignment
    Vector3DS& operator=(Vector3DS&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        val[0] = other.val[0];
        val[1] = other.val[1];
        val[2] = other.val[2];
        return *this;
    }

    // destructor
    ~Vector3DS() = default;

    void setval(double x, double y, double z) {
        val[0] = x;
        val[1] = y;
        val[2] = z;
    }

    double operator[](size_t index) const {
        // assert(index < 3);
        return val[index];
    }

    double& operator[](size_t index) {
        // assert(index < 3);
        return val[index];
    }

    void print() const;

    std::string toString() const;

    std::string toHtmlString() const;

    // operator==
    bool operator==(const Vector3DS& other) const {
        return std::abs(other.val[0] - val[0]) < MSNH_F64_EPS &&
               std::abs(other.val[1] - val[1]) < MSNH_F64_EPS &&
               std::abs(other.val[2] - val[2]) < MSNH_F64_EPS;
    }

    // operator!=
    bool operator!=(const Vector3DS& other) const {
        return !(*this == other);
    }

    bool isFuzzyNull() const {
        for (double i : val) {
            if (std::abs(i) > MSNH_F64_EPS) {
                return false;
            }
        }
        return true;
    }

    bool isNan() const {
        for (double i : val) {
            if (std::isnan(i)) {
                return true;
            }
        }
        return false;
    }

    bool closeToEps(double eps) const {
        for (double i : val) {
            if (std::abs(i - eps) > MSNH_F64_EPS) {
                return false;
            }
        }
        return true;
    }

    // operator+
    Vector3DS operator+(const Vector3DS& other) const {
        Vector3DS tmp;
        tmp.val[0] = val[0] + other.val[0];
        tmp.val[1] = val[1] + other.val[1];
        tmp.val[2] = val[2] + other.val[2];
        return tmp;
    }

    Vector3DS operator+(double a) const {
        Vector3DS tmp;
        tmp.val[0] = val[0] + a;
        tmp.val[1] = val[1] + a;
        tmp.val[2] = val[2] + a;
        return tmp;
    }

    friend Vector3DS operator+(double a, const Vector3DS& B) {
        return B + a;
    }

    // operator+=
    Vector3DS& operator+=(Vector3DS& other) {
        val[0] = val[0] + other.val[0];
        val[1] = val[1] + other.val[1];
        val[2] = val[2] + other.val[2];
        return *this;
    }

    Vector3DS& operator+=(double a) {
        val[0] = val[0] + a;
        val[1] = val[1] + a;
        val[2] = val[2] + a;
        return *this;
    }

    // operator-
    Vector3DS operator-(const Vector3DS& other) const {
        Vector3DS tmp;
        tmp.val[0] = val[0] - other.val[0];
        tmp.val[1] = val[1] - other.val[1];
        tmp.val[2] = val[2] - other.val[2];
        return tmp;
    }

    Vector3DS operator-(double b) const {
        Vector3DS tmp;
        tmp.val[0] = val[0] - b;
        tmp.val[1] = val[1] - b;
        tmp.val[2] = val[2] - b;
        return tmp;
    }

    friend Vector3DS operator-(double a, const Vector3DS& B) {
        return B - a;
    }

    // operator-=
    Vector3DS& operator-=(const Vector3DS& A) {
        val[0] = val[0] - A.val[0];
        val[1] = val[1] - A.val[1];
        val[2] = val[2] - A.val[2];
        return *this;
    }

    Vector3DS& operator-=(double a) {
        val[0] = val[0] - a;
        val[1] = val[1] - a;
        val[2] = val[2] - a;
        return *this;
    }

    // operator*
    friend Vector3DS operator*(const Vector3DS& A, const Vector3DS& B) {
        Vector3DS tmp;
        tmp.val[0] = A.val[0] * B.val[0];
        tmp.val[1] = A.val[1] * B.val[1];
        tmp.val[2] = A.val[2] * B.val[2];
        return tmp;
    }

    // operator*
    Vector3DS operator*(double b) const {
        Vector3DS tmp;
        tmp.val[0] = val[0] * b;
        tmp.val[1] = val[1] * b;
        tmp.val[2] = val[2] * b;
        return tmp;
    }

    friend Vector3DS operator*(double a, const Vector3DS& B) {
        return B * a;
    }

    // operator*=
    Vector3DS& operator*=(const Vector3DS& other) {
        val[0] = val[0] * other.val[0];
        val[1] = val[1] * other.val[1];
        val[2] = val[2] * other.val[2];
        return *this;
    }

    Vector3DS& operator*=(double a) {
        val[0] = val[0] * a;
        val[1] = val[1] * a;
        val[2] = val[2] * a;
        return *this;
    }

    // operator/
    Vector3DS operator/(double b) {
        Vector3DS tmp;
        tmp.val[0] = val[0] / b;
        tmp.val[1] = val[1] / b;
        tmp.val[2] = val[2] / b;
        return tmp;
    }

    Vector3DS operator/(const Vector3DS& other) {
        Vector3DS tmp;
        tmp.val[0] = val[0] / other.val[0];
        tmp.val[1] = val[1] / other.val[1];
        tmp.val[2] = val[2] / other.val[2];
        return tmp;
    }

    // operator/=
    Vector3DS& operator/=(const Vector3DS& other) {
        val[0] = val[0] / other.val[0];
        val[1] = val[1] / other.val[1];
        val[2] = val[2] / other.val[2];
        return *this;
    }

    Vector3DS& operator/=(double a) {
        val[0] = val[0] / a;
        val[1] = val[1] / a;
        val[2] = val[2] / a;
        return *this;
    }

    // cross product
    Vector3DS crossProduct(const Vector3DS& other) {
        Vector3DS tmp;
        tmp.val[0] = val[1] * other.val[2] - val[2] * other.val[1];
        tmp.val[1] = val[2] * other.val[0] - val[0] * other.val[2];
        tmp.val[2] = val[0] * other.val[1] - val[1] * other.val[0];
        return tmp;
    }

    friend Vector3DS crossProduct(const Vector3DS& A, const Vector3DS& B) {
        Vector3DS tmp;
        tmp.val[0] = A.val[1] * B.val[2] - A.val[2] * B.val[1];
        tmp.val[1] = A.val[2] * B.val[0] - A.val[0] * B.val[2];
        tmp.val[2] = A.val[0] * B.val[1] - A.val[1] * B.val[0];
        return tmp;
    }

    // dot product
    double dotProduct(const Vector3DS& other) {
        return (val[0] * other.val[0]) + (val[1] * other.val[1]) +
               (val[2] * other.val[2]);
    }

    friend double dotProduct(const Vector3DS& A, const Vector3DS& B) {
        return (A.val[0] * B.val[0]) + (A.val[1] * B.val[1]) +
               (A.val[2] * B.val[2]);
    }

    Vector3DS normalized() {
        Vector3DS vec;

        double len =
            (val[0] * val[0]) + (val[1] * val[1]) + (val[2] * val[2]);

        if (std::abs(len - 1.0) < MSNH_F64_EPS) {
            return *this;
        }

        if (std::abs(len) < MSNH_F64_EPS) {
            return vec;
        }

        len = sqrt(len);

        vec.val[0] = val[0] / len;
        vec.val[1] = val[1] / len;
        vec.val[2] = val[2] / len;
        return vec;
    }

    void normalize() {
        double len =
            (val[0] * val[0]) + (val[1] * val[1]) + (val[2] * val[2]);

        if (std::abs(len - 1.0) < MSNH_F64_EPS ||
            std::abs(len) < MSNH_F64_EPS) {
            return;
        }

        len = sqrt(len);

        val[0] = val[0] / len;
        val[1] = val[1] / len;
        val[2] = val[2] / len;
    }

    double length() const {
        return sqrt(
            (val[0] * val[0]) + (val[1] * val[1]) + (val[2] * val[2]));
    }

    double lengthSquared() const {
        return (val[0] * val[0]) + (val[1] * val[1]) + (val[2] * val[2]);
    }

    /* 点到点之间的距离
     * .eg ^
     *     |
     *   A x       --> -->    --->
     *     | \     OA - OB  = |BA|
     *     |   \
     *   O |-----x-->
     *           B
     */
    double distanceToPoint(const Vector3DS& point) const {
        return (*this - point).length();
    }

    /* 点到线之间的距离
     * .eg ^
     *   \ |
     *     x      x(A)
     *     | \
     *     |   x (point)
     *     |     \
     *   O |-------x--> B
     *               \(direction)
     *                 \LINE(point + direction)
     */
    double distanceToLine(
        const Vector3DS& point, const Vector3DS& direction) const {
        if (direction.isFuzzyNull()) {
            return (*this - point).length();
        }

        Vector3DS p =
            point + ((*this - point) * direction).dotProduct(direction);
        return (*this - p).length();
    }

    /*          点到线之间的距离
     *          .eg ^
     *          / \ |      *(normal)
     *         /    x    *
     *        /     | \
     *       /      |   \    x(A)
     *       \     *|     \
     *         \  O |-------x--> B
     *         * \ /       /
     *        *   /\      /
     *           /   \   / (plane)
     *          /      \/
     *
     */
    double distanceToPlane(
        const Vector3DS& plane, const Vector3DS& normal) const {
        return (*this - plane).dotProduct(normal);
    }

    Vector3DS normal(const Vector3DS& other) {
        return (*this).crossProduct(other).normalized();
    }

    static Vector3DS normal(
        const Vector3DS& v1, const Vector3DS& v2, const Vector3DS& v3) {
        return (v2 - v1).crossProduct((v3 - v1)).normalized();
    }
};

class Vector3FS {
public:
    std::array<float, 3> val {0, 0, 0};

public:
    // default constructor
    Vector3FS() : val {0, 0, 0} { }

    // constructor
    Vector3FS(float x, float y, float z) : val {x, y, z} { }

    explicit Vector3FS(const std::vector<float>& vec) {
        assert(vec.size() == 3);

        val[0] = vec[0];
        val[1] = vec[1];
        val[2] = vec[2];
    }

    explicit Vector3FS(const std::array<float, 3>& vec) : val(vec) { }

    Vector3FS(const Vector3FS& vec) = default;

    Vector3FS& operator=(const Vector3FS& vec) {
        if (this != &vec) {
            val[0] = vec.val[0];
            val[1] = vec.val[1];
            val[2] = vec.val[2];
        }
        return *this;
    }

    // move constructor
    Vector3FS(Vector3FS&& vec) noexcept : val(vec.val) { }

    // move assignment
    Vector3FS& operator=(Vector3FS&& vec) noexcept {
        if (this != &vec) {
            val = vec.val;
        }
        return *this;
    }

    // destructor
    ~Vector3FS() = default;

    void setval(float x, float y, float z) {
        val[0] = x;
        val[1] = y;
        val[2] = z;
    }

    float operator[](size_t index) const {
        assert(index < 3);
        return val.at(index);
    }

    float& operator[](size_t index) {
        assert(index < 3);
        return val.at(index);
    }

    void print() const;

    std::string toString() const;

    std::string toHtmlString() const;

    friend bool operator==(const Vector3FS& A, const Vector3FS& B) {
        return fabsf(A.val[0] - B.val[0]) < MSNH_F32_EPS &&
               fabsf(A.val[1] - B.val[1]) < MSNH_F32_EPS &&
               fabsf(A.val[2] - B.val[2]) < MSNH_F32_EPS;
    }

    friend bool operator!=(const Vector3FS& A, const Vector3FS& B) {
        return !operator==(A, B);
    }

    bool isFuzzyNull() const {
        for (int i = 0; i < 3; ++i) {
            if (fabsf(val.at(i)) > MSNH_F32_EPS) {
                return false;
            }
        }
        return true;
    }

    bool isNan() const {
        for (int i = 0; i < 3; ++i) {
            if (std::isnan(val.at(i))) {
                return true;
            }
        }
        return false;
    }

    bool closeToEps(float eps) {
        for (int i = 0; i < 3; ++i) {
            if (std::abs(val.at(i) - eps) > MSNH_F32_EPS) {
                return false;
            }
        }
        return true;
    }

    friend Vector3FS operator+(const Vector3FS& A, const Vector3FS& B) {
        Vector3FS tmp;
        tmp.val[0] = A.val[0] + B.val[0];
        tmp.val[1] = A.val[1] + B.val[1];
        tmp.val[2] = A.val[2] + B.val[2];
        return tmp;
    }

    friend Vector3FS operator+(const Vector3FS& A, const float& b) {
        Vector3FS tmp;
        tmp.val[0] = A.val[0] + b;
        tmp.val[1] = A.val[1] + b;
        tmp.val[2] = A.val[2] + b;
        return tmp;
    }

    friend Vector3FS operator+(const float& a, const Vector3FS& B) {
        Vector3FS tmp;
        tmp.val[0] = B.val[0] + a;
        tmp.val[1] = B.val[1] + a;
        tmp.val[2] = B.val[2] + a;
        return tmp;
    }

    Vector3FS& operator+=(const Vector3FS& A) {
        val[0] = val[0] + A.val[0];
        val[1] = val[1] + A.val[1];
        val[2] = val[2] + A.val[2];
        return *this;
    }

    Vector3FS& operator+=(float a) {
        val[0] = val[0] + a;
        val[1] = val[1] + a;
        val[2] = val[2] + a;
        return *this;
    }

    friend Vector3FS operator-(const Vector3FS& A, const Vector3FS& B) {
        Vector3FS tmp;
        tmp.val[0] = A.val[0] - B.val[0];
        tmp.val[1] = A.val[1] - B.val[1];
        tmp.val[2] = A.val[2] - B.val[2];
        return tmp;
    }

    friend Vector3FS operator-(const Vector3FS& A, float b) {
        Vector3FS tmp;
        tmp.val[0] = A.val[0] - b;
        tmp.val[1] = A.val[1] - b;
        tmp.val[2] = A.val[2] - b;
        return tmp;
    }

    friend Vector3FS operator-(float a, const Vector3FS& B) {
        Vector3FS tmp;
        tmp.val[0] = a - B.val[0];
        tmp.val[1] = a - B.val[1];
        tmp.val[2] = a - B.val[2];
        return tmp;
    }

    Vector3FS& operator-=(const Vector3FS& A) {
        val[0] = val[0] - A.val[0];
        val[1] = val[1] - A.val[1];
        val[2] = val[2] - A.val[2];
        return *this;
    }

    Vector3FS& operator-=(float a) {
        val[0] = val[0] - a;
        val[1] = val[1] - a;
        val[2] = val[2] - a;
        return *this;
    }

    friend Vector3FS operator*(const Vector3FS& A, const Vector3FS& B) {
        Vector3FS tmp;
        tmp.val[0] = A.val[0] * B.val[0];
        tmp.val[1] = A.val[1] * B.val[1];
        tmp.val[2] = A.val[2] * B.val[2];
        return tmp;
    }

    friend Vector3FS operator*(const Vector3FS& A, float b) {
        Vector3FS tmp;
        tmp.val[0] = A.val[0] * b;
        tmp.val[1] = A.val[1] * b;
        tmp.val[2] = A.val[2] * b;
        return tmp;
    }

    friend Vector3FS operator*(float a, const Vector3FS& B) {
        Vector3FS tmp;
        tmp.val[0] = a * B.val[0];
        tmp.val[1] = a * B.val[1];
        tmp.val[2] = a * B.val[2];
        return tmp;
    }

    Vector3FS& operator*=(const Vector3FS& A) {
        val[0] = val[0] * A.val[0];
        val[1] = val[1] * A.val[1];
        val[2] = val[2] * A.val[2];
        return *this;
    }

    Vector3FS& operator*=(float a) {
        val[0] = val[0] * a;
        val[1] = val[1] * a;
        val[2] = val[2] * a;
        return *this;
    }

    static Vector3FS crossProduct(const Vector3FS& A, const Vector3FS& B) {
        Vector3FS tmp;
        tmp.val[0] = A.val[1] * B.val[2] - A.val[2] * B.val[1];
        tmp.val[1] = A.val[2] * B.val[0] - A.val[0] * B.val[2];
        tmp.val[2] = A.val[0] * B.val[1] - A.val[1] * B.val[0];
        return tmp;
    }

    static float dotProduct(const Vector3FS& A, const Vector3FS& B) {
        return (A.val[0] * B.val[0]) + (A.val[1] * B.val[1]) +
               (A.val[2] * B.val[2]);
    }

    friend Vector3FS operator/(const Vector3FS& A, float b) {
        Vector3FS tmp;
        tmp.val[0] = A.val[0] / b;
        tmp.val[1] = A.val[1] / b;
        tmp.val[2] = A.val[2] / b;
        return tmp;
    }

    friend Vector3FS operator/(const Vector3FS& A, const Vector3FS& B) {
        Vector3FS tmp;
        tmp.val[0] = A.val[0] / B.val[0];
        tmp.val[1] = A.val[1] / B.val[1];
        tmp.val[2] = A.val[2] / B.val[2];
        return tmp;
    }

    Vector3FS& operator/=(const Vector3FS& A) {
        val[0] = val[0] / A.val[0];
        val[1] = val[1] / A.val[1];
        val[2] = val[2] / A.val[2];
        return *this;
    }

    Vector3FS& operator/=(float a) {
        val[0] = val[0] / a;
        val[1] = val[1] / a;
        val[2] = val[2] / a;
        return *this;
    }

    Vector3FS normalized() {
        Vector3FS vec;

        float len =
            (val[0] * val[0]) + (val[1] * val[1]) + (val[2] * val[2]);

        if (fabsf(len - 1.0F) < MSNH_F32_EPS) {
            return *this;
        }

        if (fabsf(len) < MSNH_F32_EPS) {
            return vec;
        }

        len = sqrtf(len);

        vec.val[0] = val[0] / len;
        vec.val[1] = val[1] / len;
        vec.val[2] = val[2] / len;
        return vec;
    }

    void normalize() {
        float len =
            (val[0] * val[0]) + (val[1] * val[1]) + (val[2] * val[2]);

        if (fabsf(len - 1.0F) < MSNH_F32_EPS || fabsf(len) < MSNH_F32_EPS) {
            return;
        }

        len = sqrtf(len);

        val[0] = val[0] / len;
        val[1] = val[1] / len;
        val[2] = val[2] / len;
    }

    float length() const {
        return sqrtf(
            (val[0] * val[0]) + (val[1] * val[1]) + (val[2] * val[2]));
    }

    float lengthSquared() const {
        return (val[0] * val[0]) + (val[1] * val[1]) + (val[2] * val[2]);
    }

    /* 点到点之间的距离
     * .eg ^
     *     |
     *   A x       --> -->    --->
     *     | \     OA - OB  = |BA|
     *     |   \
     *   O |-----x-->
     *           B
     */
    float distanceToPoint(const Vector3FS& point) const {
        return (*this - point).length();
    }

    /* 点到线之间的距离
     * .eg ^
     *   \ |
     *     x      x(A)
     *     | \
     *     |   x (point)
     *     |     \
     *   O |-------x--> B
     *               \(direction)
     *                 \LINE(point + direction)
     */
    float distanceToLine(
        const Vector3FS& point, const Vector3FS& direction) const {
        if (direction.isFuzzyNull()) {
            return (*this - point).length();
        }

        Vector3FS p = point + Vector3FS::dotProduct(
                                  (*this - point) * direction, direction);
        return (*this - p).length();
    }

    /*          点到线之间的距离
     *          .eg ^
     *          / \ |      *(normal)
     *         /    x    *
     *        /     | \
     *       /      |   \    x(A)
     *       \     *|     \
     *         \  O |-------x--> B
     *         * \ /       /
     *        *   /\      /
     *           /   \   / (plane)
     *          /      \/
     *
     */
    float distanceToPlane(
        const Vector3FS& plane, const Vector3FS& normal) const {
        return dotProduct((*this - plane), normal);
    }

    static Vector3FS normal(const Vector3FS& v1, const Vector3FS& v2) {
        return crossProduct(v1, v2).normalized();
    }

    static Vector3FS normal(
        const Vector3FS& v1, const Vector3FS& v2, const Vector3FS& v3) {
        return crossProduct((v2 - v1), (v3 - v1)).normalized();
    }
};

using EulerDS = Vector3DS;
using TranslationDS = Vector3DS;
using RotationVecDS = Vector3DS;
using LinearVelDS = Vector3DS;
using AngularVelDS = Vector3DS;

using EulerFS = Vector3FS;
using TranslationFS = Vector3FS;
using RotationVecFS = Vector3FS;
using LinearVelFS = Vector3FS;
using AngularVelFS = Vector3FS;

}  // namespace Msnhnet

#endif  // MSNHVECTOR3S_H
