#pragma once

#include "cashier_global.h"
#include <QDialog>
#include "../../common/api/gbp_int.hpp"

namespace Ui {
class Cashier;
}

namespace ns_model {
    class user_info_t;
}
namespace ns_lobby {
    struct rpc_object;
} //ns_lobby

class CASHIER_DLL CashierDialog : public QDialog
{
    Q_OBJECT
public:
    enum class page
    {
        homepage = 0,
        account,
        operations,
        personalInfo,
        sendMoney,

        deposit,
        withdrawal,
        pendingWithdrawals,
        transactions
    };
    Q_ENUM(page)

private:
    Ui::Cashier *m_ui;
    ns_model::user_info_t* m_userInfoModel;
    ns_lobby::rpc_object* m_rpcObject;
public:
    explicit CashierDialog(QWidget *parent = nullptr);
    virtual ~CashierDialog();

    void setAvatar(const QIcon& ico);
    virtual void init();

    ns_model::user_info_t* userInfoModel() const;
    void setUserInfoModel(ns_model::user_info_t* userInfoModel);

    ns_lobby::rpc_object* rpcObject() const;
    void setRpcObject(ns_lobby::rpc_object* rpcObject);

    gbp_i64 inputSendMoneyValue() const;
    void setSendMoneyValue(gbp_i64 amount);
protected:
    virtual void closeEvent(QCloseEvent *e) override;
    template <page pg>
    void initPage();


    void onUserInfoChanged(int rowIndex, int role = Qt::DisplayRole);
private slots:
    void updateSendRequestAvailability();
    void on_bttn_send_clicked();
    void on_input_sendCoins_valueChanged(int val);

public slots:
    virtual void reject() override;
};

