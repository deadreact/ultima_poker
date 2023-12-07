#pragma once

#include "converttypemapper.h"
#include <QVector2D>
#include <QPoint>
#include <QPointF>

class Transform2DMapper : public ConvertTypeMapper
{
    Q_OBJECT

    qreal m_x;
    qreal m_y;
public:
    Transform2DMapper(QWidget *parent = 0);
signals:
    void positionChanged(QVector2D);
public slots:
    void setX(qreal x);
    void setY(qreal y);
};
