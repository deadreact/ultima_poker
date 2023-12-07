#pragma once

#include "cashier_global.h"
#include <QFrame>

namespace ns_page
{

    class CASHIER_DLL CashierBase : public QFrame
    {
        Q_OBJECT
    protected:

    public:
        explicit CashierBase(QWidget *parent = nullptr);
        virtual ~CashierBase();

        virtual void init() {}
    };

}// namespace page

