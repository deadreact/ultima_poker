#include <QApplication>
#include "../cashier/cashierdialog.h"
#include <QTimer>
#include <page_deposit.h>
#include <page_transactions.h>
#include <page_withdrawal.h>
#include <page_pendingwithdrawals.h>
#include <QPushButton>
#include <api_models/model_user_info_t.hpp>

//using ParentClass = QFrame;
using ParentClass = CashierDialog;
class Test : public ParentClass {
//    Q_OBJECT
public:
    Test(QWidget* parent = nullptr): ParentClass(parent) {}
    virtual void init() /*override */{}
};

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    CashierDialog d;
    d.setUserInfoModel(new ns_model::user_info_t);
    d.init();
    d.show();

//    ns_page::PendingWithdrawals* w;
//    QPushButton* bttn = new QPushButton;
//    bttn->show();

//    bttn->connect(bttn, &QPushButton::clicked, [bttn, &w]{
//        w = new ns_page::PendingWithdrawals(bttn);
//        w->show();
//    });

    return app.exec();
}
