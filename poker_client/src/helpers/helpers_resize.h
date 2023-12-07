#pragma once

#include <tools/types_fwd.hpp>

class QWidget;
class QSize;

namespace ns_helpers
{
    void resizeEvenly(QWidget* w, const QSize& newSize);

    void scaleMinMaxSize(QWidget* w, float sxMin, float syMin, float sxMax, float syMax);
    void scaleSize(QWidget* w, float sx, float sy);

} //namespace ns_helpers
