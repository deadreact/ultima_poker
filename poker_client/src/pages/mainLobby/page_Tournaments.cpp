#include <QStandardItemModel>
#include <precompiled.h>
#include <qmenu.h>
#include <qstyleditemdelegate.h>
#include <qtreewidget.h>
#include "page_Tournaments.h"
#include "ui_page_Tournaments.h"
#include <models/model_PlayersListModel.h>
#include <models/model_GamesStatistic.h>
#include <dialogs/lobby/dialog_LobbyTournament.h>

#include <delegates/delegate_GameTableComboBoxItem.h>

#include <models/model_Filters.h>
#include <models/model_tournaments.h>

#include <QColumnView>
#include <qlistview.h>
#include <db/modeldatabase.h>
#include <api_models/model_tournament_stat_t.hpp>

namespace
{
}




namespace ns_page
{


    Tournaments::Tournaments(QWidget *parent)
        : MainLobbyBase(UserType::EFilters::TournFilters_Mask, parent)
        , m_ui(new Ui::Tournaments)
    {
        m_ui->setupUi(this);
    }

    Tournaments::~Tournaments()
    {
        delete m_ui;
    }

    void Tournaments::init()
    {
        MainLobbyBase::init();

        m_ui->descriptionPanel->init();
        connect(m_ui->descriptionPanel, &TournDescription::openLobbyRequested, this, &Tournaments::onItemActivated);

        QStringList txtlst;
        FiltersList filterslst;
        txtlst << tr("@freeroll") << tr("@micro") << tr("@low") << tr("@medium") << tr("@high");
        filterslst << UserType::EFilters::BuyIn_FreeRoll << UserType::EFilters::BuyIn_Micro << UserType::EFilters::BuyIn_Low << UserType::EFilters::BuyIn_Medium << UserType::EFilters::BuyIn_High;
        initFilterBox(m_ui->filter_buyin, txtlst, filterslst);

        initFilterBox(m_ui->filter_game, QStringList() << tr("@holdem") << tr("@omaha"), FiltersList() << UserType::EFilters::Holdem_NoLimit << UserType::EFilters::Omaha_PotLimit);
        initFilterBox(m_ui->filter_speed, QStringList() << tr("@normal") << tr("@turbo"), FiltersList() << UserType::EFilters::Speed_Normal << UserType::EFilters::Speed_Turbo);
        initFilterBox(m_ui->filter_type, QStringList() << tr("@rebuy") << tr("@regular") << tr("@knockout") << tr("@qualifier") << tr("@private")
                                       , FiltersList() << UserType::EFilters::Type_Rebuy << UserType::EFilters::Type_Regular<< UserType::EFilters::Type_Knockout << UserType::EFilters::Type_Qualifier << UserType::EFilters::Type_Private);
        initFilterBox(m_ui->filter_status, QStringList() << tr("@announced") << tr("@registration") << tr("@late_registration") << tr("@started") << tr("@finished")
                                         , FiltersList() << UserType::EFilters::Status_Announced << UserType::EFilters::Status_Registration << UserType::EFilters::Status_LateRegistration << UserType::EFilters::Status_Started<< UserType::EFilters::Status_Finished);

//        m_ui->descriptionPanel->init();
    }
    void Tournaments::initTableView()
    {
        ns_model::tournaments* model = new ns_model::tournaments;
        QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel;
        m_ui->tableView->setModel(proxyModel);

        connect(proxyModel, &QSortFilterProxyModel::modelReset, this, [this]{
            if (!m_ui->tableView->currentIndex().isValid()) {
                m_ui->tableView->selectRow(0);
            }
        });

        proxyModel->setSourceModel(model);
        model->setSourceModel(modelsDB().getTournamentStatistic());

        proxyModel->setSortRole(UserType::SortRole);
    }

    void Tournaments::updateTexts()
    {

    }

    TableView *Tournaments::tableView() const
    {
        return m_ui->tableView;
    }

    void Tournaments::onItemActivated(const QModelIndex &index)
    {
        if (!index.isValid()) {
            return;
        }
        gbp_i64 id = index.data(UserType::RowItemIdRole).value<gbp_i64>();
        auto it = m_openedLobbies.find(id);

        if (it == m_openedLobbies.end())
        {
            ns_dialog::LobbyTournament* lobby = new ns_dialog::LobbyTournament(id);

            connect(lobby, &ns_dialog::LobbyTournament::initialized, lobby, &ns_dialog::LobbyTournament::open);
            connect(lobby, &ns_dialog::LobbyTournament::finished, this, [this, id] {
                m_openedLobbies.remove(id);
            });

            lobby->setAttribute(Qt::WA_DeleteOnClose);
            lobby->setWindowFlags(Qt::Window);
            lobby->init();
            m_openedLobbies.insert(id, lobby);
        }
        else if (it.value()->isInitialized())
        {
            it.value()->showNormal();
            it.value()->raise();
            it.value()->activateWindow();
        }
    }

    void Tournaments::onCurrentChanged(const QModelIndex &index) {
        m_ui->descriptionPanel->setCurrentIndex(index);
        updateTexts();
    }


} // namespace page


void ns_page::Tournaments::changeEvent(QEvent *e)
{
    if (e != nullptr && e->type() == QEvent::LanguageChange) {
        m_ui->retranslateUi(this);
        updateTexts();
    }
    QFrame::changeEvent(e);
}

void ns_page::Tournaments::showEvent(QShowEvent *e)
{
    MainLobbyBase::showEvent(e);
}


void ns_page::Tournaments::updateFilters()
{
    UserType::FilterFlags f = m_ui->filter_game->model()->index(0, 0).data(UserType::FilterRole).value<UserType::FilterFlags>();
    f |= m_ui->filter_buyin->model()->index(0, 0).data(UserType::FilterRole).value<UserType::FilterFlags>();
    f |= m_ui->filter_speed->model()->index(0, 0).data(UserType::FilterRole).value<UserType::FilterFlags>();
    f |= m_ui->filter_type->model()->index(0, 0).data(UserType::FilterRole).value<UserType::FilterFlags>();
    f |= m_ui->filter_status->model()->index(0, 0).data(UserType::FilterRole).value<UserType::FilterFlags>();
    setFilter(f);
}

void ns_page::Tournaments::disableFilters(bool disabled)
{
    if (QSortFilterProxyModel* proxyModel = qobject_cast<QSortFilterProxyModel*>(tableView()->model()))
    {
        m_ui->filters->setDisabled(disabled);
        proxyModel->setFilterRegExp(UserType::toRegExp(disabled ? m_mask : m_filter));
        proxyModel->setFilterRole(UserType::FilterRole);
    }
}


