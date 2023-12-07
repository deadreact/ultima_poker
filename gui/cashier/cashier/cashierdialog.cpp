#include "cashierdialog.h"
#include "ui_cashierdialog.h"
#include <qeventloop.h>
#include <qdebug.h>

#ifndef Q_MOC_RUN
    #include <api/api_common.h>
    #include <rpcbase/rpcbase_lobby.h>
#endif // !Q_MOC_RUN

#include <api_models/model_user_info_t.hpp>
#include <api_models/model_transformproxy.h>
#include <api_models/api_util.hpp>

Q_DECLARE_METATYPE(gbp::ns_api::e_payment_system)


namespace
{
    typedef ns_model::user_info_t::field mField;
}

template <>
void CashierDialog::initPage<CashierDialog::page::account>()
{
    m_ui->money_cash_available->setText(userInfoModel()->text(ns_model::user_info_t::field::_money));
    m_ui->money_cash_ingame->setText(userInfoModel()->text(ns_model::user_info_t::field::_in_game_money));
    m_ui->money_cash_total->setText(Money(userInfoModel()->money(ns_model::user_info_t::field::_money)
                                         + userInfoModel()->money(ns_model::user_info_t::field::_in_game_money)).toString());
}

template <>
void CashierDialog::initPage<CashierDialog::page::operations>()
{
    connect(m_ui->bttn_addMoney, &QAbstractButton::clicked, this, [this] {
        m_ui->stackOfPages->setCurrentWidget(m_ui->page_deposit);
    });
    connect(m_ui->bttn_getMoney, &QAbstractButton::clicked, this, [this] {
        m_ui->stackOfPages->setCurrentWidget(m_ui->page_withdrawal);
    });
    connect(m_ui->bttn_pendingWithdrawal, &QAbstractButton::clicked, this, [this] {
        m_ui->stackOfPages->setCurrentWidget(m_ui->page_pendingWithdrawals);
    });
    connect(m_ui->bttn_history, &QAbstractButton::clicked, this, [this] {
        m_ui->stackOfPages->setCurrentWidget(m_ui->page_transactions);
    });
}

template <>
void CashierDialog::initPage<CashierDialog::page::personalInfo>()
{
    m_ui->value_nickname->setText(userInfoModel()->text(ns_model::user_info_t::field::_nickname));
    m_ui->info_name->setText(userInfoModel()->text(ns_model::user_info_t::field::_username));
    m_ui->info_surname->setText(userInfoModel()->text(ns_model::user_info_t::field::_last_name));
    m_ui->info_email->setText(userInfoModel()->text(ns_model::user_info_t::field::_email));
    m_ui->info_birthdate->setText(userInfoModel()->text(ns_model::user_info_t::field::_birth_date));
    m_ui->info_country->setText(userInfoModel()->text(ns_model::user_info_t::field::_country));
    m_ui->info_city->setText(userInfoModel()->text(ns_model::user_info_t::field::_city));
    m_ui->info_address->setText(userInfoModel()->text(ns_model::user_info_t::field::_mail_address));
    m_ui->info_index->setText(userInfoModel()->text(ns_model::user_info_t::field::_mail_index));
}

template <>
void CashierDialog::initPage<CashierDialog::page::sendMoney>()
{
    m_ui->input_sendMoney->setRange(0, std::min(150000ll, userInfoModel()->money(ns_model::user_info_t::field::_money)));
}

template <>
void CashierDialog::initPage<CashierDialog::page::homepage>()
{
    initPage<page::account>();
    initPage<page::operations>();
    initPage<page::personalInfo>();
    initPage<page::sendMoney>();
}

template <>
void CashierDialog::initPage<CashierDialog::page::deposit>()
{
    m_ui->page_deposit->init();
}
template <>
void CashierDialog::initPage<CashierDialog::page::withdrawal>()
{
    m_ui->page_withdrawal->init();
}
template <>
void CashierDialog::initPage<CashierDialog::page::pendingWithdrawals>()
{
    m_ui->page_pendingWithdrawals->init();
}
template <>
void CashierDialog::initPage<CashierDialog::page::transactions>()
{
    m_ui->page_transactions->init();
}

CashierDialog::CashierDialog(QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::Cashier)
    , m_userInfoModel(nullptr)
    , m_rpcObject(nullptr)
{
    m_ui->setupUi(this);
}

CashierDialog::~CashierDialog()
{
    delete m_ui;
}

void CashierDialog::init()
{
    if (m_userInfoModel == nullptr) {
        qCritical("   Loc: [%s:%i]\nFatal error! m_rpcObject is NULL!\n"
                  "Can not initialize Cashier dialog, ensure you set model before init"
                  , __FILE__, __LINE__);
        return;
    }
    if (m_rpcObject == nullptr) {
        qCritical("   Loc: [%s:%i]\nFatal error! m_userInfoModel is NULL!\n"
                  "Can not initialize Cashier dialog, ensure you set rpcObject before init"
                  , __FILE__, __LINE__);
        return;
    }

    initPage<page::homepage>();
    initPage<page::deposit>();
    initPage<page::withdrawal>();
    initPage<page::pendingWithdrawals>();
    initPage<page::transactions>();

    connect(userInfoModel(), &ns_model::user_info_t::dataChanged, this
    , [this](const QModelIndex& index, const QModelIndex&, const QVector<int> &roles){
        onUserInfoChanged(index.row(), roles.first());
    });
}

ns_model::user_info_t *CashierDialog::userInfoModel() const
{
    return m_userInfoModel;
}

void CashierDialog::setUserInfoModel(ns_model::user_info_t *userInfoModel)
{
    m_userInfoModel = userInfoModel;
}

ns_lobby::rpc_object *CashierDialog::rpcObject() const {
    return m_rpcObject;
}

void CashierDialog::setRpcObject(ns_lobby::rpc_object *rpcObject)
{
    if (m_rpcObject != rpcObject) {
        m_rpcObject = rpcObject;
    }
}

gbp_i64 CashierDialog::inputSendMoneyValue() const
{
    return m_ui->input_sendMoney->value() * 100 + m_ui->input_sendCoins->value();
}

void CashierDialog::setSendMoneyValue(gbp_i64 amount)
{
    amount = qBound(0ll, amount, userInfoModel()->money(ns_model::user_info_t::field::_money));
    m_ui->input_sendMoney->setValue(amount / 100);
    m_ui->input_sendCoins->setValue(amount % 100);
}

void CashierDialog::onUserInfoChanged(int rowIndex, int role)
{
    if (ns_model::user_info_t::field(rowIndex) == ns_model::user_info_t::field::_money) {
        initPage<page::account>();
    }
}


void CashierDialog::setAvatar(const QIcon &ico)
{
    m_ui->profile_picture->setIcon(ico);
    m_ui->profile_picture->setIconSize(QSize(120, 120));
}

void CashierDialog::updateSendRequestAvailability()
{
    static const QRegExp re("(^[a-zA-Z_]\\w{3,11}$)");

    QString input = m_ui->input_reciever->text();
    if (input.isEmpty()) {
        m_ui->infomessage->resetCurrentMsgIndex();
        return;
    }

    if (!re.exactMatch(input)) {
        m_ui->infomessage->setCurrentMsgIndex(6);
        return;
    }
    if (input == userInfoModel()->text(ns_model::user_info_t::field::_nickname)) {
        m_ui->infomessage->setCurrentMsgIndex(7);
        return;
    }

    auto callback = [this](ns_lobby::e_status status) {
        m_ui->bttn_send->setEnabled(status == ns_lobby::e_status::nick_busy);
        switch (status) {
        case ns_lobby::e_status::nick_busy: m_ui->infomessage->setCurrentMsgIndex(0); break;
        case ns_lobby::e_status::ok: m_ui->infomessage->setCurrentMsgIndex(4); break;
        case ns_lobby::e_status::unknown_error: m_ui->infomessage->setCurrentMsgIndex(5); break;
        default:
            break;
        }
    };
    m_rpcObject->nick_free(input, callback);

}

void CashierDialog::on_bttn_send_clicked()
{
    gbp_i64 value = inputSendMoneyValue();
    m_rpcObject->send_money_to_user(m_ui->input_reciever->text(), value
    , [this, value](ns_lobby::e_status status){
        if (status == ns_lobby::e_status::ok) {
            m_ui->infomessage->setCurrentMsgIndex(3);
            QVariant val = QVariant::fromValue(userInfoModel()->money(ns_model::user_info_t::field::_money) - value);
            userInfoModel()->setData(userInfoModel()->index(underlying_cast(ns_model::user_info_t::field::_money)), val);
        } else {
            m_ui->infomessage->setCurrentMsgIndex(8);
        }
    });
}

void CashierDialog::on_input_sendCoins_valueChanged(int val)
{
    if (val == -1) {
        m_ui->input_sendCoins->setValue(99);
        m_ui->input_sendMoney->setValue(m_ui->input_sendMoney->value() - 1);
    } else if (val == 100) {
        m_ui->input_sendCoins->setValue(0);
        m_ui->input_sendMoney->setValue(m_ui->input_sendMoney->value() + 1);
    }
}



void CashierDialog::closeEvent(QCloseEvent *e)
{
    if (m_ui->stackOfPages->currentWidget() == m_ui->page_homepage) {
        e->accept();
    } else {
        m_ui->stackOfPages->setCurrentWidget(m_ui->page_homepage);
        e->ignore();
    }
}

void CashierDialog::reject()
{
    if (m_ui->stackOfPages->currentWidget() == m_ui->page_homepage) {
        QDialog::reject();
    } else {
        m_ui->stackOfPages->setCurrentWidget(m_ui->page_homepage);
    }
}
