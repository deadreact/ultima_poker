#include "transform2dmapper.h"
#include "../basewidgets/basewidgets/util.h"

Transform2DMapper::Transform2DMapper(QWidget *parent)
    : ConvertTypeMapper(parent)
    , m_x(0)
    , m_y(0)
{}

void Transform2DMapper::setX(qreal x)
{
    if ( !qFuzzyCompare(m_x, x) )
    {
        m_x = x;
        emit positionChanged(QVector2D(x, m_y));
    }
}
void Transform2DMapper::setY(qreal y)
{
    if ( !qFuzzyCompare(m_y, y) )
    {
        m_y = y;
        emit positionChanged(QVector2D(m_x, y));
    }
}
