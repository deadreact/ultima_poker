#include <precompiled.h>
#include "dialog_TournamentBuyIn.h"
#include "ui_dialog_TournamentBuyIn.h"

#include <styles/shadowedtextstyle.h>
#include <core/money.h>
#include <dialogs/dialog_okmessage.h>
#include <PokerApplication.h>

#ifndef Q_MOC_RUN
    #include <rpc/rpc_common.h>
    #include <rpc/table/rpc_table.h>
#endif // !Q_MOC_RUN

static const QString txt_link = "<a href =\"%1\"><span style=\" text-decoration: underline; color: rgba(0, 204, 244, @alpha);\">%0</span></a>";

namespace ns_dialog
{
    TournamentBuyIn::TournamentBuyIn(const GameId &gameID, QWidget *parent)
        : QDialog(parent)
        , m_ui(new Ui::TournamentBuyIn)
        , m_gameID(gameID)
        , m_checkedPayment(Unchecked)
        , m_callback{ns_callback::on_register()}
        , m_callbackValid(false)
    {
        m_ui->setupUi(this);

        setProperty("processEventsTimerId", startTimer(1000));
    }

    TournamentBuyIn::TournamentBuyIn(const GameId &gameID, ns_callback::on_register callback, QWidget *parent)
        : QDialog(parent)
        , m_ui(new Ui::TournamentBuyIn)
        , m_gameID(gameID)
        , m_checkedPayment(Unchecked)
        , m_callback{std::move(callback)}
        , m_callbackValid(true)
    {
        m_ui->setupUi(this);

        setProperty("processEventsTimerId", startTimer(1000));
    }

    TournamentBuyIn::~TournamentBuyIn() {
        delete m_ui;
    }

    void TournamentBuyIn::initWithDescriptor(const ns_descriptor::TournamentBuyIn *desc)
    {
        m_ui->tableName->setText(desc->tableName);
        m_ui->title->setText(desc->title);

        m_ui->money_got->setText(Money(desc->money).toString(Money::DisplayType::Full));
        m_ui->widget_playersMoney->setVisible(desc->money >= 0);
        m_ui->gamesQuantity->setNum(desc->gamesQuantity);
        m_ui->games_quantity->setVisible(desc->gamesQuantity >= 0);

        m_ui->w->setVisible(desc->price > 0);
        m_ui->msg_freerollTickets->setVisible(desc->price <= 0 && desc->tickets >= 0);
        if (desc->price > 0)
        {
            m_ui->msg_price->setText(Money(desc->price).toString(Money::DisplayType::Full | Money::DisplayType::NoCoins));
            m_ui->msg_ticket->setText(tr("@tickets_%0").arg(desc->tickets));

            m_ui->msg_price->setDisabled(desc->price > desc->money);
            m_ui->msg_ticket->setDisabled(desc->tickets == 0);
            m_ui->bttn_apply->setDisabled(m_checkedPayment == Unchecked);
            m_ui->switcher->setValue(Unchecked);
        }
        else
        {
            m_ui->msg_freerollTickets->setDisabled(desc->tickets == 0);
            m_ui->msg_freerollTickets->setText(tr("@tickets_%0").arg(desc->tickets));
            m_ui->bttn_apply->setEnabled(desc->tickets < 0 || m_ui->msg_freerollTickets->isChecked());
        }
        m_ui->controls->setVisible(desc->price > 0 || desc->tickets >= 0);

        bool allDisabled = (desc->price > 0 && desc->price > desc->money) && desc->tickets == 0;

        m_ui->switcher->setDisabled(allDisabled);
        m_ui->msg_notEnoughMoney->setVisible(allDisabled);
        m_ui->stackOfButtons->setCurrentWidget(allDisabled ? m_ui->page_bttnDeposit : m_ui->page_bttnApply);

        QString translatedString = m_ui->lnk_autorebuySettings->property("translatedString").toString();
        m_ui->lnk_autorebuySettings->setText(txt_link.arg(translatedString));
        m_ui->frame_rebuy->setVisible(desc->rebuy);


        setTextDropShadowStyle(m_ui->title          , QPoint(0, 1));
        setTextDropShadowStyle(m_ui->tableName      , QPoint(0, 1));
        setTextDropShadowStyle(m_ui->bttn_cancel    , QPoint(0, 1));
        setTextDropShadowStyle(m_ui->bttn_deposit   , QPoint(0, 1));
        setTextDropShadowStyle(m_ui->bttn_apply     , QPoint(0, 1));

        setTextDropShadowStyle(m_ui->msg_gotmoney       , QPoint(0, -1), QColor(0x191919));
        setTextDropShadowStyle(m_ui->msg_gamesQuantity  , QPoint(0, -1), QColor(0x191919));
        setTextDropShadowStyle(m_ui->msg_choosePayment  , QPoint(0, -1), QColor(0x191919));
        setTextDropShadowStyle(m_ui->money_got          , QPoint(0, -1), QColor(0x191919));
        setTextDropShadowStyle(m_ui->gamesQuantity      , QPoint(0, -1), QColor(0x191919));

        adjustSize();
        setFixedSize(size());
    }

    void TournamentBuyIn::onSliderMoved(int val) {
        if (m_checkedPayment == val) {
            return;
        }
        switch (val) {
            case Cash:
            {
                if (m_ui->msg_price->isEnabled()) {
                    m_checkedPayment = Cash;
                    m_ui->msg_price->setChecked(true);
                } else {
                    m_ui->switcher->setValue(Unchecked);
                }
            } break;
            case Ticket:
            {
                if (m_ui->msg_ticket->isEnabled()) {
                    m_checkedPayment = Ticket;
                    m_ui->msg_ticket->setChecked(true);
                } else {
                    m_ui->switcher->setValue(Unchecked);
                }
            } break;
            default:
            {
                m_ui->msg_price->setCheckState(Qt::PartiallyChecked);
                m_ui->msg_ticket->setCheckState(Qt::PartiallyChecked);
                m_checkedPayment = Unchecked;
            } break;
        }
        m_ui->bttn_apply->setDisabled(m_checkedPayment == Unchecked);
    }

    void TournamentBuyIn::accept()
    {
        QDialog::accept();
        rpc_table* rpc = rpc_object<rpc_table>();
        rpc->tournament_register(m_gameID.m_tournamentID, m_checkedPayment == Ticket, m_callback);
    }

    void TournamentBuyIn::timerEvent(QTimerEvent *e)
    {
        //qDebug() << "TournamentBuyIn::timerEvent(" << e->timerId() << ")";
        bool result = false;
        if (property("processEventsTimerId").toInt(&result) == e->timerId() && result) {
            //qDebug() << "process events....";
            QApplication::processEvents();
        }
        QDialog::timerEvent(e);
    }

} //namespace ns_dialog



