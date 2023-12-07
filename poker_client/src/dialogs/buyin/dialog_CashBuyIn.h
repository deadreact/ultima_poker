#pragma once
#include "interface/initable.h"
#include <QDialog>
#include "core/money.h"
#include <descriptors/descriptor_CashBuyIn.h>
#include <QTime>

namespace Ui {
class BuyIn;
}
class GameHandler;

namespace ns_dialog {
    class CashBuyIn : public InitableDialog
    {
        Q_OBJECT

        Q_PROPERTY(Money money READ money WRITE setMoney NOTIFY moneyChanged)
        Q_PROPERTY(Money playersMoney READ playersMoney WRITE setPlayersMoney)
        Q_PROPERTY(bool isEnoughMoney READ isEnoughMoney NOTIFY enoughMoneyChanged)
    private:
        Ui::BuyIn *m_ui;
        const gbp_i64 m_tableId;
        Money m_minMoney;
        Money m_maxMoney;
        Money m_currentMoney;
        Money m_playersMoney;

        bool m_isEnoughMoney;
        bool m_isRatMode;

        bool m_isSitOnTable;

        QTime m_initializedTime;
    public:
        explicit CashBuyIn(gbp_i64 tableId, QWidget* parent, Qt::WindowFlags flags=0);
        virtual ~CashBuyIn();

        virtual void init() override;
        void initWithDescriptor(const ns_descriptor::CashBuyIn* desc);
        Money money() const;
        Money playersMoney() const;

        std::pair<Money, Money> range() const;
        Money minimum() const;
        Money maximum() const;

        bool isEnoughMoney() const;
        bool autorebuyChecked() const;
    public slots:
        void setRatMode(bool ratMode, gbp_i64 timestamp = -1);
        void setRange(Money min, Money max);
        void setMoney(Money);
        void setPlayersMoney(Money playersMoney);

        void setToMinimum();
        void setToMaximum();

        virtual void accept() override;
        virtual void reject() override;
    signals:
        void moneyChanged(Money money);
        void enoughMoneyChanged(bool);
    protected:
        void updateInterface();
        virtual void changeEvent(QEvent *e) override;

    public slots:
    };

} // namespace ns_dialog
