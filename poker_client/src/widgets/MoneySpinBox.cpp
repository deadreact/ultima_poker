#include <precompiled.h>
#include "MoneySpinBox.h"

MoneySpinBox::MoneySpinBox(QWidget *parent)
    : QDoubleSpinBox(parent)
{
    connect(this, &MoneySpinBox::moneyChanged, this, [this](Money money){ setValue(money.toDouble());});
    connect(this, (void(QDoubleSpinBox::*)(double))&MoneySpinBox::valueChanged, this, [this](double value)
    {
        if (!qFuzzyCompare(m_money.toDouble(), value) ) {
            overrideMoney(Money::fromDouble(value));
        }
    });
}

MoneySpinBox::~MoneySpinBox()
{

}

Money MoneySpinBox::money() const
{
    return m_money;
}

bool MoneySpinBox::overrideMoney(Money m)
{
    bool changed = (m_money != m);
    m_money = m;
    return changed;
}

void MoneySpinBox::setMoney(Money m)
{
    if (m_money == m)
        return;

    m_money = m;
    emit moneyChanged(m);
}

void MoneySpinBox::setRange(Money min, Money max)
{
    QDoubleSpinBox::setRange(min.toDouble(), max.toDouble());
}
