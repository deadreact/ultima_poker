#pragma once

#include "../../common/api/gbp_int.hpp"
#include <qregularexpression.h>

struct GameId;
class QRegularExpression;
class QAbstractItemModel;
using QModelList = QList<QAbstractItemModel*>;
namespace ns_model
{
    class LogModel;
    class user_info_t;
    class cash_stat_t;
    class tournament_stat_t;
    class table_state_t;
    class vip_system_info;
    class tourn_additional_stat_t;
    class players_list;

    class UserInfo;
} //namespace ns_model


class ModelDatabase
{
private:
    struct PrivateData;
    PrivateData* m_dPtr;
    bool m_initialized;
    gbp_i64 m_lastUserId;
public:
    ModelDatabase();
    virtual ~ModelDatabase();

    virtual void init();
    gbp_i64 lastUserId() const;

    ns_model::user_info_t*         getUserInfo() const;
    ns_model::UserInfo*            getUserAdditionalInfo() const;
    ns_model::cash_stat_t*         getCashStatistic() const;
    ns_model::tournament_stat_t*   getTournamentStatistic() const;
    ns_model::vip_system_info*     getVipSystemInfo() const;

    ns_model::tourn_additional_stat_t* getAdditionalTournStatistic(gbp_i64 id) const;
    ns_model::players_list*            getTournamentPlayersList(gbp_i64 id) const;

    ns_model::table_state_t* getTableState(const GameId& gameId) const;
    void removeTableState(const GameId& gameId) const;

//    QModelList getModels() const;
    QModelList getModels(const QRegularExpression& re = QRegularExpression()) const;

    template <typename T>
    QList<T> getModels(const QRegularExpression& re = QRegularExpression()) const;

    ns_model::LogModel* logModel() const;
};

template <typename T>
QList<T> ModelDatabase::getModels(const QRegularExpression& re) const
{
    QList<T> models;
    for (QAbstractItemModel* m: getModels(re)) {
        if (auto mt = qobject_cast<T>(m)) {
            models << mt;
        }
    }
    return models;
}


/******************************** global function ********************************/
ModelDatabase &modelsDB();
/*********************************************************************************/