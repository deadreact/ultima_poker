#pragma once

#include "api_table_types.hpp"
#include <type_traits>

template <typename T>
struct GbpRegistrator {
    static const bool is_registered = false;
};

#define GBP_REG_NS_TABLE(classname) template <> struct GbpRegistrator<gbp::ns_api::ns_table::classname> { static const bool is_registered = true; };
GBP_REG_NS_TABLE(table_state_t)
GBP_REG_NS_TABLE(tournament_state_t)
GBP_REG_NS_TABLE(money_info_t)
GBP_REG_NS_TABLE(action_info_t)
GBP_REG_NS_TABLE(user_info_t)
GBP_REG_NS_TABLE(user_state_t)
GBP_REG_NS_TABLE(table_player_info_t)
#undef GBP_REG_NS_TABLE
