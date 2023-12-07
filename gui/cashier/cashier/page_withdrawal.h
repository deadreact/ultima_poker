#pragma once

#include "cashier_global.h"
#include "page_cashierbase.h"

namespace Ui {
class Withdrawal;
}

namespace ns_page
{

    class CASHIER_DLL Withdrawal : public CashierBase
    {
        Q_OBJECT
    private:
        Ui::Withdrawal *m_ui;
    public:
        explicit Withdrawal(QWidget *parent = nullptr);
        virtual ~Withdrawal();

        virtual void init() override;
    public slots:
        void popupRulesInfo(bool show);
    };

} //namespace ns_page
