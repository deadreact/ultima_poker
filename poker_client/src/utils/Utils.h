#pragma once

#include <tools/types_fwd.hpp>
#include <QTimeZone>
class QEvent;

template <gbp_u8 p0, gbp_u8 p1, gbp_u8 p2, gbp_u8 p3>
struct IpAddress
{
    static const quint32 value = (p0 << 030) + (p1 << 020) + (p2 << 010) + (p3 << 0);
};

namespace ns_util
{
    constexpr static const float eplison32 = 0.001f;
    constexpr static const double eplison64 = 0.0001;

    QList<QTimeZone> filteredTimeZones();

    template <typename EnumType>
    QString toString(EnumType e);
}

namespace ns_debug_util
{
    const char* toCString(const QEvent* e);
}
