#pragma once

#include <qmetatype.h>
#include "defs.hpp"

Q_DECLARE_METATYPE(std::vector<gbp_i8>)
Q_DECLARE_METATYPE(gbp_i8)
Q_DECLARE_METATYPE(std::vector<gbp_i64>)
Q_DECLARE_METATYPE(std::string)

//Q_DECLARE_METATYPE(decltype(std::declval<gbp::ns_api::ns_table::table_state_t>()._waiters_count))
