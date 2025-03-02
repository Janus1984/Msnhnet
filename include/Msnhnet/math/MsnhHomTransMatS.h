#ifndef MSNHHOMTRANSMATS_H
#define MSNHHOMTRANSMATS_H

#include "Msnhnet/math/MsnhGeometryS.h"
#include "Msnhnet/math/MsnhMatrixS.h"

namespace Msnhnet {

class MsnhNet_API HomTransMatDS {
public:
    // Default constructor
    HomTransMatDS() = default;

    explicit HomTransMatDS(const RotationMatDS& rotMat) : rotMat(rotMat) { }

    explicit HomTransMatDS(const Vector3DS& trans) : trans(trans) { }

    HomTransMatDS(const RotationMatDS& rotMat, const Vector3DS& trans)
        : rotMat(rotMat), trans(trans) { }

    // Copy constructor
    HomTransMatDS(const HomTransMatDS& mat) = default;
    HomTransMatDS& operator=(const HomTransMatDS& mat) = default;

    void translate(const TranslationDS& vector) {
        trans[0] += vector[0];
        trans[1] += vector[1];
        trans[2] += vector[2];
    }

    void translate(const double x, const double y, const double z) {
        trans[0] += x;
        trans[1] += y;
        trans[2] += z;
    }

    void rotate(const double angleInRad, const double x, const double y,
        const double z) {
        rotate(angleInRad, Vector3DS(x, y, z));
    }

    void rotate(const double angleInRad, const Vector3DS& vector) {
        Vector3DS vec = vector;
        vec.normalize();
        const double x = vec[0];
        const double y = vec[1];
        const double z = vec[2];

        this->rotMat = GeometryS::euler2RotMat(
            EulerDS(x * angleInRad, y * angleInRad, z * angleInRad),
            RotSequence::ROT_ZYX);
    }

    void rotate(const EulerDS& euler) {
        rotMat = GeometryS::euler2RotMat(euler, RotSequence::ROT_ZYX);
    }

    void rotate(const QuaternionDS& quat) {
        rotMat = GeometryS::quaternion2RotMat(quat);
    }

    HomTransMatDS invert() const {
        HomTransMatDS tmp;

        tmp.rotMat = rotMat.inverse();
        tmp.trans = rotMat.invMul(trans * -1);

        return tmp;
    }

    friend HomTransMatDS operator*(
        const HomTransMatDS& A, const HomTransMatDS& B) {
        return {A.rotMat * B.rotMat, A.rotMat * B.trans + A.trans};
    }

    void print();

    std::string toString() const;

    std::string toHtmlString() const;

    bool operator==(const HomTransMatDS& A) const {
        return (rotMat == A.rotMat) && (trans == A.trans);
    }

    bool operator!=(const HomTransMatDS& A) const {
        return (rotMat != A.rotMat) || (trans != A.trans);
    }

public:
    RotationMatDS rotMat;
    Vector3DS trans;
};

class MsnhNet_API HomTransMatFS {
public:
    RotationMatFS rotMat;
    Vector3FS trans;

    HomTransMatFS() = default;

    HomTransMatFS(const RotationMatFS& rotMat, const Vector3FS& trans)
        : rotMat(rotMat), trans(trans) { }

    HomTransMatFS(const RotationMatFS& rotMat) : rotMat(rotMat) { }

    HomTransMatFS(const Vector3FS& trans) : trans(trans) { }

    HomTransMatFS& operator=(const HomTransMatFS& mat) {
        if (this != &mat) {
            rotMat = mat.rotMat;
            trans = mat.trans;
        }
        return *this;
    }

    void translate(const TranslationFS& vector) {
        trans[0] += vector[0];
        trans[1] += vector[1];
        trans[2] += vector[2];
    }

    void translate(const float& x, const float& y, const float& z) {
        trans[0] += x;
        trans[1] += y;
        trans[2] += z;
    }

    void rotate(const float& angleInRad, const float& x, const float& y,
        const float& z) {
        rotate(angleInRad, Vector3FS(x, y, z));
    }

    void rotate(const float& angleInRad, const Vector3FS& vector) {
        Vector3FS vec = vector;
        vec.normalize();
        const float x = vec[0];
        const float y = vec[1];
        const float z = vec[2];

        rotMat = GeometryS::euler2RotMat(
            EulerFS(x * angleInRad, y * angleInRad, z * angleInRad),
            RotSequence::ROT_ZYX);
    }

    void rotate(const EulerFS& euler) {
        rotMat = GeometryS::euler2RotMat(euler, RotSequence::ROT_ZYX);
    }

    void rotate(const QuaternionFS& quat) {
        rotMat = GeometryS::quaternion2RotMat(quat);
    }

    HomTransMatFS invert() const {
        HomTransMatFS tmp;

        tmp.rotMat = rotMat.inverse();
        tmp.trans = rotMat.invMul(trans * -1);
        return tmp;
    }

    friend HomTransMatFS operator*(
        const HomTransMatFS& A, const HomTransMatFS& B) {
        return {A.rotMat * B.rotMat, A.rotMat * B.trans + A.trans};
    }

    void print();

    std::string toString() const;

    std::string toHtmlString() const;

    bool operator==(const HomTransMatFS& A) const {
        return (rotMat == A.rotMat) && (trans == A.trans);
    }

    bool operator!=(const HomTransMatFS& A) const {
        return (rotMat != A.rotMat) || (trans != A.trans);
    }
};

}  // namespace Msnhnet

#endif  // MSNHHOMTRANSMATS_H
