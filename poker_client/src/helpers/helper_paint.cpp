#include <precompiled.h>
#include <qpainter.h>
#include "helper_paint.h"

namespace ns_helpers
{
    QPixmap transparentPixmap(const QPixmap &original, qreal opacity)
    {
        QPixmap pix(original.size());
        pix.fill(Qt::transparent);
        QPainter p;
        p.begin(&pix);
        p.setOpacity(opacity);
        p.drawPixmap(original.rect(), original);
        p.end();
        return pix;
    }

} //namespace ns_helpers
