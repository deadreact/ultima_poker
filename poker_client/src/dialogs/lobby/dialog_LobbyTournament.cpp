#include <precompiled.h>
#include "dialog_LobbyTournament.h"
#include "ui_dialog_LobbyTournament.h"
#include <tools/item_reference.hpp>
#include <core/money.h>
#include <basewidgets/util.h>
#include <api_models/api_util.hpp>
#include <api_models/model_tournament_stat_t.hpp>
#include <api_models/model_tourn_additional_stat_t.hpp>

#include <api/lobby_stat/types.h>

Q_DECLARE_METATYPE(gbp::ns_api::ns_statistic::tourn_additional_stat_t)

namespace
{
    using e_tournament_status = gbp::ns_api::ns_tournament_types::e_tournament_status;
    using e_tourn_button_type = gbp::ns_api::ns_statistic::e_tourn_button_type;
}

namespace ns_dialog
{
    LobbyTournament::LobbyTournament(gbp_i64 id, QWidget *parent)
        : LobbyBase(id, parent)
        , m_ui(new Ui::LobbyTournament) {
        m_ui->setupUi(this);
    }

    LobbyTournament::~LobbyTournament()
    {
        delete m_ui;
    }

    bool LobbyTournament::initHeader()
    {
        LobbyBase::initHeader();
        updateHeader();
        return true;
    }

    bool LobbyTournament::updateHeader()
    {
        TournamentRef dRef1(modelStatistic()->item(m_tournamentID).ref());
        const auto& data2 = modelAdditionalStatistic()->originData().value<gbp::ns_api::ns_statistic::tourn_additional_stat_t>();

//        ItemRef<ns_statistic::tournament_stat_t>::;
//        dRef1._addon
        updateTexts();
        m_ui->registrationDate->setVisible(dRef1._status == e_tournament_status::wait);
        m_ui->startDate->setVisible(dRef1._status == e_tournament_status::wait || dRef1._status == e_tournament_status::registering);
        m_ui->hadStartedDate->setVisible(dRef1._status == e_tournament_status::finished);
        m_ui->hadFinishedDate->setVisible(dRef1._status == e_tournament_status::finished);
        m_ui->active_time->setVisible(dRef1._status == e_tournament_status::late_registration || dRef1._status == e_tournament_status::started);
        m_ui->timebreak->setVisible(dRef1._status == e_tournament_status::registering);
        m_ui->nextTimebreak->setVisible(dRef1._status == e_tournament_status::late_registration || dRef1._status == e_tournament_status::started);

        QDateTime regDate = QDateTime::fromTime_t(dRef1._registration_start_time);
        QDateTime startDate = QDateTime::fromTime_t(dRef1._start_time);
        QDateTime finishDate = QDateTime::fromTime_t(dRef1._finish_time);
        QString regDateString = regDate.toString("hh:mm EET, dd %0").arg(ns_util::shortMonthNativeName(regDate.date().month()));
        QString startDateString = startDate.toString("hh:mm EET, dd %0").arg(ns_util::shortMonthNativeName(startDate.date().month()));
        QString finishDateString = finishDate.toString("hh:mm EET, dd %0").arg(ns_util::shortMonthNativeName(finishDate.date().month()));
        int durationSec = startDate.secsTo(QDateTime::currentDateTime());
        int durationMinutes = durationSec / 60;

        static const int breakDurationMinutes = 5;
        static const int breakFrequenceMinutes = 55;

        m_ui->value_registrationDate->setText(regDateString);
        m_ui->value_startDate->setText(startDateString);
        m_ui->value_hadStartedDate->setText(startDateString);
        m_ui->value_hadFinishedDate->setText(finishDateString);
        m_ui->value_active_time->setText(durationMinutes < 60 ? tr("@min_%0").arg(durationMinutes) : tr("@hour_%0_min_%1").arg(durationMinutes / 60).arg(durationMinutes % 60));


        for (const auto &blindItem: data2._blinds)
        {
            if (blindItem._level == dRef1._current_level) {
                m_ui->value_level->setText(QString("%0/%1").arg(Money(blindItem._big_blind/2).toString()).arg(Money(blindItem._big_blind).toString()));
            } else if (blindItem._level == dRef1._current_level + 1) {
                m_ui->value_nextLevel->setText(QString("%0/%1").arg(Money(blindItem._big_blind/2).toString()).arg(Money(blindItem._big_blind).toString()));
                break;
            }
        }
        if (durationSec > 0) {
            m_ui->clock->setText(QString("%0%1:%2%3").arg(durationMinutes/600).arg((durationMinutes/60)%10).arg((durationMinutes%60)/10).arg((durationMinutes%60)%10));
        }


        m_ui->value_timebreak->setText(tr("@timebreak_%0_min_every_%1_min").arg(breakDurationMinutes).arg(breakFrequenceMinutes));
        m_ui->value_nextTimebreak->setText(tr("@min_%0").arg(durationMinutes % (breakDurationMinutes + breakFrequenceMinutes)));

        m_ui->msg_status->setCurrentMsgIndex(underlying_cast(dRef1._status) - 1);
        m_ui->title_1->setString(dRef1._title.c_str(), 0);


        m_ui->value_fond->setText(Money(dRef1._common_prize).toString());
        if (dRef1._sit_n_go_users_count == 0) {
            m_ui->value_players_registered->setText(QString("%0").arg(dRef1._players_registered_count));
        } else {
            m_ui->value_players_registered->setText(QString("%0/%1").arg(dRef1._players_registered_count).arg(dRef1._max_players_count));
        }

        m_ui->stackOfBttn->setVisible(dRef1._button_state != e_tourn_button_type::hidden);

        switch (dRef1._button_state) {
        case e_tourn_button_type::register_in_tourn:
        case e_tourn_button_type::late_register_in_tourn:
            m_ui->stackOfBttn->setCurrentWidget(m_ui->page_bttn_register);
            break;
        case e_tourn_button_type::unregister:
            m_ui->stackOfBttn->setCurrentWidget(m_ui->page_bttn_unregister);
            break;
        case e_tourn_button_type::back_to_game:
            m_ui->stackOfBttn->setCurrentWidget(m_ui->page_bttn_backToGame);
            break;
        default:
            break;
        }

        m_ui->myPos->setVisible(data2._current_pos != -1);
        m_ui->value_myPos->setNum(data2._current_pos);

        return true;
    }

    int LobbyTournament::pagesCount() const {
        return m_ui->tabBar_left->count() + m_ui->tabBar_right->count();
    }

    ns_page::LobbyBase *LobbyTournament::page(int index) const
    {
        if (index < 0 || index >= pagesCount()) {
            return nullptr;
        }
        int count1 = m_ui->tabBar_left->count();
        return qobject_cast<ns_page::LobbyBase*>((index >= count1) ? m_ui->tabBar_right->widget(index - count1) : m_ui->tabBar_left->widget(index));
    }

    void LobbyTournament::updateTexts(bool retranslate)
    {
        if (retranslate) {
            m_ui->retranslateUi(this);
        }

        TournamentRef dRef(statistic().ref());

        m_ui->msg_level->setText(tr("@msg_curr_lvl_%0:").arg(dRef._current_level));
        m_ui->msg_nextLevel->setText(tr("@msg_next_lvl_%0:").arg(dRef._current_level+1));

        QStringList lst;
        QString color = "#fafafa";
        { using namespace gbp::ns_api::ns_tournament_types;
            e_tournament_type _types = dRef._types;

            if (_types & e_win_result::wr_main) {
                color = "#00aeef";
            }
            if (_types & e_win_result::wr_qualifier) {
                color = "#00c853";
            }

            if (_types & e_money_in::mi_free_roll) {
                lst << tr("@mi_free_roll");
                 m_ui->title_2->setProperty("isFreeRoll", true);
            } else if (_types & e_win_prize::wp_warranty) {
                lst << QString("%0 Gtd").arg(Money(dRef._common_prize).toString());
            }
            if (_types & e_chips_rebuy::cr_rebuy) {
                lst << tr("@rebuy");
            }
            if (_types & e_chips_rebuy::cr_bounty) {
                lst << tr("@knockout");
            }
            if (_types & e_chips_rebuy::cr_progressive) {
                lst << tr("@progressive");
            }
        }
        lst << (ns_util::toString(dRef._game_limit) + " " + ns_util::toString(dRef._game_type));

        if (dRef._game_speed != gbp::ns_api::ns_game::e_game_speed::normal) {
            lst << ns_util::toString(dRef._game_speed);
        }

        QString pattern = "%0";
        for (int i = 1; i < lst.size(); i++) {
            pattern += QString(", %%0").arg(i);
        }
        m_ui->title_2->setStrings(lst);
        m_ui->title_2->setText(pattern);
        m_ui->title_2->setStyleSheet(QString("QLabel { font: 11px \"Open Sans\"; color: %0; }").arg(color));

    }

} //namespace ns_dialog
