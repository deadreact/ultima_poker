#include <precompiled.h>
#include "widget_BetScrollBar.h"
#include <QStyle>
#include <QStyleOptionComplex>

namespace
{
    template <typename T>
    T correct_mult(T val) {
        Q_ASSERT(val != 0);
        return val;
    }
}

namespace ns_widget
{
    BetScrollBar::BetScrollBar(QWidget *parent)
        : QScrollBar(Qt::Horizontal, parent)
        , m_multiplier(1)
    {
        setupPageStep();
    }

    BetScrollBar::BetScrollBar(Qt::Orientation orientation, QWidget *parent)
        : QScrollBar(orientation, parent)
        , m_multiplier(1)
    {
        setupPageStep();
    }

    BetScrollBar::~BetScrollBar() {}

    int BetScrollBar::multiplier() const {
        return m_multiplier;
    }

    Money BetScrollBar::betValue() const {
        return Money(value()) * m_multiplier;
    }

    double BetScrollBar::relativeValue() const {
        return double(maximum() - minimum())/(value() - minimum());
    }

    void BetScrollBar::setupPageStep()
    {
        connect(this, &BetScrollBar::actionTriggered, this, [this](int sliderAction) {
            QStyleOptionComplex opt;
            opt.initFrom(this);
            QRect grooveRect = style()->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarGroove);

            if (sliderAction == SliderPageStepAdd) {
            } else if (sliderAction == SliderPageStepSub) {

            }
        });
    }

    void BetScrollBar::setToMinimum() {
        setValue(minimum());
    }

    void BetScrollBar::setToMaximum() {
        setValue(maximum());
    }

    void BetScrollBar::setBetValue(Money bet) {
        setValue(bet.amount() / correct_mult(m_multiplier));
    }

    void BetScrollBar::setRelativeValue(double relativeValue) {
        Q_ASSERT(relativeValue >= 0 && relativeValue <= 1.0);
        setValue(minimum() * relativeValue + maximum() * (1.0 - relativeValue));
    }

    void BetScrollBar::setBetRange(Money minBet, Money maxBet) {
        int minValue = minBet.amount() / correct_mult(m_multiplier);
        int maxValue = maxBet.amount() / correct_mult(m_multiplier);
        int rangeSize = maxValue - minValue;

        if (rangeSize < pageStep()) {
            setSingleStep(0);
            setPageStep(0);
            setRange(maxValue - 1, maxValue);
            setValue(maxValue);
        } else {
            setRange(minValue, maxValue);
        }
    }

    void BetScrollBar::setMultiplier(int multiplier) {
        Q_ASSERT(multiplier > 0);
        if (m_multiplier != multiplier) {
            Money bet = betValue();

            m_multiplier = multiplier;
            setBetValue(bet);
        }
    }

} //namespace ns_widget
