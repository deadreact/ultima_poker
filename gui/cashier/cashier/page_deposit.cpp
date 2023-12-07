#include "page_deposit.h"
#include "ui_page_deposit.h"

namespace ns_page
{
    Deposit::Deposit(QWidget *parent)
        : CashierBase(parent)
        , m_ui(new Ui::Deposit)
    {
        m_ui->setupUi(this);
    }

    Deposit::~Deposit()
    {
        delete m_ui;
    }

    void Deposit::init()
    {
        connect(m_ui->bttn_amount_10,  &QPushButton::clicked, this, [this]{m_ui->input_amount->setText("10.00");});
        connect(m_ui->bttn_amount_50,  &QPushButton::clicked, this, [this]{m_ui->input_amount->setText("50.00");});
        connect(m_ui->bttn_amount_100, &QPushButton::clicked, this, [this]{m_ui->input_amount->setText("100.00");});
        connect(m_ui->bttn_amount_200, &QPushButton::clicked, this, [this]{m_ui->input_amount->setText("200.00");});
        connect(m_ui->bttn_amount_500, &QPushButton::clicked, this, [this]{m_ui->input_amount->setText("500.00");});

        int idx = 0; // default empty page
        m_ui->bttnGrp_paymentSystems->setId(m_ui->bttn_visa, ++idx);
        m_ui->bttnGrp_paymentSystems->setId(m_ui->bttn_mastercard, ++idx);
        m_ui->bttnGrp_paymentSystems->setId(m_ui->bttn_skrill, ++idx);
        m_ui->bttnGrp_paymentSystems->setId(m_ui->bttn_neteller, ++idx);
        m_ui->bttnGrp_paymentSystems->setId(m_ui->bttn_paysafecard, ++idx);
        m_ui->bttnGrp_paymentSystems->setId(m_ui->bttn_entropay, ++idx);
        m_ui->bttnGrp_paymentSystems->setId(m_ui->bttn_webmoney, ++idx);
        m_ui->bttnGrp_paymentSystems->setId(m_ui->bttn_bitcoin, ++idx);
        m_ui->bttnGrp_paymentSystems->setId(m_ui->bttn_bankOperation, ++idx);

        connect(m_ui->bttnGrp_paymentSystems, (void(QButtonGroup::*)(QAbstractButton* bttn))&QButtonGroup::buttonClicked, this
        , [this](QAbstractButton* bttn) {
            m_ui->title_depositType->setString(bttn->property("displayName").toString(), 0);
            m_ui->stackOfPaymentForms->setCurrentIndex(m_ui->bttnGrp_paymentSystems->id(bttn));
        });
    }

} //namespace ns_page

