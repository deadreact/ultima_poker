#pragma once

#include <QDoubleSpinBox>
#include <core/money.h>

class MoneySpinBox : public QDoubleSpinBox
{
    Q_OBJECT
    Q_PROPERTY(Money money READ money WRITE setMoney NOTIFY moneyChanged)
    Money m_money;

public:
    explicit MoneySpinBox(QWidget *parent = 0);
    virtual ~MoneySpinBox();
    Money money() const;
    //! Во избежания взаимных зацикливаний
    //! Этот метод не шлет сигнал а возвращает результат изменилось ли значение
    bool overrideMoney(Money m);
signals:
    void moneyChanged(Money);

public slots:
    void setMoney(Money m);
    void setRange(Money min, Money max);
};

