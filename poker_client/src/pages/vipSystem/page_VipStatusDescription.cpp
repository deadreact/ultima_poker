#include <precompiled.h>
#include "page_VipStatusDescription.h"
#include "ui_page_vipStatusDescription.h"
#include <QPainter>
#include <QBitmap>
#include <basewidgets/util.h>
#include <qabstractbutton.h>
#include <QGraphicsEffect>
#include <api_models/model_vip_system_info.hpp>
#include <models/model_RakebackHistory.h>
#include <core/money.h>
#include <PokerApplication.h>
#include <styles/shadowedtextstyle.h>
#include <delegates/delegate_customcolumn.h>

#ifndef Q_MOC_RUN
    #include <rpc/network_client.h>
    #include <rpc/rpc_lobby.h>
#endif // !Q_MOC_RUN

// =================== VipSystemToolBoxStyle ===================

namespace ns_page
{
    VipStatusDescription::VipStatusDescription(QWidget *parent)
        : QWidget(parent)
        , m_ui(new Ui::VipStatusDescription)
        , m_sourceModel(new ns_model::vip_system_info)
        , m_model(new ns_model::RakebackHistory)
    {
        m_ui->setupUi(this);

        m_model->setSourceModel(m_sourceModel.data());
        m_ui->tableView->setModel(m_model.data());

    }



    VipStatusDescription::~VipStatusDescription()
    {
        delete m_ui;
    }


    void VipStatusDescription::init()
    {
        rpc_lobby *rpc = netClient().rpc<rpc_lobby>();

        rpc->get_vip_system_info([this](reply_lobby::e_lobby status, const gbp::ns_api::ns_lobby::vip_system_info& info){
            if (ns_helpers::isOk(status)) {
//                std::cout << "vip status info:\n" << info << std::endl;
                m_sourceModel->resetData(new gbp::ns_api::ns_lobby::vip_system_info(info));
            }
        });

        m_model->refresh();
        m_ui->progress_section_info->setAttribute(Qt::WA_TransparentForMouseEvents);
        m_ui->calendar_section_info->setAttribute(Qt::WA_TransparentForMouseEvents);

        connect(m_ui->progress_rakeback, &QProgressBar::valueChanged, this, [this](int /*value*/){
            int persents = 100. * (qreal(m_ui->progress_rakeback->value()) / (m_ui->progress_rakeback->maximum() - m_ui->progress_rakeback->minimum()));
            m_ui->msg_rakeback_current->setString(QString::number(persents), 0);
            m_ui->value_rakeback_current->setText(Money(m_ui->progress_rakeback->value()).toString());
            m_ui->msg_progress->setString(Money(m_ui->progress_rakeback->maximum() - m_ui->progress_rakeback->value()).toString(), 0);
        });

        m_ui->msg_progress->setString(Money(m_ui->progress_rakeback->maximum() - m_ui->progress_rakeback->value()).toString(), 0);

        updateFilterBttnTexts();

        initShadows();

//#ifdef DEBUG_VIPSYSTEM_WIDGETS
//        m_ui->dateEdit_1->setMinimumDate(QDate::currentDate().addDays(-6));
//        m_ui->dateEdit_1->setMaximumDate(QDate::currentDate());
//        updateCalendarPage(m_ui->dateEdit_1->date());
//        connect(m_ui->dateEdit_1, &QDateEdit::dateChanged, this, &VipStatusDescription::updateCalendarPage);

//        connect(m_ui->debug_input_rakeback_max_bills, (void(QSpinBox::*)(int))&QSpinBox::valueChanged, this, [this](int bills) {
//            m_ui->progress_rakeback->setMaximum(100*bills + m_ui->debug_input_rakeback_max_coins->value());
//            m_ui->debug_input_rakeback_current_bills->setMaximum(bills);
//            emit m_ui->progress_rakeback->valueChanged(m_ui->progress_rakeback->value());
//        });
//        connect(m_ui->debug_input_rakeback_max_coins, (void(QSpinBox::*)(int))&QSpinBox::valueChanged, this, [this](int coins) {
//            m_ui->progress_rakeback->setMaximum(coins + 100*m_ui->debug_input_rakeback_max_bills->value());
//            emit m_ui->progress_rakeback->valueChanged(m_ui->progress_rakeback->value());
//        });
//        connect(m_ui->debug_input_rakeback_current_bills, (void(QSpinBox::*)(int))&QSpinBox::valueChanged, this, [this](int bills) {
//            m_ui->progress_rakeback->setValue(100*bills + m_ui->debug_input_rakeback_current_coins->value());
//            m_ui->debug_input_rakeback_max_bills->setMinimum(bills);
//        });
//        connect(m_ui->debug_input_rakeback_current_coins, (void(QSpinBox::*)(int))&QSpinBox::valueChanged, this, [this](int coins) {
//            m_ui->progress_rakeback->setValue(coins + 100*m_ui->debug_input_rakeback_current_bills->value());
//        });
//#else
//        m_ui->dateEdit_1->hide();
////        m_ui->dateEdit_1->deleteLater();
//        m_ui->debug_input_rakeback_container->hide();
////        m_ui->debug_input_rakeback_container->deleteLater();

//#endif //DEBUG_VIPSYSTEM_WIDGETS

        updateCalendarPage(QDate::currentDate().addDays(1-QDate::currentDate().dayOfWeek()));
    }

    void VipStatusDescription::updateCalendarPage(const QDate &firstDayDate)
    {
        m_ui->day_1->setDate(firstDayDate);
        QDate nextPayment = firstDayDate.addDays(7);
        int daysLeft = QDate::currentDate().daysTo(nextPayment);

        int persents = 100. * (qreal(m_ui->progress_rakeback->value()) / (m_ui->progress_rakeback->maximum() - m_ui->progress_rakeback->minimum()));
        m_ui->msg_rakeback_current->setString(QString::number(persents), 0);
        m_ui->value_rakeback_current->setText(Money(m_ui->progress_rakeback->value()).toString());
        m_ui->value_rakeback_daysLeft->setNum(daysLeft);
        m_ui->value_next_payment->setText(locale().dayName(nextPayment.dayOfWeek()) + ", " + nextPayment.toString(locale().dateFormat(QLocale::ShortFormat)/*"dd.MM.yyyy"*/));

        // from progress page
        m_ui->msg_progress->setString(QString::number(nextPayment.day()) + " " + locale().monthName(nextPayment.month(), QLocale::LongFormat), 1);
    }

    void VipStatusDescription::onLanguageChanged()
    {
        m_ui->retranslateUi(this);
        QDate nextPayment = m_ui->day_1->date().addDays(7);
        m_ui->msg_progress->setString(QString::number(nextPayment.day()) + " " + locale().monthName(nextPayment.month(), QLocale::LongFormat), 1);

        updateFilterBttnTexts();
    }

    void VipStatusDescription::updateFilterBttnTexts()
    {
        QString trString = tr("@days_%0");
        m_ui->bttn_filterDays_0->setText(trString.arg(7));
        m_ui->bttn_filterDays_1->setText(trString.arg(14));
        m_ui->bttn_filterDays_2->setText(trString.arg(30));
        m_ui->bttn_filterDays_3->setText(trString.arg(60));
    }

    void VipStatusDescription::changeEvent(QEvent *e)
    {
        if (e && e->type() == QEvent::LanguageChange) {
            onLanguageChanged();
        }
        QWidget::changeEvent(e);
    }

    void VipStatusDescription::initShadows()
    {
        QList<QLabel*> lst = findChildren<QLabel*>(QRegularExpression("^(msg|value|title)_\\w+$"));
        for (QLabel* label: lst) {
            setTextDropShadowStyle(label, QPoint(0, -1), QColor(25, 25, 25, 125));
        }
        QList<QAbstractButton*> bttns = findChildren<QAbstractButton*>(QRegularExpression("^(opt|bttn)_\\w+$"));
        for (QAbstractButton* bttn: bttns) {
            setTextDropShadowStyle(bttn, QPoint(0, -1), QColor(25, 25, 25, 125));
        }

        setTextDropShadowStyle(m_ui->current_status_name, QPoint(0, 1), QColor(255, 255, 255, 255*0.4), E::Blending::Overlay);
        setTextDropShadowStyle(m_ui->current_status, QPoint(0, 1), QColor(255, 255, 255, 255*0.4), E::Blending::Overlay);
        setTextDropShadowStyle(m_ui->next_status_name, QPoint(0, 1), QColor(255, 255, 255, 255*0.4), E::Blending::Overlay);
        setTextDropShadowStyle(m_ui->next_status, QPoint(0, 1), QColor(255, 255, 255, 255*0.4), E::Blending::Overlay);


        setTextDropShadowStyle(m_ui->tableView->horizontalHeader(), QPoint(0, -1), QColor(255, 255, 255, 225));
    }

} //namespace ns_page

