#include "item_reference.hpp"
#include <qstringlist.h>
#include <iostream>
#include <api/lobby_stat/types.h>

ns_statistic::cash_stat_t &ItemRefCash::invalidItem()
{
    static ns_statistic::cash_stat_t item;
    item._id = -1;
    return item;
}

ItemRefCash::operator const ItemRefCash::T &() const {
    return _item;
}
ItemRefCash::operator ItemRefCash::T &() {
    return _item;
}

TournamentRef::T& TournamentRef::invalidItem() {
    static ns_statistic::tournament_stat_t invalid;
    invalid._id = -1;
    return invalid;
}

bool TournamentRef::isValid() const { Q_ASSERT(this); return (const void*)this != (const void*)&invalidItem(); }

TournamentRef::operator const TournamentRef::T &() const { return _item; }
TournamentRef::operator TournamentRef::T &() { return _item; }
gbp_i64 &TournamentRef::qualifier(int idx) {
    return _qualifiers_ids.at(idx);
}

// struct TournamentRef::user_info {
//     gbp_i64& _id;
//     std::string& _nickname;
//     gbp_i32& _country;
//     bool& _is_ticket;
//     gbp_i64& _money;
//     gbp_i32& _bounty_knocked_out_count;
//     gbp_i64& _bounty_progr_money;
//     user_info(ns_statistic::tournament_stat_t::user_info& info)
//         : _id(info._id)
//         , _nickname(info._nickname)
//         , _country(info._country)
//         , _is_ticket(info._is_ticket)
//         , _money(info._money)
//         , _bounty_knocked_out_count(info._bounty_knocked_out_count)
//         , _bounty_progr_money(info._bounty_progr_money)
//     {}
// };

// template <typename v1, typename v2>
// v2 convert_sequence(const v1& seq) {
//     v2 result;
//     for (const typename v1::value_type& value: seq) {
//         result.push_back(value);
//     }
//     return result;
// }
struct TournamentRef::user_info : ItemRef<ns_statistic::tournament_stat_t::user_info>
{
    using T = ns_statistic::tournament_stat_t::user_info;
    using Super = ItemRef<T>;
    user_info(ns_statistic::tournament_stat_t::user_info& info)
        : Super(info)
    {}
};

TournamentRef::user_info& TournamentRef::user(int index) const {
    static TournamentRef::user_info info(static_cast<std::vector<ns_statistic::tournament_stat_t::user_info>*>(_users)->at(index));
//    info = static_cast<std::vector<ns_statistic::tournament_stat_t::user_info>*>(_users)->at(index);
    return info;
}

TournamentRef::TournamentRef(TournamentRef::T &item)
    : Super(item)
    , _id(item._id)
    , _seats_count(item._seats_count)
    , _max_players_count(item._max_players_count)
    , _is_current_user_registered(item._is_current_user_registered)
    , _players_registered_count(item._players_registered_count)
    , _sit_n_go_users_count(item._sit_n_go_users_count)
    , _main_tournament_id(item._main_tournament_id)
    , _buyin(item._buyin)
    , _big_blind(item._big_blind)
    , _round_duration(item._round_duration)
    , _start_stack(item._start_stack)
    , _prize_places_count(item._prize_places_count)
    , _common_prize(item._common_prize)
    , _types(item._types)
    , _game_type(item._game_type)
    , _game_speed(item._game_speed)
    , _game_limit(item._game_limit)
    , _title(item._title)
    , _status(item._status)
    , _is_partner_tourn(item._is_partner_tourn)
    , _registration_start_time(item._registration_start_time)
    , _start_time(item._start_time)
    , _finish_time(item._finish_time)
    , _button_state(item._button_state)
    , _late_registration_duration(item._late_registration_duration)
    , _qualifiers_ids(item._qualifiers_ids)
    , _rebuys(item._rebuys)
    , _addon(item._addon)
    , _bounty(item._bounty)
    , _player_bounty_value(item._player_bounty_value)
    , _current_level(item._current_level)
    , _users(&item._users)
{}

ItemRefCash::ItemRefCash(ItemRefCash::T &cashitem)
    : Super(cashitem)
    , _id(_item._id)
    , _title(_item._title)
    , _is_freeroll(_item._is_freeroll)
    , _big_blind(_item._big_blind)
    , _limit(_item._limit)
    , _type(_item._type)
    , _players_count(_item._players_count)
    , _pot(_item._pot)
    , _flp(_item._flp)
    , _hands_per_hour(_item._hands_per_hour)
    , _wait_players(_item._wait_players)
    , _players(_item._players)
    , _is_flash(_item._is_flash)
    , _flash_id(_item._flash_id)
{}


/**  ***********************/

template <typename T>
struct Checker
{
    mutable QStringList m_lst;

    Checker(const T* obj, const QStringList& lst)
        : m_lst(lst)
    {
        obj->apply(*this);
        Q_ASSERT_X(m_lst.isEmpty(), __FUNCTION__, m_lst.join(", ").toLocal8Bit());
    }
    template <typename Type>
    void operator()(const char* name, const Type& member) const {
        if (!m_lst.contains(name)) {
            std::ostringstream os;
            os << member;
            qFatal("no member %s %s", name, os.str().c_str());
        }
        std::cout << m_lst.takeAt(m_lst.indexOf(name)).toStdString() << std::endl;
    }
};

template <typename T>
void check(const T* obj, const QStringList& lst) {
    static Checker<T> checker(obj, lst);
}

template<>
void static_check(const gbp::ns_api::ns_statistic::tournament_stat_t& obj)
{
    static const QStringList lst
    = QStringList() << "_id"
                    << "_seats_count"
                    << "_max_players_count"
                    << "_is_current_user_registered"
                    << "_players_registered_count"
                    << "_sit_n_go_users_count"
                    << "_main_tournament_id"
                    << "_buyin"
                    << "_big_blind"
                    << "_round_duration"
                    << "_start_stack"
                    << "_prize_places_count"
                    << "_common_prize"
                    << "_types"
                    << "_game_type"
                    << "_game_speed"
                    << "_game_limit"
                    << "_title"
                    << "_status"
                    << "_is_partner_tourn"
                    << "_registration_start_time"
                    << "_start_time"
                    << "_finish_time"
                    << "_button_state"
                    << "_late_registration_duration"
                    << "_qualifiers_ids"
                    << "_rebuys"
                    << "_addon"
                    << "_bounty"
                    << "_player_bounty_value"
                    << "_current_level"
                    << "_users";
    check(&obj, lst);
}
