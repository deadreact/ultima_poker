#pragma once

#include <qscrollbar.h>
#include <core/money.h>

#ifndef Q_MOC_RUN
#endif // Q_MOC_RUN

namespace ns_widget
{
    class BetScrollBar : public QScrollBar
    {
        Q_OBJECT
        Q_PROPERTY(int multiplier READ multiplier WRITE setMultiplier)
        Q_PROPERTY(double relativeValue READ relativeValue WRITE setRelativeValue STORED false)
    private:
        /** Для того что-бы оставить привычный интерфейс операций с int, но обеспечить корректную поддержку gbp_i64 (Money)
            Редкий случай, когда банк превышает сумму $21,474,836.48
        */
        int m_multiplier;
    public:
        explicit BetScrollBar(QWidget *parent = Q_NULLPTR);
        explicit BetScrollBar(Qt::Orientation, QWidget *parent = Q_NULLPTR);
        virtual ~BetScrollBar();

        void setMultiplier(int multiplier);
        int multiplier() const;

        Money betValue() const;
        double relativeValue() const;
    protected:
        virtual void setupPageStep();
    public slots:
        void setToMinimum();
        void setToMaximum();
        void setRelativeValue(double relativeValue);
        void setBetRange(Money minBet, Money maxBet);
        void setBetValue(Money bet);
        inline void setBetValue(double money);
    };

    void BetScrollBar::setBetValue(double money) {
        setBetValue(Money::fromDouble(money));
    }

} //namespace widget

