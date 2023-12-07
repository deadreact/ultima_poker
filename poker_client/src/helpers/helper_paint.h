#pragma once
#include <qpixmap.h>


namespace ns_helpers
{
    QPixmap transparentPixmap(const QPixmap &original, qreal opacity);
    void drawWithOpacity(QPainter &p, QPaintDevice* target, qreal opacity);
}
