#pragma once

#include "animation_global.h"

class QPropertyAnimation;
class QWidget;
class QRect;
class QPoint;
class QSize;

namespace ns_anim
{
    ANIMATION_DLL QPropertyAnimation* moveTo(QWidget* w, const QPoint& position, int msec = 500);
    ANIMATION_DLL QPropertyAnimation* resizeTo(QWidget* w, const QSize& size, int msec = 500);
    ANIMATION_DLL QPropertyAnimation* changeGeometry(QWidget* w, const QRect& geometry, int msec = 500);
} //namespace ns_anim
