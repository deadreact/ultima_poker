#pragma once
#include "algorithm.hpp"
#include "api_models_global.h"
#include <basewidgets/types.h>
#include "defs_metatype.hpp"
#include <tools/accessor.hpp>
#include <tools/hackindex_p.hpp>
#include <functional>
#include "model_apibase.hpp"
#include "helper_get_set_data.hpp"

namespace ns_meta
{
    struct state_type
    {
        Q_GADGET
    public:
        enum state {
          no_flag                = 0x00
        , _left_table            = 0x01
        , _sit_out               = 0x02
        , _frozen                = 0x04
        , _anticipant            = 0x08
        , _folded                = 0x10
        , _all_ined              = 0x20
        , _all_ined_in_cur_round = 0x40
        };
        Q_FLAG(state)
        Q_DECLARE_FLAGS(edit_type, state)
        using value_type = gbp::ns_api::ns_table::user_state_t;
        using display_type = QString;

        static QStringList buff;
        void operator()(const char* name, bool value);

        static display_type display(const value_type& value);
        static edit_type edit(const value_type& value);
    };
}
Q_DECLARE_OPERATORS_FOR_FLAGS(ns_meta::state_type::edit_type)
Q_DECLARE_METATYPE(ns_meta::state_type::edit_type)

namespace ns_model
{
    class LogModel;

    struct TSIndex
    {
        using parentId = ns_detail::table_state_parent_type;
        constexpr inline static HackIndex root()                                      { return HackIndex::prototype(0,  0, 0); }
        constexpr static inline HackIndex tournament_id()                             { return HackIndex::prototype(0,  0, parentId::root); }
        constexpr static inline HackIndex game_id()                                   { return HackIndex::prototype(1,  0, parentId::root); }
        constexpr static inline HackIndex big_blind()                                 { return HackIndex::prototype(2,  0, parentId::root); }
        constexpr static inline HackIndex seats_count()                               { return HackIndex::prototype(3,  0, parentId::root); }
        constexpr static inline HackIndex players()                                   { return HackIndex::prototype(4,  0, parentId::root); }
        constexpr static inline HackIndex current_user_pos()                          { return HackIndex::prototype(5,  0, parentId::root); }
        constexpr static inline HackIndex table_cards()                               { return HackIndex::prototype(6,  0, parentId::root); }
        constexpr static inline HackIndex table_card(int r)                           { return HackIndex::prototype(r,  0, parentId::tableCards); }
        constexpr static inline HackIndex all_pots()                                  { return HackIndex::prototype(7,  0, parentId::root); }
        constexpr static inline HackIndex pot(int r)                                  { return HackIndex::prototype(r,  0, parentId::tablePots); }
        constexpr static inline HackIndex waiters_count()                             { return HackIndex::prototype(8,  0, parentId::root); }
        constexpr static inline HackIndex time_short()                                { return HackIndex::prototype(9,  0, parentId::root); }
        constexpr static inline HackIndex time_main()                                 { return HackIndex::prototype(10, 0, parentId::root); }
        constexpr static inline HackIndex time_action_passed()                        { return HackIndex::prototype(11, 0, parentId::root); }
        constexpr static inline HackIndex time_action_left()                          { return HackIndex::prototype(12, 0, parentId::root); }
        constexpr static inline HackIndex distribution_number()                       { return HackIndex::prototype(13, 0, parentId::root); }
        constexpr static inline HackIndex player_id(int r)                            { return HackIndex::prototype(r,  0, parentId::players); }
        constexpr static inline HackIndex player_is_user(int r)                       { return HackIndex::prototype(r,  1, parentId::players); }
        constexpr static inline HackIndex player_pos(int r)                           { return HackIndex::prototype(r,  2, parentId::players); }
        constexpr static inline HackIndex player_nickname(int r)                      { return HackIndex::prototype(r,  3, parentId::players); }
        constexpr static inline HackIndex player_country(int r)                       { return HackIndex::prototype(r,  4, parentId::players); }
        constexpr static inline HackIndex player_money(int r)                         { return HackIndex::prototype(r,  5, parentId::players); }
        constexpr static inline HackIndex player_start_stack(int r)                   { return HackIndex::prototype(r,  6, parentId::players); }
        constexpr static inline HackIndex player_bounty_progressive_money(int r)      { return HackIndex::prototype(r,  7, parentId::players); }
        constexpr static inline HackIndex player_hash_avatar(int r)                   { return HackIndex::prototype(r,  8, parentId::players); }
        constexpr static inline HackIndex player_is_dealer(int r)                     { return HackIndex::prototype(r,  9, parentId::players); }
        constexpr static inline HackIndex player_cards(int r)                         { return HackIndex::prototype(r, 10, parentId::players); }
        constexpr static inline HackIndex player_round_bet(int r)                     { return HackIndex::prototype(r, 11, parentId::players); }
        constexpr static inline HackIndex player_is_current(int r)                    { return HackIndex::prototype(r, 12, parentId::players); }
        constexpr static inline HackIndex player_vip_status(int r)                    { return HackIndex::prototype(r, 13, parentId::players); }
        constexpr static inline HackIndex player_state(int r)                         { return HackIndex::prototype(r, 14, parentId::players); }
        constexpr static inline HackIndex player_card(int idx, int c)                 { return HackIndex::prototype(c,  0, *(parentId::player_cards+idx)); }
        constexpr static inline HackIndex player_state_left_table(int idx)            { return HackIndex::prototype(0,  0, *(parentId::player_state+idx)); }
        constexpr static inline HackIndex player_state_anticipant(int idx)            { return HackIndex::prototype(1,  0, *(parentId::player_state+idx)); }
        constexpr static inline HackIndex player_state_folded(int idx)                { return HackIndex::prototype(2,  0, *(parentId::player_state+idx)); }
        constexpr static inline HackIndex player_state_frozen(int idx)                { return HackIndex::prototype(3,  0, *(parentId::player_state+idx)); }
        constexpr static inline HackIndex player_state_sit_out(int idx)               { return HackIndex::prototype(4,  0, *(parentId::player_state+idx)); }
        constexpr static inline HackIndex player_state_all_ined(int idx)              { return HackIndex::prototype(5,  0, *(parentId::player_state+idx)); }
        constexpr static inline HackIndex player_state_all_ined_in_cur_round(int idx) { return HackIndex::prototype(6,  0, *(parentId::player_state+idx)); }

        constexpr inline static HackIndex root(const QAbstractItemModel* m) { return HackIndex::fromPrototype(root(), m); }
    };


    class API_MODEL_DLL table_state_t : public ApiBase
    {
        Q_OBJECT
    public:
        enum class element : int {
            begin = 0,
            _tournament_id = begin,
            _game_id,
            _big_blind,
            _seats_count,
            _players,
            _current_user_pos,
            _table_cards,
            _all_pots,
            _waiters_count,
            _time_short,
            _time_main,
            _time_action_passed,
            _time_action_left,
            _distribution_number,
            end,
            count = end
        };
        Q_ENUM(element)

        enum class element_of_players : int {
            offset = 0,//100 * int(element::_players),
            begin = offset,

            _id = begin,
            _is_user,
            _pos,
            _nickname,
            _country,
            _money,
            _start_stack,
            _bounty_progressive_money,
            _hash_avatar,
            _is_dealer,
            _cards,
            _round_bet,
            _is_current,
            _vip_status,
            _state,

            end,
            count = end - offset
        };
        Q_ENUM(element_of_players)

        uint calcMaxIndices(const gbp::ns_api::ns_table::table_state_t& data);
    private:
        QScopedPointer<table_state_data> m_data;
    public:
        explicit table_state_t(QObject *parent = nullptr);
        virtual ~table_state_t();

        QModelIndex root() const;
        static bool isRoot(const QModelIndex& index);
        static bool isPlayerCard(const QModelIndex& index);

        Q_SLOT void update(const table_state_data &data);
    public:

        inline QModelIndex index_tournament_id() const                            { return index(0, 0, root()); }
        inline QModelIndex index_game_id() const                                  { return index(1, 0, root()); }
        inline QModelIndex index_big_blind() const                                { return index(2, 0, root()); }
        inline QModelIndex index_seats_count() const                              { return index(3, 0, root()); }
        inline QModelIndex index_players() const                                  { return index(4, 0, root()); }
        inline QModelIndex index_current_user_pos() const                         { return index(5, 0, root()); }
        inline QModelIndex index_table_cards() const                              { return index(6, 0, root()); }
        inline QModelIndex index_table_card(int idx) const                        { return index(idx, 0, index_table_cards()); }
        inline QModelIndex index_all_pots() const                                 { return index(7, 0, root()); }
        inline QModelIndex index_pot(int idx) const                               { return index(idx, 0, index_all_pots()); }
        inline QModelIndex index_waiters_count() const                            { return index(8, 0, root()); }
        inline QModelIndex index_time_short() const                               { return index(9, 0, root()); }
        inline QModelIndex index_time_main() const                                { return index(10, 0, root()); }
        inline QModelIndex index_time_action_passed() const                       { return index(11, 0, root()); }
        inline QModelIndex index_time_action_left() const                         { return index(12, 0, root()); }
        inline QModelIndex index_distribution_number() const                      { return index(13, 0, root()); }
        inline QModelIndex index_player_id(int idx) const                         { return index(idx, 0, index_players()); }
        inline QModelIndex index_player_is_user(int idx) const                    { return index(idx, 1, index_players()); }
        inline QModelIndex index_player_pos(int idx) const                        { return index(idx, 2, index_players()); }
        inline QModelIndex index_player_nickname(int idx) const                   { return index(idx, 3, index_players()); }
        inline QModelIndex index_player_country(int idx) const                    { return index(idx, 4, index_players()); }
        inline QModelIndex index_player_money(int idx) const                      { return index(idx, 5, index_players()); }
        inline QModelIndex index_player_start_stack(int idx) const                { return index(idx, 6, index_players()); }
        inline QModelIndex index_player_bounty_progressive_money(int idx) const   { return index(idx, 7, index_players()); }
        inline QModelIndex index_player_hash_avatar(int idx) const                { return index(idx, 8, index_players()); }
        inline QModelIndex index_player_is_dealer(int idx) const                  { return index(idx, 9, index_players()); }
        inline QModelIndex index_player_cards(int idx) const                      { return index(idx, 10, index_players()); }
        inline QModelIndex index_player_round_bet(int idx) const                  { return index(idx, 11, index_players()); }
        inline QModelIndex index_player_is_current(int idx) const                 { return index(idx, 12, index_players()); }
        inline QModelIndex index_player_vip_status(int idx) const                 { return index(idx, 13, index_players()); }
        inline QModelIndex index_player_state(int idx) const                      { return index(idx, 14, index_players()); }
        inline QModelIndex index_player_card(int idx, int card) const             { return index(card, 0, index_player_cards(idx)); }
        inline QModelIndex index_player_state_left_table(int idx) const           { return index(0, 0, index_player_state(idx)); }
        inline QModelIndex index_player_state_anticipant(int idx) const           { return index(1, 0, index_player_state(idx)); }
        inline QModelIndex index_player_state_folded(int idx) const               { return index(2, 0, index_player_state(idx)); }
        inline QModelIndex index_player_state_frozen(int idx) const               { return index(3, 0, index_player_state(idx)); }
        inline QModelIndex index_player_state_sit_out(int idx) const              { return index(4, 0, index_player_state(idx)); }
        inline QModelIndex index_player_state_all_ined(int idx) const             { return index(5, 0, index_player_state(idx)); }
        inline QModelIndex index_player_state_all_ined_in_cur_round(int idx) const{ return index(6, 0, index_player_state(idx)); }

        inline QModelIndexRange indices_player_ids() const {
            return QModelIndexRange(index_player_id(0), index_player_id(rowCount(index_players())));
        }

    public:
        int playersCount() const;
        int potsCount() const;
        int tableCardsCount() const;
        int playersCardsCount(int playerRow) const;

        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int rowsCapacity(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnsCapacity(const QModelIndex &parent = QModelIndex()) const override;

        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        virtual bool setData(const QModelIndex &index, const QVariant &value, int role = UserType::OriginDataRole) override;
        template <typename T>
        bool setOriginData(const QModelIndex& index, const T& value) {
            return setData(index, QVariant::fromValue(value), UserType::OriginDataRole);
        }
        template <typename T>
        bool setOriginData(const T& value);

        virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
        virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
        virtual bool removeItems(const QList<gbp_i64> &ids, const QModelIndex &parent) override;
        virtual bool insertItems(const QList<gbp_i64> &ids, const QModelIndex &parent) override;


        gbp::ns_api::ns_table::table_state_t& origin();
        const gbp::ns_api::ns_table::table_state_t& origin() const;

        QModelIndex findElementOfPlayer(gbp_i64 id, element_of_players el) const;
        QModelIndex indexOf(element el) const;
        QModelIndex indexOf(element_of_players el, int player) const;
        virtual QModelIndex index(int row, int col, const QModelIndex &parent = QModelIndex()) const override;
        virtual QModelIndex parent(const QModelIndex &child) const override;
        virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;


        inline QModelIndexRange row_slice(int row, const QModelIndex& parent) const {
            return QModelIndexRange::rowRange(parent, row, 0, columnCount(parent));
        }
        inline QModelIndexRange column_slice(int column, const QModelIndex& parent) const {
            return QModelIndexRange::columnRange(parent, column, 0, rowCount(parent));
        }
        inline QModelIndexRange childrenOf(const QModelIndex& index) const {
            return QModelIndexRange::parentRange(index);
        }
        void refresh();
    protected:
        int returnDataChanged(const QModelIndex& index, const QVector<int>& roles = QVector<int>()) {
            emit dataChanged(index, index, roles);
            return 0;
        }
        // 0 - set, 1 - invalid index, 2 - same value otherwise __LINE__
        ns_model::SetDataInfo::Results trySetData(const QModelIndex &index, const QVariant &value, int role = UserType::OriginDataRole);
        QModelIndex createTableElementIndex(int row, int column) const;
        QModelIndex createPlayerElementIndex(int row, int column) const;
        QModelIndex createTableCardIndex(int row, int column) const;
        QModelIndex createTablePotIndex(int row, int column) const;
        QModelIndex createPlayerCardIndex(int row, int column, int player) const;
        QModelIndex createPlayerStateIndex(int row, int column, int player) const;
    public:
        virtual QModelIndexList match(const QModelIndex &start, int role, const QVariant &value, int hits, Qt::MatchFlags flags) const override;
        virtual bool read(QIODevice *iodevice) override;
        virtual bool write(QIODevice *iodevice) const override;
        virtual bool isItemOrientedData() const override;
        virtual int rowById(gbp_i64 idAt, const void *data, const QModelIndex &parent) const override;
        virtual int propertyIndex(const QByteArray &name, const QModelIndex &parent) const override;
    };

} //namespace ns_model
