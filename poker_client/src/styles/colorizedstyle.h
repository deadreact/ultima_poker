#pragma once

#include "styles_global.h"
#include <QWidget>
#include <QProxyStyle>

class STYLES_DLL ColorizedStyle : public QProxyStyle
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
private:
    QColor m_color;
    qreal m_opacity;
public:
    ColorizedStyle(QStyle* style = nullptr);
    virtual ~ColorizedStyle() {}
    QColor color() const;
    qreal opacity() const;

    public slots:
    void setColor(QColor color);
    void setOpacity(qreal opacity);
signals:
    void colorChanged(QColor color);
    void opacityChanged(qreal opacity);
public:
    virtual void drawItemPixmap(QPainter *painter, const QRect &rect, int alignment, const QPixmap &pixmap) const override;

//    // QStyle interface
//public:
//    virtual void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const override;
//    virtual void drawControl(ControlElement element, const QStyleOption *opt, QPainter *p, const QWidget *w) const override;
//    virtual void drawComplexControl(ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p, const QWidget *widget) const override;
};
