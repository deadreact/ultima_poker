#pragma once

#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../api_common.h"
#include "../adminko/types.h"
#include "../bots/types.h"
#include "../api_special_offers.h"

namespace gbp 
{
 namespace ns_translators
 {

  inline std::string str_to_lower(const std::string& str) 
  {
   std::string data = str;
   std::transform(data.begin(), data.end(), data.begin(), tolower);
   return data;
  }

  inline std::string get_str_money(ns_api::money amount)
  {
   std::ostringstream os;
   double d_amount = (double)amount / (double)100;
   if ((amount % 100) > 0) {
    os << std::fixed << std::setprecision(2) << d_amount;
   }
   else {
    os << (int)d_amount;
   }

   return os.str();
  }

  inline std::string get_str_admin_type(ns_api::ns_admin::e_admin_type a_type)
  {
   std::string result = "superadmin";

   switch (a_type)
   {
    case ns_api::ns_admin::e_admin_type::superadmin:
    break;
    case ns_api::ns_admin::e_admin_type::support:
     result = "support";
    break;
    case ns_api::ns_admin::e_admin_type::marketing:
     result = "marketing";
    break;
   }

   return result;
  }

  inline ns_api::ns_admin::e_admin_type
  get_admin_type_from_str(const std::string& str_type)
  {
   ns_api::ns_admin::e_admin_type result = ns_api::ns_admin::e_admin_type::support;

   if (str_type == "superadmin") {
    result = ns_api::ns_admin::e_admin_type::superadmin;
   }
   else if (str_type == "marketing") {
    result = ns_api::ns_admin::e_admin_type::marketing;
   }

   return result;
  }

  inline std::string get_str_payment_system(ns_api::e_payment_system payment_system)
  {
   std::string result = "skrill";

   switch (payment_system)
   {
    case ns_api::e_payment_system::all:
    break;
    case ns_api::e_payment_system::manual:
     result = "manual";
    break;
    case ns_api::e_payment_system::skrill:
    break;
    case ns_api::e_payment_system::neteller:
     result = "neteller";
    break;
    case ns_api::e_payment_system::paysafecard:
     result = "paysafecard";
    break;
    case ns_api::e_payment_system::visa:
     result = "visa";
    break;
    case ns_api::e_payment_system::mastercard:
     result = "mastercard";
    break;
    case ns_api::e_payment_system::banktransfer:
     result = "banktransfer";
    break;
    case ns_api::e_payment_system::entropay:
     result = "entropay";
    break;
    case ns_api::e_payment_system::bitcoin:
     result = "bitcoin";
    break;
    case ns_api::e_payment_system::ecopayz:
     result = "ecopayz";
    break;
   }

   return result;
  }

  inline ns_api::e_payment_system
  get_payment_system_from_str(const std::string& payment_system)
  {
   ns_api::e_payment_system result = ns_api::e_payment_system::skrill;

   if (payment_system == "manual") {
    result = ns_api::e_payment_system::manual;
   }
   else if (payment_system == "skrill") {
    result = ns_api::e_payment_system::skrill;
   }
   else if (payment_system == "neteller") {
    result = ns_api::e_payment_system::neteller;
   }
   else if (payment_system == "paysafecard") {
    result = ns_api::e_payment_system::paysafecard;
   }
   else if (payment_system == "visa") {
    result = ns_api::e_payment_system::visa;
   }
   else if (payment_system == "mastercard") {
    result = ns_api::e_payment_system::mastercard;
   }
   else if (payment_system == "banktransfer") {
    result = ns_api::e_payment_system::banktransfer;
   }
   else if (payment_system == "entropay") {
    result = ns_api::e_payment_system::entropay;
   }
   else if (payment_system == "bitcoin") {
    result = ns_api::e_payment_system::bitcoin;
   }
   else if (payment_system == "ecopayz") {
    result = ns_api::e_payment_system::ecopayz;
   }

   return result;
  }

  inline std::string get_str_payment_out_status(ns_api::e_payment_out_status po_status)
  {
   std::string result = "wait";

   switch (po_status)
   {
    case ns_api::e_payment_out_status::all:
     break;
    case ns_api::e_payment_out_status::wait:
     break;
    case ns_api::e_payment_out_status::fail:
     result = "fail";
     break;
    case ns_api::e_payment_out_status::success:
     result = "success";
     break;
   }

   return result;
  }

  inline ns_api::e_payment_out_status
  get_payment_out_status_from_str(const std::string& str_payment)
  {
   ns_api::e_payment_out_status result = ns_api::e_payment_out_status::wait;

   if (str_payment == "fail") {
    result = ns_api::e_payment_out_status::fail;
   }
   else if (str_payment == "success") {
    result = ns_api::e_payment_out_status::success;
   }

   return result;
  }

  // тип запланированной раздачи
  inline std::string 
  get_str_planned_distr(const ns_api::ns_bots::ns_distr_templates::e_type distr_type)
  {
   std::string result = "light";

   switch (distr_type)
   {
    case ns_api::ns_bots::ns_distr_templates::e_type::light:
     result = "light";
    break;

    case ns_api::ns_bots::ns_distr_templates::e_type::middle:
     result = "middle";
    break;

    case ns_api::ns_bots::ns_distr_templates::e_type::good:
     result = "good";
    break;

    case ns_api::ns_bots::ns_distr_templates::e_type::high:
     result = "high";
    break;
   }

   return result;
  }

  inline ns_api::ns_bots::ns_distr_templates::e_type
  get_planned_distr_from_str(const std::string& distr_type)
  {
   ns_api::ns_bots::ns_distr_templates::e_type result = ns_api::ns_bots::ns_distr_templates::e_type::middle;

   if (distr_type == "light") {
    result = ns_api::ns_bots::ns_distr_templates::e_type::light;
   }
   else if (distr_type == "middle") {
    result = ns_api::ns_bots::ns_distr_templates::e_type::middle;
   }
   else if (distr_type == "good") {
    result = ns_api::ns_bots::ns_distr_templates::e_type::good;
   }
   else if (distr_type == "high") {
    result = ns_api::ns_bots::ns_distr_templates::e_type::high;
   }

   return result;
  }

  // строковый вид комбинации
  inline std::string 
  get_str_win_combination(const ns_api::ns_table::e_cards_combination& comb)
  {
   using namespace ns_api::ns_table;

   std::ostringstream os;

   switch (comb)
   {
    case e_cards_combination::hight_card:
     os << "hight card";
     break;
    case e_cards_combination::pair:
     os << "pair";
     break;
    case e_cards_combination::doper:
     os << "doper";
     break;
    case e_cards_combination::set:
     os << "set";
     break;
    case e_cards_combination::wheel:
     os << "wheel";
     break;
    case e_cards_combination::straight:
     os << "straight";
     break;
    case e_cards_combination::steel_wheel:
     os << "steel_wheel";
     break;
    case e_cards_combination::flush:
     os << "flush";
     break;
    case e_cards_combination::full_house:
     os << "full house";
     break;
    case e_cards_combination::four:
     os << "four";
     break;
    case e_cards_combination::straight_flush:
     os << "straight flush";
     break;
    case e_cards_combination::royal_flush:
     os << "royal flush";
     break;
   }

   return os.str();
  }

  inline std::string
  get_str_vip_bonus(ns_vip_system::e_bonus vip_bonus)
  {
   std::string result = "rakeback_bonus";

   switch (vip_bonus)
   {
    case ns_vip_system::e_bonus::all:
     result = "all";
    break;

    case ns_vip_system::e_bonus::rakeback_bonus:
     result = "rakeback_bonus";
    break;

    case ns_vip_system::e_bonus::status_bonus:
     result = "status_bonus";
    break;
   }

   return result;
  }

  inline ns_vip_system::e_bonus
  get_vip_bonus_from_str(const std::string& stack_type)
  {
   ns_vip_system::e_bonus result = ns_vip_system::e_bonus::rakeback_bonus;

   if (stack_type == "rakeback_bonus") {
    result = ns_vip_system::e_bonus::rakeback_bonus;
   }
   else if (stack_type == "status_bonus") {
    result = ns_vip_system::e_bonus::status_bonus;
   }

   return result;
  }

  inline ns_vip_system::e_status
  get_vip_status_from_int(int vip_status)
  {
   ns_vip_system::e_status result = ns_vip_system::e_status::all;

   if (vip_status == 1) {
    result = ns_vip_system::e_status::beginner;
   }
   else if (vip_status == 2) {
    result = ns_vip_system::e_status::semi_pro;
   }
   else if (vip_status == 3) {
    result = ns_vip_system::e_status::professional;
   }
   else if (vip_status == 4) {
    result = ns_vip_system::e_status::elite;
   }
   else if (vip_status == 5) {
    result = ns_vip_system::e_status::master;
   }
   else if (vip_status == 6) {
    result = ns_vip_system::e_status::legend;
   }

   return result;
  }

  inline ns_vip_system::e_status
  get_vip_status_from_str(const std::string& vip_status)
  {
   ns_vip_system::e_status result = ns_vip_system::e_status::beginner;

   if (vip_status == "semi_pro") {
    result = ns_vip_system::e_status::semi_pro;
   }
   else if (vip_status == "professional") {
    result = ns_vip_system::e_status::professional;
   }
   else if (vip_status == "elite") {
    result = ns_vip_system::e_status::elite;
   }
   else if (vip_status == "master") {
    result = ns_vip_system::e_status::master;
   }
   else if (vip_status == "legend") {
    result = ns_vip_system::e_status::legend;
   }

   return result;
  }

  inline std::string
  get_str_vip_status(ns_vip_system::e_status vip_status)
  {
   std::string result = "beginner";

   switch (vip_status)
   {
    case ns_vip_system::e_status::all:
     result = "all";
    break;

    case ns_vip_system::e_status::beginner:
     result = "beginner";
    break;

    case ns_vip_system::e_status::semi_pro:
     result = "semi_pro";
    break;

    case ns_vip_system::e_status::professional:
     result = "professional";
    break;

    case ns_vip_system::e_status::elite:
     result = "elite";
    break;

    case ns_vip_system::e_status::master:
     result = "master";
    break;

    case ns_vip_system::e_status::legend:
     result = "legend";
    break;
   }

   return result;
  }

  inline ns_api::ns_tournament_types::e_tournament_stack
  get_tourn_stack_type_from_str(const std::string& stack_type)
  {
   ns_api::ns_tournament_types::e_tournament_stack result = ns_api::ns_tournament_types::e_tournament_stack::normal_stack;

   if (stack_type == "double_stack") {
    result = ns_api::ns_tournament_types::e_tournament_stack::double_stack;
   }
   else if (stack_type == "super_stack") {
    result = ns_api::ns_tournament_types::e_tournament_stack::super_stack;
   }
   else if (stack_type == "manual_stack") {
    result = ns_api::ns_tournament_types::e_tournament_stack::manual_stack;
   }

   return result;
  }

  inline std::string
  get_str_tourn_stack_type(ns_api::ns_tournament_types::e_tournament_stack tourn_stack_type)
  {
   std::string result;

   switch (tourn_stack_type)
   {
    case ns_api::ns_tournament_types::e_tournament_stack::normal_stack:
     result = "normal_stack";
    break;

    case ns_api::ns_tournament_types::e_tournament_stack::double_stack:
     result = "double_stack";
    break;

    case ns_api::ns_tournament_types::e_tournament_stack::super_stack:
     result = "super_stack";
    break;

    case ns_api::ns_tournament_types::e_tournament_stack::manual_stack:
     result = "manual_stack";
    break;
   }

   return result;
  }

  inline std::string
  get_str_game_type(ns_api::ns_game::e_game_type game_type)
  {
   std::string result;

   switch (game_type)
   {
    case ns_api::ns_game::e_game_type::texas_holdem:
     result = "texas_holdem";
    break;

    case ns_api::ns_game::e_game_type::omaha:
     result = "omaha";
    break;

    case ns_api::ns_game::e_game_type::omaha_hi_low:
     result = "omaha_hi_low";
    break;

    case ns_api::ns_game::e_game_type::stud:
     result = "stud";
    break;

    case ns_api::ns_game::e_game_type::stud_hi_low:
     result = "stud_hi_low";
    break;
   }

   return result;
  }

  inline ns_api::ns_game::e_game_type
  get_game_type_from_str(const std::string& game_type)
  {
   ns_api::ns_game::e_game_type result = ns_api::ns_game::e_game_type::texas_holdem;

   if (game_type == "omaha") {
    result = ns_api::ns_game::e_game_type::omaha;
   }
   else if (game_type == "omaha_hi_low") {
    result = ns_api::ns_game::e_game_type::omaha_hi_low;
   }
   else if (game_type == "stud") {
    result = ns_api::ns_game::e_game_type::stud;
   }
   else if (game_type == "stud_hi_low") {
    result = ns_api::ns_game::e_game_type::stud_hi_low;
   }

   return result;
  }

  inline std::string
  get_str_game_limit(ns_api::ns_game::e_game_limit game_limit)
  {
   std::string result;

   switch (game_limit)
   {
    case ns_api::ns_game::e_game_limit::fixed_limit:
     result = "fixed_limit";
    break;

    case ns_api::ns_game::e_game_limit::no_limit:
     result = "no_limit";
    break;

    case ns_api::ns_game::e_game_limit::pot_limit:
     result = "pot_limit";
    break;
   }

   return result;
  }

  inline ns_api::ns_game::e_game_limit
  get_game_limit_from_str(const std::string& game_limit)
  {
   ns_api::ns_game::e_game_limit result = ns_api::ns_game::e_game_limit::fixed_limit;

   if (game_limit == "no_limit") {
    result = ns_api::ns_game::e_game_limit::no_limit;
   }
   else if (game_limit == "pot_limit") {
    result = ns_api::ns_game::e_game_limit::pot_limit;
   }

   return result;
  }
 
  inline std::string
  get_str_game_speed(ns_api::ns_game::e_game_speed game_speed)
  {
   std::string result;

   switch (game_speed)
   {
    case ns_api::ns_game::e_game_speed::normal:
     result = "normal";
    break;

    case ns_api::ns_game::e_game_speed::turbo:
     result = "turbo";
    break;

    case ns_api::ns_game::e_game_speed::super_turbo:
     result = "super_turbo";
    break;
   }

   return result;
  }

  inline ns_api::ns_game::e_game_speed
  get_game_speed_from_str(const std::string& game_speed)
  {
   ns_api::ns_game::e_game_speed result = ns_api::ns_game::e_game_speed::normal;

   if (game_speed == "turbo") {
    result = ns_api::ns_game::e_game_speed::turbo;
   }
   else if (game_speed == "super_turbo") {
    result = ns_api::ns_game::e_game_speed::super_turbo;
   }

   return result;
  }

  inline std::string
  get_str_admin_filter_game_type(ns_api::ns_admin::ns_filter::e_game_type game_type)
  {
   std::string result;

   switch (game_type)
   {
    case ns_api::ns_admin::ns_filter::e_game_type::all:
     result = "all";
    break;

    case ns_api::ns_admin::ns_filter::e_game_type::texas_holdem:
     result = "texas_holdem";
    break;

    case ns_api::ns_admin::ns_filter::e_game_type::omaha:
     result = "omaha";
    break;

    case ns_api::ns_admin::ns_filter::e_game_type::omaha_hi_low:
     result = "omaha_hi_low";
    break;

    case ns_api::ns_admin::ns_filter::e_game_type::stud:
     result = "stud";
    break;

    case ns_api::ns_admin::ns_filter::e_game_type::stud_hi_low:
     result = "stud_hi_low";
    break;
   }

   return result;
  }

  inline ns_api::ns_admin::ns_filter::e_game_type
  get_admin_filter_game_type_from_str(const std::string& game_type)
  {
   ns_api::ns_admin::ns_filter::e_game_type result = ns_api::ns_admin::ns_filter::e_game_type::texas_holdem;

   if (game_type == "all") {
    result = ns_api::ns_admin::ns_filter::e_game_type::all;
   }
   else if (game_type == "omaha") {
    result = ns_api::ns_admin::ns_filter::e_game_type::omaha;
   }
   else if (game_type == "omaha_hi_low") {
    result = ns_api::ns_admin::ns_filter::e_game_type::omaha_hi_low;
   }
   else if (game_type == "stud") {
    result = ns_api::ns_admin::ns_filter::e_game_type::stud;
   }
   else if (game_type == "stud_hi_low") {
    result = ns_api::ns_admin::ns_filter::e_game_type::stud_hi_low;
   }

   return result;
  }

  inline std::string
  get_str_admin_filter_game_limit(ns_api::ns_admin::ns_filter::e_game_limit game_limit)
  {
   std::string result;

   switch (game_limit)
   {
    case ns_api::ns_admin::ns_filter::e_game_limit::all:
     result = "all";
    break;

    case ns_api::ns_admin::ns_filter::e_game_limit::fixed_limit:
     result = "fixed_limit";
    break;

    case ns_api::ns_admin::ns_filter::e_game_limit::no_limit:
     result = "no_limit";
    break;

    case ns_api::ns_admin::ns_filter::e_game_limit::pot_limit:
     result = "pot_limit";
    break;
   }

   return result;
  }

  inline ns_api::ns_admin::ns_filter::e_game_limit
  get_admin_filter_game_limit_from_str(const std::string& game_limit)
  {
   ns_api::ns_admin::ns_filter::e_game_limit result = ns_api::ns_admin::ns_filter::e_game_limit::fixed_limit;

   if (game_limit == "all") {
    result = ns_api::ns_admin::ns_filter::e_game_limit::all;
   }
   else if (game_limit == "no_limit") {
    result = ns_api::ns_admin::ns_filter::e_game_limit::no_limit;
   }
   else if (game_limit == "pot_limit") {
    result = ns_api::ns_admin::ns_filter::e_game_limit::pot_limit;
   }

   return result;
  } 

  inline std::string
  get_str_admin_filter_game_speed(ns_api::ns_admin::ns_filter::e_game_speed game_speed)
  {
   std::string result;

   switch (game_speed)
   {
    case ns_api::ns_admin::ns_filter::e_game_speed::all:
     result = "all";
    break;

    case ns_api::ns_admin::ns_filter::e_game_speed::normal:
     result = "normal";
    break;

    case ns_api::ns_admin::ns_filter::e_game_speed::turbo:
     result = "turbo";
    break;

    case ns_api::ns_admin::ns_filter::e_game_speed::super_turbo:
     result = "super_turbo";
    break;
   }

   return result;
  }

  inline ns_api::ns_admin::ns_filter::e_game_speed
  get_admin_filter_game_speed_from_str(const std::string& game_speed)
  {
   ns_api::ns_admin::ns_filter::e_game_speed result = ns_api::ns_admin::ns_filter::e_game_speed::normal;

   if (game_speed == "all") {
    result = ns_api::ns_admin::ns_filter::e_game_speed::all;
   }
   else if (game_speed == "turbo") {
    result = ns_api::ns_admin::ns_filter::e_game_speed::turbo;
   }
   else if (game_speed == "super_turbo") {
    result = ns_api::ns_admin::ns_filter::e_game_speed::super_turbo;
   }

   return result;
  }

  inline std::string
  get_str_admin_filter_tourn_status(ns_api::ns_admin::ns_filter::e_tourn_status tourn_status)
  {
   std::string result;

   switch (tourn_status)
   {
    case ns_api::ns_admin::ns_filter::e_tourn_status::all:
     result = "all";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_status::finished:
     result = "finished";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_status::late_registration:
     result = "late_registration";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_status::not_activated:
     result = "not_activated";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_status::registering:
     result = "registering";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_status::started:
     result = "started";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_status::wait:
     result = "wait";
    break;
   }

   return result;
  }

  inline ns_api::ns_admin::ns_filter::e_tourn_status
  get_admin_filter_tourn_status_from_str(const std::string& tourn_status)
  {
   ns_api::ns_admin::ns_filter::e_tourn_status result = ns_api::ns_admin::ns_filter::e_tourn_status::started;

   if (tourn_status == "all") {
    result = ns_api::ns_admin::ns_filter::e_tourn_status::all;
   }
   else if (tourn_status == "finished") {
    result = ns_api::ns_admin::ns_filter::e_tourn_status::finished;
   }
   else if (tourn_status == "late_registration") {
    result = ns_api::ns_admin::ns_filter::e_tourn_status::late_registration;
   }
   else if (tourn_status == "not_activated") {
    result = ns_api::ns_admin::ns_filter::e_tourn_status::not_activated;
   }
   else if (tourn_status == "registering") {
    result = ns_api::ns_admin::ns_filter::e_tourn_status::registering;
   }
   else if (tourn_status == "wait") {
    result = ns_api::ns_admin::ns_filter::e_tourn_status::wait;
   }

   return result;
  }

  inline std::string
  get_str_admin_filter_tourn_stack_type(ns_api::ns_admin::ns_filter::e_tourn_stack tourn_stack)
  {
   std::string result;

   switch (tourn_stack)
   {
    case ns_api::ns_admin::ns_filter::e_tourn_stack::all:
     result = "all";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_stack::double_stack:
     result = "double_stack";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_stack::manual_stack:
     result = "manual_stack";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_stack::normal_stack:
     result = "normal_stack";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_stack::super_stack:
     result = "super_stack";
    break;
   }

   return result;
  }

  inline ns_api::ns_admin::ns_filter::e_tourn_stack
  get_admin_filter_tourn_stack_type_from_str(const std::string& tourn_status)
  {
   ns_api::ns_admin::ns_filter::e_tourn_stack result = ns_api::ns_admin::ns_filter::e_tourn_stack::normal_stack;

   if (tourn_status == "all") {
    result = ns_api::ns_admin::ns_filter::e_tourn_stack::all;
   }
   else if (tourn_status == "double_stack") {
    result = ns_api::ns_admin::ns_filter::e_tourn_stack::double_stack;
   }
   else if (tourn_status == "manual_stack") {
    result = ns_api::ns_admin::ns_filter::e_tourn_stack::manual_stack;
   }
   else if (tourn_status == "super_stack") {
    result = ns_api::ns_admin::ns_filter::e_tourn_stack::super_stack;
   }

   return result;
  }

  inline std::string
  get_str_admin_filter_tourn_type1(ns_api::ns_admin::ns_filter::e_tourn_type1 tourn_type1)
  {
   std::string result;

   switch (tourn_type1)
   {
    case ns_api::ns_admin::ns_filter::e_tourn_type1::all:
     result = "all";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_type1::freeroll:
     result = "freeroll";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_type1::main_tourn:
     result = "main_tourn";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_type1::no_warranty:
     result = "no_warranty";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_type1::normal:
     result = "normal";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_type1::qualifier:
     result = "qualifier";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_type1::sit_n_go:
     result = "sit_n_go";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_type1::warranty:
     result = "warranty";
    break;
   }

   return result;
  }

  inline ns_api::ns_admin::ns_filter::e_tourn_type1
  get_admin_filter_tourn_type1_from_str(const std::string& tourn_type1)
  {
   ns_api::ns_admin::ns_filter::e_tourn_type1 result = ns_api::ns_admin::ns_filter::e_tourn_type1::normal;

   if (tourn_type1 == "all") {
    result = ns_api::ns_admin::ns_filter::e_tourn_type1::all;
   }
   else if (tourn_type1 == "freeroll") {
    result = ns_api::ns_admin::ns_filter::e_tourn_type1::freeroll;
   }
   else if (tourn_type1 == "main_tourn") {
    result = ns_api::ns_admin::ns_filter::e_tourn_type1::main_tourn;
   }
   else if (tourn_type1 == "no_warranty") {
    result = ns_api::ns_admin::ns_filter::e_tourn_type1::no_warranty;
   }
   else if (tourn_type1 == "normal") {
    result = ns_api::ns_admin::ns_filter::e_tourn_type1::normal;
   }
   else if (tourn_type1 == "qualifier") {
    result = ns_api::ns_admin::ns_filter::e_tourn_type1::qualifier;
   }
   else if (tourn_type1 == "sit_n_go") {
    result = ns_api::ns_admin::ns_filter::e_tourn_type1::sit_n_go;
   }
   else if (tourn_type1 == "warranty") {
    result = ns_api::ns_admin::ns_filter::e_tourn_type1::warranty;
   }

   return result;
  }

  inline std::string
  get_str_admin_filter_tourn_type2(ns_api::ns_admin::ns_filter::e_tourn_type2 tourn_stack)
  {
   std::string result;

   switch (tourn_stack)
   {
    case ns_api::ns_admin::ns_filter::e_tourn_type2::all:
     result = "all";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_type2::freezout:
     result = "freezout";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_type2::knockout:
     result = "knockout";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_type2::knockout_progr:
     result = "knockout_progr";
    break;

    case ns_api::ns_admin::ns_filter::e_tourn_type2::rebuy:
     result = "rebuy";
    break;
   }

   return result;
  }

  inline ns_api::ns_admin::ns_filter::e_tourn_type2
  get_admin_filter_tourn_type2_from_str(const std::string& tourn_type2)
  {
   ns_api::ns_admin::ns_filter::e_tourn_type2 result = ns_api::ns_admin::ns_filter::e_tourn_type2::freezout;

   if (tourn_type2 == "all") {
    result = ns_api::ns_admin::ns_filter::e_tourn_type2::all;
   }
   else if (tourn_type2 == "freezout") {
    result = ns_api::ns_admin::ns_filter::e_tourn_type2::freezout;
   }
   else if (tourn_type2 == "knockout") {
    result = ns_api::ns_admin::ns_filter::e_tourn_type2::knockout;
   }
   else if (tourn_type2 == "knockout_progr") {
    result = ns_api::ns_admin::ns_filter::e_tourn_type2::knockout_progr;
   }
   else if (tourn_type2 == "rebuy") {
    result = ns_api::ns_admin::ns_filter::e_tourn_type2::rebuy;
   }

   return result;
  }

  inline ns_api::e_user_status
  get_user_status_from_str(const std::string& user_status)
  {
   ns_api::e_user_status result = ns_api::e_user_status::offline;

   if (user_status == "all") {
    result = ns_api::e_user_status::all;
   }
   else if (user_status == "blocked") {
    result = ns_api::e_user_status::blocked;
   }
   else if (user_status == "offline") {
    result = ns_api::e_user_status::offline;
   }
   else if (user_status == "online") {
    result = ns_api::e_user_status::online;
   }

   return result;
  }

  inline std::string get_str_user_status(ns_api::e_user_status u_status)
  {
   std::string status;

   switch (u_status)
   {
    case ns_api::e_user_status::all:
     status = "all";
    break;

    case ns_api::e_user_status::blocked:
     status = "blocked";
    break;

    case ns_api::e_user_status::offline:
     status = "offline";
    break;

    case ns_api::e_user_status::online:
     status = "online";
    break;
   }

   return status;
  }

  inline std::string get_tourn_str_status(ns_api::ns_tournament_types::e_tournament_status tourn_status)
  {
   typedef ns_api::ns_tournament_types::e_tournament_status t_tourn_status;
   
   std::string result;
    
   switch (tourn_status)
   {
    case t_tourn_status::finished:
     result = "finished";
    break;
    
    case t_tourn_status::not_activated:
     result = "not_activated";
    break;

    case t_tourn_status::registering:
     result = "registering";
    break;
    
    case t_tourn_status::started:
     result = "started";
    break;
    
    case t_tourn_status::wait:
     result = "wait";
    break;
    
    case t_tourn_status::late_registration:
     result = "late_registration";
    break;
   }

   return result;  
  }

  inline ns_api::ns_tournament_types::e_tournament_status
  get_tourn_status_from_str(const std::string& tourn_status)
  {
   ns_api::ns_tournament_types::e_tournament_status result = 
    ns_api::ns_tournament_types::e_tournament_status::not_activated;
 
   if (tourn_status == "finished") {
    result = ns_api::ns_tournament_types::e_tournament_status::finished;
   }
   else if (tourn_status == "not_activated") {
    result = ns_api::ns_tournament_types::e_tournament_status::not_activated;
   }
   else if (tourn_status == "registering") {
    result = ns_api::ns_tournament_types::e_tournament_status::registering;
   }
   else if (tourn_status == "started") {
    result = ns_api::ns_tournament_types::e_tournament_status::started;
   }
   else if (tourn_status == "wait") {
    result = ns_api::ns_tournament_types::e_tournament_status::wait;
   }
   else if (tourn_status == "late_registration") {
    result = ns_api::ns_tournament_types::e_tournament_status::late_registration;
   }

   return result;  
  }

  inline std::string
  get_soffer_str_from_type(ns_api::ns_special_offers::e_type type_)
  {
   using namespace ns_api;

   std::string type_str = "rake_race_cash";

   if (type_ == ns_special_offers::e_type::rake_race_tournament) {
    type_str = "rake_race_tournament";
   }
   else if (type_ == ns_special_offers::e_type::nodeposit_bonus) {
    type_str = "nodeposit_bonus";
   }
   else if (type_ == ns_special_offers::e_type::first_deposit_bonus) {
    type_str = "first_deposit_bonus";
   }
   else if (type_ == ns_special_offers::e_type::random_prize) {
    type_str = "random_prize";
   }
   else if (type_ == ns_special_offers::e_type::status_for_deposit) {
    type_str = "status_for_deposit";
   }
   else if (type_ == ns_special_offers::e_type::status_rake_back) {
    type_str = "status_rake_back";
   }

   return type_str;
  }

  inline ns_api::ns_special_offers::e_type get_soffer_type_from_str(const std::string& type_)
  {
   using namespace ns_api;

   ns_special_offers::e_type result = ns_special_offers::e_type::rake_race_cash;

   if (type_ == "rake_race_tournament") {
    result = ns_special_offers::e_type::rake_race_tournament;
   }
   else if (type_ == "nodeposit_bonus") {
    result = ns_special_offers::e_type::nodeposit_bonus;
   }
   else if (type_ == "first_deposit_bonus") {
    result = ns_special_offers::e_type::first_deposit_bonus;
   }
   else if (type_ == "random_prize") {
    result = ns_special_offers::e_type::random_prize;
   }
   else if (type_ == "status_for_deposit") {
    result = ns_special_offers::e_type::status_for_deposit;
   }
   else if (type_ == "status_rake_back") {
    result = ns_special_offers::e_type::status_rake_back;
   }

   return result;
  }

  inline ns_api::ns_special_offers::random_prize_item::e_win_type
  get_soffer_random_prize_win_type_from_str(const std::string& type_)
  {
   using namespace ns_api;

   ns_special_offers::random_prize_item::e_win_type result = 
    ns_special_offers::random_prize_item::e_win_type::type1;

   if (type_ == "e_win_type::type1") {
    result = ns_special_offers::random_prize_item::e_win_type::type1;
   }
   else if (type_ == "e_win_type::type2") {
    result = ns_special_offers::random_prize_item::e_win_type::type2;
   }
   else if (type_ == "e_win_type::type3") {
    result = ns_special_offers::random_prize_item::e_win_type::type3;
   }
   else if (type_ == "e_win_type::type4") {
    result = ns_special_offers::random_prize_item::e_win_type::type4;
   }
   else if (type_ == "e_win_type::type5") {
    result = ns_special_offers::random_prize_item::e_win_type::type5;
   }
   else if (type_ == "e_win_type::type6") {
    result = ns_special_offers::random_prize_item::e_win_type::type6;
   }
   else if (type_ == "e_win_type::type7") {
    result = ns_special_offers::random_prize_item::e_win_type::type7;
   }
   else if (type_ == "e_win_type::type8") {
    result = ns_special_offers::random_prize_item::e_win_type::type8;
   }
   else if (type_ == "e_win_type::type9") {
    result = ns_special_offers::random_prize_item::e_win_type::type9;
   }
   else if (type_ == "e_win_type::type10") {
    result = ns_special_offers::random_prize_item::e_win_type::type10;
   }
   else if (type_ == "e_win_type::type11") {
    result = ns_special_offers::random_prize_item::e_win_type::type11;
   }
   else if (type_ == "e_win_type::type12") {
    result = ns_special_offers::random_prize_item::e_win_type::type12;
   }

   return result;
  }

  inline ns_api::ns_special_offers::e_player_status
  get_soffer_player_status_from_str(const std::string& type_)
  {
   using namespace ns_api;

   ns_special_offers::e_player_status result = 
    ns_special_offers::e_player_status::empty;

   if (type_ == "e_player_status::beginner") {
    result = ns_special_offers::e_player_status::beginner;
   }
   else if (type_ == "e_player_status::semi_pro") {
    result = ns_special_offers::e_player_status::semi_pro;
   }
   else if (type_ == "e_player_status::professional") {
    result = ns_special_offers::e_player_status::professional;
   }
   else if (type_ == "e_player_status::elite") {
    result = ns_special_offers::e_player_status::elite;
   }
   else if (type_ == "e_player_status::master") {
    result = ns_special_offers::e_player_status::master;
   }
   else if (type_ == "e_player_status::legend") {
    result = ns_special_offers::e_player_status::legend;
   }

   return result;
  }

  inline ns_api::ns_special_offers::soffer_status_rake_back::e_prize
  get_soffer_status_rake_back_prize_from_str(const std::string& type_)
  {
   using namespace ns_api;

   ns_special_offers::soffer_status_rake_back::e_prize result =
    ns_special_offers::soffer_status_rake_back::e_prize::empty;

   if (type_ == "e_prize::empty") {
    result = ns_special_offers::soffer_status_rake_back::e_prize::empty;
   }
   else if (type_ == "e_prize::rake_back_100") {
    result = ns_special_offers::soffer_status_rake_back::e_prize::rake_back_100;
   }

   return result;
  }
  
  inline std::string
  get_soffer_operation_str_from_type(ns_api::ns_special_offers::e_operation_type operation)
  {
   std::string result = "";

   if (operation == ns_api::ns_special_offers::e_operation_type::start) {
    result = "start";
   }
   else if (operation == ns_api::ns_special_offers::e_operation_type::stop) {
    result = "stop";
   }

   return result;
  }

 } // ns_translators

} // ns gbp
