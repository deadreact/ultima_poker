#pragma once

#include <api_models/model_apibase.hpp>
#include <QIdentityProxyModel>
#include <api/lobby_stat/types.h>
#include <tools/accessor.hpp>

namespace ns_model
{
    class players_list : public QAbstractTableModel//ApiBase
    {
        Q_OBJECT

    public:
        struct Player {
            QString _nickname;
            gbp_i64 _money;
            union {
                gbp_i64 _bounty_progr_money;
                gbp_i32 _bounty_knocked_out_count;
            };
            gbp_i16 _country;
            enum flags : gbp_i8 {
                _no_flag         = 0x00,
                _is_ticket       = 0x01
            };
            gbp_i8 _flags;

            Player(const QString& nickname = "invalid"): _nickname(nickname), _money(0), _bounty_progr_money(0), _country(0), _flags(0) {}

            QVariant get(int i) const {
                switch (i) {
                case 0: return QVariant();
                case 1: return QVariant::fromValue(_nickname);
                case 2: return QVariant::fromValue(_country);
                case 3: return QVariant::fromValue(_bounty_knocked_out_count);
                case 4: return QVariant::fromValue(_bounty_progr_money);
                case 5: return QVariant::fromValue(_money);
                default:
                    return QVariant();
                }
            }
        };
    private:
        QMap<gbp_i64, Player> m_data;
        QList<gbp_i64> m_positions;
        gbp_i64 m_id;
        enum ColumnsSetType { Common, Bounty, Progressive, All };
        ColumnsSetType m_colsType;
        using tourn_user_info_t = games_stat_t::second_type::value_type::user_info;

        bool addPlayer(const tourn_user_info_t& pl);
        void updatePlayer(const tourn_user_info_t& pl);
        QModelIndex index(gbp_i64 playerId, int column = 0) const;
        void updatePositions();
//        std::vector<tourn_user_info_t> m_players;
        bool m_fetched;
    public:
        players_list(QObject* parent = nullptr);

        bool isRoot(const QModelIndex& idx) const;
        QModelIndex root() const;

        static QModelIndex rootOf(gbp_i64 id);

        gbp_i64 currentId() const;
        bool setCurrentId(gbp_i64 id);

        Accessor<ns_statistic::tournament_stat_t> currentTournStat() const;
    public:
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    public:
        virtual void fetchMore(const QModelIndex &parent = QModelIndex()) override;
        virtual bool canFetchMore(const QModelIndex &parent = QModelIndex()) const override;
        virtual QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal, int role = Qt::DisplayRole) const override;

        // QAbstractItemModel interface
    public:
        virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
        virtual QModelIndex parent(const QModelIndex &child) const override;
        virtual bool hasChildren(const QModelIndex &parent) const override;

//        // QAbstractProxyModel interface
//    public:
//        virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
//        virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    };
}

