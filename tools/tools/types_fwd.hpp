#pragma once
#include "../../common/api/gbp_int.hpp"

struct GameId;
struct GamePlayerId;


namespace std {
template<typename _Key, typename _Compare /*= std::less<_Key>*/,
     typename _Alloc/* = std::allocator<_Key> */>
  class set;
template <typename T> struct less;
template <typename T> class allocator;
template <typename _T1, typename _T2> struct pair;
template <typename _Tp, typename _Alloc /* = std::allocator<_Tp> */>
class vector;

template <typename _Tp> using stdvec = vector<_Tp, std::allocator<_Tp>>;

template <typename _Key, typename _Tp, typename _Compare /* = std::less<_Key>*/
              ,  typename _Alloc /* = std::allocator<std::pair<const _Key, _Tp> >*/>
class map;

} // namespace std

namespace gbp {
namespace net {
    struct object_registry;
} // namespace net
namespace ns_api {
    enum class e_payment_system : gbp_u8;
    struct birth_date;
    struct bounty_item;
    struct rebuys_t;
    struct addon_t;
    struct user_prize;
namespace ns_tourn {
    struct cancel_tourn_register_t;
} // namespace ns_tourn
namespace ns_tournament_types {
    enum class e_tournament_stack : gbp_u8;
    enum class e_tournament_status : gbp_u8;
} // namespace ns_tournament_types
namespace ns_special_offers {
    struct random_prize_item;
} // namespace ns_special_offers
namespace ns_lobby {
    struct user_info_t;
    struct user_register_info_t;
    struct user_ticket;
    struct vip_system_info;
namespace ns_reply {
    enum class e_lobby : gbp_u8;
} // namespace ns_reply
} // namespace ns_lobby
namespace ns_game {
    enum class e_game_limit : gbp_u8;
    enum class e_game_speed : gbp_u8;
    enum class e_game_type : gbp_u8;
} // namespace ns_game
namespace ns_table {
    enum class e_cards_combination : gbp_u8;
    enum class e_game_round_type : gbp_u8;
    enum class e_user_action : gbp_u8;
    struct action_info_t;
    struct available_user_course_t;
    struct money_info_t;
    struct recharge_money_t;
    struct sit_on_table_info_t;
    struct table_player_info_t;
    struct table_state_t;
    struct tourn_register_info;
    struct tournament_dialog_info;
    struct tournament_state_t;
    struct user_action_info_t;
    struct user_cashes_item_t;
    struct user_info_t;
    struct user_state_t;
    struct winner_info_t;
namespace ns_reply {
    enum class e_result : gbp_u8;
} // namespace ns_reply
} // namespace ns_table
namespace ns_tourn {
    struct cancel_tourn_register_t;
} // namespace ns_tourn
namespace ns_tournament_types {
    enum class e_tournament_status : gbp_u8;
} // namespace ns_tournament_types
namespace ns_statistic {
    enum class e_tourn_button_type : gbp_u8;
    struct cash_stat_t;
    struct player_t;
    struct tourn_additional_stat_t;
    struct tournament_stat_t;
namespace ns_reply {
    enum class e_stat : gbp_u8;
} // namespace ns_reply
} // namespace ns_statistic
} // namespace ns_api
namespace ns_vip_system {
    enum class e_bonus : gbp_u8;
    enum class e_status : gbp_u8;
} // namespace ns_vip_system
namespace ns_details {
    enum class e_gender : gbp_u8;
} // namespace ns_details
} // namespace gbp


template <typename T> class QList;
template <typename T> class QVector;
class QModelIndexRange;
using QModelIndexRangeList = QList<QModelIndexRange>;

using GameAction = std::pair<gbp::ns_api::ns_table::e_user_action, gbp_i64>;
using GameActions = std::map<gbp::ns_api::ns_table::e_user_action, gbp_i64, std::less<gbp::ns_api::ns_table::e_user_action>, std::allocator<GameAction>>;

struct AutoAction;

struct AutoAction {
private:
    enum { Invalid };
public:
    using e_user_action = gbp::ns_api::ns_table::e_user_action;
    e_user_action type;
    gbp_i64 money;
    const AutoAction* alternate;
    bool checked;
    bool available;
    constexpr AutoAction(): type(e_user_action(0)), money(0), alternate(nullptr), checked(false), available(true) {}
    constexpr AutoAction(e_user_action type, gbp_i64 money): type(type), money(money), alternate(nullptr), checked(false), available(true) {}
    constexpr AutoAction(e_user_action type, gbp_i64 money, const AutoAction* alternate): type(type), money(money), alternate(alternate), checked(false), available(true) {}
    constexpr bool isValid() const { return this != alternate; }

    constexpr AutoAction(decltype(Invalid)): type(), money(0), alternate(this), checked(false), available(false) {}
    constexpr static AutoAction invalid() { return AutoAction(Invalid); }

    static AutoAction updated(const AutoAction& action) {
        if (!action.isValid() || (!action.available && action.alternate == nullptr)) {
            return invalid();
        }
        if (action.available) {
            return action;
        }
        if (action.checked) {
            AutoAction action(*action.alternate);
            action.checked = true;
            return action;
        }
        return *action.alternate;
    }
};

using games_stat_t = std::pair<std::stdvec<gbp::ns_api::ns_statistic::cash_stat_t>, std::stdvec<gbp::ns_api::ns_statistic::tournament_stat_t>>;

class QByteArray;

typedef QByteArray sha1_t;
typedef QByteArray base64_t;

//! @namespace ns_lobby
namespace ns_lobby
{
    using e_status                 = gbp::ns_api::ns_lobby::ns_reply::e_lobby;
    using e_payment_system         = gbp::ns_api::e_payment_system;
    using random_prize_item        = gbp::ns_api::ns_special_offers::random_prize_item;
    using user_info_t              = gbp::ns_api::ns_lobby::user_info_t;
    using user_ticket              = gbp::ns_api::ns_lobby::user_ticket;
    using vip_system_info          = gbp::ns_api::ns_lobby::vip_system_info;
    using user_register_info_t     = gbp::ns_api::ns_lobby::user_register_info_t;
} //namespace ns_lobby

//! @namespace ns_statistic
namespace ns_statistic {
    using e_status                 = gbp::ns_api::ns_statistic::ns_reply::e_stat;
    using cash_stat_t              = gbp::ns_api::ns_statistic::cash_stat_t;
    using tournament_stat_t        = gbp::ns_api::ns_statistic::tournament_stat_t;
    using tourn_additional_stat_t  = gbp::ns_api::ns_statistic::tourn_additional_stat_t;
} //namespace ns_statistic

namespace ns_table {
    using e_status                 = gbp::ns_api::ns_table::ns_reply::e_result;
    using rebuys_t                 = gbp::ns_api::rebuys_t;
    using cancel_tourn_register_t  = gbp::ns_api::ns_tourn::cancel_tourn_register_t;
    using e_tourn_button_type      = gbp::ns_api::ns_statistic::e_tourn_button_type;
    using e_tournament_status      = gbp::ns_api::ns_tournament_types::e_tournament_status;
    using e_user_action            = gbp::ns_api::ns_table::e_user_action;

    using action_info_t            = gbp::ns_api::ns_table::action_info_t;
    using available_user_course_t  = gbp::ns_api::ns_table::available_user_course_t;
    using money_info_t             = gbp::ns_api::ns_table::money_info_t;
    using recharge_money_t         = gbp::ns_api::ns_table::recharge_money_t;
    using sit_on_table_info_t      = gbp::ns_api::ns_table::sit_on_table_info_t;
    using table_player_info_t      = gbp::ns_api::ns_table::table_player_info_t;
    using table_state_t            = gbp::ns_api::ns_table::table_state_t;
    using tourn_register_info      = gbp::ns_api::ns_table::tourn_register_info;
    using tournament_dialog_info   = gbp::ns_api::ns_table::tournament_dialog_info;
    using tournament_state_t       = gbp::ns_api::ns_table::tournament_state_t;
    using user_action_info_t       = gbp::ns_api::ns_table::user_action_info_t;
    using user_cashes_item_t       = gbp::ns_api::ns_table::user_cashes_item_t;
    using users_cashes_t           = std::stdvec<user_cashes_item_t>;
    using user_info_t              = gbp::ns_api::ns_table::user_info_t;
    using user_state_t             = gbp::ns_api::ns_table::user_state_t;
    using winner_info_t            = gbp::ns_api::ns_table::winner_info_t;

} // namespace ns_table
