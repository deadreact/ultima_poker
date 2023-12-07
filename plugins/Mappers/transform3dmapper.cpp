#include "transform3dmapper.h"
#include "../basewidgets/basewidgets/util.h"
#include <qmath.h>
#include <QQuaternion>

Transform3DMapper::Transform3DMapper(QWidget *parent)
    : ConvertTypeMapper(parent)
    , m_position(0, 0, 0)
    , m_angles(0, 0, 0)
    , m_scale(1, 1, 1)
    , m_color(Qt::white)
{}

void Transform3DMapper::setX(float x)
{
    if (qFuzzyCompare(m_position.x(), x))
        return;

    m_position.setX(x);
    emit positionChanged(m_position);
}
void Transform3DMapper::setX(double x) {
    setX(static_cast<float>(x));
}
void Transform3DMapper::setX(int x) {
    setX(static_cast<float>(x));
}

void Transform3DMapper::setY(float y)
{
    if (qFuzzyCompare(m_position.y(), y))
        return;

    m_position.setY(y);
    emit positionChanged(m_position);
}
void Transform3DMapper::setY(double y) {
    setY(static_cast<float>(y));
}
void Transform3DMapper::setY(int y) {
    setY(static_cast<float>(y));
}

void Transform3DMapper::setZ(float z)
{
    if (qFuzzyCompare(m_position.z(), z))
        return;

    m_position.setZ(z);
    emit positionChanged(m_position);
}
void Transform3DMapper::setZ(double z) {
    setZ(static_cast<float>(z));
}
void Transform3DMapper::setZ(int z) {
    setZ(static_cast<float>(z));
}

void Transform3DMapper::setPitchDeg(float pitchDeg)
{
    if (qFuzzyCompare(m_angles.x(), pitchDeg))
        return;

    m_angles.setX(pitchDeg);
    emit rotationChanged(QQuaternion::fromEulerAngles(m_angles));
}
void Transform3DMapper::setPitchDeg(double pitchDeg) {
    setPitchDeg(static_cast<float>(pitchDeg));
}

void Transform3DMapper::setPitchDeg(int pitchDeg) {
    setPitchDeg(static_cast<float>(pitchDeg));
}

void Transform3DMapper::setPitchRad(float pitchRad) {
    setPitchDeg(qRadiansToDegrees(pitchRad));
}

void Transform3DMapper::setPitchRad(double pitchRad) {
    setPitchRad(static_cast<float>(pitchRad));
}

void Transform3DMapper::setYawDeg(float yawDeg)
{
    if (qFuzzyCompare(m_angles.y(), yawDeg))
        return;

    m_angles.setY(yawDeg);
    emit rotationChanged(QQuaternion::fromEulerAngles(m_angles));
}
void Transform3DMapper::setYawDeg(double yawDeg) {
    setYawDeg(static_cast<float>(yawDeg));
}

void Transform3DMapper::setYawDeg(int yawDeg) {
    setYawDeg(static_cast<float>(yawDeg));
}

void Transform3DMapper::setYawRad(float yawRad) {
    setYawDeg(qRadiansToDegrees(yawRad));
}

void Transform3DMapper::setYawRad(double yawRad) {
    setYawRad(static_cast<float>(yawRad));
}

void Transform3DMapper::setRollDeg(float rollDeg)
{
    if (qFuzzyCompare(m_angles.z(), rollDeg))
        return;

    m_angles.setZ(rollDeg);
    emit rotationChanged(QQuaternion::fromEulerAngles(m_angles));
}
void Transform3DMapper::setRollDeg(double rollDeg) {
    setRollDeg(static_cast<float>(rollDeg));
}

void Transform3DMapper::setRollDeg(int rollDeg) {
    setRollDeg(static_cast<float>(rollDeg));
}

void Transform3DMapper::setRollRad(float rollRad) {
    setRollDeg(qRadiansToDegrees(rollRad));
}

void Transform3DMapper::setRollRad(double rollRad) {
    setRollRad(static_cast<float>(rollRad));
}

void Transform3DMapper::setScaleX(float sx)
{
    if (qFuzzyCompare(m_scale.x(), sx))
        return;

    m_scale.setX(sx);
    emit scaleChanged(m_scale);
}
void Transform3DMapper::setScaleX(double sx) {
    setScaleX(static_cast<float>(sx));
}

void Transform3DMapper::setScaleY(float sy)
{
    if (qFuzzyCompare(m_scale.y(), sy))
        return;

    m_scale.setY(sy);
    emit scaleChanged(m_scale);
}
void Transform3DMapper::setScaleY(double sy) {
    setScaleY(static_cast<float>(sy));
}
void Transform3DMapper::setScaleZ(float sz)
{
    if (qFuzzyCompare(m_scale.z(), sz))
        return;

    m_scale.setZ(sz);
    emit scaleChanged(m_scale);
}
void Transform3DMapper::setScaleZ(double sz) {
    setScaleZ(static_cast<float>(sz));
}

void Transform3DMapper::setRed(int red)
{
//    red = red % 255;
    if (m_color.red() == red)
        return;

    m_color.setRed(red);
    emit colorChanged(m_color);
}

void Transform3DMapper::setRed(uint red) {
    setRed(static_cast<int>(red));
}

void Transform3DMapper::setRed(double red)
{
    if ( qFuzzyCompare(m_color.redF(), red))
        return;

    m_color.setRedF(red);
    emit colorChanged(m_color);
}

void Transform3DMapper::setRed(float red) {
    setRed(static_cast<qreal>(red));
}

void Transform3DMapper::setGreen(int green)
{
    if (m_color.green() == green)
        return;

    m_color.setGreen(green);
    emit colorChanged(m_color);
}

void Transform3DMapper::setGreen(uint green) {
    setGreen(static_cast<int>(green));
}

void Transform3DMapper::setGreen(double green)
{
    if ( qFuzzyCompare(m_color.greenF(), green))
        return;

    m_color.setGreenF(green);
    emit colorChanged(m_color);
}

void Transform3DMapper::setGreen(float green) {
    setGreen(static_cast<qreal>(green));
}

void Transform3DMapper::setBlue(int blue)
{
    if (m_color.blue() == blue)
        return;

    m_color.setBlue(blue);
    emit colorChanged(m_color);
}

void Transform3DMapper::setBlue(uint blue) {
    setBlue(static_cast<int>(blue));
}

void Transform3DMapper::setBlue(double blue)
{
    if ( qFuzzyCompare(m_color.blueF(), blue))
        return;

    m_color.setBlueF(blue);
    emit colorChanged(m_color);
}

void Transform3DMapper::setBlue(float blue) {
    setBlue(static_cast<qreal>(blue));
}

void Transform3DMapper::setAlpha(int alpha)
{
    if (m_color.alpha() == alpha)
        return;

    m_color.setAlpha(alpha);
    emit colorChanged(m_color);
}

void Transform3DMapper::setAlpha(uint alpha) {
    setAlpha(static_cast<int>(alpha));
}

void Transform3DMapper::setAlpha(double alpha)
{
    if ( qFuzzyCompare(m_color.alphaF(), alpha))
        return;

    m_color.setAlphaF(alpha);
    emit colorChanged(m_color);
}

void Transform3DMapper::setAlpha(float alpha) {
    setAlpha(static_cast<qreal>(alpha));
}
