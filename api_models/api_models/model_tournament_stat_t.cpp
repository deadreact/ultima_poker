#include "model_tournament_stat_t.hpp"
#include "../poker_client/src/core/money.h"

#ifndef Q_MOC_RUN
    #include <api/api_common.h>
    #include <api/lobby_stat/types.h>
    #include <api/lobby_stat/reply.h>
#endif // !Q_MOC_RUN

#include <rpcbase/rpcbase_statistic.h>
#include "qdatastream_io.hpp"
#include <qdatetime.h>
#include <qlocale.h>
#include <basewidgets/types.h>
#include "api_util.hpp"
#include "itemtype_concept.hpp"


#define COMPARE(__item1, __item2, __cnt, __role, __property)\
if ((__item1.__property) != (__item2.__property)) {\
    __cnt[underlying_cast(column::__property)].push_back(__role);\
    setData(indexOf(__item1._id, column::__property), QVariant::fromValue(__item2.__property), __role);\
}

namespace
{
    using e_tournament_type     = gbp::ns_api::ns_tournament_types::e_tournament_type;
    using e_start_time          = gbp::ns_api::ns_tournament_types::e_start_time;
    using e_chips_rebuy         = gbp::ns_api::ns_tournament_types::e_chips_rebuy;
    using e_win_result          = gbp::ns_api::ns_tournament_types::e_win_result;
    using e_win_prize           = gbp::ns_api::ns_tournament_types::e_win_prize;
    using e_money_in            = gbp::ns_api::ns_tournament_types::e_money_in;
    using e_registration        = gbp::ns_api::ns_tournament_types::e_registration;
    using e_tournament_status   = gbp::ns_api::ns_tournament_types::e_tournament_status;
    using e_tournament_stack    = gbp::ns_api::ns_tournament_types::e_tournament_stack;

    using e_tourn_button_type   = gbp::ns_api::ns_statistic::e_tourn_button_type;

    using e_game_type           = gbp::ns_api::ns_game::e_game_type;
    using e_game_speed          = gbp::ns_api::ns_game::e_game_speed;
    using e_game_limit          = gbp::ns_api::ns_game::e_game_limit;


}

template <typename enum_type> struct identifier;
template <> struct identifier<ns_model::tournament_stat_t::column> {
    using parent_type = void;
    using type = gbp_i64;
};
template <> struct identifier<ns_model::tournament_stat_t::users> {
    using parent_type = typename identifier<ns_model::tournament_stat_t::column>::type;
    using type = QPair<parent_type, gbp_i64>;
    static const ns_model::tournament_stat_t::column root = ns_model::tournament_stat_t::column::_users;
};
template <> struct identifier<ns_model::tournament_stat_t::rebuys> {
    using parent_type = typename identifier<ns_model::tournament_stat_t::column>::type;
    using type = QPair<parent_type, int>;
    static const ns_model::tournament_stat_t::column root = ns_model::tournament_stat_t::column::_rebuys;
};
template <> struct identifier<ns_model::tournament_stat_t::addon> {
    using parent_type = typename identifier<ns_model::tournament_stat_t::column>::type;
    using type = QPair<parent_type, int>;
    static const ns_model::tournament_stat_t::column root = ns_model::tournament_stat_t::column::_addon;
};

template <typename enum_type, typename identifier_type = std::enable_if<std::is_enum<enum_type>::value, typename identifier<enum_type>::type>>
struct internal_id {
    using type = QPair<enum_type, typename identifier_type::type>;
};

Q_DECLARE_METATYPE(gbp::ns_api::ns_statistic::tournament_stat_t)
Q_DECLARE_METATYPE(decltype(gbp::ns_api::ns_statistic::tournament_stat_t::_rebuys))
Q_DECLARE_METATYPE(decltype(gbp::ns_api::ns_statistic::tournament_stat_t::_addon))
Q_DECLARE_METATYPE(decltype(gbp::ns_api::ns_statistic::tournament_stat_t::_users))
Q_DECLARE_METATYPE(gbp::ns_api::ns_statistic::tournament_stat_t::user_info)
Q_DECLARE_METATYPE(e_tournament_type)
Q_DECLARE_METATYPE(e_tourn_button_type)
Q_DECLARE_METATYPE(e_tournament_status)

Q_DECLARE_METATYPE(e_game_type)
Q_DECLARE_METATYPE(e_game_speed)
Q_DECLARE_METATYPE(e_game_limit)

namespace ns_meta
{
    using tournament_types_type = generic_type<e_tournament_type, uint, uint>;

    template<> tournament_types_type::value_type tournament_types_type::convert(const edit_type &value) {
        return (e_tournament_type)value;
    }
    template<> tournament_types_type::edit_type tournament_types_type::edit(const value_type &value) {
        return (edit_type)value;
    }
    template<> tournament_types_type::display_type tournament_types_type::display(const value_type &value) {
        return (display_type)value;
    }
} //namespace ns_meta


template <typename Type, bool is_top_level = std::is_void<typename identifier<Type>::parent_type>::value >
struct internalIdMatcher
{
    using id_type = typename internal_id<Type>::type;

    static QModelIndex match(const ns_model::tournament_stat_t* m, const QVariant& value, const QModelIndex& start = QModelIndex(), bool recursive = false) {
        if (!value.canConvert<id_type>()) {
            return QModelIndex();
        }
        auto id = value.value<id_type>();
        QModelIndex idx = m->indexOf(id.second, id.first);
        if (!start.isValid() || start < idx || recursive) {
            return idx;
        }
        return QModelIndex();
    }
};

template <typename Type>
struct internalIdMatcher<Type, false>
{
    using id_type = typename internal_id<Type>::type;

    static QModelIndex match(const ns_model::tournament_stat_t* m, const QVariant& value, const QModelIndex& start = QModelIndex(), bool recursive = false) {
        if (!value.canConvert<id_type>()) {
            return QModelIndex();
        }
        auto id = value.value<id_type>();
        QModelIndex parentIndex = m->indexOf(id.second.first, identifier<Type>::root);
        if (parentIndex.isValid()) {
            QModelIndex idx = m->index(id.second.second, underlying_cast(id.first), parentIndex);
            if (!start.isValid() || start < idx || recursive) {
                return idx;
            }
        }
        return QModelIndex();
    }
};

namespace ns_model
{


void tournament_stat_t::updateData(const games_stat_t &data)
{
//    games_stat_t::second_type buff = data.second;
    updateItems(*this, m_data, data.second);
//    std::stable_sort(buff.begin(), buff.end(), [](const games_stat_t::second_type::value_type& item1, const games_stat_t::second_type::value_type& item2)
//                                               { return item1._id < item2._id; });

//    if (m_data.size() != buff.size())
//    {
//        beginResetModel();
//        std::swap(m_data, buff);
//        endResetModel();
//    }
//    else
//    {
//        for (data_t::size_type i = 0; i < buff.size(); i++) {
//            updateRowItem(i, buff.at(i));
//        }
//    }
}

tournament_stat_t::tournament_stat_t(QObject *parent)
        : ApiBase(parent)
        , m_data()
        , m_pData()
        , m_rpc(nullptr)
        , m_canFetchMore(false)
    {

        qRegisterMetaType<e_tournament_type>("e_tournament_type");
        qRegisterMetaType<e_tourn_button_type>("e_tourn_button_type");
        qRegisterMetaType<e_tournament_status>("e_tournament_status");

        qRegisterMetaType<e_game_type>("e_game_type");
        qRegisterMetaType<e_game_speed>("e_game_speed");
        qRegisterMetaType<e_game_limit>("e_game_limit");
    }

    tournament_stat_t::~tournament_stat_t()
    {
        qDeleteAll(m_pData);
    }

    void tournament_stat_t::init()
    {
        //        connect(pApp->client().emitter<reply_lobby_stat>(), &emitter_statistic::statisticUpdated, this, &tournament_stat_t::onStatisticUpdated);
    }

    void tournament_stat_t::setRpc(ns_statistic::rpc_object *rpc) {
        m_rpc = rpc;
    }

    void tournament_stat_t::updateRowItem(int row, const gbp::ns_api::ns_statistic::tournament_stat_t &newItem)
    {
        const gbp::ns_api::ns_statistic::tournament_stat_t& oldItem = m_data.at(row);

        QMap<int, QVector<int>> diffs;

        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _id)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _seats_count)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _max_players_count)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _is_current_user_registered)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _is_current_user_registered)

        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _players_registered_count)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _sit_n_go_users_count)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _main_tournament_id)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _buyin)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _big_blind)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _round_duration)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _start_stack)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _prize_places_count)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _common_prize)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _types)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _game_type)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _game_speed)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _game_limit)
//            COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _title)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _status)
//                COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _is_partner_tourn)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _registration_start_time)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _start_time)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _finish_time)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _button_state)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _late_registration_duration)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _bounty)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _player_bounty_value)
        COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _current_level)

        if (oldItem._qualifiers_ids.size() != newItem._qualifiers_ids.size()) {
            QModelIndex parent = indexOf(oldItem._id, column::_qualifiers_ids);

            if (oldItem._qualifiers_ids.size() < newItem._qualifiers_ids.size()) {
                insertRows(rowCount(parent) - 1, newItem._qualifiers_ids.size() - oldItem._qualifiers_ids.size(), parent);
            } else {
                removeRows(rowCount(parent) - 1, oldItem._qualifiers_ids.size() - newItem._qualifiers_ids.size(), parent);
            }
        }

//            COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _rebuys)
//            COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _addon)
//            COMPARE(oldItem, newItem, diffs, Qt::DisplayRole, _users)
    }

    QModelIndex tournament_stat_t::indexOf(gbp_i64 id) const
    {
        return indexOf(id, column(0));
    }

    QModelIndex tournament_stat_t::indexOf(gbp_i64 id, column field) const
    {
        for (size_t i = 0; i < m_data.size(); i++) {
            if (m_data.at(i)._id == id) {
                return index(i, underlying_cast(field));
            }
        }
        return QModelIndex();
    }



    Accessor<tournament_stat_t::value_t> tournament_stat_t::itemAt(int row) const
    {
        return Accessor<gbp::ns_api::ns_statistic::tournament_stat_t>(m_data.data() + row);
    }

    Accessor<tournament_stat_t::value_t> tournament_stat_t::item(gbp_i64 id) const
    {
        for (const gbp::ns_api::ns_statistic::tournament_stat_t& d: m_data) {
            if (d._id == id) {
                return Accessor<gbp::ns_api::ns_statistic::tournament_stat_t>(&d);
            }
        }
        return Accessor<gbp::ns_api::ns_statistic::tournament_stat_t>();
    }

    const gbp::ns_api::ns_statistic::tournament_stat_t &tournament_stat_t::defaultRowItem()
    {
        static value_t item;
        return item;
    }

    int tournament_stat_t::rowCount(const QModelIndex &parent) const
    {
        if (m_data.empty()) {
            return 0;
        }
        if (parent.isValid())
        {
            if (!parent.parent().isValid())
            {
                if (static_cast<int>(m_data.size()) <= parent.row()) {
                    return 0;
                }
                const auto& item = m_data.at(parent.row());
                switch (column(parent.column())) {
                case column::_qualifiers_ids: return item._qualifiers_ids.size();
                case column::_rebuys:         return item._types & e_chips_rebuy::cr_rebuy ? 1 : 0;
                case column::_addon:          return item._addon._is_available ? 1 : 0;
                case column::_users:          return item._users.size();
                default:
                    return 0;
                }
            }
            return 0;
        }

        return m_data.size();
    }

    int tournament_stat_t::columnCount(const QModelIndex &parent) const
    {
        if (parent.isValid())
        {
            if (!parent.parent().isValid())
            {
                switch (column(parent.column())) {
                case column::_qualifiers_ids: return 1;
                case column::_rebuys: return 3;
                case column::_addon: return 3/*2*/;
                case column::_users: return 7;
                default:
                    return 0;
                }
            }
            return 0;
        }

        return underlying_cast(column::count);
    }

    int foo(const int&) {return 42;}

    QVariant tournament_stat_t::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid()) {
            return QVariant();
        }

        if (role == UserType::SortRole) {
            return dataSortRole(index);
        } /*else if (role == UserType::FilterRole) {
            return dataFilterRole(index);
        }*/

        if (!index.parent().isValid())
        {
            const gbp::ns_api::ns_statistic::tournament_stat_t& item = itemAt(index.row()).ref();
            switch (role) {
            case UserType::RowDataRole:
                return QVariant::fromValue(item);
            case UserType::RowItemIdRole:
                return QVariant::fromValue(item._id);
            case UserType::InternalIdRole:
                return QVariant::fromValue(qMakePair(column(index.column()), item._id));
            case UserType::OriginDataRole:
                switch (column(index.column())) {
                    case column::_id:                         return QVariant::fromValue(item._id);
                    case column::_seats_count:                return QVariant::fromValue(item._seats_count);
                    case column::_max_players_count:          return QVariant::fromValue(item._max_players_count);
                    case column::_is_current_user_registered: return QVariant::fromValue(item._is_current_user_registered);
                    case column::_players_registered_count:   return QVariant::fromValue(item._players_registered_count);
                    case column::_sit_n_go_users_count:       return QVariant::fromValue(item._sit_n_go_users_count);
                    case column::_main_tournament_id:         return QVariant::fromValue(item._main_tournament_id);
                    case column::_buyin:                      return QVariant::fromValue(item._buyin);
                    case column::_big_blind:                  return QVariant::fromValue(item._big_blind);
                    case column::_round_duration:             return QVariant::fromValue(item._round_duration);
                    case column::_start_stack:                return QVariant::fromValue(item._start_stack);
                    case column::_prize_places_count:         return QVariant::fromValue(item._prize_places_count);
                    case column::_common_prize:               return QVariant::fromValue(item._common_prize);
                    case column::_types:                      return QVariant::fromValue(item._types);
                    case column::_game_type:                  return QVariant::fromValue(item._game_type);
                    case column::_game_speed:                 return QVariant::fromValue(item._game_speed);
                    case column::_game_limit:                 return QVariant::fromValue(item._game_limit);
                    case column::_title:                      return QVariant::fromValue(item._title);
                    case column::_status:                     return QVariant::fromValue(item._status);
                    case column::_is_partner_tourn:           return QVariant::fromValue(item._is_partner_tourn);
                    case column::_registration_start_time:    return QVariant::fromValue(item._registration_start_time);
                    case column::_start_time:                 return QVariant::fromValue(item._start_time);
                    case column::_finish_time:                return QVariant::fromValue(item._finish_time);
                    case column::_button_state:               return QVariant::fromValue(item._button_state);
                    case column::_late_registration_duration: return QVariant::fromValue(item._late_registration_duration);
                    case column::_qualifiers_ids:             return QVariant::fromValue(item._qualifiers_ids);
                    case column::_rebuys:                     return QVariant::fromValue(item._rebuys);
                    case column::_addon:                      return QVariant::fromValue(item._addon);
                    case column::_bounty:                     return QVariant::fromValue(item._bounty);
                    case column::_player_bounty_value:        return QVariant::fromValue(item._player_bounty_value);
                    case column::_current_level:              return QVariant::fromValue(item._current_level);
                    case column::_users:                      return QVariant::fromValue(item._users);
                default:
                    return QVariant();
                }
            case Qt::DisplayRole:
                switch (column(index.column())) {
                case column::_id:                         return ns_meta::id_type::display(item._id);
                case column::_seats_count:                return ns_meta::count_type::display(item._seats_count);
                case column::_max_players_count:          return ns_meta::count_type::display(item._max_players_count);
                case column::_is_current_user_registered: return ns_meta::flag_type::display(item._is_current_user_registered);
                case column::_players_registered_count:   return ns_meta::count_type::display(item._players_registered_count);
                case column::_sit_n_go_users_count:       return ns_meta::count_type::display(item._sit_n_go_users_count);
                case column::_main_tournament_id:         return ns_meta::id_type::display(item._main_tournament_id);
                case column::_buyin:                      return ns_meta::money_type::display(item._buyin);
                case column::_big_blind:                  return ns_meta::money_type::display(item._big_blind);
                case column::_round_duration:             return ns_meta::duration_type::display(item._round_duration);
                case column::_start_stack:                return ns_meta::money_type::display(item._start_stack);
                case column::_prize_places_count:         return ns_meta::count_type::display(item._prize_places_count);
                case column::_common_prize:               return ns_meta::money_type::display(item._common_prize);
                case column::_types:                      return QVariant::fromValue(item._types);
                case column::_game_type:                  return ns_meta::tr_enum_type::display(item._game_type);
                case column::_game_speed:                 return ns_meta::tr_enum_type::display(item._game_speed);
                case column::_game_limit:                 return ns_meta::tr_enum_type::display(item._game_limit);
                case column::_title:                      return ns_meta::name_type::display(item._title);
                case column::_status:                     return ns_meta::tr_enum_type::display(item._status);
                case column::_is_partner_tourn:           return ns_meta::flag_type::display(item._is_partner_tourn);
                case column::_registration_start_time:    return ns_meta::datetime_type::display(item._registration_start_time);
                case column::_start_time:                 return ns_meta::datetime_type::display(item._start_time);
                case column::_finish_time:                return ns_meta::datetime_type::display(item._finish_time);
                case column::_button_state:               return ns_meta::enum_type::display(item._button_state);
                case column::_late_registration_duration: return ns_meta::duration_type::display(item._late_registration_duration);
                case column::_qualifiers_ids:             return QVariant::fromValue(item._qualifiers_ids);
                case column::_rebuys:                     return QVariant::fromValue(item._rebuys);
                case column::_addon:                      return QVariant::fromValue(item._addon);
                case column::_bounty:                     return ns_meta::money_type::display(item._bounty);
                case column::_player_bounty_value:        return ns_meta::money_type::display(item._player_bounty_value);
                case column::_current_level:              return ns_meta::count_type::display(item._current_level);
                case column::_users:                      return QVariant::fromValue(item._users);
                default:
                    return QVariant();
                }
            case Qt::CheckStateRole:
                if (column(index.column()) == column::_is_current_user_registered) {
                    return item._is_current_user_registered;
                }
                return false;
            default:
                return QVariant();
            }
        }
        else if (!index.parent().parent().isValid())
        {
            const gbp::ns_api::ns_statistic::tournament_stat_t& parentItem = m_data.at(index.parent().row());

            switch (column(index.parent().column())) {
            case column::_qualifiers_ids:
                switch (role) {
                case UserType::OriginDataRole:
                case Qt::DisplayRole:
                case Qt::ToolTipRole:
                case UserType::RowItemIdRole:
                case UserType::InternalIdRole:
                    return QVariant::fromValue(parentItem._qualifiers_ids.at(index.row()));
                case UserType::RowDataRole:
                    return QVariant::fromValue(parentItem._qualifiers_ids);
                default:
                    return QVariant();
                }
            case column::_rebuys:
                if (index.row() == 0)
                {
                    const auto &item = parentItem._rebuys;

                    switch (role) {
                    case UserType::RowDataRole:
                        return QVariant::fromValue(item);
                    case UserType::RowItemIdRole:
                        return QVariant::fromValue(parentItem._id);
                    case UserType::OriginDataRole:
                        switch (rebuys(index.column())) {
                        case rebuys::_count: return QVariant::fromValue(item._count);
                        case rebuys::_money: return QVariant::fromValue(item._money);
                        case rebuys::_chips: return QVariant::fromValue(item._chips);
                        default:
                            return QVariant();
                        }
                    case Qt::DisplayRole:
                    case Qt::ToolTipRole:
                        switch (rebuys(index.column())) {
                            case rebuys::_count: return ns_meta::count_type::display(item._count);
                            case rebuys::_money: return ns_meta::money_type::display(item._money);
                            case rebuys::_chips: return ns_meta::money_type::display(item._chips);
                            default:
                                return QVariant();
                        }
                    case Qt::EditRole:
                        switch (rebuys(index.column())) {
                            case rebuys::_count: return ns_meta::count_type::edit(item._count);
                            case rebuys::_money: return ns_meta::money_type::edit(item._money);
                            case rebuys::_chips: return ns_meta::money_type::edit(item._chips);
                            default:
                                return QVariant();
                        }
                    case UserType::InternalIdRole:
                        return QVariant::fromValue(qMakePair(rebuys(index.column()), qMakePair(parentItem._id, index.row())));
                    default:
                        return QVariant();
                    }

                }
                return QVariant();
            case column::_addon:
                if (index.row() == 0)
                {
                    const auto& item = parentItem._addon;
                    addon field = addon(index.column());

                    switch (role) {
                    case UserType::RowDataRole:
                        return QVariant::fromValue(item);
                    case UserType::RowItemIdRole:
                        return QVariant::fromValue(parentItem._id);
                    case UserType::OriginDataRole:
                        switch (field) {
                            case addon::_is_available: return QVariant::fromValue(item._is_available);
                            case addon::_money:        return QVariant::fromValue(item._money);
                            case addon::_chips:        return QVariant::fromValue(item._chips);
                            default:
                                return QVariant();
                        }
                    case Qt::DisplayRole:
                    case Qt::ToolTipRole:
                        switch (field) {
                            case addon::_is_available: return ns_meta::flag_type::display(item._is_available);
                            case addon::_money:        return ns_meta::money_type::display(item._money);
                            case addon::_chips:        return ns_meta::money_type::display(item._chips);
                            default:
                                return QVariant();
                        }
                    case Qt::EditRole:
                        switch (field) {
                            case addon::_is_available: return ns_meta::flag_type::edit(item._is_available);
                            case addon::_money:        return ns_meta::money_type::edit(item._money);
                            case addon::_chips:        return ns_meta::money_type::edit(item._chips);
                            default:
                                return QVariant();
                        }
                    case UserType::InternalIdRole:
                        return QVariant::fromValue(qMakePair(field, qMakePair(parentItem._id, index.row())));
                    default:
                        return QVariant();
                    }
                }
                return QVariant();
            case column::_users:
            {
                const auto& item = parentItem._users.at(index.row());
                users field = users(index.column());

                switch (role) {
                case UserType::RowDataRole:
                    return QVariant::fromValue(item);
                case UserType::RowItemIdRole:
                    return QVariant::fromValue(item._id);
                case UserType::OriginDataRole:
                    switch (field) {
                        case users::_id:                       return QVariant::fromValue(item._id);
                        case users::_nickname:                 return QVariant::fromValue(item._nickname);
                        case users::_country:                  return QVariant::fromValue(item._country);
                        case users::_is_ticket:                return QVariant::fromValue(item._is_ticket);
                        case users::_money:                    return QVariant::fromValue(item._money);
                        case users::_bounty_knocked_out_count: return QVariant::fromValue(item._bounty_knocked_out_count);
                        case users::_bounty_progr_money:       return QVariant::fromValue(item._bounty_progr_money);
                        default:
                            return QVariant();
                    }
                case Qt::DisplayRole:
                case Qt::ToolTipRole:
                    switch (field) {
                        case users::_id:                       return ns_meta::id_type::display(item._id);
                        case users::_nickname:                 return ns_meta::name_type::display(item._nickname);
                        case users::_country:                  return ns_meta::country_type::display(item._country);
                        case users::_is_ticket:                return ns_meta::flag_type::display(item._is_ticket);
                        case users::_money:                    return ns_meta::money_type::display(item._money);
                        case users::_bounty_knocked_out_count: return ns_meta::money_type::display(item._bounty_knocked_out_count);
                        case users::_bounty_progr_money:       return ns_meta::money_type::display(item._bounty_progr_money);
                        default:
                            return QVariant();
                    }
                case Qt::EditRole:
                    switch (field) {
                        case users::_id:                       return ns_meta::id_type::edit(item._id);
                        case users::_nickname:                 return ns_meta::name_type::edit(item._nickname);
                        case users::_country:                  return ns_meta::country_type::edit(item._country);
                        case users::_is_ticket:                return ns_meta::flag_type::edit(item._is_ticket);
                        case users::_money:                    return ns_meta::money_type::edit(item._money);
                        case users::_bounty_knocked_out_count: return ns_meta::money_type::edit(item._bounty_knocked_out_count);
                        case users::_bounty_progr_money:       return ns_meta::money_type::edit(item._bounty_progr_money);
                        default:
                            return QVariant();
                    }
                case UserType::InternalIdRole:
                    return QVariant::fromValue(qMakePair(field, qMakePair(parentItem._id, item._id)));
                default:
                    return QVariant();
                }
            }
            default:
                return QVariant();
            }
        }

        return QVariant();
    }

    bool tournament_stat_t::setData(const QModelIndex &idx, const QVariant &value, int role)
    {
        if (!idx.isValid()) {
            return false;
        }
        if (data(idx, role) != value) {
            if (!idx.parent().isValid())
            {
                if (role == UserType::RowDataRole)
                {
                    if (value.canConvert<value_t>()) {
                        value_t newItem = value.value<value_t>();
                        value_t& itemRef = m_data.at(idx.row());
                        if (itemRef._id == newItem._id) {
                            itemRef = newItem;
                            emit dataChanged(idx.sibling(idx.row(), 0), idx.sibling(idx.row(), columnCount(idx.parent()) - 1));
                            return true;
                        }
                    }
                    return false;
                }

                gbp::ns_api::ns_statistic::tournament_stat_t& item = m_data.at(idx.row());
                if (role == Qt::DisplayRole || role == UserType::OriginDataRole)
                {
                    switch (column(idx.column())) {
                    case column::_id:                         return false;
                    case column::_seats_count:                return false;
                    case column::_max_players_count:          return false;
                    case column::_is_current_user_registered: item._is_current_user_registered = value.toBool();             break;
                    case column::_players_registered_count:   item._players_registered_count = value.toInt();                break;
                    case column::_sit_n_go_users_count:       return false;
                    case column::_main_tournament_id:         return false;
                    case column::_buyin:                      item._buyin = value.value<gbp_i64>();                          break;
                    case column::_big_blind:                  item._big_blind = value.value<gbp_i64>();                      break;
                    case column::_round_duration:             item._round_duration = value.value<gbp_i64>();                 break;
                    case column::_start_stack:                item._start_stack = value.value<gbp_i64>();                    break;
                    case column::_prize_places_count:         item._prize_places_count = value.toInt();                      break;
                    case column::_common_prize:               item._common_prize = value.value<gbp_i64>();                   break;
                    case column::_types:                      item._types = value.value<e_tournament_type>();                break;
                    case column::_game_type:                  item._game_type  = value.value<e_game_type>();                 break;
                    case column::_game_speed:                 item._game_speed = value.value<e_game_speed>();                break;
                    case column::_game_limit:                 item._game_limit = value.value<e_game_limit>();                break;
                    case column::_title:                      item._title = value.toString().toStdString();                  break;
                    case column::_status:                     item._status = value.value<e_tournament_status>();             break;
                    case column::_is_partner_tourn:           item._is_partner_tourn = value.toBool();                       break;
                    case column::_registration_start_time:    item._registration_start_time = value.toDateTime().toTime_t(); break;
                    case column::_start_time:                 item._start_time  = value.toDateTime().toTime_t();             break;
                    case column::_finish_time:                item._finish_time = value.toDateTime().toTime_t();             break;
                    case column::_button_state:               item._button_state = value.value<e_tourn_button_type>();       break;
                    case column::_late_registration_duration: item._late_registration_duration = value.value<gbp_i64>();     break;
                    case column::_qualifiers_ids:             item._qualifiers_ids = value.value<std::vector<gbp_i64>>();    break;
                    case column::_rebuys:                     return false;
                    case column::_addon:                      return false;
                    case column::_bounty:                     item._bounty = value.value<gbp_i64>();                         break;
                    case column::_player_bounty_value:        item._player_bounty_value = value.value<gbp_i64>();            break;
                    case column::_current_level:              item._current_level = value.toInt();                           break;
                    case column::_users:
                    {
                        if (value.canConvert<decltype(item._users)>()) {
                            auto val = value.value<decltype(item._users)>();
                            updateItems(*this, item._users, val, idx);
                        }
                        return false;
                    }
//                        item._users = value.value<std::vector<gbp::ns_api::ns_statistic::tournament_stat_t::user_info>>();
//                        break;
                    default:
                        return false;
                    }
                }
                else if (role == Qt::EditRole)
                {
                    using namespace ns_meta;
                    auto on_edit = [this](bool result, const QModelIndex& idx) {
                        if (result) {
                            emit dataChanged(idx, idx);
                            return true;
                        }
                        return false;
                    };

                    switch (column(idx.column())) {
                    case column::_id:                         return on_edit(editModelData<id_type>(item._id, value), idx);
                    case column::_seats_count:                return on_edit(editModelData<pos_type>(item._seats_count, value), idx);
                    case column::_max_players_count:          return on_edit(editModelData<count_type>(item._max_players_count, value), idx);
                    case column::_is_current_user_registered: return on_edit(editModelData<flag_type>(item._is_current_user_registered, value), idx);
                    case column::_players_registered_count:   return on_edit(editModelData<count_type>(item._players_registered_count, value), idx);
                    case column::_sit_n_go_users_count:       return on_edit(editModelData<count_type>(item._sit_n_go_users_count, value), idx);
                    case column::_main_tournament_id:         return on_edit(editModelData<id_type>(item._main_tournament_id, value), idx);
                    case column::_buyin:                      return on_edit(editModelData<money_type>(item._buyin, value), idx);
                    case column::_big_blind:                  return on_edit(editModelData<money_type>(item._big_blind, value), idx);
                    case column::_round_duration:             return on_edit(editModelData<duration_type>(item._round_duration, value), idx);
                    case column::_start_stack:                return on_edit(editModelData<money_type>(item._start_stack, value), idx);
                    case column::_prize_places_count:         return on_edit(editModelData<count_type>(item._prize_places_count, value), idx);
                    case column::_common_prize:               return on_edit(editModelData<money_type>(item._common_prize, value), idx);
                    case column::_types:                      return on_edit(editModelData<tournament_types_type>(item._types, value), idx);
                    case column::_game_type:                  return on_edit(editModelData<generic_enum_type<decltype(item._game_type)>>(item._game_type, value), idx);
                    case column::_game_speed:                 return on_edit(editModelData<generic_enum_type<decltype(item._game_speed)>>(item._game_speed, value), idx);
                    case column::_game_limit:                 return on_edit(editModelData<generic_enum_type<decltype(item._game_limit)>>(item._game_limit, value), idx);
                    case column::_title:                      return on_edit(editModelData<text_type>(item._title, value), idx);
                    case column::_status:                     return on_edit(editModelData<generic_enum_type<decltype(item._status)>>(item._status, value), idx);
                    case column::_is_partner_tourn:           return on_edit(editModelData<flag_type>(item._is_partner_tourn, value), idx);
                    case column::_registration_start_time:    return on_edit(editModelData<datetime_type>(item._registration_start_time, value), idx);
                    case column::_start_time:                 return on_edit(editModelData<datetime_type>(item._start_time, value), idx);
                    case column::_finish_time:                return on_edit(editModelData<datetime_type>(item._finish_time, value), idx);
                    case column::_button_state:               return on_edit(editModelData<generic_enum_type<decltype(item._button_state)>>(item._button_state, value), idx);
                    case column::_late_registration_duration: return on_edit(editModelData<duration_type>(item._late_registration_duration, value), idx);
                    case column::_qualifiers_ids:             return on_edit(editModelData<generic_container_type<decltype(item._qualifiers_ids)>>(item._qualifiers_ids, value), idx);
                    case column::_rebuys:                     return false;
                    case column::_addon:                      return false;
                    case column::_bounty:                     return on_edit(editModelData<money_type>(item._bounty, value), idx);
                    case column::_player_bounty_value:        return on_edit(editModelData<money_type>(item._player_bounty_value, value), idx);
                    case column::_current_level:              return on_edit(editModelData<generic_count_type<gbp_i16>>(item._current_level, value), idx);
                    case column::_users:                      return false;
//                        item._users = value.value<std::vector<gbp::ns_api::ns_statistic::tournament_stat_t::user_info>>();
//                        break;
                    default:
                        return false;
                    }
                }
            }
            else if (!idx.parent().parent().isValid())
            {
                if (role == Qt::DisplayRole)
                {
                    gbp::ns_api::ns_statistic::tournament_stat_t& parentItem = m_data.at(idx.parent().row());
                    switch (column(idx.parent().column())) {
                    case column::_qualifiers_ids:
                    {
                        if (idx.column() != 0) {
                            return false;
                        }
                        parentItem._qualifiers_ids[idx.row()] = value.value<gbp_i64>();
                        emit dataChanged(idx, idx);
                        return true;
                    }
                    case column::_rebuys:
                    {
                        switch (rebuys(idx.column())) {
                        case rebuys::_count: parentItem._rebuys._count = value.value<gbp_i16>(); break;
                        case rebuys::_money: parentItem._rebuys._money = value.value<gbp_i64>(); break;
                        case rebuys::_chips: parentItem._rebuys._chips = value.value<gbp_i64>(); break;
                        default:
                            return false;
                        }
                        emit dataChanged(idx, idx);
                        return true;
                    }
                    case column::_addon:
                    {
                        switch (addon(idx.column())) {
                        case addon::_is_available: parentItem._addon._is_available = value.toBool();  break;
                        case addon::_money:        parentItem._addon._money = value.value<gbp_i64>(); break;
                        case addon::_chips:        parentItem._addon._chips = value.value<gbp_i64>(); break;
                        default:
                            return false;
                        }
                        emit dataChanged(idx, idx);
                        return true;
                    }
                    case column::_users:
                    {
                        auto& item = parentItem._users.at(idx.row());
                        switch (role) {
                        case UserType::RowDataRole:
                            return setData(idx.parent(), value, UserType::OriginDataRole);
                        default:
                            switch (users(idx.column())) {
                            case users::_id:                       item._id                       = value.value<gbp_i64>(); break;
                            case users::_nickname:                 item._nickname                 = value.toString().toStdString(); break;
                            case users::_country:                  item._country                  = underlying_cast(value.value<QLocale::Country>()); break;
                            case users::_money:                    item._money                    = value.value<gbp_i64>(); break;
                            case users::_is_ticket:                item._is_ticket                = value.toBool(); break;
                            case users::_bounty_knocked_out_count: item._bounty_knocked_out_count = value.value<gbp_i64>(); break;
                            case users::_bounty_progr_money:       item._bounty_progr_money       = value.value<gbp_i64>(); break;
                            default:
                                return false;
                            }
                            emit dataChanged(idx, idx);
                            return true;
                        }
                        emit dataChanged(idx, idx);
                        return true;
                    }
                    default:
                        return false;
                    }
                }
            }
            else
            {
                return false;
            }

            emit dataChanged(idx, idx, QVector<int>() << role);
            return true;
        }
        return false;
    }

    Qt::ItemFlags tournament_stat_t::flags(const QModelIndex &index) const
    {
        if (!index.isValid()) {
            return Qt::NoItemFlags;
        }

        auto commonFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

        if (!index.parent().isValid())
        {
            gbp::ns_api::ns_statistic::tournament_stat_t item = m_data.at(index.row());
            switch (column(index.column())) {
            case column::_id:                         return Qt::ItemIsEnabled;
            case column::_seats_count:                return commonFlags;
            case column::_max_players_count:          return commonFlags;
            case column::_is_current_user_registered: return /*(pApp->client().userId() != -1) ? */Qt::ItemIsEnabled/* | Qt::ItemIsUserCheckable : Qt::NoItemFlags*/;
            case column::_players_registered_count:   return commonFlags | Qt::ItemIsEditable;
            case column::_sit_n_go_users_count:       return (item._types & e_start_time::st_sit_n_go) ? commonFlags : Qt::NoItemFlags;
            case column::_main_tournament_id:         return (item._types & e_win_result::wr_qualifier) ? commonFlags : Qt::NoItemFlags;
            case column::_buyin:                      return commonFlags | Qt::ItemIsEditable;
            case column::_big_blind:                  return commonFlags | Qt::ItemIsEditable;
            case column::_round_duration:             return commonFlags | Qt::ItemIsEditable;
            case column::_start_stack:                return commonFlags | Qt::ItemIsEditable;
            case column::_prize_places_count:         return commonFlags | Qt::ItemIsEditable;
            case column::_common_prize:               return commonFlags | Qt::ItemIsEditable;
            case column::_types:                      return commonFlags;
            case column::_game_type:                  return commonFlags;
            case column::_game_speed:                 return commonFlags;
            case column::_game_limit:                 return commonFlags;
            case column::_title:                      return commonFlags | Qt::ItemIsEditable;
            case column::_status:                     return commonFlags | Qt::ItemIsEditable;
            case column::_is_partner_tourn:           return commonFlags | Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
            case column::_registration_start_time:    return (item._types & e_start_time::st_sit_n_go) ? Qt::NoItemFlags : commonFlags | Qt::ItemIsEditable;
            case column::_start_time:                 return commonFlags | Qt::ItemIsEditable;
            case column::_finish_time:                return (item._status != e_tournament_status::finished) ? commonFlags | Qt::ItemIsEditable : Qt::NoItemFlags;
            case column::_button_state:               return commonFlags | Qt::ItemIsEditable;
            case column::_late_registration_duration: return (item._late_registration_duration > 0) ? commonFlags : Qt::NoItemFlags;
            case column::_qualifiers_ids:             return (item._types & e_win_result::wr_main) ? commonFlags : Qt::NoItemFlags;
            case column::_rebuys:                     return (item._types & e_chips_rebuy::cr_rebuy) ? commonFlags : Qt::NoItemFlags;
            case column::_addon:                      return (item._addon._is_available) ? commonFlags : Qt::NoItemFlags;
            case column::_bounty:                     return (item._types & e_chips_rebuy::cr_bounty) ? commonFlags : Qt::NoItemFlags;
            case column::_player_bounty_value:        return (item._types & e_chips_rebuy::cr_progressive) ? commonFlags : Qt::NoItemFlags;
            case column::_current_level:              return commonFlags | Qt::ItemIsEditable;
            case column::_users:                      return (item._status != e_tournament_status::wait) ? commonFlags : Qt::NoItemFlags;
            default:
                break;
            }
        }

        if (!index.parent().parent().isValid())
        {
            if (flags(index.parent()) & Qt::ItemIsEnabled)
            {
                switch (column(index.parent().column())) {
                case column::_qualifiers_ids: return Qt::ItemIsEnabled;
                case column::_rebuys: return Qt::ItemIsEnabled;
                case column::_addon: return Qt::ItemIsEnabled;
                case column::_users:
                {
                    gbp::ns_api::ns_statistic::tournament_stat_t parentItem = m_data.at(index.parent().row());
                    switch (users(index.column())) {
                    case users::_id:
                        return Qt::ItemIsEnabled;
                    case users::_nickname:
                        return commonFlags;
                    case users::_country:
                        return commonFlags;
                    case users::_is_ticket:
                        return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
                    case users::_money:
                        return commonFlags | Qt::ItemIsEditable;
                    case users::_bounty_knocked_out_count:
                        return (parentItem._types & e_chips_rebuy::cr_bounty) ? commonFlags | Qt::ItemIsEditable : Qt::NoItemFlags;
                    case users::_bounty_progr_money:
                        return (parentItem._types & e_chips_rebuy::cr_progressive) ? commonFlags | Qt::ItemIsEditable : Qt::NoItemFlags;
                    default:
                        return Qt::NoItemFlags;
                    }
                }
                default:
                    return Qt::NoItemFlags;
                }
            }
        }

        return Qt::NoItemFlags;
    }

    bool tournament_stat_t::removeItems(const QList<gbp_i64> &ids, const QModelIndex &parent)
    {
//        QList<ids> copiedIds;
        bool result = false;
        if (!parent.isValid()) {
            auto& data = m_data;
            for (gbp_i64 id: ids) {
                QModelIndex idx = indexOf(id);
                if (idx.isValid()) {
                    beginRemoveRows(parent, idx.row(), idx.row());
                    data.erase(data.begin() + idx.row());
                    endRemoveRows();
                    result = true;
                }
            }
            return result;
        }
        else if (!parent.parent().isValid() && column(parent.column()) == column::_users) {
            auto &data = itemAt(parent.row()).ref()._users;
            for (gbp_i64 _id: ids) {
                QModelIndex idx = indexOfUser(idAt(parent.row()), _id);
                if (idx.isValid()) {
                    beginRemoveRows(parent, idx.row(), idx.row());
                    data.erase(data.begin() + idx.row());
                    endRemoveRows();
                    result = true;
                }
            }
            return result;
        }
        return false;
    }
    bool tournament_stat_t::insertItems(const QList<gbp_i64> &ids, const QModelIndex &parent)
    {
        QList<gbp_i64> copiedIds = ids;
        if (!parent.isValid()) {
            for (gbp_i64 id: ids)
            {
                if (item(id).ptr() != nullptr) {
                    copiedIds.removeAll(id);
                }
            }
            if (copiedIds.empty()) {
                return false;
            }
            auto& data = m_data;
            beginInsertRows(parent, data.size(), data.size() + copiedIds.size() - 1);
            ns_util::push_empty(data, copiedIds.size());
            for (gbp_i64 id: copiedIds)  {
                data.back()._id = id;
            }
            endInsertRows();
            return true;
        }
        else if (!parent.parent().isValid() && column(parent.column()) == column::_users)
        {
            auto& parentData = m_data.at(parent.row());
            for (gbp_i64 id: ids) {
                if (indexOfUser(parentData._id, id).isValid()) {
                    copiedIds.removeAll(id);
                }
            }
            if (copiedIds.empty()) {
                return false;
            }
            auto& data = parentData._users;

            beginInsertRows(parent, data.size(), data.size() + copiedIds.size() - 1);
            ns_util::push_empty(data, copiedIds.size());
            for (gbp_i64 id: copiedIds)  {
                data.back()._id = id;
            }
            endInsertRows();
        }
        return false;
    }

    bool tournament_stat_t::insertRows(int row, int count, const QModelIndex &parent)
    {
        if (count < 1) {
            return false;
        }
        if (!parent.isValid())
        {
            beginInsertRows(parent, row, row + count - 1);
            m_data.insert(m_data.begin()+row, count, takeWaitingForInsertItem<value_t>());
            endInsertRows();
            return true;
        }
        if (!parent.parent().isValid())
        {
            if (column(parent.column()) == column::_users)
            {
                beginInsertRows(parent, row, row + count - 1);
                std::vector<value_t::user_info>& players = itemAt(parent.row()).ref()._users;
                players.insert(players.begin() + row, count, takeWaitingForInsertItem<value_t::user_info>());
                endInsertRows();
                return true;
            }
        }
        return false;
    }

    bool tournament_stat_t::removeRows(int row, int count, const QModelIndex &parent)
    {
        if (!parent.isValid())
        {
            beginRemoveRows(parent, row, row + count - 1);
            m_data.erase(m_data.begin() + row, m_data.begin() + row + count);
            endRemoveRows();
            return true;
        }
        if (!parent.parent().isValid() && column(parent.column()) == column::_users)
        {
            if (auto* parentItemPtr = itemAt(parent.row()).ptr()) {
                std::vector<gbp::ns_api::ns_statistic::tournament_stat_t::user_info> &items = parentItemPtr->_users;
                int size = items.size();
                if (size > row)
                {
                    if (size <= row + count)
                    {
                        beginRemoveRows(parent, row, size - 1);
                        items.resize(row);
                        endRemoveRows();
                    }
                    else
                    {
                        beginRemoveRows(parent, row, row + count - 1);
                        items.erase(items.begin() + row, items.begin() + row + count);
                        endRemoveRows();
                    }
                }
                return false;
            }
        }
        return false;
    }

    bool tournament_stat_t::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
    {
        if (sourceParent != destinationParent) {
            return false;
        }
        if (rowCount(destinationParent) > destinationChild && rowCount(sourceParent) > sourceRow + count) {
            if (!sourceParent.isValid())
            {
                beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild);
                data_t buffer;
                auto itFirst = m_data.begin() + sourceRow;
                auto itLast = m_data.begin() + sourceRow + count - 1;
                buffer.assign(itFirst, itLast);
                m_data.erase(itFirst, itLast);
                m_data.insert(m_data.end(), buffer.begin(), buffer.end());
                endMoveRows();
                return true;
            }
            else if (!sourceParent.parent().isValid() && column(sourceParent.column()) == column::_users)
            {
                std::vector<gbp::ns_api::ns_statistic::tournament_stat_t::user_info> &data = itemAt(sourceParent.parent().row()).ref()._users;
                beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild);
                std::vector<gbp::ns_api::ns_statistic::tournament_stat_t::user_info> buffer;
                auto itFirst = data.begin() + sourceRow;
                auto itLast = data.begin() + sourceRow + count - 1;
                buffer.assign(itFirst, itLast);
                data.erase(itFirst, itLast);
                data.insert(data.end(), buffer.begin(), buffer.end());
                endMoveRows();
                return true;
            }
        }
        return false;
    }

//    bool tournament_stat_t::removeColumns(int column, int count, const QModelIndex &parent)
//    {
//        beginRemoveColumns(parent, column, column + count - 1);
//        // FIXME: Implement me!
//        endRemoveColumns();
//    }

    QModelIndex tournament_stat_t::index(int row, int col, const QModelIndex &parent) const
    {
        if (row < 0 || row >= rowCount(parent) || col < 0 || col >= columnCount(parent)) {
            return QModelIndex();
        }
        if (!parent.isValid()) {
            return createIndex(row, col, nullptr);
        }
        if (!parent.parent().isValid()) {
            const gbp::ns_api::ns_statistic::tournament_stat_t& parentItem = m_data.at(parent.row());
            switch (column(parent.column())) {
            case column::_qualifiers_ids:
            {
                if (col != 0) {
                    return QModelIndex();
                }
                parent_data* d = new parent_data(parentItem._id, parent.column());
                m_pData << d;
                return createIndex(row, 0, d);
            }
            case column::_rebuys:
            case column::_addon:
            case column::_users:
            {
                parent_data* d = new parent_data(parentItem._id, parent.column());
                m_pData << d;
                return createIndex(row, col, d);
            }
            default:
                return QModelIndex();
            }
        }
        return QModelIndex();
    }

    QModelIndex tournament_stat_t::usersRootAt(int row) const {
        return index(row, underlying_cast(column::_users));
    }

    QModelIndex tournament_stat_t::usersRootOf(gbp_i64 id) const {
        return indexOf(id, column::_users);
    }

    QModelIndex tournament_stat_t::indexOfUser(gbp_i64 tournamentId, gbp_i64 userId, users field) const
    {
        QModelIndex usrRoot = usersRootOf(tournamentId);
        if (usrRoot.isValid())
        {
            for (int row = 0; row < rowCount(usrRoot); row++) {
                QModelIndex usrIndex = index(row, underlying_cast(field), usrRoot);
                if (usrIndex.data(UserType::RowItemIdRole).value<gbp_i64>() == userId) {
                    return usrIndex;
                }
            }
        }
        return QModelIndex();
    }

    QModelIndex tournament_stat_t::parent(const QModelIndex &child) const
    {
        if (!child.isValid()) {
            return QModelIndex();
        }
        if (child.internalPointer() == nullptr) {
            return QModelIndex();
        }
        const parent_data* d = static_cast<const parent_data*>(child.internalPointer());
        for (size_t i = 0; i < m_data.size(); i++) {
            if (m_data.at(i)._id == d->id) {
                return index(i, d->col, QModelIndex());
            }
        }

        return QModelIndex();
    }

    bool tournament_stat_t::hasChildren(const QModelIndex &parent) const
    {
        if (!parent.isValid()) {
            return true;
        }
        if (!parent.parent().isValid())
        {
            switch (column(parent.column())) {
            case column::_qualifiers_ids:
            case column::_rebuys:
            case column::_addon:
            case column::_users:
                return true;
            default:
                return false;
            }
        }

        return false;
    }

} //namespace ns_model




QVariant ns_model::tournament_stat_t::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) {
        return QVariant();
    }
    switch (role) {
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        return ns_util::enumToString(column(section));
    default:
        return QVariant();
    }

    return QVariant();
}

bool ns_model::tournament_stat_t::isItemOrientedData() const { return false; }



QModelIndexList ns_model::tournament_stat_t::match(const QModelIndex &start, int role, const QVariant &value, int hits, Qt::MatchFlags flags) const
{
    if (role == UserType::RowItemIdRole)
    {
        QModelIndexList lst;
        if (value.canConvert<gbp_i64>())
        {
            gbp_i64 id = value.value<gbp_i64>();
            if (!start.isValid()) {
                return lst << indexOf(id);
            }
            if (!start.parent().isValid()) {
                if (flags & Qt::MatchRecursive) {
                    return lst << indexOf(id);
                } else {
                    for (int row = start.row(); row < rowCount(); row++) {
                        if (itemAt(row).ref()._id == id) {
                            return lst << index(row, 0);
                        }
                    }
                    return lst;
                }
            }
            if (column(start.parent().column()) == column::_users) {
                int usrRow = start.row();
                for (int parentRow = start.parent().row(); parentRow < rowCount(); parentRow++, usrRow = 0) {
                    QModelIndex parent = usersRootAt(parentRow);
                    for (; usrRow < rowCount(parent); usrRow++) {
                        QModelIndex usrIndex = index(usrRow, 0, parent);
                        if (usrIndex.data(role).value<gbp_i64>() == id) {
                            lst << usrIndex;
                            hits--;
                            if (hits == 0) {
                                return lst;
                            }
                        }
                    }
                }
                if (flags & Qt::MatchRecursive)
                {
                    int parentRow = 0;
                    for (; parentRow < start.parent().row(); parentRow++) {
                        QModelIndex parent = usersRootAt(parentRow);
                        for (int usrRow = 0; usrRow < rowCount(parent); usrRow++) {
                            QModelIndex usrIndex = index(usrRow, 0, parent);
                            if (usrIndex.data(role).value<gbp_i64>() == id) {
                                lst << usrIndex;
                                hits--;
                                if (hits == 0) {
                                    return lst;
                                }
                            }
                        }
                    }
                    for (int usrRow = 0; usrRow < start.row(); usrRow++) {
                        QModelIndex usrIndex = start.sibling(usrRow, 0);
                        if (usrIndex.data(role).value<gbp_i64>() == id) {
                            lst << usrIndex;
                            hits--;
                            if (hits == 0) {
                                return lst;
                            }
                        }
                    }
                }
            }

            if (!start.parent().isValid() || hits == 1) {
                for (int row = start.row(); row < rowCount(); row++) {

                }
            }
        }
        return lst;
    }
    if (role == UserType::InternalIdRole)
    {
        QModelIndexList lst;

//        lst  internalIdMatcher<column>::match(this, value, start, (flags & Qt::MatchRecursive));


        if (value.canConvert<typename internal_id<column>::type>())
        {
            auto id = value.value<typename internal_id<column>::type>();
            QModelIndex idx = indexOf(id.second, id.first);
            if (!start.isValid() || start < idx || (flags & Qt::MatchRecursive)) {
                lst << idx;
            }
            return lst;
        }

        if (value.canConvert<typename internal_id<users>::type>())
        {
            auto id = value.value<typename internal_id<users>::type>();
            QModelIndex idx = indexOfUser(id.second.first, id.second.second, id.first);
            if (!start.isValid() || start < idx || (flags & Qt::MatchRecursive)) {
                lst << idx;
            }
            return lst;
        }

        if (value.canConvert<typename internal_id<rebuys>::type>())
        {
            auto id = value.value<typename internal_id<rebuys>::type>();
            QModelIndex parentIndex = indexOf(id.second.first, identifier<rebuys>::root);
            if (parentIndex.isValid()) {
                QModelIndex idx = this->index(id.second.second, underlying_cast(id.first), parentIndex);
                if (!start.isValid() || start < idx || (flags & Qt::MatchRecursive)) {
                    lst << idx;
                }
            }
            return lst;
        }

        if (value.canConvert<typename internal_id<addon>::type>())
        {
            typename internal_id<addon>::type id = value.value<typename internal_id<addon>::type>();
            QModelIndex parentIndex = indexOf(id.second.first, identifier<addon>::root);
            if (parentIndex.isValid()) {
                QModelIndex idx = this->index(id.second.second, underlying_cast(id.first), parentIndex);
                if (!start.isValid() || start < idx || (flags & Qt::MatchRecursive)) {
                    lst << idx;
                }
            }
            return lst;
        }



    }

    return ApiBase::match(start, role, value, hits, flags);
}




void ns_model::tournament_stat_t::fetchMore(const QModelIndex &parent)
{
    if (canFetchMore(parent))
    {
//        m_canFetchMore = true;
//        int userCount = rowCount(parent);
//        gbp_i64 id = parent.data(UserType::RowItemIdRole).value<gbp_i64>();
//        m_rpc->get_tourn_users_stat(id
//       , [this, userCount, id, parent](ns_statistic::e_status s, const std::vector<gbp::ns_api::ns_statistic::tournament_stat_t::user_info>& info){
//            if (s != ns_statistic::e_status::ok) {
//                return;
//            }
//            if (static_cast<gbp_i32>(info.size()) > userCount) {
//                if (insertRows(userCount, info.size() - userCount, parent)) {
//                    if (auto* itemPtr = item(id).ptr()) {
//                        itemPtr->_users = info;
//                    }
//                }
//            }
//        });
    }
}

bool ns_model::tournament_stat_t::canFetchMore(const QModelIndex &parent) const
{
    return ApiBase::canFetchMore(parent);
//    return m_rpc != nullptr && !isFetchMoreBlocked() && parent.isValid() && column(parent.column()) == column::_users && !parent.parent().isValid()
//        && rowCount(parent) < parent.sibling(parent.row(), underlying_cast(column::_players_registered_count)).data(UserType::OriginDataRole).toInt();
}

bool ns_model::tournament_stat_t::isFetchMoreBlocked() const {
    return m_canFetchMore;
}


bool ns_model::tournament_stat_t::write(QIODevice *device) const
{
    if (device != nullptr && device->open(QIODevice::WriteOnly))
    {
        QDataStream stream(device);
        stream << m_data;
        device->close();
        return true;
    }
    std::cout << "Write Failed! " << __FILE__ << ":" << __LINE__ << std::endl;
    return false;
}

bool ns_model::tournament_stat_t::read(QIODevice *device)
{
    if (device != nullptr && device->open(QIODevice::ReadOnly))
    {
        QDataStream stream(device);
        games_stat_t::second_type buff;
        stream >> buff;
        device->close();
        updateItems(*this, m_data, buff);
//        beginResetModel();
//        m_data = buff;
//        endResetModel();
        return true;
    }
    std::cout << "Read Failed! " << __FILE__ << ":" << __LINE__ << std::endl;
    return false;
}


int ns_model::tournament_stat_t::rowById(gbp_i64 id, const void *data, const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        const games_stat_t::second_type* d = (data != nullptr) ? static_cast<const games_stat_t::second_type*>(data) : (&m_data);
        for (size_t row = 0; row < d->size(); row++) {
            if (d->at(row)._id == id) {
                return row;
            }
        }
    }
    else if (!parent.parent().isValid() && column(parent.column()) == column::_users)
    {
        const std::vector<games_stat_t::second_type::value_type::user_info>* d = (data != nullptr) ? static_cast<const std::vector<games_stat_t::second_type::value_type::user_info>*>(data) : (&m_data.at(parent.row())._users);
        for (size_t row = 0; row < d->size(); row++) {
            if (d->at(row)._id == id) {
                return row;
            }
        }
    }
    return -1;
}

int ns_model::tournament_stat_t::propertyIndex(const QByteArray &name, const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        static const PropertyIndex<value_t> propertyIdx;
        return propertyIdx.indexOf(name);
    }
    else if (!parent.parent().isValid()) {
        switch (column(parent.column()))
        {
            case column::_rebuys:
            {
                static const PropertyIndex<decltype(value_t::_rebuys)> propertyIdx;
                return propertyIdx.indexOf(name);
            }
            case column::_addon:
            {
                static const PropertyIndex<decltype(value_t::_addon)> propertyIdx;
                return propertyIdx.indexOf(name);
            }
            case column::_users:
            {
                static const PropertyIndex<value_t::user_info> propertyIdx;
                return propertyIdx.indexOf(name);
            }
        default:
            return -1;
        }
    }
    return -1;
}

QVariant ns_model::tournament_stat_t::dataSortRole(const QModelIndex &index) const
{
    return data(index, UserType::OriginDataRole);
}
