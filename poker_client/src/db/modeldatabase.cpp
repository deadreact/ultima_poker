#include <precompiled.h>
#include "modeldatabase.h"
#include <basewidgets/types.h>
#include <api_models/model_user_info_t.hpp>
#include <api_models/model_cash_stat_t.hpp>
#include <api_models/model_tournament_stat_t.hpp>
#include <api_models/model_table_state_t.hpp>
#include <api_models/model_vip_system_info.hpp>
#include <helpers/helper_enum.h>
#include <PokerApplication.h>

#ifndef Q_MOC_RUN
    #include <api/api_common.h>
    #include <api/table/types.h>
    #include <rpc/rpc_common.h>
    #include <rpc/rpc_statistic.h>
    #include <rpc/rpc_lobby.h>
    #include <rpc/table/rpc_table.h>
#endif // !Q_MOC_RUN
#include <api_models/model_tourn_additional_stat_t.hpp>
#include <api_models/model_log.h>
#include <models/model_PlayersTable.h>
#include <models/model_user_additional_info.h>
#include <tools/item_reference.hpp>

#include <notes/model_io.h>


ModelDatabase &modelsDB() {
    return pApp->db().modelDB();
}

class ModelDataSync
{
public:
    static void setWaitersCount(gbp_i64 id, gbp_u16 waiters) {
        ns_model::cash_stat_t* cash_model = modelsDB().getCashStatistic();
        QModelIndex index_statistic = cash_model->indexOf(id, ns_model::cash_stat_t::column::_wait_players);
        ItemRefCash iRef = (cash_model->item(id).ref());
        if (iRef._wait_players != waiters) {
            iRef._wait_players = waiters;
            emit cash_model->dataChanged(index_statistic, index_statistic);
        }
    }
};


struct ModelDatabase::PrivateData
{
    QObject* const m_modelsParent;
    QScopedPointer<ns_model::user_info_t>         m_user_info_t;
    QScopedPointer<ns_model::UserInfo>            m_user_additional_info;
    QScopedPointer<ns_model::cash_stat_t>         m_cash_stat_t;
    QScopedPointer<ns_model::tournament_stat_t>   m_tournament_stat_t;
    QMap<gbp_i64, ns_model::tourn_additional_stat_t*> m_tourn_additional_stat_t;
    QMap<gbp_i64, ns_model::players_list*>            m_tourn_players;
    QScopedPointer<ns_model::vip_system_info>     m_vip_system_info;

    struct TableModels {
        QSharedPointer<ns_model::table_state_t>           first;

        TableModels(QObject* parent = nullptr)
            : first(new ns_model::table_state_t(parent))
        {}
        inline bool operator==(const TableModels& other) const { return first.data() == other.first.data(); }
        inline bool operator<(const TableModels& other) const { return first.data() < other.first.data(); }
    };

    QMap<GameId, TableModels> m_table_models;
    /**************************/
    PrivateData(QObject* modelsParent = nullptr)
        : m_modelsParent(modelsParent)
        , m_user_info_t(nullptr)
        , m_user_additional_info(nullptr)
        , m_cash_stat_t(nullptr)
        , m_tournament_stat_t(nullptr)
        , m_vip_system_info(nullptr)
        , m_table_models()
    {}
    ~PrivateData()
    {
        for (auto it = m_tourn_additional_stat_t.begin(); it != m_tourn_additional_stat_t.end(); it++) {
            //qDebug() << "del" << m_tourn_additional_stat_t.size();
            delete it.value();
            //qDebug() << "/del" << m_tourn_additional_stat_t.size();
        }
    }

    TableModels tableModels(const GameId& gameId)
    {
        decltype(m_table_models.begin()) it = m_table_models.find(gameId);
        if (it == m_table_models.end()) {
            it = m_table_models.insert(gameId, TableModels());
            it.value().first->resetLogModel(modelsDB().logModel());
            it.value().first->setObjectName(QString("model_tableState_%0").arg(gameId.repr()));
            synchronize(gameId, it.value().first.data());
        }
        return it.value();
    }

    QList<QAbstractItemModel*> all() const
    {
        QSet<QAbstractItemModel*> models;
        models << m_user_info_t.data() << m_cash_stat_t.data() << m_tournament_stat_t.data() << m_vip_system_info.data();
        for (const TableModels& tm : m_table_models) {
            models << tm.first.data();
        }
        for (auto* m: m_tourn_additional_stat_t) {
            models << m;
        }
        for (auto* m: m_tourn_players) {
            models << m;
        }
        models << m_user_additional_info.data();
        models.remove(nullptr);
        QList<QAbstractItemModel*> modelsList = models.toList();

        return modelsList;
    }

    void synchronize(const GameId& gameID, ns_model::table_state_t* model);
};

void ModelDatabase::PrivateData::synchronize(const GameId &gameID, ns_model::table_state_t* model) {
    if (gameID.isCash()) {
        ns_model::cash_stat_t* sourceModel = m_cash_stat_t.data();
        auto fnSetWaiters = [&] { model->setOriginData(model->index_waiters_count(), sourceModel->item(gameID.m_tableID).ref()._wait_players);};
        /** ******************************/
        model->connect(sourceModel, &ns_model::cash_stat_t::modelReset, model, fnSetWaiters);
        model->connect(sourceModel, &ns_model::cash_stat_t::dataChanged, model, [&](const QModelIndex& tl, const QModelIndex& br, const QVector<int>& roles){
            QModelIndexRange indexRange(tl, br);
            if (indexRange.contains(sourceModel->index_waiters_count(gameID.m_tableID))) {
                fnSetWaiters();
            }
        });
    }
}



ModelDatabase::ModelDatabase()
    : m_dPtr(new PrivateData)
    , m_initialized(false)
    , m_lastUserId(-1)
{}

ModelDatabase::~ModelDatabase()
{
    delete m_dPtr;
}

void ModelDatabase::init()
{
    if (m_initialized) {
        return;
    }
    rpc_statistic* rpc_stat = rpc_object<rpc_statistic>();
    rpc_lobby* rpc_login = rpc_object<rpc_lobby>();

    QObject::connect(rpc_stat->emitter(), &ns_statistic::emitter_t::games_statistic, [this](const games_stat_t& statistic) {
        getCashStatistic()->updateData(statistic);
        getTournamentStatistic()->updateData(statistic);
        static int counter = 0;
        // Каждые 15 секунд бэкап
        if (counter > 5) {
            getCashStatistic()->backup("backup_cashes.dat");
            getCashStatistic()->backup("backup_tournaments.dat");
            counter = 0;
        } else {
            counter++;
        }
    });

    QObject::connect(rpc_login->emitter(), &ns_lobby::emitter_t::login, [this](ns_lobby::e_status status, const ns_lobby::user_info_t& info) {
        if (ns_helpers::isOk(status)) {
            if (getUserInfo()->nativeData() != nullptr) {
                qFatal("user_info_t should be nullptr when nobody is logged in");
            }
            m_lastUserId = info._user_id;
            m_dPtr->m_user_info_t->resetData(new ns_lobby::user_info_t(info));

            if (!m_dPtr->m_vip_system_info.isNull()) {
                qFatal("vip_system_info should be nullptr when nobody is logged in");
            }
            m_dPtr->m_vip_system_info.reset(new ns_model::vip_system_info(m_dPtr->m_modelsParent));
            m_dPtr->m_vip_system_info->setObjectName(QString("model_vipSystem"));

            ns_model::IOModel::setUserId(info._user_id);
        }
    });


    QObject::connect(rpc_login->emitter(), &ns_lobby::emitter_t::logout, [this](ns_lobby::e_status status) {
        if (ns_helpers::isOk(status)) {
            if (getUserInfo()->nativeData() == nullptr) {
                qFatal("user_info_t should not be nullptr when there is a logged in user");
            }

            m_dPtr->m_user_info_t->resetData();

            if (m_dPtr->m_vip_system_info.isNull()) {
                qFatal("vip_system_info should not be nullptr when there is a logged in user");
            }
            m_dPtr->m_vip_system_info.reset();

            ns_model::IOModel::setUserId(-1);
        }
    });

    m_initialized = true;
}

gbp_i64 ModelDatabase::lastUserId() const {
    return m_lastUserId;
}

ns_model::user_info_t *ModelDatabase::getUserInfo() const {
    if (m_dPtr->m_user_info_t.isNull()) {
        m_dPtr->m_user_info_t.reset(new ns_model::user_info_t(m_dPtr->m_modelsParent));
        m_dPtr->m_user_info_t->setObjectName(QString("model_user_info"));
    }
    return m_dPtr->m_user_info_t.data();
}

ns_model::UserInfo *ModelDatabase::getUserAdditionalInfo() const
{
    if (m_dPtr->m_user_additional_info.isNull()) {
        m_dPtr->m_user_additional_info.reset(new ns_model::UserInfo(m_dPtr->m_modelsParent));
        auto* model = getUserInfo();
        model->connect(model, &ns_model::user_info_t::modelAboutToBeReset, m_dPtr->m_user_additional_info.data(), &ns_model::UserInfo::onUserInfoAboutToBeReset);
        model->connect(model, &ns_model::user_info_t::modelReset, m_dPtr->m_user_additional_info.data(), &ns_model::UserInfo::onUserInfoReset);
        model->connect(model, &ns_model::user_info_t::dataChanged, m_dPtr->m_user_additional_info.data(), &ns_model::UserInfo::onUserInfoDataChanged);

        m_dPtr->m_user_additional_info->setObjectName("model_user_additional_info");
        m_dPtr->m_user_additional_info->init();
    }
    return m_dPtr->m_user_additional_info.data();
}

ns_model::cash_stat_t *ModelDatabase::getCashStatistic() const {
    if (m_dPtr->m_cash_stat_t.isNull()) {
        m_dPtr->m_cash_stat_t.reset(new ns_model::cash_stat_t(m_dPtr->m_modelsParent));
        m_dPtr->m_cash_stat_t->setObjectName(QString("model_cash_stat"));
    }
    return m_dPtr->m_cash_stat_t.data();
}

ns_model::tournament_stat_t *ModelDatabase::getTournamentStatistic() const {
    if (m_dPtr->m_tournament_stat_t.isNull()) {
        m_dPtr->m_tournament_stat_t.reset(new ns_model::tournament_stat_t(m_dPtr->m_modelsParent));
        m_dPtr->m_tournament_stat_t->setObjectName(QString("model_tournament_stat"));
        m_dPtr->m_tournament_stat_t->setRpc(rpc_object<rpc_statistic>());
    }
    return m_dPtr->m_tournament_stat_t.data();
}

ns_model::table_state_t *ModelDatabase::getTableState(const GameId &gameId) const {
    return m_dPtr->tableModels(gameId).first.data();
}

void ModelDatabase::removeTableState(const GameId &gameId) const
{
    m_dPtr->m_table_models.remove(gameId);
}

QModelList ModelDatabase::getModels(const QRegularExpression &re) const
{
    if (!re.isValid()) {
        return m_dPtr->all();
    }
    QModelList lst;
    for (QAbstractItemModel* m: m_dPtr->all()) {
        if (re.match(m->objectName()).hasMatch()) {
            lst << m;
        }
    }
    return lst;
}

ns_model::LogModel *ModelDatabase::logModel() const
{
    static ns_model::LogModel m;
    return &m;
}

ns_model::vip_system_info *ModelDatabase::getVipSystemInfo() const {
    return m_dPtr->m_vip_system_info.data();
}

ns_model::tourn_additional_stat_t *ModelDatabase::getAdditionalTournStatistic(gbp_i64 id) const
{
    if (id == -1) {
        //qDebug() << "invalid id" << "ns_model::tourn_additional_stat_t *ModelDatabase::getAdditionalTournStatistic(gbp_i64 id) const";
    }
    if (!m_dPtr->m_tourn_additional_stat_t.contains(id)) {
        ns_model::tourn_additional_stat_t* m = new ns_model::tourn_additional_stat_t(id, m_dPtr->m_modelsParent);
        m->setObjectName(QString("model_tourn_additional_stat_t_%0").arg(id));

        m_dPtr->m_tourn_additional_stat_t.insert(id, m);

        m->connect(m, &ns_model::tourn_additional_stat_t::destroyed, [this, id] {
            //qDebug() << "removing model tourn_additional_stat_t";
            m_dPtr->m_tourn_additional_stat_t.remove(id);
        });

        rpc_statistic* rpc_stat = rpc_object<rpc_statistic>();
        rpc_stat->get_additional_tourn_stat(id, [this, id](ns_statistic::e_status status, const ns_statistic::tourn_additional_stat_t& data){
            std::cout << "rpc_stat->get_additional_tourn_stat callback, status" << status << std::endl;
            if (ns_helpers::isOk(status)) {
                m_dPtr->m_tourn_additional_stat_t.value(id)->resetData(data);
            }
        });
    }
    return m_dPtr->m_tourn_additional_stat_t.value(id);
}

ns_model::players_list *ModelDatabase::getTournamentPlayersList(gbp_i64 id) const
{
    if (!m_dPtr->m_tourn_players.contains(id)) {
        auto* m = new ns_model::players_list(m_dPtr->m_modelsParent);
        m->setCurrentId(id);
        m_dPtr->m_tourn_players.insert(id, m);
        m->connect(m, &ns_model::players_list::destroyed, [this, id] {
            //qDebug() << "removing model players_list";
            m_dPtr->m_tourn_players.remove(id);
        });
    }
    return m_dPtr->m_tourn_players.value(id);
}

