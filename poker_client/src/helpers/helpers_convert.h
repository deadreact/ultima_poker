#pragma once

#include <QSize>
#include <QPoint>

namespace ns_helpers
{
    inline QSize point2size(const QPoint& p) {
        return QSize(p.x(), p.y());
    }
    inline QPoint size2point(const QSize& s) {
        return QPoint(s.width(), s.height());
    }
    inline QSize operator+(const QSize& s, const QPoint& p) {
        return s + point2size(p);
    }
} //namespace ns_helpers

template <typename T1, typename T2>
T1 gbp_cast(const T2& value);
