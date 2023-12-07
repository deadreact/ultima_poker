#include "animation.h"
#include <qwidget.h>
#include <qpropertyanimation.h>

namespace ns_anim
{

    QPropertyAnimation *moveTo(QWidget *w, const QPoint &position, int msec)
    {
        QPropertyAnimation *anim = new QPropertyAnimation(w, "pos", w);
        anim->setDuration(msec);
        anim->setStartValue(w->pos());
        anim->setEndValue(position);

        return anim;
    }

    QPropertyAnimation *resizeTo(QWidget *w, const QSize &size, int msec)
    {
        QPropertyAnimation *anim = new QPropertyAnimation(w, "size", w);
        anim->setDuration(msec);
        anim->setStartValue(w->size());
        anim->setEndValue(size);

        return anim;
    }

    QPropertyAnimation *changeGeometry(QWidget *w, const QRect &geometry, int msec)
    {
        QPropertyAnimation *anim = new QPropertyAnimation(w, "geometry", w);
        anim->setDuration(msec);
        anim->setStartValue(w->geometry());
        anim->setEndValue(geometry);

        return anim;
    }
} //ns_anim
