#pragma once

#include <qmatrix4x4.h>
#include <qvector4d.h>

class ColorTransform
{
private:
    template <int loc, int step=1>
    struct Channel {
        static const int R   = loc;
        static const int G   = R + step;
        static const int B   = G + step;
        static const int A   = B + step;

    };
public:
    static const Channel<0> r;
    static const Channel<4> g;
    static const Channel<8> b;
    static const Channel<12> a;
    static const Channel<16> off;

private:
    QMatrix4x4 m_mat;
    QVector4D  m_off;
public:
    ColorTransform(const QMatrix4x4& m = QMatrix4x4(), const QVector4D& o = QVector4D(0.f, 0.f, 0.f, 0.f));

    ColorTransform(float rR, float gR, float bR, float aR, float offR
                 , float rG, float gG, float bG, float aG, float offG
                 , float rB, float gB, float bB, float aB, float offB
                 , float rA, float gA, float bA, float aA, float offA);

    inline const QMatrix4x4& matrix() const;
    inline const QVector4D& offset() const;

    float value(int index) const;
    float& value(int index);

    void setMatrix(const QMatrix4x4& m);
    void setOffset(const QVector4D& off);

    void print() const;
};

QDataStream& operator<<(QDataStream& os, const ColorTransform& colorTransform);
QDataStream& operator>>(QDataStream& is, ColorTransform& colorTransform);

// inline methods
inline const QMatrix4x4 &ColorTransform::matrix() const {
    return m_mat;
}

inline const QVector4D &ColorTransform::offset() const {
    return m_off;
}

