#include <precompiled.h>
#include "panel_SitNGoDescription.h"
#include "ui_panel_SitNGoDescription.h"
#include "utils/Utils.h"
#include <qmetaobject.h>
#include "db/Database.h"
#include "core/money.h"
#include <styles/shadowedtextstyle.h>
#include <api_models/model_tournament_stat_t.hpp>
#include <models/model_PlayersTable.h>

#ifndef Q_MOC_RUN
    #include <rpc/request_statistic.h>
    #include <rpc/reply_statistic.h>
#endif // !Q_MOC_RUN


#include <dialogs/buyin/dialog_TournamentBuyIn.h>
#include <descriptors/descriptor_SitNGoBuyIn.h>
#include <db/modeldatabase.h>
#include <qsortfilterproxymodel.h>

Q_DECLARE_METATYPE(games_stat_t::second_type::value_type)

namespace
{
    using e_tournament_status = gbp::ns_api::ns_tournament_types::e_tournament_status;
    using e_chips_rebuy       = gbp::ns_api::ns_tournament_types::e_chips_rebuy;
    using e_win_result        = gbp::ns_api::ns_tournament_types::e_win_result;
    using e_win_prize         = gbp::ns_api::ns_tournament_types::e_win_prize;
    using e_money_in          = gbp::ns_api::ns_tournament_types::e_money_in;
    using e_registration      = gbp::ns_api::ns_tournament_types::e_registration;
    using e_tourn_button_type = gbp::ns_api::ns_statistic::e_tourn_button_type;

}

SitNGoDescription::SitNGoDescription(QWidget *parent)
    : Description(parent)
    , m_ui(new Ui::SitNGoDescription)
    , m_model(new QSortFilterProxyModel)
{
    m_ui->setupUi(this);
}

SitNGoDescription::~SitNGoDescription()
{
    delete m_ui;
}

QTableView *SitNGoDescription::playersList() const {
    return m_ui->tournamentPlayersList;
}

bool SitNGoDescription::isPlayersFetched() const {
    return m_ui->bttn_fetchPlayers->isChecked();
}

void SitNGoDescription::onCurrentIdChanged()
{
    updateLeft();
    updateCentral();
    updatePlayers();
}

void SitNGoDescription::init()
{
    if (sourceModel()->rowCount() > 0) {
        setCurrentId(sourceModel()->idAt(0));
    }
    Description::init();
}

QModelIndex SitNGoDescription::currentIndex() const {
    return sourceModel()->indexOf(currentId());
}

void SitNGoDescription::updateLeft()
{
    QVariant data = rootData();
    if (data.isValid())
    {
        auto val = data.value<games_stat_t::second_type::value_type>();

        m_ui->value_game->setText(ns_util::toString(val._game_limit) + " " + ns_util::toString(val._game_type));
        m_ui->value_speed->setText(ns_util::toString(val._game_speed));
        m_ui->value_table->setText(tr("@max_%0").arg(val._seats_count));
        m_ui->value_start_stack->setText(QString("<html><head/><body><p><img src=\":/textures/chip.png\"/><span style=\" font-size:12px;\"> %0 </span></p></body></html>").arg(val._start_stack));

        bool qualifier = val._types & gbp::ns_api::ns_tournament_types::wr_qualifier;
        m_ui->qualifier->setVisible(qualifier);

        bool bounty      = val._types & gbp::ns_api::ns_tournament_types::cr_bounty;
        bool progressive = val._types & gbp::ns_api::ns_tournament_types::cr_progressive;

        m_ui->knockout->setVisible(bounty || progressive);

        if (bounty) {
            m_ui->value_knockout_reward->setText(Money(val._bounty).toString());
        }
        if (progressive) {
            m_ui->value_knockout_reward->setText(Money(val._player_bounty_value).toString());
        }

    }
}

void SitNGoDescription::updateCentral()
{
    QVariant data = rootData();

    m_ui->bttn_tolobby->setEnabled(data.isValid());
    m_ui->bttn_showPlayersList->setEnabled(data.isValid());
    if (data.isValid())
    {
        auto val = data.value<games_stat_t::second_type::value_type>();
        m_ui->msg_status->setCurrentMsgIndex(underlying_cast(val._status) - 1);

        QStringList lst;

        lst << Money(val._buyin).toString() << ns_util::toString(val._game_limit) << ns_util::toString(val._game_type);
        lst << QString::number(val._sit_n_go_users_count) << tr("@players") << ns_util::toString(val._game_speed);
        m_ui->title->setStrings(lst);

        m_ui->value_fond->setText(Money(val._common_prize).toString());
        m_ui->value_seats->setNum(val._prize_places_count);
        m_ui->value_players_registered->setText(QString("%0/%1").arg(val._players_registered_count).arg(val._sit_n_go_users_count));

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
}

void SitNGoDescription::updatePlayers()
{
    QVariant data = rootData();

    if (data.isValid())
    {
        auto val = data.value<games_stat_t::second_type::value_type>();
        m_ui->tournamentPlayersList->setColumnHidden(3, !(val._types & e_chips_rebuy::cr_bounty));
        m_ui->tournamentPlayersList->setColumnHidden(4, !(val._types & e_chips_rebuy::cr_progressive));
        if (QAbstractProxyModel* proxyModel = qobject_cast<QAbstractProxyModel*>(m_ui->tournamentPlayersList->model()))
        {
            if (ns_model::players_list* plModel = qobject_cast<ns_model::players_list*>(proxyModel->sourceModel())) {
                plModel->setCurrentId(val._id);
            }
        }
    }
}

void SitNGoDescription::updateTexts()
{
    m_ui->title->setString(tr("@players"), 4);
}

void SitNGoDescription::initLeft()
{
    m_ui->value_game->clear();
    m_ui->value_speed->clear();
    m_ui->value_table->clear();
    m_ui->value_start_stack->clear();
    m_ui->value_blinds_growing->clear();
    m_ui->value_qualifier->clear();
    m_ui->value_knockout_reward->clear();

    m_ui->qualifier->hide();
    m_ui->knockout->hide();

    connect(m_ui->bttn_showPlayersList, &QPushButton::toggled, this, [this](bool checked) {
        m_ui->left_info->setFixedWidth(checked ? 660 : 319);
    });
}

void SitNGoDescription::initCentral()
{
    bool isRootDataValid = rootData().isValid();
    m_ui->stackOfBttn->hide();
    m_ui->bttn_tolobby->setEnabled(isRootDataValid);
    m_ui->bttn_showPlayersList->setChecked(false);
    m_ui->bttn_showPlayersList->setEnabled(isRootDataValid);
    m_ui->value_fond->clear();
    m_ui->value_seats->clear();
    m_ui->value_players_registered->clear();

    connect(m_ui->bttn_register, &QPushButton::clicked, this, [this] { action(action_get_info)->trigger();} );
    connect(m_ui->bttn_unregister, &QPushButton::clicked, this, [this] { action(action_unregister)->trigger();} );
    connect(m_ui->bttn_backToGame, &QPushButton::clicked, this, [this] { action(action_back_to_game)->trigger();} );
}

void SitNGoDescription::initPlayers()
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

void SitNGoDescription::changeEvent(QEvent *e)
{
    if (e != nullptr && e->type() == QEvent::LanguageChange) {
        m_ui->retranslateUi(this);
        updateTexts();
    }
    Description::changeEvent(e);
}

ns_model::tournament_stat_t *SitNGoDescription::sourceModel() const {
    return modelsDB().getTournamentStatistic();
}
