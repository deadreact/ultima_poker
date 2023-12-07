#pragma once

#include <string>
#include <vector>

#include "types.h"
#include "../api_special_offers.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_lobby
  {
   namespace ns_reply
   {
    GBP_DECLARE_ENUM(
     e_lobby
     , gbp_u8
     , inline
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
    )

    struct lobby_common
    {
     virtual ~lobby_common() { }

     // security
     virtual void hardware_checking(gbp::ns_api::ns_lobby::ns_reply::e_lobby result) = 0;

     // registration
     virtual void nick_free(gbp::ns_api::ns_lobby::ns_reply::e_lobby result) = 0;
     virtual void email_free(gbp::ns_api::ns_lobby::ns_reply::e_lobby result) = 0;

     // authorization
     virtual void restore_password_step1(gbp::ns_api::ns_lobby::ns_reply::e_lobby result) = 0;
     virtual void restore_password_step2(gbp::ns_api::ns_lobby::ns_reply::e_lobby result) = 0;
     virtual void restore_password_step3(gbp::ns_api::ns_lobby::ns_reply::e_lobby result) = 0;
     virtual void change_password(gbp::ns_api::ns_lobby::ns_reply::e_lobby result) = 0;
     virtual void logout(gbp::ns_api::ns_lobby::ns_reply::e_lobby result) = 0;
     virtual void change_avatar(gbp::ns_api::ns_lobby::ns_reply::e_lobby result) = 0;
     virtual void change_email_step1(gbp::ns_api::ns_lobby::ns_reply::e_lobby result) = 0;
     virtual void change_email_step2(gbp::ns_api::ns_lobby::ns_reply::e_lobby result) = 0;

     // payments
     virtual void make_payment_out(gbp::ns_api::ns_lobby::ns_reply::e_lobby result) = 0;

     // special offers
     virtual void get_random_prize_item(
      gbp::ns_api::ns_lobby::ns_reply::e_lobby result
      , const gbp::ns_api::ns_special_offers::random_prize_item& info) = 0;

     // misc
     virtual void send_money_to_user(gbp::ns_api::ns_lobby::ns_reply::e_lobby result) = 0;
     // first - ID юзера, second - base64 аватара
	 virtual void get_avatars(
      gbp::ns_api::ns_lobby::ns_reply::e_lobby result
      , const std::unordered_map<gbp_i64, std::string>& avatars) = 0;
     virtual void get_tickets(
      gbp::ns_api::ns_lobby::ns_reply::e_lobby result
      , const std::vector<ns_api::ns_lobby::user_ticket>& tickets) = 0;
     virtual void update_address(gbp::ns_api::ns_lobby::ns_reply::e_lobby result) = 0;
     virtual void update_phone_number(gbp::ns_api::ns_lobby::ns_reply::e_lobby result) = 0;
     virtual void get_vip_system_info(
      gbp::ns_api::ns_lobby::ns_reply::e_lobby result
      , const gbp::ns_api::ns_lobby::vip_system_info& info) = 0;

     /**
      * result:
      * 'ok', 'unknown_error', 'already_exists' (бонус уже получен) ,
      * 'not_verified' (бездепозитный бонус 
      * возможен только после верификации) */
     virtual void get_nodeposit_bonus(gbp::ns_api::ns_lobby::ns_reply::e_lobby result) = 0;

    }; // struct lobby_common

    struct lobby_desktop
    {
     virtual ~lobby_desktop() { }

     // 'ok', 'not_registered'
     virtual void set_user_lang(gbp::ns_api::ns_lobby::ns_reply::e_lobby result) = 0;

     /**
      * result:
      * 'ok', 'unknown_error', already_online',
      * 'bad_username_or_password', 'not_registered',
      * 'user_banned', 'email_not_verified' */
     virtual void login(gbp::ns_api::ns_lobby::ns_reply::e_lobby result, const gbp::ns_api::ns_lobby::user_info_t& u_info) = 0;
    };

    struct lobby_mobile
    {
     virtual ~lobby_mobile() { }

     virtual void mobile_ping() = 0;

     /**
      * авторизация для мобайла
      * 
      * result:
      * 'ok', 'unknown_error', 'already_online', 
      * 'bad_username_or_password', 'not_activated', 'user_banned'
      * 'token_expired', 'wrong_token', 'email_not_verified' */
     virtual void mobile_login(gbp::ns_api::ns_lobby::ns_reply::e_lobby result, const gbp::ns_api::ns_lobby::mobile_user_info_t& u_info) = 0;
    };

   } // ns_reply
  } // ns_lobby

  namespace ns_bots
  {
   namespace ns_reply
   {
    struct authorization
    {
     GBP_DECLARE_ENUM(
      e_bot_lobby
      , gbp_u8
      , friend
      , (ok)
       (unknown_error)
     )
     virtual ~authorization() { }

     virtual void bot_login(gbp::ns_api::ns_bots::ns_reply::authorization::e_bot_lobby result, gbp_i64 bot_id) = 0;
    };
   } // ns_reply
  } // ns_bots

 } // namespace ns_api
} // namespace gbp