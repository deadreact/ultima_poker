#pragma once

#include <qpushbutton.h>
#include <core/money.h>

#ifndef Q_MOC_RUN
#endif // Q_MOC_RUN

namespace ns_widget
{
    class MoneyButton : public QPushButton
    {
        Q_OBJECT

        Q_PROPERTY(gbp_i64 moneyMin READ moneyMin WRITE setMoneyMin)
        Q_PROPERTY(gbp_i64 moneyMax READ moneyMax WRITE setMoneyMax)
        Q_PROPERTY(gbp_i64 money READ money WRITE setMoney NOTIFY moneyChanged)
        Q_PROPERTY(QString text READ text WRITE setText)
    private:
        gbp_i64 m_moneyMin;
        gbp_i64 m_moneyMax;
        gbp_i64 m_money;
        QString m_text;

        struct ConnCounter
        {
            gbp_i16 moneyType, intType, doubleType;
            ConnCounter() : moneyType(0), intType(0), doubleType(0) {}
        };
        ConnCounter m_connCounter;

    public:
        MoneyButton(QWidget* parent=nullptr);
        virtual ~MoneyButton();
        gbp_i64 money() const;
        gbp_i64 moneyMin() const;
        gbp_i64 moneyMax() const;

        static QMetaMethod signalClickedMoney();
        static QMetaMethod signalClickedDouble();
        static QMetaMethod signalClickedInt();
        QString text() const;

    signals:
        void clicked(gbp_i64);
        void clicked(double);
        void clicked(int);
        void moneyChanged(gbp_i64 money);
    public slots:
        void setMoney(gbp_i64 money);
        void setMoneyMin(gbp_i64 money);
        void setMoneyMax(gbp_i64 money);
        void setText(const QString &text);

    protected slots:
        void emitClicked();
    protected:
        virtual void connectNotify(const QMetaMethod &signal) override;
        virtual void disconnectNotify(const QMetaMethod &signal) override;
    };

} //namespace widget
