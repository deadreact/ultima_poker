#include <precompiled.h>
#include "widget_MoneyButton.h"
#include <QMetaMethod>

namespace ns_widget
{
    MoneyButton::MoneyButton(QWidget *parent)
        : QPushButton(parent)
        , m_moneyMin(0)
        , m_moneyMax(0)
        , m_money(0)
    {
        connect(this, SIGNAL(clicked(bool)), this, SLOT(emitClicked()));
    }

    MoneyButton::~MoneyButton() {}

    gbp_i64 MoneyButton::money() const
    {
        return m_money;
    }

    gbp_i64 MoneyButton::moneyMin() const {
        return m_moneyMin;
    }

    gbp_i64 MoneyButton::moneyMax() const {
        return m_moneyMax;
    }

    void MoneyButton::setMoney(gbp_i64 money)
    {
        if (m_money == money)
            return;

        m_money = money;
        QString txt = Money(m_money).toString();
        setToolTip(txt);
        if (m_text.contains("%0")) {
            QPushButton::setText(m_text.arg(txt));
        }
        emit moneyChanged(money);
    }

    void MoneyButton::setMoneyMin(gbp_i64 money)
    {
        if (money != m_moneyMin) {
            m_moneyMin = money;
            if (m_moneyMax < m_moneyMin) {
                setMoneyMax(m_moneyMin);
            }
            if (m_money < m_moneyMin) {
                setMoney(m_moneyMin);
            } else if (m_money > m_moneyMax) {
                setMoney(m_moneyMax);
            }
        }
    }

    void MoneyButton::setMoneyMax(gbp_i64 money)
    {
        if (money != m_moneyMax) {
            m_moneyMax = money;
            if (m_moneyMax < m_moneyMin) {
                setMoneyMin(m_moneyMax);
            }
            if (m_money > m_moneyMax) {
                setMoney(m_moneyMax);
            } else if (m_money < m_moneyMin) {
                setMoney(m_moneyMin);
            }
        }
    }

    void MoneyButton::setText(const QString& text)
    {
        if (m_text != text)
        {
            m_text = text;
            if (m_text.contains("%0")) {
                QPushButton::setText(m_text.arg(Money(m_money).toString()));
            } else {
                QPushButton::setText(m_text);
            }
        }
    }

    void MoneyButton::emitClicked()
    {
        if (m_connCounter.moneyType > 0) emit clicked(m_money);
        if (m_connCounter.doubleType > 0) emit clicked(Money(m_money).toDouble());
        if (m_connCounter.intType > 0) emit clicked((int)m_money);
    }

    void MoneyButton::connectNotify(const QMetaMethod &signal)
    {
        if (signal == signalClickedMoney()) {
            m_connCounter.moneyType++;
        } else if (signal == signalClickedDouble()) {
            m_connCounter.doubleType++;
        } else if (signal == signalClickedInt()) {
            m_connCounter.intType++;
        }
    }

    void MoneyButton::disconnectNotify(const QMetaMethod &signal)
    {
        if (signal == signalClickedMoney()) {
            m_connCounter.moneyType--;
        } else if (signal == signalClickedDouble()) {
            m_connCounter.doubleType--;
        } else if (signal == signalClickedInt()) {
            m_connCounter.intType--;
        }
    }

    QMetaMethod MoneyButton::signalClickedMoney()  { return QMetaMethod::fromSignal((void(MoneyButton::*)(gbp_i64))&MoneyButton::clicked); }
    QMetaMethod MoneyButton::signalClickedDouble() { return QMetaMethod::fromSignal((void(MoneyButton::*)(double))&MoneyButton::clicked); }
    QMetaMethod MoneyButton::signalClickedInt()    { return QMetaMethod::fromSignal((void(MoneyButton::*)(int))&MoneyButton::clicked); }

    QString MoneyButton::text() const {
        return QPushButton::text();
    }

} //namespace ns_widget



