#pragma once

#include "../../tools/tools/types_fwd.hpp"
#include <functional>


//! @namespace ns_table
namespace ns_table
{
    using on_common_reply           = std::function<void(e_status/*, const GameId&*/)>;
    using on_set_guest_blind        = std::function<void(e_status/*, gbp_i64*/, bool)>;
    using on_set_autorecharge       = std::function<void(e_status/*, gbp_i64*/, bool)>;
    using on_set_sit_out            = std::function<void(e_status/*, gbp_i64*/, bool)>;
    using on_add_waiter             = std::function<void(e_status/*, gbp_i64*/, gbp_u16)>;
    using on_remove_from_waiters    = std::function<void(e_status/*, gbp_i64*/, gbp_u16)>;
    using on_set_sit_money          = std::function<void(e_status/*, gbp_i64*/, gbp_i64)>;
    using on_add_game_money         = on_set_sit_money;
    using on_from_viewer_to_player  = std::function<void(e_status/*, gbp_i64*/, const sit_on_table_info_t&)>;
    using on_sit_on_table           = on_from_viewer_to_player;
    using on_get_recharge_money     = std::function<void(e_status/*, gbp_i64*/, const recharge_money_t&)>;

    using on_get_table_state        = std::function<void(e_status, const table_state_t&)>;

    using add_viewer                = std::function<void(e_status/*, const GameId&*/, gbp_i8)>;

    using on_request_rebuy          = std::function<void(e_status/*, const GameId&*/, gbp_i64, const rebuys_t& , bool)>;
    using on_open_own_tournament    = std::function<void(e_status/*, const GameId&*/, const tournament_state_t&, const table_state_t&, int)>;
    using on_cancel_tourn_register  = std::function<void(e_status/*, gbp_i64*/, const cancel_tourn_register_t&)>;
    using on_get_tourn_register_info= std::function<void(e_status/*, gbp_i64*/, const tourn_register_info&)>;
    using on_tournament_get_state   = std::function<void(e_status/*, gbp_i64*/, const tournament_state_t&)>;
    using on_tournament_register    = std::function<void(e_status/*, gbp_i64*/, e_tourn_button_type, e_tournament_status)>;
} //namespace ns_table

class QByteArray;
template <typename, typename> class QHash;

//! @namespace ns_lobby
namespace ns_lobby
{
    using on_common_reply          = std::function<void(e_status)>;
    using on_get_partner_code      = std::function<void(e_status, const QByteArray&)>;
    using on_activation            = std::function<void(e_status, const user_info_t&)>;
    using on_login                 = on_activation;
    using on_get_random_prize_item = std::function<void(e_status, const random_prize_item&)>;
    using on_get_tickets           = std::function<void(e_status, const std::stdvec<user_ticket>&)>;
    using on_get_avatars           = std::function<void(e_status, const QHash<gbp_i64, QByteArray>&)>;
    using on_get_vip_system_info   = std::function<void(e_status, const vip_system_info&)>;
} //namespace ns_lobby


//! @namespace ns_statistic
namespace ns_statistic
{
    using on_games_statistic           = std::function<void(const games_stat_t&)>;
    using on_get_additional_tourn_stat = std::function<void(e_status, const tourn_additional_stat_t&)>;
    template <typename T>
    using on_get_tourn_users_stat      = std::function<void(e_status, const std::stdvec<T>&)>;
} //namespace ns_statistic

