#include "qdatastream_io.hpp"
#include "api_util.hpp"

#ifndef Q_MOC_RUN
    #include <api/api_common.h>
#include <api/lobby/types.h>
    #include <api/lobby_stat/types.h>
    #include <api/table/types.h>
#endif // !Q_MOC_RUN



QDataStream &operator<<(QDataStream &stream, const std::string &data) {
    stream << QString::fromStdString(data);
    return stream;
}
QDataStream &operator>>(QDataStream &stream, std::string &data)
{
    QString buff;
    stream >> buff;
    data = buff.toStdString();
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::ns_statistic::cash_stat_t &data)
{
    stream << data._id
           << data._title
           << data._is_freeroll
           << data._big_blind
           << data._type
           << data._limit
           << data._players_count
           << data._pot
           << data._flp
           << data._hands_per_hour
           << data._wait_players
           << data._players
           << data._is_flash
           << data._flash_id;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_statistic::cash_stat_t &data)
{
    stream >> data._id
           >> data._title
           >> data._is_freeroll
           >> data._big_blind
           >> data._type
           >> data._limit
           >> data._players_count
           >> data._pot
           >> data._flp
           >> data._hands_per_hour
           >> data._wait_players
           >> data._players
           >> data._is_flash
           >> data._flash_id;
    return stream;
}


QDataStream &operator<<(QDataStream &stream, gbp::ns_api::ns_game::e_game_limit data) {
    stream << underlying_cast(data);
    return stream;
}
QDataStream &operator<<(QDataStream &stream, gbp::ns_api::ns_game::e_game_type data) {
    stream << underlying_cast(data);
    return stream;
}
QDataStream &operator<<(QDataStream &stream, gbp::ns_api::ns_game::e_game_speed data) {
    stream << underlying_cast(data);
    return stream;
}

QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_game::e_game_limit &data)
{
    typename std::underlying_type<gbp::ns_api::ns_game::e_game_limit>::type underlying;
    stream >> underlying;
    data = gbp::ns_api::ns_game::e_game_limit(underlying);
    return stream;
}
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_game::e_game_type &data)
{
    typename std::underlying_type<gbp::ns_api::ns_game::e_game_type>::type underlying;
    stream >> underlying;
    data = gbp::ns_api::ns_game::e_game_type(underlying);
    return stream;
}
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_game::e_game_speed &data)
{
    typename std::underlying_type<gbp::ns_api::ns_game::e_game_speed>::type underlying;
    stream >> underlying;
    data = gbp::ns_api::ns_game::e_game_speed(underlying);
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const std::pair<gbp_i8, gbp_i8> &data)
{
    stream << data.first << data.second;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, std::pair<gbp_i8, gbp_i8> &data)
{
    stream >> data.first >> data.second;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::ns_statistic::player_t &data)
{
    stream << data._nick
           << data._pos
           << data._money
           << data._country;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_statistic::player_t &data)
{
    stream >> data._nick
           >> data._pos
           >> data._money
           >> data._country;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::ns_statistic::tournament_stat_t::user_info &data) {
    stream << data._id
           << data._nickname
           << data._country
           << data._is_ticket
           << data._money
           << data._bounty_knocked_out_count
           << data._bounty_progr_money;
    return stream;
}
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_statistic::tournament_stat_t::user_info &data) {
    stream >> data._id
           >> data._nickname
           >> data._country
           >> data._is_ticket
           >> data._money
           >> data._bounty_knocked_out_count
           >> data._bounty_progr_money;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::ns_statistic::tournament_stat_t &data)
{
    stream << data._id
           << data._seats_count
           << data._max_players_count
           << data._is_current_user_registered
           << data._players_registered_count
           << data._sit_n_go_users_count
           << data._main_tournament_id
           << data._buyin
           << data._big_blind
           << data._round_duration
           << data._start_stack
           << data._prize_places_count
           << data._common_prize
           << data._types
           << data._game_type
           << data._game_speed
           << data._game_limit
           << data._title
           << data._status
           << data._registration_start_time
           << data._start_time
           << data._finish_time
           << data._button_state
           << data._late_registration_duration
           << data._qualifiers_ids
           << data._rebuys
           << data._addon
           << data._bounty
           << data._player_bounty_value
           << data._current_level
           << data._users;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_statistic::tournament_stat_t &data)
{
    stream >> data._id
           >> data._seats_count
           >> data._max_players_count
           >> data._is_current_user_registered
           >> data._players_registered_count
           >> data._sit_n_go_users_count
           >> data._main_tournament_id
           >> data._buyin
           >> data._big_blind
           >> data._round_duration
           >> data._start_stack
           >> data._prize_places_count
           >> data._common_prize
           >> data._types
           >> data._game_type
           >> data._game_speed
           >> data._game_limit
           >> data._title
           >> data._status
           >> data._registration_start_time
           >> data._start_time
           >> data._finish_time
           >> data._button_state
           >> data._late_registration_duration
           >> data._qualifiers_ids
           >> data._rebuys
           >> data._addon
           >> data._bounty
           >> data._player_bounty_value
           >> data._current_level
           >> data._users;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, gbp::ns_api::ns_tournament_types::e_tournament_status data) {
    stream << underlying_cast(data);
    return stream;
}

QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_tournament_types::e_tournament_status &data)
{
    typename std::underlying_type<gbp::ns_api::ns_tournament_types::e_tournament_status>::type underlying;
    stream >> underlying;
    data = gbp::ns_api::ns_tournament_types::e_tournament_status(underlying);
    return stream;
}
//enum class users {
//    _id = 0,
//    _nickname,
//    _country,
//    _is_ticket,
//    _money,
//    _bounty_knocked_out_count,
//    _bounty_progr_money,

//    count
//};
//Q_ENUM(users)

//enum class rebuys {
//    _count = 0,
//    _money,
//    _chips,

//    count
//};

QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::rebuys_t &data)
{
    stream << data._count
           << data._money
           << data._chips;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, gbp::ns_api::rebuys_t &data)
{
    stream >> data._count
           >> data._money
           >> data._chips;
    return stream;

}

QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::addon_t &data)
{
    stream << data._is_available
           << data._money
           << data._chips;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, gbp::ns_api::addon_t &data)
{
    stream >> data._is_available
           >> data._money
           >> data._chips;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, gbp::ns_api::ns_statistic::e_tourn_button_type data) { return read_enum(stream, data), stream; }
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_statistic::e_tourn_button_type &data) { return write_enum(stream, data), stream; }

QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::ns_table::table_state_t &data)
{
    stream  << data._tournament_id
            << data._game_id
            << data._big_blind
            << data._seats_count
            << data._players
            << data._current_user_pos
            << data._table_cards
            << data._all_pots
            << data._waiters_count
            << data._time_short
            << data._time_main
            << data._time_action_passed
            << data._time_action_left
            << data._distribution_number;
    return stream;
}
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_table::table_state_t &data)
{
    stream  >> data._tournament_id
            >> data._game_id
            >> data._big_blind
            >> data._seats_count
            >> data._players
            >> data._current_user_pos
            >> data._table_cards
            >> data._all_pots
            >> data._waiters_count
            >> data._time_short
            >> data._time_main
            >> data._time_action_passed
            >> data._time_action_left
            >> data._distribution_number;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::ns_table::user_state_t &data)
{
    stream  << data._left_table
            << data._sit_out
            << data._frozen
            << data._anticipant
            << data._folded
            << data._all_ined
            << data._all_ined_in_cur_round;
    return stream;
}
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_table::user_state_t &data)
{
    stream  >> data._left_table
            >> data._sit_out
            >> data._frozen
            >> data._anticipant
            >> data._folded
            >> data._all_ined
            >> data._all_ined_in_cur_round;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::ns_table::table_player_info_t &data)
{
    stream  << data._id
            << data._is_user
            << data._pos
            << data._nickname
            << data._country
            << data._money
            << data._start_stack
            << data._bounty_progressive_money
            << data._hash_avatar
            << data._is_dealer
            << data._cards
            << data._round_bet
            << data._is_current
            << data._state;
    return stream;
}
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_table::table_player_info_t &data)
{
    stream  >> data._id
            >> data._is_user
            >> data._pos
            >> data._nickname
            >> data._country
            >> data._money
            >> data._start_stack
            >> data._bounty_progressive_money
            >> data._hash_avatar
            >> data._is_dealer
            >> data._cards
            >> data._round_bet
            >> data._is_current
            >> data._state;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::ns_table::tournament_state_t &data)
{
    stream  << data._start_time
            << data._current_level
            << data._rebuys
            << data._round_conditions;
    return stream;
}
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_table::tournament_state_t &data)
{
    stream  >> data._start_time
            >> data._current_level
            >> data._rebuys
            >> data._round_conditions;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::round_conditions_t &data)
{
    stream  << data._level
            << data._big_blind
            << data._round_duration
            << data._ante
            << data._time_user_action_main
            << data._time_user_action_additional;
    return stream;
}
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::round_conditions_t &data)
{
    stream  >> data._level
            >> data._big_blind
            >> data._round_duration
            >> data._ante
            >> data._time_user_action_main
            >> data._time_user_action_additional;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::ns_statistic::tourn_additional_stat_t::game &data)
{
    stream << data._id
           << data._title
           << data._players;
    return stream;
}
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_statistic::tourn_additional_stat_t::game &data)
{
    stream >> data._id
           >> data._title
           >> data._players;
    return stream;
}
QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::ns_statistic::tourn_additional_stat_t::blind &data)
{
    stream << data._level
           << data._big_blind
           << data._ante
           << data._duration;
    return stream;
}
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_statistic::tourn_additional_stat_t::blind &data)
{
    stream >> data._level
           >> data._big_blind
           >> data._ante
           >> data._duration;
    return stream;
}
QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::ns_statistic::tourn_additional_stat_t::prize &data)
{
    stream << data._winner_pos
           << data._prize;
    return stream;
}
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_statistic::tourn_additional_stat_t::prize &data)
{
    stream >> data._winner_pos
           >> data._prize;
    return stream;
}
QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::ns_statistic::tourn_additional_stat_t::user_info &data)
{
    stream << data._id
           << data._nickname
           << data._stack
           << data._pos;
    return stream;
}
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_statistic::tourn_additional_stat_t::user_info &data)
{
    stream >> data._id
           >> data._nickname
           >> data._stack
           >> data._pos;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::ns_statistic::tourn_additional_stat_t &data)
{
    stream << data._id
           << data._games
           << data._blinds
           << data._prizes
           << data._late_register_ends_in
           << data._rebuys_count
           << data._addons_count
           << data._current_pos;
    return stream;
}
QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_statistic::tourn_additional_stat_t &data)
{
    stream >> data._id
           >> data._games
           >> data._blinds
           >> data._prizes
           >> data._late_register_ends_in
           >> data._rebuys_count
           >> data._addons_count
           >> data._current_pos;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, decltype(gbp::ns_api::ns_lobby::user_info_t::_gender) data) { return read_enum(stream, data), stream; }
QDataStream &operator>>(QDataStream &stream, decltype(gbp::ns_api::ns_lobby::user_info_t::_gender) &data) { return write_enum(stream, data), stream; }
QDataStream &operator<<(QDataStream &stream, decltype(gbp::ns_api::ns_lobby::user_info_t::_verifying) data) { return read_enum(stream, data), stream; }
QDataStream &operator>>(QDataStream &stream, decltype(gbp::ns_api::ns_lobby::user_info_t::_verifying) &data) { return write_enum(stream, data), stream; }
QDataStream &operator<<(QDataStream &stream, decltype(gbp::ns_api::ns_lobby::user_info_t::_vip_status) data) { return read_enum(stream, data), stream; }
QDataStream &operator>>(QDataStream &stream, decltype(gbp::ns_api::ns_lobby::user_info_t::_vip_status) &data) { return write_enum(stream, data), stream; }

QDataStream &operator<<(QDataStream &stream, const gbp::ns_api::ns_lobby::user_info_t &data)
{
    stream << data._user_id
           << data._nickname
           << data._hash_avatar
           << data._email
           << data._username
           << data._last_name
           << data._birth_date._year << data._birth_date._month << data._birth_date._day
           << data._city
           << data._mail_index
           << data._mail_address
           << data._country
           << data._lang
           << data._phone
           << data._money
           << data._in_game_money
           << data._wait_out_money
           << data._gender
           << data._verifying
           << data._vip_status;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, gbp::ns_api::ns_lobby::user_info_t &data)
{
    stream >> data._user_id
           >> data._nickname
           >> data._hash_avatar
           >> data._email
           >> data._username
           >> data._last_name
           >> data._birth_date._year >> data._birth_date._month >> data._birth_date._day
           >> data._city
           >> data._mail_index
           >> data._mail_address
           >> data._country
           >> data._lang
           >> data._phone
           >> data._money
           >> data._in_game_money
           >> data._wait_out_money
           >> data._gender
           >> data._verifying
           >> data._vip_status;
    return stream;
}
