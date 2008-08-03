#ifndef KMXX_MAT4_H
#define KMXX_MAT4_H

#include <kazmath/mat4.h>

/**
Class that represents a 4x4 matrix.
*/

struct Mat4 : public kmMat4 {
    Mat4(); ///<Constructor
    Mat4(const Mat4& m); ///<Copy constructor

    const Mat4 operator*(const Mat4& m) const;
    const Mat4 operator*(const kmScalar s) const;

    Mat4& operator*=(const Mat4& m);
    Mat4& operator*=(const kmScalar s);

    const Mat4 operator/(const Mat4& m) const;
    const Mat4 operator/(const kmScalar s) const;

    Mat4& operator/=(const Mat4& m);
    Mat4& operator/=(const kmScalar s);
};

#endif
