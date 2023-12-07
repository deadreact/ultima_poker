#ifdef NOT_A_PLUGIN
    #include <precompiled.h>
#endif //NOT_A_PLUGIN
#include <qpainter.h>
#include "colorizedstyle.h"
#include <QVariant>

namespace
{
//    QPixmap transparentPixmap(const QPixmap &original, qreal opacity)
//    {
//        QPixmap pix(original.size());
//        pix.fill(Qt::transparent);
//        QPainter p;
//        p.begin(&pix);
//        p.setOpacity(opacity);
//        p.drawPixmap(original.rect(), original);
//        p.end();
//        return pix;
//    }

//    struct pixel {
//        union {
//            gbp_u32 _rgba;
//            struct
//            {
//                gbp_u8 _b;
//                gbp_u8 _g;
//                gbp_u8 _r;
//                gbp_u8 _a;
//            };
//        };
//    };

//    union It {
//        pixel* _32;
//        uchar* _8;
//    };

//    QImage colorize(const QImage& image, const QColor& color)
//    {
//        QImage result = image.copy();
////        result.convertToFormat(QImage::Format_ARGB32);

//        It it;
//        It itEnd;
//        it._8 = result.bits();
//        itEnd._8 = result.bits() + result.byteCount();
//        int buffer[4];
//        for (; it._32 != itEnd._32; it._32 += 1) {
//            buffer[0] = it._32->_r * color.redF();
//            buffer[1] = it._32->_g * color.greenF();
//            buffer[2] = it._32->_b * color.blueF();
//            buffer[3] = it._32->_a * color.alphaF();
//            it._32->_r = qBound(0, buffer[0], 255);
//            it._32->_g = qBound(0, buffer[1], 255);
//            it._32->_b = qBound(0, buffer[2], 255);
//            it._32->_a = qBound(0, buffer[3], 255);
//        }
//        return result;
//    }
}

ColorizedStyle::ColorizedStyle(QStyle *style)
    : QProxyStyle(style)
    , m_color(Qt::transparent)
    , m_opacity(1.0)
{
}

QColor ColorizedStyle::color() const
{
    return m_color;
}

qreal ColorizedStyle::opacity() const
{
    return m_opacity;
}

void ColorizedStyle::setColor(QColor color)
{
    if (m_color == color)
        return;

    m_color = color;
    emit colorChanged(color);
}

void ColorizedStyle::setOpacity(qreal opacity)
{
    if (qFuzzyCompare(m_opacity, opacity)) {
        return;
    }

    m_color.setAlphaF(opacity);
    emit opacityChanged(opacity);
}


void ColorizedStyle::drawItemPixmap(QPainter *painter, const QRect &rect, int alignment, const QPixmap &pixmap) const
{
    QWidget* widget = dynamic_cast<QWidget*>(painter->device());

    if (widget && !pixmap.isNull())
    {
//        QImage img = pixmap.toImage();
//        img = colorize(img, m_color);
//        QProxyStyle::drawItemPixmap(painter, rect, alignment, QPixmap::fromImage(img));
        bool ok = false;
        qreal opacity = widget->property("opacity").toReal(&ok);
        if (ok) {
            painter->setOpacity(opacity);
        }
        QProxyStyle::drawItemPixmap(painter, rect, alignment, pixmap);
    }
}


//void ColorizedStyle::drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const
//{
//}

//void ColorizedStyle::drawControl(QStyle::ControlElement element, const QStyleOption *opt, QPainter *p, const QWidget *w) const
//{
//}

//void ColorizedStyle::drawComplexControl(QStyle::ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p, const QWidget *widget) const
//{
//}
