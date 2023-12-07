#include <precompiled.h>
#include "camera.h"

Camera::Camera(QObject *parent)
    : QObject(parent)
    , m_position(0, 0, -300)
    , m_fov(45.f)
    , m_nearPlane(0.01f)
    , m_farPlane(10000.f)
    , m_aspectRatio(16.f/9.f)
    , m_isRespondable(true)
    , m_isActive(true)
{}

QMatrix4x4 Camera::orthoMatrix(QRect& viewportRect) const
{
    QMatrix4x4 orthoMat;
    orthoMat.ortho(viewportRect);
    return orthoMat;
}

float Camera::fov() const
{
    return m_fov;
}

float Camera::nearPlane() const
{
    return m_nearPlane;
}

float Camera::farPlane() const
{
    return m_farPlane;
}

const QVector3D &Camera::position() const
{
    return m_position;
}

const QQuaternion &Camera::rotation() const
{
    return m_rotation;
}

float Camera::aspectRatio() const
{
    return m_aspectRatio;
}

QMatrix4x4 Camera::projMatrix(float aspectRatio) const
{
    QMatrix4x4 matrix;
    matrix.perspective(m_fov, aspectRatio, m_nearPlane, m_farPlane);
    return matrix;
}
QMatrix4x4 Camera::projMatrix() const
{
    return projMatrix(m_aspectRatio);
}

QMatrix4x4 Camera::projViewMatrix() const
{
    QMatrix4x4 projection = projMatrix();
    projection.rotate(m_rotation);
    projection.translate(m_position);
    return projection;
}

void Camera::setFov(float fov)
{
    if (m_fov == fov)
        return;

    m_fov = fov;
    emit fovChanged(fov);
}

void Camera::setNearPlane(float nearPlane)
{
    if (m_nearPlane == nearPlane)
        return;

    m_nearPlane = nearPlane;
    emit nearPlaneChanged(nearPlane);
}

void Camera::setFarPlane(float farPlane)
{
    if (m_farPlane == farPlane)
        return;

    m_farPlane = farPlane;
    emit farPlaneChanged(farPlane);
}

void Camera::setPosition(const QVector3D &position)
{
    if (m_position == position)
        return;

    m_position = position;
    emit positionChanged(position);
}

void Camera::setRotation(const QQuaternion &quat)
{
    if (m_rotation == quat)
        return;

    m_rotation = quat;
    emit rotationChanged(quat);
}

void Camera::setAspectRatio(float aspectRatio)
{
    if (qFuzzyCompare(m_aspectRatio, aspectRatio))
        return;

    m_aspectRatio = aspectRatio;
    emit aspectRatioChanged(aspectRatio);
}


//bool Camera::isContainer() const {
//    return false;
//}

//bool Camera::hasVisibility() const {
//    return false;
//}

//bool Camera::isResponding() const {
//    return true;
//}

//bool Camera::isStatic() const {
//    return false;
//}

//bool Camera::hasPosition() const {
//    return true;
//}

//bool Camera::hasDirection() const {
//    return true;
//}

//bool Camera::hasTransform() const {
//    return true;
//}

//bool Camera::hasGeometry() const {
//    return false;
//}

//bool Camera::hasInternalState() const {
//    return false;
//}

//uint Camera::dimensionsCount() const {
//    return 3;
//}

bool Camera::isRespondable() const
{
    return m_isRespondable;
}

bool Camera::isActive() const
{
    return m_isActive;
}

QVector3D Camera::direction() const
{
    return rotation().rotatedVector(QVector3D(0, 0, 1));
}

void Camera::setRespondable(bool respondable)
{
    if (m_isRespondable != respondable)
    {
        m_isRespondable = respondable;
        //emit
    }
}

void Camera::setActive(bool active)
{
    if (m_isActive != active)
    {
        m_isActive = active;
        //emit
    }
}

void Camera::setDirection(const QVector3D &direction)
{
    setRotation(QQuaternion::fromDirection(direction, QVector3D(0, 1, 0)));
}


bool Camera::event(QEvent *e)
{
    return QObject::event(e);
}
