#include <precompiled.h>
#include <qsortfilterproxymodel.h>
#include "page_CashTables.h"
#include "ui_page_CashTables.h"
#include <models/model_PlayersListModel.h>
#include <models/model_cashtables.h>
#include <models/model_user_additional_info.h>
#include <qtimer.h>
#include <api_models/api_models.hpp>
#include <api_models/model_cash_stat_t.hpp>
#include <db/modeldatabase.h>
#include <QTableView>
#include <game/actions/complex_actions.h>
#include <game/actions/user_triggered_actions.h>

Q_DECLARE_METATYPE(gbp::ns_api::ns_statistic::cash_stat_t)

namespace
{

//    QModelIndex mapToModel(const QAbstractItemModel* m, const QModelIndex& idx) {
//        if (!idx.isValid()) {
//            return QModelIndex();
//        }
//        if (idx.model() == m) {
//            return idx;
//        }
//        if (const QAbstractProxyModel* proxyModel = qobject_cast<const QAbstractProxyModel*>(idx.model())) {
//            return mapToModel(m, proxyModel->mapToSource(idx));
//        }
//        return QModelIndex();
//    }
} //namespace


namespace ns_page
{

    CashTables::CashTables(QWidget *parent)
        : MainLobbyBase(UserType::EFilters::CashFilters_Mask, parent)
        , m_ui(new Ui::CashTables)
    {
        m_ui->setupUi(this);
    }

    CashTables::~CashTables()
    {
        delete m_ui;
    }

    void CashTables::init()
    {
        MainLobbyBase::init();

        m_ui->minitbl_container->init();
        m_ui->stack_waitOrSit->setCurrentIndex(0);


//        ns_model::PlayersListModel* plModel = new ns_model::PlayersListModel;
//        m_ui->playersList->setModel(plModel);
//        m_ui->playersList->hideColumn(0);

//        QTimer::singleShot(100, Qt::CoarseTimer, [this]{
//            m_ui->playersList->horizontalHeader()->setFixedHeight(24);
//        });

//        m_ui->playersList->horizontalHeader()->resizeSection(1, 170);
//        m_ui->playersList->horizontalHeader()->resizeSection(2, 36);
//        m_ui->playersList->horizontalHeader()->resizeSection(3, 100);
//        m_ui->playersList->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
//        m_ui->playersList->horizontalHeader()->setIconSize(QSize(36, 18));

//        m_ui->playersList->horizontalHeader()->setObjectName("horizontalHeader_playersList");
//        m_ui->playersList->horizontalHeader()->setIconSize(QSize(24, 24));
//        QLabel* iconWidget = new QLabel(m_ui->playersList->horizontalHeader());
//        iconWidget->setPixmap(QPixmap(":/textures/Common/icons/icon_geo.png"));
//        iconWidget->setAlignment(Qt::AlignCenter);
//        m_ui->playersList->horizontalHeader()->setIndexWidget(m_ui->playersList->model()->index(0, 2), iconWidget);
//        iconWidget->raise();
//        iconWidget->show();


        connect(m_ui->minitbl_container, &ns_widget::MiniTable::requestTakeASit, this, [this](int seatIndex) {
            QModelIndex modelIndex = tableView()->currentIndex();
            if (modelIndex.isValid())
            {
                emit sitOnTableRequested(modelIndex.data(UserType::RowItemIdRole).value<gbp_i64>(), seatIndex);
            }
        });

        QGraphicsDropShadowEffect* eff = new QGraphicsDropShadowEffect(this);
        eff->setColor(Qt::black);
        eff->setOffset(0, -0.8);
        eff->setBlurRadius(3);
        m_ui->ico_playersCount->setGraphicsEffect(eff);
        eff->setEnabled(true);

        FiltersList filtersList;
        QStringList filterNames;
        filtersList.append(UserType::EFilters::Holdem_FixedLimit);
        filtersList.append(UserType::EFilters::Holdem_NoLimit);
        filtersList.append(UserType::EFilters::Omaha_PotLimit);
        filterNames.append(ns_util::toString(gbp::ns_api::ns_game::e_game_limit::fixed_limit) + " " + ns_util::toString(gbp::ns_api::ns_game::e_game_type::texas_holdem));
        filterNames.append(ns_util::toString(gbp::ns_api::ns_game::e_game_limit::no_limit) + " " + ns_util::toString(gbp::ns_api::ns_game::e_game_type::texas_holdem));
        filterNames.append(ns_util::toString(gbp::ns_api::ns_game::e_game_limit::pot_limit) + " " + ns_util::toString(gbp::ns_api::ns_game::e_game_type::omaha));

        initFilterBox(m_ui->filter_game, filterNames, filtersList);

        filtersList.clear();
        filterNames.clear();
        filtersList << UserType::EFilters::BuyIn_Micro << UserType::EFilters::BuyIn_Low << UserType::EFilters::BuyIn_Medium << UserType::EFilters::BuyIn_High;
        filterNames << tr("@micro") << tr("@low") << tr("@medium") << tr("@high");
        initFilterBox(m_ui->filter_blinds, filterNames, filtersList);

        filtersList.clear();
        filterNames.clear();
        filtersList << UserType::EFilters::SeatsCount_2 << UserType::EFilters::SeatsCount_6 << UserType::EFilters::SeatsCount_9;
        filtersList << UserType::EFilters::FreeSeats_Empty << UserType::EFilters::FreeSeats_Full;
        filterNames << tr("@max_2") << tr("@max_6") << tr("@max_9") << tr("@empty_tables") << tr("@full_tables");
        initFilterBox(m_ui->filter_maxPlayers, filterNames, filtersList);


//        if (!tableView()->currentIndex().isValid()) {
//        }
    }

    void CashTables::updateTexts()
    {
        if ( tableView()->model() == nullptr) {
            return;
        }

        QModelIndex idx = tableView()->currentIndex();
        gbp::ns_api::ns_statistic::cash_stat_t cashStatInfo = idx.data(UserType::RowDataRole).value<gbp::ns_api::ns_statistic::cash_stat_t>();
//        ns_model::bind(m_ui->ico_playersCount)

        m_ui->ico_playersCount->setText(QString("%0/%1").arg(cashStatInfo._players_count.first).arg(cashStatInfo._players_count.second));
        m_ui->title_cashInfo->setText(cashStatInfo._title.c_str());


        m_ui->info_title->setText(QString("%0/%1 %2 %3").arg(Money(cashStatInfo._big_blind/2).toString())
                                                        .arg(Money(cashStatInfo._big_blind).toString())
                                                        .arg(ns_util::toString(cashStatInfo._limit))
                                                        .arg(ns_util::toString(cashStatInfo._type)));


        m_ui->num_average_pot->setText(Money(cashStatInfo._pot).toString());
        Money::money_t averageStack = 0;
        if (!cashStatInfo._players.empty())
        {
            for (auto player : cashStatInfo._players) {
                averageStack += player._money;
            }
            averageStack /= cashStatInfo._players.size();
        }

        m_ui->num_average_stack->setText(Money(averageStack).toString());

        ns_model::UserInfo* m = modelsDB().getUserAdditionalInfo();
        m->getItem(ns_util::enumToString(ns_model::UserInfo::field::waiter_pos), "gameinfo");

        m_ui->stack_waitOrSit->setCurrentIndex((cashStatInfo._players_count.first != cashStatInfo._players_count.second) ? 0 : 1);
    }

    TableView *CashTables::tableView() const {
        return m_ui->statisticView;
    }

    void CashTables::changeEvent(QEvent *e)
    {
        if (e != nullptr && e->type() == QEvent::LanguageChange) {
            m_ui->retranslateUi(this);
            updateTexts();
        }
        QFrame::changeEvent(e);
    }

    void CashTables::showEvent(QShowEvent *e)
    {
        MainLobbyBase::showEvent(e);
    }

    void CashTables::updateFilters()
    {
        UserType::FilterFlags f = m_ui->filter_game->model()->index(0, 0).data(UserType::FilterRole).value<UserType::FilterFlags>();
        f |= m_ui->filter_blinds->model()->index(0, 0).data(UserType::FilterRole).value<UserType::FilterFlags>();
        f |= m_ui->filter_maxPlayers->model()->index(0, 0).data(UserType::FilterRole).value<UserType::FilterFlags>();
        setFilter(f);
    }

    void CashTables::onItemActivated(const QModelIndex &/*index*/)
    {
        m_ui->bttn_open_tbl->click();
    }

    void CashTables::onCurrentChanged(const QModelIndex &index) {
        updateTexts();
        QModelIndex idx = mapToModel(modelsDB().getCashStatistic(), index);
        if (idx.isValid()) {
            static const int column = underlying_cast(ns_model::cash_stat_t::column::_players);
            m_ui->playersList->setRootIndex(qobject_cast<QAbstractProxyModel*>(m_ui->playersList->model())->mapFromSource(idx.sibling(idx.row(), column)));
        }
        m_ui->minitbl_container->onIndexChanged(index);

//        static auto slot = ns_model::ModelColumn::bindByRow(m_ui->num_average_pot_2, modelsDB().getCashStatistic()
//                                                            , underlying_cast(ns_model::cash_stat_t::column::_type));
//        qDebug() << "result:" << slot(idx.row());

    }

    void CashTables::disableFilters(bool disabled)
    {
        if (QSortFilterProxyModel* proxyModel = qobject_cast<QSortFilterProxyModel*>(tableView()->model()))
        {
            m_ui->filters->setDisabled(disabled);
            proxyModel->setFilterRegExp(UserType::toRegExp(disabled ? m_mask : m_filter));
            proxyModel->setFilterRole(UserType::FilterRole);
        }
    }

    void CashTables::initTableView()
    {
        ns_model::cash_tables* model = new ns_model::cash_tables;
        QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel;
        m_ui->statisticView->setModel(proxyModel);

        connect(proxyModel, &QSortFilterProxyModel::modelReset, this, [this]{
            if (!m_ui->statisticView->currentIndex().isValid()) {
                m_ui->statisticView->selectRow(0);
            }
        });

        proxyModel->setSourceModel(model);
        model->setSourceModel(modelsDB().getCashStatistic());
//        m_ui->tableView->init();

        proxyModel->setSortRole(UserType::SortRole);

        ns_model::cash_players* plModel = new ns_model::cash_players;
        m_ui->playersList->setModel(plModel);
        plModel->setSourceModel(modelsDB().getCashStatistic());

//        m_ui->playersList->setR
        m_ui->playersList->horizontalHeader()->resizeSection(0, 176);
        m_ui->playersList->horizontalHeader()->resizeSection(1, 24);
        m_ui->playersList->horizontalHeader()->resizeSection(2, 100);
        m_ui->playersList->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        m_ui->playersList->horizontalHeader()->setSectionsClickable(false);


    }


} // namespace page



void ns_page::CashTables::on_bttn_sit_clicked()
{
    QModelIndex modelIndex = tableView()->currentIndex();
    if (modelIndex.isValid())
    {
        gbp_i64 id = modelIndex.data(UserType::RowItemIdRole).value<gbp_i64>();
        ns_action::JoinCash* action = new ns_action::JoinCash(id, window(), this);
        action->initialize();

//        connect(action, &ActionSitOnCashTable::actionDone, [action](Action* action, bool res){

//        });

        connect(action, &ns_action::JoinCash::done, action, &ns_action::JoinCash::deleteLater);

        action->execute();
    }
}

void ns_page::CashTables::on_bttn_open_tbl_clicked()
{
    QModelIndex modelIndex = tableView()->currentIndex();
    if (modelIndex.isValid())
    {
        ns_action::WatchCash* action = new ns_action::WatchCash(qvariant_cast<gbp_i64>(modelIndex.data(UserType::RowItemIdRole)), nullptr, this);

        action->initialize();
        connect(action, &ns_action::WatchCash::done, action, &ns_action::WatchCash::deleteLater);
        action->execute();

//        emit openTableRequested(modelIndex.data(UserType::RowItemIdRole).value<gbp_i64>());
    }
}

void ns_page::CashTables::on_bttn_joinQueue_clicked()
{
    QModelIndex modelIndex = tableView()->currentIndex();
    if (modelIndex.isValid())
    {
        ns_action::UserJoiningQueue* action = new ns_action::UserJoiningQueue(qvariant_cast<gbp_i64>(modelIndex.data(UserType::RowItemIdRole)), this);

        action->initialize();
        connect(action, &ns_action::UserJoiningQueue::done, action, &ns_action::UserJoiningQueue::deleteLater);
        connect(action, &ns_action::UserJoiningQueue::done, m_ui->bttn_joinQueue, &QPushButton::setHidden);
        connect(action, &ns_action::UserJoiningQueue::done, m_ui->bttn_unjoinQueue, &QPushButton::setVisible);
        action->execute();
    }
}

void ns_page::CashTables::on_bttn_unjoinQueue_clicked()
{

}
