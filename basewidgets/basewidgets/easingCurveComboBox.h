#pragma once

#include "basewidgets_global.h"
#include <QComboBox>
#include <QEasingCurve>

class BASEWIDGETS_DLL EasingCurveComboBox : public QComboBox
{
    Q_OBJECT
public:
    Q_INVOKABLE EasingCurveComboBox(QWidget* parent = nullptr);
signals:
    Q_SIGNAL void easingChanged(QEasingCurve::Type);
};
