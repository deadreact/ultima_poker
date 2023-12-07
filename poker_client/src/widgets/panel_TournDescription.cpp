#include <precompiled.h>
#include "panel_TournDescription.h"
#include "ui_panel_TournDescription.h"
#include "utils/Utils.h"
#include <qmetaobject.h>
#include "db/Database.h"
#include "core/money.h"
#include <dialogs/dialog_okmessage.h>
#include <styles/shadowedtextstyle.h>
#include <qsortfilterproxymodel.h>
#include <models/model_PlayersTable.h>
#include <models/model_GamesStatistic.h>
#include <api_models/model_tournament_stat_t.hpp>

#include <pages/lobbyTournament/page_Qualifiers.h>
#include <dialogs/buyin/dialog_TournamentBuyIn.h>
#include <descriptors/descriptor_TournamentBuyIn.h>
#include <iostream>
#include <db/modeldatabase.h>
#include <api_models/model_tournament_stat_t.hpp>
#include <styles/scaledtextstyle.h>

Q_DECLARE_METATYPE(games_stat_t::second_type::value_type)

// QLabel *value_active_time
// QLabel *value_blinds
// QLabel *value_blinds_growing
// QLabel *value_fond
// QLabel *value_hadFinishedDate
// QLabel *value_hadStartedDate
// QLabel *value_knockout_reward
//! QLabel *value_level
// QLabel *value_nextTimebreak
// QLabel *value_players_registered
// QLabel *value_registrationDate
// QLabel *value_seats
// QLabel *value_start_stack
//! QLabel *value_startDate
// QPushButton *value_main_event
// TextWidget *value_timebreak


// _id = 0,
// _seats_count,
// _max_players_count,
// _is_current_user_registered,
// _players_registered_count,
// _sit_n_go_users_count,
// _main_tournament_id,
// _buyin,
// _big_blind,
// _round_duration,
// _start_stack,
// _prize_places_count,
// _common_prize,
// _types,
// _game_type,
// _game_speed,
// _game_limit,
// _title,
// _status,
// _registration_start_time,
//! _start_time,
// _finish_time,
// _button_state,
// _late_registration_duration,
// _qualifiers_ids, //array
// _rebuys,// struct
// _addon, // struct
// _bounty,
// _player_bounty_value,
//! _current_level,
// _users, // array


namespace
{
    using e_tournament_status = gbp::ns_api::ns_tournament_types::e_tournament_status;
    using e_chips_rebuy       = gbp::ns_api::ns_tournament_types::e_chips_rebuy;
    using e_win_result        = gbp::ns_api::ns_tournament_types::e_win_result;
    using e_win_prize         = gbp::ns_api::ns_tournament_types::e_win_prize;
    using e_money_in          = gbp::ns_api::ns_tournament_types::e_money_in;
    using e_registration      = gbp::ns_api::ns_tournament_types::e_registration;
    using e_tourn_button_type = gbp::ns_api::ns_statistic::e_tourn_button_type;

    void setBttnStyle(QAbstractButton* bttn)
    {
        ShadowedTextStyle* shStyle = new ShadowedTextStyle(bttn->style());
        shStyle->setShadowOffset(0, 1);
        shStyle->setShadowColor(25, 25, 25, 255);

        bttn->setStyle(new ScaledTextStyle(12, 14, shStyle));
    }
}


TournDescription::TournDescription(QWidget *parent)
    : Description(parent)
    , m_ui(new Ui::TournDescription)
    , m_model(new ProxyModelType)
{
    m_ui->setupUi(this);
    m_model->setSourceModel(sourceModel());
    m_ui->tournamentPlayersList->setModel(m_model);
}

TournDescription::~TournDescription()
{
    delete m_ui;
}

void TournDescription::init()
{
    if (sourceModel()->rowCount() > 0) {
        setCurrentId(sourceModel()->idAt(0));
    }
    Description::init();
}


QTableView *TournDescription::playersList() const
{
    return m_ui->tournamentPlayersList;
}

bool TournDescription::isPlayersFetched() const
{
    return m_ui->bttn_fetchPlayers->isChecked();
}

QModelIndex TournDescription::currentIndex() const {
    return sourceModel()->indexOf(currentId());
}

void TournDescription::onCurrentIdChanged()
{
    updateLeft();
    updateCentral();
    updatePlayers();
}

void TournDescription::changeEvent(QEvent *e)
{
    if (e != nullptr && e->type() == QEvent::LanguageChange) {
        m_ui->retranslateUi(this);
        updateTexts();
    }
    Description::changeEvent(e);
}

void TournDescription::openCurrentQualifiers()
{
    static const gbp::ns_api::ns_statistic::tourn_additional_stat_t invalid;
    QVariant data = rootData();

    if (data.isValid())
    {
        auto val = data.value<games_stat_t::second_type::value_type>();
        auto it = m_openedQualifiers.find(currentId());
        if (it == m_openedQualifiers.end())
        {
            ns_page::Qualifiers* dialog = new ns_page::Qualifiers(this);
            dialog->setWindowFlags(Qt::Window);
            auto id = val._id;
            dialog->init(id);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            connect(dialog, &ns_page::Qualifiers::destroyed, this, [this, id]{ m_openedQualifiers.remove(id); });
            it = m_openedQualifiers.insert(id, dialog);
        }
        it.value()->showNormal();
        it.value()->raise();
        it.value()->activateWindow();
    }
}


void TournDescription::updateLeft()
{
    QVariant data = rootData();

    if (data.isValid())
    {
        auto val = data.value<games_stat_t::second_type::value_type>();

        m_ui->_registration_start_time->setVisible(val._status == e_tournament_status::wait);
        m_ui->_start_time->setVisible(val._status == e_tournament_status::wait || val._status == e_tournament_status::registering);
        m_ui->hadStartedDate->setVisible(val._status == e_tournament_status::finished);
        m_ui->hadFinishedDate->setVisible(val._status == e_tournament_status::finished);
        m_ui->active_time->setVisible(val._status == e_tournament_status::late_registration || val._status == e_tournament_status::started);
        m_ui->_current_level->setVisible(val._status != e_tournament_status::wait && val._status != e_tournament_status::finished);
        m_ui->blinds->setVisible(val._status != e_tournament_status::wait && val._status != e_tournament_status::finished);
        m_ui->timebreak->setVisible(val._status == e_tournament_status::wait || val._status == e_tournament_status::registering);
        m_ui->nextTimebreak->setVisible(val._status == e_tournament_status::late_registration || val._status == e_tournament_status::started);

        QDateTime regDate = QDateTime::fromTime_t(val._registration_start_time);
        QDateTime startDate = QDateTime::fromTime_t(val._start_time);
        QDateTime finishDate = QDateTime::fromTime_t(val._finish_time);
        QString regDateString = regDate.toString("hh:mm EET, dd %0").arg(ns_util::shortMonthNativeName(regDate.date().month()));
        QString startDateString = startDate.toString("hh:mm EET, dd %0").arg(ns_util::shortMonthNativeName(startDate.date().month()));
        QString finishDateString = finishDate.toString("hh:mm EET, dd %0").arg(ns_util::shortMonthNativeName(finishDate.date().month()));
        int durationMinutes = startDate.secsTo(QDateTime::currentDateTime()) / 60;

        static const int breakDurationMinutes = 5;
        static const int breakFrequenceMinutes = 55;

        m_ui->value_registration_start_time->setText(regDateString);
        m_ui->value_start_time->setText(startDateString);
        m_ui->value_hadStartedDate->setText(startDateString);
        m_ui->value_hadFinishedDate->setText(finishDateString);
        m_ui->value_active_time->setText(durationMinutes < 60 ? tr("@min_%0").arg(durationMinutes) : tr("@hour_%0_min_%1").arg(durationMinutes / 60).arg(durationMinutes % 60));
        m_ui->value_current_level->setText(QString::number(val._current_level));
        m_ui->value_blinds->setText(QString("%0/%1").arg(Money(val._big_blind / 2).toString()).arg(Money(val._big_blind).toString()));
        m_ui->value_timebreak->setString(QString::number(breakDurationMinutes), 0);
        m_ui->value_timebreak->setString(QString::number(breakFrequenceMinutes), 1);
        m_ui->value_nextTimebreak->setText(tr("@min_%0").arg(durationMinutes % (breakDurationMinutes + breakFrequenceMinutes)));

        // ------------------------------------------------------------------------------------------------------------------------------

        m_ui->blinds_growing->setVisible(true);
        m_ui->start_stack->setVisible(true);
        m_ui->knockout_reward->setVisible(val._types & e_chips_rebuy::cr_bounty || val._types & e_chips_rebuy::cr_progressive);

        m_ui->value_blinds_growing->setText(tr("@min_%0").arg(val._round_duration/60));
        m_ui->value_start_stack->setText(QString("<html><head/><body><p><img src=\":/textures/chip.png\"/><span style=\" font-size:12px;\"> %0 </span></p></body></html>").arg(val._start_stack));
        m_ui->value_knockout_reward->setText(QString("%0/%1").arg(Money(val._bounty).toString()).arg(Money(val._player_bounty_value).toString()));

        m_ui->main_event->setVisible(val._types & e_win_result::wr_qualifier);
        m_ui->qualifiersLink->setVisible(val._types & e_win_result::wr_main);
        if (val._types & e_win_result::wr_qualifier) {
            int index = sourceModel()->indexOf(val._main_tournament_id).row();
            QModelIndex modelIndex = sourceModel()->index(index, 4);
            if (modelIndex.isValid()) {
                m_ui->value_main_event->setText(QString("%0").arg(modelIndex.data().toString()));

                gbp_i64 id = val._main_tournament_id;
                connect(m_ui->value_main_event, &QPushButton::clicked, this, [this, id] {
                    emit openLobbyRequested(sourceModel()->indexOf(id));
                });
            }
        }
    }
}

void TournDescription::updateCentral()
{
    QVariant data = rootData();

    m_ui->bttn_tolobby->setEnabled(data.isValid());
    m_ui->bttn_showPlayersList->setEnabled(data.isValid());
    if (data.isValid())
    {
        auto val = data.value<games_stat_t::second_type::value_type>();

        m_ui->msg_status->setCurrentMsgIndex(underlying_cast(val._status) - 1);
        m_ui->title_1->setString(val._title.c_str(), 0);

        QStringList lst;
        QString color = "#fafafa";
        { using namespace gbp::ns_api::ns_tournament_types;
            e_tournament_type _types = val._types;

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
                lst << QString("%0 Gtd").arg(Money(val._common_prize).toString());
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
        lst << (ns_util::toString(val._game_limit) + " " + ns_util::toString(val._game_type));

        if (val._game_speed != gbp::ns_api::ns_game::e_game_speed::normal) {
            lst << ns_util::toString(val._game_speed);
        }

        QString pattern = "%0";
        for (int i = 1; i < lst.size(); i++) {
            pattern += QString(", %%0").arg(i);
        }
        m_ui->title_2->setStrings(lst);
        m_ui->title_2->setText(pattern);
        m_ui->title_2->setStyleSheet(QString("QLabel { font: 11px \"Open Sans\"; color: %0; }").arg(color));

        m_ui->value_fond->setText(Money(val._common_prize).toString());
        m_ui->value_seats->setNum(val._prize_places_count);
        m_ui->value_players_registered->setText(QString("%0/%1").arg(val._players_registered_count).arg(val._max_players_count));
//        std::cout << val._button_state << std::endl;

        updateButtons(val);
    }
}

template <typename ...Args, typename Arg>
auto sum(const Args& ...args, const Arg& arg) -> decltype(arg + sum(args...)) {
    return arg + sum(args...);
}
template <typename Arg1, typename Arg2>
auto sum(const Arg1& arg1, const Arg2& arg2) -> decltype(arg1 + arg2) {
    return arg1 + arg2;
}

template <typename T>
T sum(std::initializer_list<T> values) {
    T res = 0;
    for (const T& v: values) {
        res += v;
    }
    return res;
}

void TournDescription::updatePlayers()
{
    QVariant data = currentIndex().data(UserType::RowDataRole);

    if (data.isValid())
    {
        auto val = data.value<games_stat_t::second_type::value_type>();
        if (QAbstractProxyModel* proxyModel = qobject_cast<QAbstractProxyModel*>(m_ui->tournamentPlayersList->model())) {
            if (ns_model::players_list* plModel = qobject_cast<ns_model::players_list*>(proxyModel->sourceModel())) {
                plModel->setCurrentId(val._id);
            }
        }
        m_ui->tournamentPlayersList->setColumnHidden(3, !(val._types & e_chips_rebuy::cr_bounty));
        m_ui->tournamentPlayersList->setColumnHidden(4, !(val._types & e_chips_rebuy::cr_progressive));
    }
    QHeaderView* header = m_ui->tournamentPlayersList->horizontalHeader();
//    static const int section[] = {30, 124, 24, 80, 80};
    header->setSectionResizeMode(QHeaderView::Stretch);
////    m_ui->tournamentPlayersList->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
//    header->resizeSection(0, section[0]);
//    header->resizeSection(1, section[1]);
//    header->resizeSection(2, section[2]);
//    header->resizeSection(3, section[3]);
//    header->resizeSection(4, section[4]);
////    m_ui->tournamentPlayersList->horizontalHeader()->resizeSection(5, section[5]);
}

void TournDescription::updateTexts()
{
}

void TournDescription::initLeft()
{
    QList<QLabel*> labelList = m_ui->leftPanelPart->findChildren<QLabel*>(QRegExp("^value_\\w+$"));

    for (QLabel* label: labelList)
    {
        label->clear();
        label->parentWidget()->hide();
    }

}

void TournDescription::initCentral()
{
    bool isRootDataValid = rootData().isValid();
    m_ui->stackOfBttn->hide();
    m_ui->bttn_tolobby->setEnabled(isRootDataValid);
    m_ui->bttn_showPlayersList->setChecked(false);
    m_ui->bttn_showPlayersList->setEnabled(isRootDataValid);

    connect(m_ui->bttn_register, &QPushButton::clicked, this, [this] { action(action_get_info)->trigger();} );
    connect(m_ui->bttn_unregister, &QPushButton::clicked, this, [this] { action(action_unregister)->trigger();} );
    connect(m_ui->bttn_backToGame, &QPushButton::clicked, this, [this] { action(action_back_to_game)->trigger();} );
}

void TournDescription::initPlayers()
{
    ns_model::players_list* plModel = modelsDB().getTournamentPlayersList(currentId());

    m_model->setSourceModel(plModel);
    connect(plModel, &ns_model::players_list::modelReset, this, [plModel, this]{
        m_ui->tournamentPlayersList->setRootIndex(m_model->mapFromSource(plModel->root()));
    });
    m_ui->tournamentPlayersList->setModel(m_model);
    m_ui->tournamentPlayersList->setRootIndex(m_model->mapFromSource(plModel->root()));

    updatePlayers();
}

void TournDescription::initTexts()
{
    setBttnStyle(m_ui->bttn_register);
    setBttnStyle(m_ui->bttn_unregister);
    setBttnStyle(m_ui->bttn_backToGame);
    setBttnStyle(m_ui->bttn_tolobby);
}

void TournDescription::updateButtons(const games_stat_t::second_type::value_type &val)
{
    m_ui->stackOfBttn->setVisible(val._button_state != e_tourn_button_type::hidden);

    switch (val._button_state) {
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
}

ns_model::tournament_stat_t *TournDescription::sourceModel() const {
    return modelsDB().getTournamentStatistic();
}
