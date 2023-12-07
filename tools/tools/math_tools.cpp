#include "math_tools.hpp"

#include <qrect.h>
#include <qwidget.h>

namespace ns_util
{
    void sincosf(float __x, float *p_sin, float *p_cos)
    {
    #ifdef Q_OS_DARWIN
        __sincosf(__x, p_sin, p_cos);
    #else
        ::sincosf(__x, p_sin, p_cos);
    #endif // MAC_OS
    }

}

QRect square(const QRect &rc) {
    int h = std::min(rc.height(), rc.width());
    return QRect(rc.x(), rc.y(), h, h);
}

template <>
QObject* get_parent(QObject* child) {
    return child->parent();
}
template <>
QWidget* get_parent(QWidget* child) {
    return child->parentWidget();
}
