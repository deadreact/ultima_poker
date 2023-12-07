#pragma once

#include <QSlider>
#include <core/money.h>

class MoneySlider : public QSlider
{
    Q_OBJECT

    Q_PROPERTY(Money money READ getMoney WRITE setMoney NOTIFY moneyChanged)
private:
    Qt::CursorShape m_lastCursor;
public:
    explicit MoneySlider(QWidget *parent = 0);
    explicit MoneySlider(Qt::Orientation orientation, QWidget *parent = 0);
    virtual ~MoneySlider();
    Money getMoney() const;
signals:
    void moneyChanged(Money);
public slots:
    void setToMinimum();
    void setToMaximum();
    void setRange(Money min, Money max);
    void setMoney(Money moneyValue);

};
