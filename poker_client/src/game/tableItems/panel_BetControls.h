#pragma once

#include <core/money.h>
#include <basewidgets/entitywidget.h>

namespace Ui {
class BetControls;
}

namespace ns_panel
{
    class BetControls : public EntityWidget
    {
        Q_OBJECT
        Q_PROPERTY(Money value READ value WRITE setValue NOTIFY valueChanged USER true)
        Q_PROPERTY(Money min READ min WRITE setMin)
        Q_PROPERTY(Money max READ max WRITE setMax)
//        Q_PROPERTY(long long max READ max WRITE setMax)
    private:
        Ui::BetControls *m_ui;
        Money m_value;
        Money m_min;
        Money m_max;
    signals:
        void valueChanged(Money);
        void rangeChanged(Money, Money);
    public:
        explicit BetControls(QWidget *parent = 0);
        virtual ~BetControls();

        virtual void init() override;
        Money value() const;
        Money min() const;
        Money max() const;

        void enableBlindButtons(Money bigBlind, Money userRoundBet = 0);
        void enablePotButtons(Money pot, Money half);
    public slots:
        void setRange(Money, Money);
        inline void setToMin() { setValue(m_min); }
        inline void setToMax() { setValue(m_max); }
        void setValue(Money m);
    private slots:
        void setMin(Money m);
        void setMax(Money m);
    private:
        void updateMoneyBttns();
    };

} //namespace ns_panel

