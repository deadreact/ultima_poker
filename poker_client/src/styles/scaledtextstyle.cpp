#ifdef NOT_A_PLUGIN
    #include <precompiled.h>
#endif //NOT_A_PLUGIN
#include "scaledtextstyle.h"
#include <QAbstractButton>
#include <QGraphicsEffect>
#include <qlabel.h>
#include <QPainter>
#include <QDebug>


namespace
{
    bool isInRect(const QRect rc, const QSize& size)
    {
        return rc.width() >= size.width() && rc.height() >= size.height();
    }
}

ScaledTextStyle::ScaledTextStyle(int min, int def, QStyle *style)
    : QProxyStyle(style)
    , m_minimumSize(min)
    , m_defaultSize(def)
{
//    qDebug() << "ScaledTextStyle";
}

int ScaledTextStyle::minimumSize() const {
    return m_minimumSize;
}

int ScaledTextStyle::defaultSize() const {
    return m_defaultSize;
}

void ScaledTextStyle::setMinimumSize(int minSize)
{
    if (m_minimumSize != minSize) {
        m_minimumSize = minSize;
    }
}

void ScaledTextStyle::setDefaultSize(int size)
{
    if (m_defaultSize != size) {
        m_defaultSize = size;
    }
}


 void ScaledTextStyle::drawItemText(QPainter *p
                                  , const QRect &rect
                                  , int alignment
                                  , const QPalette &pal
                                  , bool enabled
                                  , const QString &text
                                  , QPalette::ColorRole textRole) const
{
    QWidget* widget = dynamic_cast<QWidget*>(p->device());
    if (widget) {
//        p->
        QFont fnt = p->font();
        fnt.setStyleStrategy(QFont::StyleStrategy(QFont::PreferQuality | QFont::PreferOutline | QFont::PreferAntialias));
        fnt.setPixelSize(m_defaultSize);
        if (fnt.pixelSize() < m_minimumSize) {
            fnt.setPixelSize(m_minimumSize);
            p->setFont(fnt);
            widget->setFont(fnt);
        } else {
            fnt.setPixelSize(m_defaultSize);
            p->setFont(fnt);
            widget->setFont(fnt);

            while (fnt.pixelSize() > m_minimumSize && !isInRect(rect, p->fontMetrics().size(alignment, text))) {
                fnt.setPixelSize(fnt.pixelSize() - 1);
                p->setFont(fnt);
            }
        }
        p->setFont(fnt);
    }
    QProxyStyle::drawItemText(p, rect, alignment|Qt::TextWordWrap|Qt::ElideLeft, pal, enabled, text, textRole);
}
