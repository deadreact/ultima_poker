#pragma once

#include <QMatrix4x4>
#include <QObject>
#include "utils/Utils.h"

class Camera : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QMatrix4x4 projMatrix READ projMatrix)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QQuaternion rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(float fov READ fov WRITE setFov NOTIFY fovChanged)
    Q_PROPERTY(float nearPlane READ nearPlane WRITE setNearPlane NOTIFY nearPlaneChanged)
    Q_PROPERTY(float farPlane READ farPlane WRITE setFarPlane NOTIFY farPlaneChanged)
    Q_PROPERTY(float aspectRatio READ aspectRatio WRITE setAspectRatio NOTIFY aspectRatioChanged)
private:
    QVector3D m_position;
    QQuaternion m_rotation;
    float m_fov;
    float m_nearPlane;
    float m_farPlane;
    float m_aspectRatio;
    bool m_isRespondable;
    bool m_isActive;
public:
    explicit Camera(QObject *parent = 0);

    QMatrix4x4 orthoMatrix(QRect &viewportRect) const;
    QMatrix4x4 projMatrix(float aspectRatio) const;
    QMatrix4x4 projMatrix() const;
    QMatrix4x4 projViewMatrix() const;
    float fov() const;
    float nearPlane() const;
    float farPlane() const;
    const QVector3D &position() const;    
    const QQuaternion& rotation() const;
    float aspectRatio() const;

signals:

    void fovChanged(float fov);
    void nearPlaneChanged(float nearPlane);
    void farPlaneChanged(float farPlane);
    void positionChanged(const QVector3D &position);
    void rotationChanged(const QQuaternion& rotation);
    void aspectRatioChanged(float aspectRatio);

public slots:
    void setFov(float fov);
    void setNearPlane(float nearPlane);
    void setFarPlane(float farPlane);
    void setPosition(const QVector3D &position);
    void setRotation(const QQuaternion& quat);
    void setAspectRatio(float aspectRatio);

    // SceneObjectInterface interface
//protected:
//    virtual bool isContainer() const override;
//    virtual bool hasVisibility() const override;
//    virtual bool isResponding() const override;
//    virtual bool isStatic() const override;
//    virtual bool hasPosition() const override;
//    virtual bool hasDirection() const override;
//    virtual bool hasTransform() const override;
//    virtual bool hasGeometry() const override;
//    virtual bool hasInternalState() const override;
//    virtual uint dimensionsCount() const override;
public:
    bool isRespondable() const;
    bool isActive() const;
    QVector3D direction() const;
public slots:
    void setRespondable(bool respondable);
    void setActive(bool active);
    void setDirection(const QVector3D& direction);


    // QObject interface
public:
    virtual bool event(QEvent *e) override;
};
