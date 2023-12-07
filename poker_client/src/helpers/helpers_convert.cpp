#include <precompiled.h>
#include "helpers_convert.h"

#ifndef Q_MOC_RUN
    #include <api/lobby_stat/types.h>
    #include <api/table/types.h>
#endif // !Q_MOC_RUN

namespace ns_helpers
{




} //namespace ns_helpers


template <>
gbp::ns_api::ns_statistic::player_t gbp_cast(const gbp::ns_api::ns_table::table_player_info_t& value)
{
    gbp::ns_api::ns_statistic::player_t player;
    player._nick    = value._nickname;
    player._pos     = value._pos;
    player._money   = value._money;
    player._country = value._country;

    return player;
}
