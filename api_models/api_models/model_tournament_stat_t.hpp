#pragma once

#include "api_models_global.h"
#include <QAbstractTableModel>
#include "defs.hpp"
#include <tools/accessor.hpp>
#include "model_apibase.hpp"

namespace ns_statistic
{
    struct rpc_object;
}

namespace ns_model
{
    // -----------------------------
    struct parent_data
    {
        gbp_i64 id;
        int col;
        parent_data(gbp_i64 id, int col) : id(id), col(col) {}
    };
    // -----------------------------
    class API_MODEL_DLL tournament_stat_t : public ApiBase
    {
        Q_OBJECT
    public:
        using data_t = games_stat_t::second_type;
        using value_t = data_t::value_type;

        enum class column : int {
            _id = 0,
            _seats_count,
            _max_players_count,
            _is_current_user_registered,
            _players_registered_count,
            _sit_n_go_users_count,
            _main_tournament_id,
            _buyin,
            _big_blind,
            _round_duration,
            _start_stack,
            _prize_places_count,
            _common_prize,
            _types,
            _game_type,
            _game_speed,
            _game_limit,
            _title,
            _status,
            _is_partner_tourn,
            _registration_start_time,
            _start_time,
            _finish_time,
            _button_state,
            _late_registration_duration,
            _qualifiers_ids, //array
            _rebuys,// struct
            _addon, // struct
            _bounty,
            _player_bounty_value,
            _current_level,
            _users, // array

            count
        };
        Q_ENUM(column)

        enum class users {
            _id = 0,
            _nickname,
            _country,
            _is_ticket,
            _money,
            _bounty_knocked_out_count,
            _bounty_progr_money,

            count
        };
        Q_ENUM(users)

        enum class rebuys {
            _count = 0,
            _money,
            _chips,

            count
        };
        Q_ENUM(rebuys)

        enum class addon {
            _is_available = 0,
            _money,
            _chips,

            count
        };
        Q_ENUM(addon)

    private:
        std::vector<value_t> m_data;
        mutable QList<parent_data*> m_pData;
        ns_statistic::rpc_object* m_rpc;
        bool m_canFetchMore;
    public slots:
        void updateData(const games_stat_t& data);
    public:
        explicit tournament_stat_t(QObject *parent = 0);
        virtual ~tournament_stat_t();

        void init();
        void setRpc(ns_statistic::rpc_object* rpc);
        virtual void updateRowItem(int row, const gbp::ns_api::ns_statistic::tournament_stat_t& newItem);

        virtual QModelIndex indexOf(gbp_i64 idAt) const override;
        QModelIndex indexOf(gbp_i64 idAt, column field) const;

        Accessor<value_t> item(gbp_i64 idAt) const;
        Accessor<value_t> itemAt(int row) const;

        static const gbp::ns_api::ns_statistic::tournament_stat_t& defaultRowItem();

        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
        virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

        virtual bool removeItems(const QList<gbp_i64>& ids, const QModelIndex& parent = QModelIndex()) override;
        virtual bool insertItems(const QList<gbp_i64>& ids, const QModelIndex &parent = QModelIndex()) override;
        virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
        virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
        virtual bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override;

        virtual QModelIndex index(int row, int col, const QModelIndex &parent = QModelIndex()) const override;

        QModelIndex usersRootAt(int row) const;
        QModelIndex usersRootOf(gbp_i64 idAt) const;
        QModelIndex indexOfUser(gbp_i64 tournamentId, gbp_i64 userId, users field = users(0)) const;


        virtual QModelIndex parent(const QModelIndex &child) const override;
        virtual bool hasChildren(const QModelIndex &parent) const override;

        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        virtual bool isItemOrientedData() const override;

        virtual QModelIndexList match(const QModelIndex &start, int role, const QVariant &value, int hits, Qt::MatchFlags flags) const override;

        virtual void fetchMore(const QModelIndex &parent) override;
        virtual bool canFetchMore(const QModelIndex &parent) const override;
        bool isFetchMoreBlocked() const;
        virtual bool read(QIODevice *device) override;
        virtual bool write(QIODevice *device) const override;
        virtual int rowById(gbp_i64 idAt, const void *data, const QModelIndex& parent = QModelIndex()) const override;
        virtual int propertyIndex(const QByteArray& name, const QModelIndex& parent = QModelIndex()) const override;
    protected:
        virtual QVariant dataSortRole(const QModelIndex& index) const;

    };
} //namespace ns_model
