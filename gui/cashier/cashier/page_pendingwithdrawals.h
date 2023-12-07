#pragma once

#include "cashier_global.h"
#include "page_cashierbase.h"

namespace Ui {
class PendingWithdrawals;
}

namespace ns_page
{
    class CASHIER_DLL PendingWithdrawals : public CashierBase
    {
        Q_OBJECT
    private:
        Ui::PendingWithdrawals *m_ui;
    public:
        explicit PendingWithdrawals(QWidget *parent = nullptr);
        virtual ~PendingWithdrawals();

        virtual void init() override;
    public slots:
        void popupInfo(bool show);
    };

} //namespace ns_page
