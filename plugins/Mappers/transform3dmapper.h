#pragma once

#include "converttypemapper.h"
#include <QVector3D>
#include <QQuaternion>

class Transform3DMapper : public ConvertTypeMapper
{
    Q_OBJECT

    QVector3D m_position;
    QVector3D m_angles;
    QVector3D m_scale;
    QColor m_color;
public:
    Transform3DMapper(QWidget *parent = 0);
signals:
    void positionChanged(const QVector3D&);
    void rotationChanged(QQuaternion);
    void scaleChanged(const QVector3D&);
    void colorChanged(const QColor&);
public slots:
    void setX(float x);
    void setX(double x);
    void setX(int x);
    void setY(float y);
    void setY(double y);
    void setY(int y);
    void setZ(float z);
    void setZ(double z);
    void setZ(int z);

    void setPitchDeg(float pitchDeg);
    void setPitchDeg(double pitchDeg);
    void setPitchDeg(int pitchDeg);
    void setPitchRad(float pitchRad);
    void setPitchRad(double pitchRad);

    void setYawDeg(float yawDeg);
    void setYawDeg(double yawDeg);
    void setYawDeg(int   yawDeg);
    void setYawRad(float yawRad);
    void setYawRad(double yawRad);

    void setRollDeg(float rollDeg);
    void setRollDeg(double rollDeg);
    void setRollDeg(int   rollDeg);
    void setRollRad(float rollRad);
    void setRollRad(double rollRad);

    void setScaleX(float sx);
    void setScaleX(double sx);
    void setScaleY(float sy);
    void setScaleY(double sy);
    void setScaleZ(double sz);
    void setScaleZ(float sz);

    void setRed(int red);
    void setRed(uint red);
    void setRed(double red);
    void setRed(float red);

    void setGreen(int    green);
    void setGreen(uint   green);
    void setGreen(double green);
    void setGreen(float  green);

    void setBlue(int    blue);
    void setBlue(uint   blue);
    void setBlue(double blue);
    void setBlue(float  blue);

    void setAlpha(int    alpha);
    void setAlpha(uint   alpha);
    void setAlpha(double alpha);
    void setAlpha(float  alpha);
};
