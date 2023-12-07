#include <precompiled.h>
#include "MoneySlider.h"

MoneySlider::MoneySlider(QWidget *parent)
    : QSlider(parent)
{
    connect(this, &MoneySlider::sliderPressed, this, [this]{
        m_lastCursor = cursor().shape();
        setCursor(Qt::ClosedHandCursor);
    });
    connect(this, &MoneySlider::sliderReleased, this, [this]{
        setCursor(m_lastCursor);
    });
}

MoneySlider::MoneySlider(Qt::Orientation orientation, QWidget *parent)
    : QSlider(orientation, parent)
{
    connect(this, &MoneySlider::sliderPressed, this, [this]{
        m_lastCursor = cursor().shape();
        setCursor(Qt::ClosedHandCursor);
    });
    connect(this, &MoneySlider::sliderReleased, this, [this]{
        setCursor(m_lastCursor);
    });
}

MoneySlider::~MoneySlider()
{

}

Money MoneySlider::getMoney() const {
    return value();
}

void MoneySlider::setToMinimum()
{
    setValue(minimum());
}

void MoneySlider::setToMaximum()
{
    setValue(maximum());
}

void MoneySlider::setRange(Money min, Money max)
{
    QSlider::setRange(min.amount(), max.amount());
}

void MoneySlider::setMoney(Money moneyValue)
{
    if (moneyValue.amount() != value()) {
        setValue(moneyValue.amount());
        emit moneyChanged(moneyValue);
    }
}
