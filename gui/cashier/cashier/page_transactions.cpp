#include "page_transactions.h"
#include "ui_page_transactions.h"

namespace ns_page
{
    Transactions::Transactions(QWidget *parent)
        : CashierBase(parent)
        , m_ui(new Ui::Transactions)
    {
        m_ui->setupUi(this);
    }

    Transactions::~Transactions()
    {
        delete m_ui;
    }


    void Transactions::init()
    {

    }

} //namespace ns_page

