#include <precompiled.h>
#include "dialog_CashBuyIn.h"
#include "ui_dialog_CashBuyIn.h"
#include <styles/shadowedtextstyle.h>
#include <QTime>

#ifndef Q_MOC_RUN
    #include <rpc/network_client.h>
    #include <rpc/rpc_common.h>
    #include <rpc/table/rpc_table.h>
#endif // !Q_MOC_RUN

#include <api_models/api_models.hpp>

static const QString txt_link = "<a href =\"%1\"><span style=\" text-decoration: underline; color: rgba(0, 204, 244, @alpha);\">%0</span></a>";

namespace ns_dialog {
    CashBuyIn::CashBuyIn(gbp_i64 tableId, QWidget *parent, Qt::WindowFlags flags)
        : InitableDialog(parent, flags)
        , m_ui(new Ui::BuyIn)
        , m_tableId(tableId)
        , m_minMoney(0)
        , m_maxMoney(5000)
        , m_currentMoney(m_maxMoney)
        , m_playersMoney(0)
        , m_isEnoughMoney(false)
        , m_isRatMode(false)
        , m_isSitOnTable(false)
        , m_initializedTime()
    {
       m_ui->setupUi(this);

       connect(m_ui->money_slider, &MoneySlider::valueChanged, this, [this](int value){
           m_ui->bttn_apply->setText(tr("@bttn_buyin_%0").arg(Money::fromCoins(value).toString()));
       });
//       setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    }

    CashBuyIn::~CashBuyIn()
    {
        //qDebug() << "~BuyIn()";
        delete m_ui;
    }



} // namespace ns_dialog


void ns_dialog::CashBuyIn::init()
{
    if (isInitialized())
        return;

//    m_ui->money_got->setText(m_playersMoney.toString());

     connect(m_ui->money_slider, &MoneySlider::valueChanged, this, [this](int value){
        Money m(value);
        if (m != m_currentMoney && m_minMoney <= m && m <= m_maxMoney) {
            m_ui->money_buyin->setMoney(m);
            m_currentMoney = m;
            emit moneyChanged(m);
        }
    });

    connect(m_ui->money_buyin, (void(QDoubleSpinBox::*)(double))&MoneySpinBox::valueChanged, this, [this](double value){
        Money m = Money::fromDouble(value);
        if (m != m_currentMoney && m_minMoney <= m && m <= m_maxMoney) {
            m_ui->money_slider->setMoney(m);
            m_currentMoney = m;
            emit moneyChanged(m);
        }
    });


    QString translatedString = m_ui->lnk_autorebuySettings->property("translatedString").toString();
    m_ui->lnk_autorebuySettings->setText(txt_link.arg(translatedString));

    setTextDropShadowStyle(m_ui->bttn_apply, QPoint(0, 1));
    setTextDropShadowStyle(m_ui->bttn_deposit, QPoint(0, 1));
    setTextDropShadowStyle(m_ui->min_ammount, QPoint(0, 1));
    setTextDropShadowStyle(m_ui->max_ammount, QPoint(0, 1));
    setTextDropShadowStyle(m_ui->tableName, QPoint(0, 1));
    setTextDropShadowStyle(m_ui->title, QPoint(0, 1));

    setTextDropShadowStyle(m_ui->msg_choosesum, QPoint(0, -1), 0x191919);
    setTextDropShadowStyle(m_ui->msg_gotmoney, QPoint(0, -1), 0x191919);
    setTextDropShadowStyle(m_ui->bttn_min, QPoint(0, -1), 0x191919);
    setTextDropShadowStyle(m_ui->bttn_max, QPoint(0, -1), 0x191919);
    setTextDropShadowStyle(m_ui->money_got, QPoint(0, -1), 0x191919);
    setTextDropShadowStyle(m_ui->check_autorebuy, QPoint(0, -1), 0x191919);

    updateInterface();

    m_ui->controls->setEnabled(isEnoughMoney());
    m_ui->msg_notEnoughMoney->setHidden(isEnoughMoney());
    m_ui->bttn_apply->setEnabled(isEnoughMoney());
    m_ui->frame_rebuy->setEnabled(isEnoughMoney());
    m_ui->msg_ratMode->setVisible(m_isRatMode);
//    QRect rc = m_ui->msg_ratMode->fontMetrics().tightBoundingRect(m_ui->msg_ratMode->text());
//    m_ui->msg_ratMode->setFixedHeight(m_ui->msg_ratMode->fontMetrics().height() * 3);

    adjustSize();
    setFixedHeight(height());

    m_initializedTime = QTime::currentTime();
    onInitialized();
}

void ns_dialog::CashBuyIn::initWithDescriptor(const ns_descriptor::CashBuyIn *desc)
{
    m_ui->tableName->setText(desc->tableName);
    m_ui->title->setText(desc->title);

    m_minMoney     = desc->range.first;
    m_maxMoney     = desc->range.second;
    m_playersMoney = desc->playersMoney;
    m_currentMoney = m_maxMoney;
    m_isSitOnTable = desc->playerSitOnTable;

    setRatMode(desc->ratTime > 0, desc->ratTime);

    init();

}

std::pair<Money, Money> ns_dialog::CashBuyIn::range() const
{
    return std::make_pair(m_minMoney, m_maxMoney);
}

Money ns_dialog::CashBuyIn::minimum() const
{
    return m_minMoney;
}

Money ns_dialog::CashBuyIn::maximum() const
{
    return m_maxMoney;
}

bool ns_dialog::CashBuyIn::isEnoughMoney() const
{
    return m_isEnoughMoney;
}

bool ns_dialog::CashBuyIn::autorebuyChecked() const {
    return m_ui->check_autorebuy->isChecked();
}

void ns_dialog::CashBuyIn::setRatMode(bool ratMode, gbp_i64 timestamp)
{
    if (m_isRatMode != ratMode) {
        m_ui->msg_ratMode->setVisible(ratMode);
        m_isRatMode = ratMode;
        if (ratMode) {
            gbp_i64 mins = QDateTime::fromTime_t(timestamp).secsTo(QDateTime::currentDateTime()) / 60;

            QStringList lst;
            lst << QString::number(mins) << minimum().toString() << QString::number(120 - mins);
            m_ui->msg_ratMode->setStrings(lst);
//            m_ui->msg_ratMode->setText(tr("@msg_rat_mode_%0_%1_%2_%1").arg(mins).arg(minimum().toString()).arg(120 - mins));
        }
//        setFixedHeight(height() + ((ratMode ? 1 : -1) * 40));
    }
}

Money ns_dialog::CashBuyIn::money() const
{
    return m_currentMoney;
}

Money ns_dialog::CashBuyIn::playersMoney() const
{
    return m_playersMoney;
}

void ns_dialog::CashBuyIn::setRange(Money min, Money max)
{
    if (m_minMoney == min && m_maxMoney == max) {
        return;
    }
    m_minMoney = min;
    m_maxMoney = max;
    updateInterface();
}

void ns_dialog::CashBuyIn::setMoney(Money m)
{
    if (m_currentMoney == m)
        return;

    m_currentMoney = m;
    updateInterface();
    emit moneyChanged(m_currentMoney);
}

void ns_dialog::CashBuyIn::setPlayersMoney(Money playersMoney)
{
    if (m_playersMoney == playersMoney) {
        return;
    }

    m_playersMoney = playersMoney;
    updateInterface();
}

void ns_dialog::CashBuyIn::setToMinimum()
{
    setMoney(m_minMoney);
}

void ns_dialog::CashBuyIn::setToMaximum()
{
    setMoney(qMin(m_maxMoney, m_playersMoney));
}



void ns_dialog::CashBuyIn::updateInterface()
{
    Money actualMax = qMin(maximum(), playersMoney());
    m_currentMoney = qBound(minimum(), m_currentMoney, actualMax);

    m_ui->money_got->setText(playersMoney().toString());
    m_ui->money_buyin->setRange(minimum(), actualMax);
    m_ui->money_slider->setRange(minimum(), actualMax);
    m_ui->money_buyin->setMoney(m_currentMoney);
    m_ui->money_slider->setMoney(m_currentMoney);
    m_ui->min_ammount->setText(minimum().toString());
    m_ui->max_ammount->setText(actualMax.toString());
    m_ui->bttn_apply->setText(tr("@bttn_buyin_%0").arg(m_currentMoney.toString()));

    if ((actualMax >= minimum()) != isEnoughMoney()) {
        m_isEnoughMoney = !m_isEnoughMoney;
        emit enoughMoneyChanged(m_isEnoughMoney);
    }
}

void ns_dialog::CashBuyIn::changeEvent(QEvent *e)
{
    if (e != nullptr && e->type() == QEvent::LanguageChange)
    {
        m_ui->retranslateUi(this);
        QString trString = m_ui->lnk_autorebuySettings->property("translatedString").toString();

        if (!trString.isNull()) {
            m_ui->lnk_autorebuySettings->setText(txt_link.arg(trString).replace("@alpha", isEnoughMoney() ? "255" : "100"));
        }
        m_ui->money_got->setText(m_playersMoney.toString());
    }
}

void ns_dialog::CashBuyIn::accept()
{
    rpc_table* rpc = rpc_object<rpc_table>();
    auto callback = [](ns_table::e_status status, gbp_i64 money) {
        if (ns_helpers::isOk(status)) {
            modelsDB().getUserInfo()->moveMoneyToIngameMoney(money);
        }
    };

    rpc->set_autorecharge(m_tableId, autorebuyChecked());

    if (m_isSitOnTable) {
        rpc->add_game_money(m_tableId, money().amount(), callback);
        InitableDialog::accept();
    } else {
        if (m_initializedTime.secsTo(QTime::currentTime()) >= 60) {
            if (m_ui->stackOfPages->currentWidget() == m_ui->page_timeout) {
                reject();
            } else {
                m_ui->stackOfPages->setCurrentWidget(m_ui->page_timeout);
                setFixedHeight(minimumHeight());
            }

        } else {
            rpc->set_sit_money(m_tableId, money().amount(), callback);
            InitableDialog::accept();
        }
    }

}

void ns_dialog::CashBuyIn::reject()
{
    rpc_table* rpc = rpc_object<rpc_table>();
    rpc->sit_on_table_canceled(m_tableId);
    InitableDialog::reject();
}
