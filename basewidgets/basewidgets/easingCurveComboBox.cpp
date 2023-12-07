//#ifdef NOT_A_PLUGIN
//    #include <precompiled.h>
//#endif // NOT_A_PLUGIN
#include "easingCurveComboBox.h"
#include <QMetaEnum>

EasingCurveComboBox::EasingCurveComboBox(QWidget *parent)
    : QComboBox(parent)
{
    QMetaEnum curveEnum = QMetaEnum::fromType<QEasingCurve::Type>();

    for (int i = 0; i < curveEnum.keyCount(); ++i) {
        this->addItem(curveEnum.key(i), QVariant::fromValue(QEasingCurve::Type(i)));
    }

    connect(this, (void(EasingCurveComboBox::*)(int))&EasingCurveComboBox::currentIndexChanged, this, [this](int index){ emit easingChanged(QEasingCurve::Type(index)); });
}
