#pragma once

#include "../api_common.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_mail
  {

   GBP_DECLARE_ENUM(
    e_from_type
    , gbp_u8
    , inline
    ,
    (news)
    (offers)
    (support)
    (security)
   )

   // для игрового
   GBP_DECLARE_TYPE(
    gameserver_mail_item
    ,
    // почта
    (_email, (std::string))
    // заголовок
    (_title, (std::string))
    // текст
    (_text, (std::string))
    // от кого слать
    (_from_type, (ns_api::ns_mail::e_from_type))
   )

   // для mail-sender'а
   GBP_DECLARE_TYPE(
    sender_data
    ,
    // заголовок
    (_subject_en, (std::string))
    (_subject_es, (std::string))
    (_subject_pt, (std::string))
    // тело письма
    (_content_en, (std::string))
    (_content_es, (std::string))
    (_content_pt, (std::string))
    // тип
    (_from_type, (ns_api::ns_mail::e_from_type))
   )

   // для ответа от БД-сервера к mail-серверу
   GBP_DECLARE_TYPE(
   db_mail_user
    ,
    (_nickname, (std::string))
    (_lang, (std::string)) // язык юзера
    (_email, (std::string))
   )

  } // ns ns_mail
 } // ns ns_api
} // ns gbp