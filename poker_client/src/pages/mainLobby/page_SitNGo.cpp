#include <precompiled.h>
#include "page_SitNGo.h"
#include "ui_page_SitNGo.h"
#include <qsortfilterproxymodel.h>
#include <models/model_GamesStatistic.h>
#include <models/model_tournaments.h>
#include <dialogs/lobby/dialog_LobbyTournament.h>
#include <db/modeldatabase.h>
#include <api_models/model_tournament_stat_t.hpp>

namespace ns_page
{
    SitNGo::SitNGo(QWidget *parent)
        : MainLobbyBase(UserType::EFilters::SitNGoFilters_Mask, parent)
        , m_ui(new Ui::SitNGo)
    {
        m_ui->setupUi(this);
    }

    SitNGo::~SitNGo()
    {
        delete m_ui;
    }

    void SitNGo::init()
    {
        MainLobbyBase::init();

//        m_ui->descriptionPanel->setRootIndex(model()->mapToSource(m_ui->tableView->currentIndex()));

        m_ui->descriptionPanel->init();

        QStringList txtlst;
        FiltersList filterslst;
        txtlst << tr("@freeroll") << tr("@micro") << tr("@low") << tr("@medium") << tr("@high");
        filterslst << UserType::EFilters::BuyIn_FreeRoll << UserType::EFilters::BuyIn_Micro << UserType::EFilters::BuyIn_Low << UserType::EFilters::BuyIn_Medium << UserType::EFilters::BuyIn_High;
        initFilterBox(m_ui->filter_buyin, txtlst, filterslst);

        initFilterBox(m_ui->filter_game, QStringList() << tr("@holdem") << tr("@omaha"), FiltersList() << UserType::EFilters::Holdem_NoLimit << UserType::EFilters::Omaha_PotLimit);
        initFilterBox(m_ui->filter_speed, QStringList() << tr("@normal") << tr("@turbo"), FiltersList() << UserType::EFilters::Speed_Normal << UserType::EFilters::Speed_Turbo);
        initFilterBox(m_ui->filter_type, QStringList() << tr("@rebuy") << tr("@regular") << tr("@knockout") << tr("@qualifier") << tr("@private")
                                       , FiltersList() << UserType::EFilters::Type_Rebuy << UserType::EFilters::Type_Regular<< UserType::EFilters::Type_Knockout << UserType::EFilters::Type_Qualifier << UserType::EFilters::Type_Private);
        initFilterBox(m_ui->filter_size, QStringList() << tr("@pvp_ili_zascal") << tr("@single") << tr("@multiple")
                                       , FiltersList() << UserType::EFilters::SNGSize_Pvp << UserType::EFilters::SNGSize_Single << UserType::EFilters::SNGSize_Multiple);
    }

    void SitNGo::updateTexts()
    {

    }
    void SitNGo::changeEvent(QEvent *e)
    {
        if (e != nullptr && e->type() == QEvent::LanguageChange) {
            m_ui->retranslateUi(this);
            updateTexts();
        }
        QFrame::changeEvent(e);
    }

    TableView *SitNGo::tableView() const {
        return m_ui->tableView;
    }

    void SitNGo::updateFilters()
    {
        UserType::FilterFlags f = m_ui->filter_game->model()->index(0, 0).data(UserType::FilterRole).value<UserType::FilterFlags>();
        f |= m_ui->filter_buyin->model()->index(0, 0).data(UserType::FilterRole).value<UserType::FilterFlags>();
        f |= m_ui->filter_speed->model()->index(0, 0).data(UserType::FilterRole).value<UserType::FilterFlags>();
        f |= m_ui->filter_type->model()->index(0, 0).data(UserType::FilterRole).value<UserType::FilterFlags>();
        f |= m_ui->filter_size->model()->index(0, 0).data(UserType::FilterRole).value<UserType::FilterFlags>();
        setFilter(f);
    }

    void SitNGo::onItemActivated(const QModelIndex &index)
    {
       if (!index.isValid()) {
           return;
       }
//        quintptr sharedDataId = m_ui->tableView->rootModel()->sharedDataId();
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

    void SitNGo::onCurrentChanged(const QModelIndex &index)
    {
        m_ui->descriptionPanel->setCurrentIndex(index);
        updateTexts();
    }

    void SitNGo::disableFilters(bool disabled)
    {
        if (QSortFilterProxyModel* proxyModel = qobject_cast<QSortFilterProxyModel*>(tableView()->model()))
        {
            m_ui->filters->setDisabled(disabled);
            proxyModel->setFilterRegExp(UserType::toRegExp(disabled ? m_mask : m_filter));
            proxyModel->setFilterRole(UserType::FilterRole);
        }
    }


    void SitNGo::initTableView()
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

//        ns_model::cash_players* plModel = new ns_model::cash_players;
//        m_ui->playersList->setModel(plModel);
//        plModel->setSourceModel(modelsDB().getCashStatistic());
    }


} //namespace ns_page

