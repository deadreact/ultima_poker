#include <precompiled.h>
#include "helpers_resize.h"

#include <QWidget>

namespace ns_helpers
{
    void resizeEvenly(QWidget *w, const QSize &newSize)
    {
        qreal sxMin = (w->minimumWidth() > newSize.width()) ? qreal(newSize.width()) / w->minimumWidth() : 1.0;
        qreal syMin = (w->minimumHeight() > newSize.height()) ? qreal(newSize.height()) / w->minimumHeight() : 1.0;

        qreal sxMax = (w->maximumWidth() < newSize.width()) ? qreal(newSize.width()) / w->maximumWidth() : 1.0;
        qreal syMax = (w->maximumHeight() < newSize.height()) ? qreal(newSize.height()) / w->maximumHeight() : 1.0;

        qreal sx = qreal(newSize.width()) / w->width();
        qreal sy = qreal(newSize.height()) / w->height();

        scaleMinMaxSize(w, sxMin, syMin, sxMax, syMax);
        scaleSize(w, sx, sy);
    }

    void scaleMinMaxSize(QWidget* w, float sxMin, float syMin, float sxMax, float syMax)
    {
        for (QObject* child: w->children()) {
            if (child->isWidgetType()) {
                scaleMinMaxSize(static_cast<QWidget*>(child), sxMin, syMin, sxMax, syMax);
            }
        }
        w->setMinimumSize(w->minimumWidth() * sxMin, w->minimumHeight() * syMin);
        w->setMaximumSize(w->maximumWidth() * sxMax, w->maximumHeight() * syMax);
    }

    void scaleSize(QWidget *w, float sx, float sy)
    {
        for (QObject* child: w->children()) {
            if (child->isWidgetType()) {
                scaleSize(static_cast<QWidget*>(child), sx, sy);
            }
        }
        w->resize(w->width() * sx, w->height() * sy);
    }

} //namespace ns_helpers
