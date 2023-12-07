#pragma once
#include "../../common/api/gbp_int.hpp"

namespace std {
    template<typename _T1, typename _T2>
    struct pair;
    template<typename _Tp, typename _Alloc/* = std::allocator<_Tp> */>
    class vector;
} //namespace std

namespace gbp {
namespace ns_details {
    enum class e_gender: gbp_u8;
} //namespace ns_details
namespace ns_vip_system {
    enum class e_status: gbp_u8;
    enum class e_bonus:  gbp_u8;
} //namespace ns_vip_system
namespace ns_api {
    struct birth_date;
    struct rebuys_t;
    struct addon_t;
    struct round_conditions_t;
namespace ns_game {
    enum class e_game_limit: gbp_u8;
    enum class e_game_type: gbp_u8;
    enum class e_game_speed: gbp_u8;
} //namespace ns_game
namespace ns_tournament_types {
    enum class e_tournament_status : gbp_u8;
} //namespace ns_tournament_types
namespace ns_statistic {
    struct cash_stat_t;
    struct player_t;
    struct tournament_stat_t;
    struct tourn_additional_stat_t;
    enum class e_tourn_button_type : gbp_u8;
} //namespace ns_statistic
namespace ns_table {
    struct table_state_t;
    struct user_state_t;
    struct tournament_state_t;
    struct table_player_info_t;
    struct user_state_t;
} //namespace ns_table
namespace ns_lobby {
    struct user_info_t;
} //namespace ns_lobby
} //namespace ns_api
} //namespace gbp

using games_stat_t = std::pair<std::vector<gbp::ns_api::ns_statistic::cash_stat_t, std::allocator<gbp::ns_api::ns_statistic::cash_stat_t>>
                             , std::vector<gbp::ns_api::ns_statistic::tournament_stat_t, std::allocator<gbp::ns_api::ns_statistic::tournament_stat_t>>>;

namespace ns_model {
    using table_state_data = gbp::ns_api::ns_table::table_state_t;
} //namespace ns_model

