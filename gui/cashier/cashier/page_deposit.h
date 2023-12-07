#pragma once

#include "cashier_global.h"
#include "page_cashierbase.h"

namespace Ui {
class Deposit;
}

namespace ns_page
{
    class CASHIER_DLL Deposit : public CashierBase
    {
        Q_OBJECT
    private:
        Ui::Deposit *m_ui;
    public:
        explicit Deposit(QWidget *parent = nullptr);
        virtual ~Deposit();

        virtual void init() /*override*/;
    };

} //namespace ns_page
