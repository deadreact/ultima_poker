#include "rotatedwidget.h"
#include <qpainter.h>
#include <cmath>

struct RotatedWidget::PrivateData {
    qreal rad;
    QMatrix parentMat;
    QMatrix offsetMat;
    QMatrix rotMat;
    QMatrix hotSpotMat;
    QMatrix modelMat;

    PrivateData(const QWidget* owner)
        : rad(0)
        , parentMat()
        , offsetMat(1, 0, 0, 1, -owner->width()/2, -owner->height()/2)
        , rotMat()
        , hotSpotMat(1, 0, 0, 1, owner->width()/2, owner->height()/2)
        , modelMat(offsetMat * rotMat * hotSpotMat)
    {}

    void setRotationRad(qreal value) {
        rad = value;
        qreal cos = ::cos(rad);
        qreal sin = ::sin(rad);
        rotMat.setMatrix(cos,-sin, sin, cos, 0, 0);
        modelMat = offsetMat * rotMat * hotSpotMat;
    }
    void setHotSpot(qreal x, qreal y) {
        offsetMat.setMatrix(1, 0, 0, 1, -x, -y);
        hotSpotMat.setMatrix(1, 0, 0, 1, x, y);
        modelMat = offsetMat * rotMat * hotSpotMat;
    }

    void setParentMatrix(const QMatrix& m) {
        parentMat = m;
    }
};

RotatedWidget::RotatedWidget(QWidget *parent)
    : EntityWidget(parent)
    , m_dPtr(new PrivateData(this))
    , m_rttMode(false)
{}

RotatedWidget::~RotatedWidget() {
    delete m_dPtr;
}

qreal RotatedWidget::rotationRad() const {
    return m_dPtr->rad;
}

QMatrix RotatedWidget::parentMatrix() const
{
    return m_dPtr->parentMat;
}

void RotatedWidget::initPainter(QPainter *p) const
{
    EntityWidget::initPainter(p);
    p->setMatrix(m_dPtr->parentMat * m_dPtr->modelMat);
}

void RotatedWidget::resizeEvent(QResizeEvent *e)
{
    EntityWidget::resizeEvent(e);
    m_dPtr->setHotSpot(width()/2, height()/2);
}

void RotatedWidget::paintEvent(QPaintEvent *e)
{
    if (!m_rttMode) {
        EntityWidget::paintEvent(e);
    }
}

void RotatedWidget::renderChildren(QPainter *p)
{
    if (p && p->isActive()) {

    }
}

void RotatedWidget::setRotationRad(qreal rotationRad)
{
    if (!qFuzzyCompare(m_dPtr->rad, rotationRad))
    {
        m_dPtr->setRotationRad(rotationRad);
        update();
        for (QObject* child: children()) {
            child->setProperty("parentMatrix", QVariant::fromValue(m_dPtr->modelMat));
        }
        emit rotationRadChanged(rotationRad);
    }
}

void RotatedWidget::setParentMatrix(const QMatrix &matrix)
{
    if (parentMatrix() != matrix) {
        m_dPtr->setParentMatrix(matrix);
        for (QObject* child: children()) {
            child->setProperty("parentMatrix", QVariant::fromValue(matrix));
//            QVariant prop = child->property("parentMatrix");
//            if (prop.isValid()) {
//            }
        }
    }
}
