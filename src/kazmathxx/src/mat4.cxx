#include <cassert>
#include <memory>
#include "mat4.h"

Mat4::Mat4() {
    memset(m_Mat, 0, sizeof(kmScalar) * 16);
}

Mat4::Mat4(const Mat4& m) {
    memcpy(m_Mat, m.m_Mat, sizeof(kmScalar) * 16);
}

const Mat4 Mat4::operator*(const Mat4& m) const {
    Mat4 result;
    kmMat4Multiply(&result, this, &m);
    return result;
}

const Mat4 Mat4::operator*(const kmScalar s) const {
    assert(0 && "Must implement scalar multiplication in kazmath");
}

Mat4& Mat4::operator*=(const Mat4& m) {
    assert(0);
}

Mat4& Mat4::operator*=(const kmScalar s) {
    assert(0);
}

const Mat4 Mat4::operator/(const Mat4& m) const {
    assert(0);
}

const Mat4 Mat4::operator/(const kmScalar s) const {
    assert(0);
}

Mat4& Mat4::operator/=(const Mat4& m) {
    assert(0);
}

Mat4& Mat4::operator/=(const kmScalar s) {
    assert(0);
}
