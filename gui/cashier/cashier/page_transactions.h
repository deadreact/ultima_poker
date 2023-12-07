#pragma once

#include "cashier_global.h"
#include "page_cashierbase.h"

namespace Ui {
class Transactions;
}

namespace ns_page
{
    class CASHIER_DLL Transactions : public CashierBase
    {
        Q_OBJECT
    private:
        Ui::Transactions *m_ui;
    public:
        explicit Transactions(QWidget *parent = nullptr);
        virtual ~Transactions();

        virtual void init() override;
    
    };

} //namespace ns_page
