#pragma once

#include "../api_common.h"
#include "../mail/types.hpp"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_mail
  {
   namespace ns_request
   {
    // game_server -> mail_server
    struct game_server_mail_server
    {
     virtual ~game_server_mail_server() { }

     virtual void send_mail(const gbp::ns_api::ns_mail::gameserver_mail_item& email) = 0;
    };

    // mail_sender -> mail_server
    struct sender_mail_server
    {
     virtual ~sender_mail_server() { }

     // логинимся
     virtual void login(const std::string &pass) = 0;

     // запускаем рассылку
     virtual void send_mail(const gbp::ns_api::ns_mail::sender_data& data) = 0;
     
     // прерываем рассылку
     virtual void stop() = 0;
    };

    // mail_server -> mail_sender
    struct mail_server_sender
    {
     virtual ~mail_server_sender() { }

     // ответ на логин
     virtual void login(bool ok) = 0;
     
     // прогресс рассылки
     virtual void progress(gbp_u32 total, gbp_u32 current, const std::string& message) = 0;
     
     // сообщение об ошибках при отправках/рассылках
     virtual void error(gbp_i32 ec, const std::string& message) = 0;
    };

   } // ns_request
  } // ns_mail
 } // ns_api

 namespace ns_db_server
 {
  namespace ns_request
  {
   // mail_server -> db_server
   struct mail_server_db_server   
   {
    virtual ~mail_server_db_server() { }

    virtual void get_mail_items() = 0;
   };
  } // ns_request
 } // ns_db_server

} // ns gbp