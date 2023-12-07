#include "colortransform.h"

#include <QDebug>

ColorTransform::ColorTransform(const QMatrix4x4 &m, const QVector4D &o)
    : m_mat(m)
    , m_off(o)
{}

ColorTransform::ColorTransform(float rR, float gR, float bR, float aR, float offR
                             , float rG, float gG, float bG, float aG, float offG
                             , float rB, float gB, float bB, float aB, float offB
                             , float rA, float gA, float bA, float aA, float offA)
    : m_mat(rR, gR, bR, aR
          , rG, gG, bG, aG
          , rB, gB, bB, aB
          , rA, gA, bA, aA)
    , m_off(offR, offG, offB, offA)
{

}

float ColorTransform::value(int index) const
{
    Q_ASSERT(index >= 0 && index < 20);
    return (index < 16) ? m_mat.data()[index] : m_off[index-16];
}

float &ColorTransform::value(int index)
{
    Q_ASSERT(index >= 0 && index < 20);
    return (index < 16) ? m_mat.data()[index] : m_off[index-16];
}

void ColorTransform::setMatrix(const QMatrix4x4 &m)
{
    if (m_mat != m) {
        m_mat = m;
    }
}

void ColorTransform::setOffset(const QVector4D &off)
{
    if (m_off != off) {
        m_off = off;
    }
}

void ColorTransform::print() const
{
    qDebug() << value(r.R) << value(g.R) << value(b.R) << value(a.R) << value(off.R);
    qDebug() << value(r.G) << value(g.G) << value(b.G) << value(a.G) << value(off.G);
    qDebug() << value(r.B) << value(g.B) << value(b.B) << value(a.B) << value(off.B);
    qDebug() << value(r.A) << value(g.A) << value(b.A) << value(a.A) << value(off.A);
}


QDataStream &operator<<(QDataStream &os, const ColorTransform &colorTransform) {
    os << colorTransform.matrix() << colorTransform.offset();
    return os;
}

QDataStream &operator>>(QDataStream &is, ColorTransform &colorTransform) {
    QMatrix4x4 m;
    QVector4D o;
    is >> m >> o;
    colorTransform.setMatrix(m);
    colorTransform.setOffset(o);

    return is;
}
