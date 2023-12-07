#pragma once
#include "api_models_global.h"
#include "model_apibase.hpp"

#ifndef Q_MOC_RUN
    #include <api/api_common.h>
    #include <api/lobby_stat/types.h>
#endif // !Q_MOC_RUN

#include "defs.hpp"
#include <basewidgets/types.h>
#include <tools/accessor.hpp>
#include "itemtype_concept.hpp"


namespace
{
    using e_game_type           = gbp::ns_api::ns_game::e_game_type;
    using e_game_limit          = gbp::ns_api::ns_game::e_game_limit;
    using players_pair          = std::pair<gbp_u8, gbp_u8>;
    using players_lst           = std::vector<gbp::ns_api::ns_statistic::player_t>;
}

Q_DECLARE_METATYPE(players_pair)
Q_DECLARE_METATYPE(players_lst)

namespace ns_meta
{
    struct game_type_type : generic_enum_type<e_game_type> {
        static display_type display(const value_type &value) {
            return tr_enum_type::display(value);
        }
    };
    struct game_limit_type : generic_enum_type<e_game_limit> {
        static display_type display(const value_type &value) {
            return tr_enum_type::display(value);
        }
    };

    using players_count_type = generic_pair_value< generic_count_type<gbp_i8>, generic_count_type<gbp_i8>>;
} //namespace ns_meta

namespace ns_model
{
    struct cash_parent_data;

    class API_MODEL_DLL cash_stat_t : public ApiBase
    {
        Q_OBJECT
    public:
        enum class column : int {
            row_data = -1,
            _id = 0,
            _title,
            _is_freeroll,
            _big_blind,
            _type,
            _limit,
            _players_count,
            _pot,
            _flp,
            _hands_per_hour,
            _wait_players,
            _players,
            _is_flash,
            _flash_id,

            count
        };
        Q_ENUM(column)

        enum class players {
            _nick = 0,
            _pos,
            _money,
            _country,

            count
        };
        Q_ENUM(players)
    public:
        inline QModelIndex index_waiters_count(gbp_i64 id) const { return indexOf(id, column::_wait_players); }
        inline QModelIndex index_big_blind(gbp_i64 id) const { return indexOf(id, column::_big_blind); }
        inline QModelIndex index_limit(gbp_i64 id) const { return indexOf(id, column::_limit); }
    private:
        using data_t = games_stat_t::first_type;
        using value_t = data_t::value_type;
        data_t m_data;
        mutable QList<cash_parent_data*> m_pData;
    public slots:
        void updateData(const games_stat_t& d);
        void updateData(const data_t &d);
    public:
        explicit cash_stat_t(QObject *parent = nullptr);
        virtual ~cash_stat_t();

        void init();

        virtual bool write(QIODevice* device) const override;
        virtual bool read(QIODevice *device) override;

        virtual QModelIndex indexOf(gbp_i64 idAt) const override;
        QModelIndex indexOf(gbp_i64 idAt, column field) const;

        virtual int rowById(gbp_i64 idAt, const void *data = nullptr, const QModelIndex& parent = QModelIndex()) const override;
        virtual int propertyIndex(const QByteArray& name, const QModelIndex& parent = QModelIndex()) const override;

        Accessor<value_t> item(gbp_i64 idAt) const;
        Accessor<value_t> itemAt(int row) const;

        static const gbp::ns_api::ns_statistic::cash_stat_t& defaultRowItem(cash_stat_t *that = nullptr);

        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
        virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

        virtual bool removeItems(const QList<gbp_i64>& ids, const QModelIndex& parent = QModelIndex()) override;
        virtual bool insertItems(const QList<gbp_i64>& ids, const QModelIndex& parent = QModelIndex()) override;

        virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
        virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
        virtual bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override;


        virtual QModelIndex index(int row, int col, const QModelIndex &parent = QModelIndex()) const override;
        inline QModelIndex index(int row, column col) const {
            return index(row, underlying_cast(col), QModelIndex());
        }
        virtual QModelIndex parent(const QModelIndex &child) const override;
        virtual bool hasChildren(const QModelIndex &parent) const override;
        void refresh();
        // QAbstractItemModel interface
    public:
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        virtual bool isItemOrientedData() const override { return false; }

        virtual QModelIndexList match(const QModelIndex &start
                                    , int role
                                    , const QVariant &value
                                    , int hits              = 1
                                    , Qt::MatchFlags flags  = Qt::MatchFlags( Qt::MatchStartsWith | Qt::MatchWrap )) const override;
    };

} //namespace ns_model
