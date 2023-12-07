#include <precompiled.h>
#include "panel_BetControls.h"
#include "ui_panel_BetControls.h"

namespace ns_panel
{

    BetControls::BetControls(QWidget *parent)
        : EntityWidget(parent)
        , m_ui(new Ui::BetControls)
        , m_value(0)
    {
        m_ui->setupUi(this);
    }

    BetControls::~BetControls()
    {
        delete m_ui;
    }

    void BetControls::init()
    {
        connect(m_ui->bttnGrp_moneyBttns, static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked), this,
        [this](QAbstractButton* bttn) {
            setValue(qvariant_cast<Money>(bttn->property("money")));
        });
        connect(m_ui->bet_spinbox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this,
        [this](int value) {
            setValue(Money::fromBills(value));
        });
        connect(m_ui->bet_slider, &ns_widget::BetScrollBar::valueChanged, this,
        [this](int value) {
            setValue(static_cast<Money>(value));
        });

        // ------------------------

        connect(this, &BetControls::valueChanged, this, [this](Money val) {
            m_ui->bet_spinbox->setValue(val.bills());
            m_ui->bet_spinbox->setSuffix(QString(".%0").arg(val.coins()));
            m_ui->bet_slider->setBetValue(val);
        });
        connect(m_ui->bttn_min, &QAbstractButton::clicked, this, &BetControls::setToMin);
        connect(m_ui->bttn_max, &QAbstractButton::clicked, this, &BetControls::setToMax);
    }

    Money BetControls::value() const {
        return m_value;
    }

    Money BetControls::min() const
    {
        return m_min;
    }

    Money BetControls::max() const
    {
        return m_max;
    }

    void BetControls::enableBlindButtons(Money bigBlind, Money userRoundBet)
    {
        Money tripleBlind = bigBlind*3 - userRoundBet;
        Money quatroBlind = bigBlind*4 - userRoundBet;
        m_ui->bttn_3bb->setMoney(tripleBlind);
        m_ui->bttn_4bb->setMoney(quatroBlind);
        m_ui->bttn_3bb->setEnabled(tripleBlind >= m_min && tripleBlind <= m_max);
        m_ui->bttn_4bb->setEnabled(quatroBlind >= m_min && quatroBlind <= m_max);
        m_ui->stackOfMoneyButtons->setCurrentWidget(m_ui->page_blinds);
    }

    void BetControls::enablePotButtons(Money pot, Money half)
    {
        m_ui->bttn_pot->setMoney(pot);
        m_ui->bttn_half->setMoney(half);
        m_ui->bttn_pot->setEnabled(pot >= m_min && pot <= m_max);
        m_ui->bttn_half->setEnabled(half >= m_min && half <= m_max);
        m_ui->stackOfMoneyButtons->setCurrentWidget(m_ui->page_pots);
    }

    void BetControls::setRange(Money min, Money max)
    {
        if (m_min == min && m_max == max) {
            return;
        }
        m_min = min;
        m_max = max;
        m_ui->bet_slider->setBetRange(min, max);
        m_ui->bet_spinbox->setRange(min/100, max/100);
//        updateMoneyBttns();
        emit rangeChanged(min, max);
    }

    void BetControls::setValue(Money m)
    {
        m = qBound(m_min, m, m_max);
        if (m_value != m)
        {
            m_value = m;
            emit valueChanged(m);
        }
    }

    void BetControls::setMin(Money m)
    {
        setRange(m, m_max);
    }
    void BetControls::setMax(Money m)
    {
        setRange(m_min, m);
    }

    void BetControls::updateMoneyBttns()
    {

    }

} //namespace ns_panel
