#pragma once

#ifndef Q_MOC_RUN
    #include <api/table/types.h>
#endif // !Q_MOC_RUN
#include "defs_metatype.hpp"

Q_DECLARE_METATYPE(gbp::ns_api::ns_table::table_state_t)
Q_DECLARE_METATYPE(gbp::ns_api::ns_table::tournament_state_t)
Q_DECLARE_METATYPE(gbp::ns_api::ns_table::money_info_t)
Q_DECLARE_METATYPE(gbp::ns_api::ns_table::action_info_t)
Q_DECLARE_METATYPE(gbp::ns_api::ns_table::user_info_t)
Q_DECLARE_METATYPE(gbp::ns_api::ns_table::user_state_t)
Q_DECLARE_METATYPE(gbp::ns_api::ns_table::table_player_info_t)
Q_DECLARE_METATYPE(std::vector<gbp::ns_api::ns_table::table_player_info_t>)
