#include <precompiled.h>
#include "helper_enum.h"
#include <api/lobby/reply.h>
#include <api/lobby_stat/reply.h>
#include <api/table/reply.h>

template<> const gbp_u8 status_enum<ns_lobby::e_status>::ok                = static_cast<gbp_u8>(ns_lobby::e_status::ok);
template<> const gbp_u8 status_enum<ns_lobby::e_status>::unknown_error     = static_cast<gbp_u8>(ns_lobby::e_status::unknown_error);
template<> const gbp_u8 status_enum<ns_statistic::e_status>::ok            = static_cast<gbp_u8>(ns_statistic::e_status::ok);
template<> const gbp_u8 status_enum<ns_statistic::e_status>::unknown_error = static_cast<gbp_u8>(ns_statistic::e_status::unknown_error);
template<> const gbp_u8 status_enum<ns_table::e_status>::ok                = static_cast<gbp_u8>(ns_table::e_status::ok);
template<> const gbp_u8 status_enum<ns_table::e_status>::unknown_error     = static_cast<gbp_u8>(ns_table::e_status::unknown_error);


template<> QByteArray status_enum<ns_lobby::e_status>::to_string(ns_lobby::e_status e) {
    std::ostringstream os; os << e; return QByteArray::fromStdString(os.str());
}
template<> QByteArray status_enum<ns_statistic::e_status>::to_string(ns_statistic::e_status e) {
    std::ostringstream os; os << e; return QByteArray::fromStdString(os.str());
}
template<> QByteArray status_enum<ns_table::e_status>::to_string(ns_table::e_status e) {
    std::ostringstream os; os << e; return QByteArray::fromStdString(os.str());
}


namespace ns_helpers
{


} //namespace ns_helpers


/**
     , (ok)
     (unknown_error)
     (already_running)
     (banned)
     (not_valid)
     (not_free)
     (nick_busy)
     (nick_not_free)
     (reg_login_busy)
     (reg_email_busy)
     (already_activated)
     (bad_username_or_password)
     (not_activated)
     (hardware_banned)
     (user_hardware_banned)
     (user_banned)
     (already_online)
     (wrong_token)
     (token_expired)
     (not_registered)
     (invalid_key)
     (empty_password)
     (passwords_do_not_match)
     (new_email_already_exists)
     (key_wrong)
     (email_wrong)
     (not_enough_account_money)
     (wrong_available_money)
     (wrong_receiver_nickname)
     (email_not_verified)
     (not_verified)
     (already_exists)

 */
