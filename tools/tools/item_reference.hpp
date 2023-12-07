#pragma once

#include "tools_global.h"
#include "types_fwd.hpp"
#include <functional>
#include <vector>

#include <QString>

template <typename T, bool is_struct = std::is_class<T>::value>
struct ItemRef;

template <typename T>
struct ItemRef<T, false>
{
    static T& invalidItem() {
        static T invalid;
        return invalid;
    }
    bool isValid() const { return this != nullptr && this != &invalidItem(); }

    T& _item;
    operator const T& () const { return _item; }
    operator T& () { return _item; }

    ItemRef(T& item): _item(item) {}
//        ItemRef(): _item(invalidItem) {}

};

template <typename T>
struct ItemRef<T, true>
{
    static T& invalidItem() {
        static T invalid;
        return invalid;
    }
    bool isValid() const { return this != nullptr && this != &invalidItem(); }

    T& _item;
    operator const T& () const { return _item; }
    operator T& () { return _item; }

    ItemRef(T& item): _item(item) {}
//        ItemRef(): _item(invalidItem) {}
};


template <typename T, typename RefType = ItemRef<T>>
RefType reference(T& obj) {
    return RefType(obj);
}
template <typename T, typename RefType = ItemRef<T>>
RefType reference(const T& obj) {
    return RefType(obj);
}


template <typename T>
struct ItemRefContainer
{
    using value_type = T;
    using ContainerType = std::vector<T>;
    using size_type = typename ContainerType::size_type;
    ContainerType& _items;
    operator const ContainerType& () const {
        return _items;
    }
    operator ContainerType& () {
        return _items;
    }
private:
    using on_about_to_insert = std::function<void(int, int)>;
    using on_about_to_remove = std::function<void(int, int)>;
    using on_inserted        = std::function<void()>;
    using on_removed         = std::function<void()>;
    on_about_to_insert m_beforeInsert;
    on_about_to_insert m_beforeRemove;
    on_inserted        m_afterInsert;
    on_removed         m_afterRemove;
public:
    ItemRefContainer(std::vector<T>& items)
        : _items(items)
        , m_beforeInsert(on_about_to_insert())
        , m_beforeRemove(on_about_to_remove())
        , m_afterInsert(on_inserted())
        , m_afterRemove(on_removed())
    {}


    void setInsertHandlers(on_about_to_insert beforeInsert, on_inserted afterInsert) {
        Q_ASSERT(static_cast<bool>(beforeInsert) == static_cast<bool>(afterInsert));
        m_beforeInsert = std::move(beforeInsert);
        m_afterInsert = std::move(afterInsert);
    }
    void setRemoveHandlers(on_about_to_remove beforeRemove, on_removed afterRemove) {
        Q_ASSERT(static_cast<bool>(beforeRemove) == static_cast<bool>(afterRemove));
        m_beforeRemove = std::move(beforeRemove);
        m_afterRemove = std::move(afterRemove);
    }



    T& at(size_type idx) {
        if (_items.size() <= idx) {
//            std::cout << "out_of_range" << __FILE__ << ":" << __LINE__ << std::endl;
//                return ItemRef<T>::invalidItem();
        }
        return _items.at(idx);
    }
    ItemRef<T> refAt(size_t idx) const {
        if (_items.size() <= idx) {
            return ItemRef<T>::invalidItem();
        }
        return reference(_items.at(idx));
    }
    size_type size() const { return _items.size(); }
    bool insert(size_type pos, size_type count, const T& value = T()) {
        if (pos >= _items.size()) {
            return false;
        }
        if (m_beforeInsert) m_beforeInsert(pos, count);
        //qDebug() << QStringLiteral("INSERT") << pos << count;
        _items.insert(_items.begin()+pos, count, value);
        if (m_afterInsert) m_afterInsert();
        return true;
    }
    bool remove(size_type pos, size_type count) {
        if (pos + count >= _items.size()) {
            return false;
        }
        if (m_beforeRemove) m_beforeRemove(pos, count);
        //qDebug() << QStringLiteral("ERASE") << pos << count;
        _items.erase(_items.begin()+pos, _items.begin()+pos+count);
        if (m_afterRemove) m_afterRemove();
        return true;
    }
};


/************************************ FWD DECLARATIONS *************************************/
//struct ItemRef<ns_statistic::cash_stat_t>;
//struct ItemRef<ns_statistic::tournament_stat_t>;
//struct ItemRef<ns_statistic::tourn_additional_stat_t>;

//struct TOOLS_DLL TournamentUserRef : ItemRef<ns_statistic::tournament_stat_t::user_info>
//{
//    using T = ns_statistic::tournament_stat_t::user_info;
//    using Super = ItemRef<T>;

//    gbp_i64& _id;
//    std::string& _nickname;
//    gbp_i32& _country;
//    bool& _is_ticket;
//    gbp_i64& _money;
//    gbp_i32& _bounty_knocked_out_count;
//    gbp_i64& _bounty_progr_money;

//    TournamentUserRef(T& item);
//};

struct TOOLS_DLL TournamentRef : ItemRef<ns_statistic::tournament_stat_t>
{
private:
    using e_game_type         = gbp::ns_api::ns_game::e_game_type;
    using e_game_speed        = gbp::ns_api::ns_game::e_game_speed;
    using e_game_limit        = gbp::ns_api::ns_game::e_game_limit;
    using e_tournament_status = gbp::ns_api::ns_tournament_types::e_tournament_status;
    using e_tourn_button_type = gbp::ns_api::ns_statistic::e_tourn_button_type;
public:
    using T = ns_statistic::tournament_stat_t;
    using Super = ItemRef<T>;
    static T& invalidItem();

    bool isValid() const;


    gbp_i64&               _id;
    gbp_i8&                _seats_count;
    gbp_i32&               _max_players_count;
    bool&                  _is_current_user_registered;
    gbp_i32&               _players_registered_count;
    gbp_i32&               _sit_n_go_users_count;
    gbp_i64&               _main_tournament_id;
    gbp_i64&               _buyin;
    gbp_i64&               _big_blind;
    gbp_i64&               _round_duration;
    gbp_i64&               _start_stack;
    gbp_i32&               _prize_places_count;
    gbp_i64&               _common_prize;
    gbp_i32&               _types;
    e_game_type&           _game_type;
    e_game_speed&          _game_speed;
    e_game_limit&          _game_limit;
    std::string&           _title;
    e_tournament_status&   _status;
    bool&                  _is_partner_tourn;
    gbp_i64&               _registration_start_time;
    gbp_i64&               _start_time;
    gbp_i64&               _finish_time;
    e_tourn_button_type&   _button_state;
    gbp_i64&               _late_registration_duration;
    std::vector<gbp_i64>&  _qualifiers_ids;
    gbp::ns_api::rebuys_t& _rebuys;
    gbp::ns_api::addon_t&  _addon;
    gbp_i64&               _bounty;
    gbp_i64&               _player_bounty_value;
    gbp_i16&               _current_level;
    void*                  _users; // std::vector<ns_statistic::tournament_stat_t::user_info>

    struct user_info;
    user_info& user(int index) const;

    operator const T& () const;
    operator T& ();

    TournamentRef(T& item);

    gbp_i64& qualifier(int idx);
};


struct TOOLS_DLL ItemRefCash : ItemRef<gbp::ns_api::ns_statistic::cash_stat_t>
{
    using T = gbp::ns_api::ns_statistic::cash_stat_t;
    using Super = ItemRef<T>;
    static T& invalidItem();

    bool isValid() const;

//    T& _item;
    gbp_i64& _id;
    std::string& _title;
    bool& _is_freeroll;
    gbp_i64& _big_blind;
    gbp::ns_api::ns_game::e_game_limit& _limit;
    gbp::ns_api::ns_game::e_game_type& _type;
    std::pair<gbp_i8, gbp_i8>& _players_count;
    gbp_i64& _pot;
    gbp_u8& _flp;
    gbp_i32& _hands_per_hour;
    gbp_i32& _wait_players;
    std::vector<gbp::ns_api::ns_statistic::player_t>& _players;
    bool& _is_flash;
    gbp_i64& _flash_id;

    operator const T& () const;
    operator T& ();

    ItemRefCash(T& item);
};


/**  ***********************/
template <typename T>
void static_check(const T& obj);

template <> void TOOLS_DLL static_check(const gbp::ns_api::ns_statistic::tournament_stat_t& obj);

