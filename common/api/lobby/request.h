#pragma once

#include <string>
#include <vector>

#include "types.h"
#include "../bots/types.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_lobby
  {
   namespace ns_request
   {
    struct lobby_common
    {
     virtual ~lobby_common() { }

     // проверка железа на бан / внесение нового
     virtual void hardware_checking(const std::string& hardware_key) = 0;

     /**
      * вызывается когда снимается фокус из поля ввода и/или
      * при нажатии "регистрация" */
     virtual void nick_free(const std::string& nick) = 0;

     /**
      * вызывается когда снимается фокус из
      * поля ввода и/или при нажатии "регистрация" */
     virtual void email_free(const std::string& email) = 0;

     /**
      * юзер восстанавливает свой пароль, для этого в первом шаге 
      * передаёт на сервер ник или почту.
      * Сервер проверяет данные и если всё ок, то генерит ключ (1 час жизни), шлёт его юзеру на 
      * почту и юзер вводит этот ключ в поле своего диалога восстановления пароля */
     virtual void restore_password_step1(const std::string& email_or_login) = 0;

     /**
      * Если ключ правильный, то у юзера появляется возможность изменить 
      * пароль (отдельный диалог у юзера) */
     virtual void restore_password_step2(const std::string& email_or_login, const std::string& key) = 0;

     // запрос на смену пароля. Может выполниться только при наличии валидного key
     virtual void restore_password_step3(
      const std::string& email_or_login
      , const std::string& password
      , const std::string& retyped_password
      , const std::string& key) = 0;

     virtual void change_password(const std::string& current_pass, const std::string& new_pass) = 0;

     // установка статуса offline для юзера
     virtual void logout() = 0;

     // изменение аватара
     virtual void change_avatar(const std::string& new_avatar) = 0;

     // изменение мыла
     virtual void change_email_step1(const std::string& new_email) = 0;
     virtual void change_email_step2(const std::string& key) = 0;

     /**
      * description - описание заявки (желаемая платёжная система с 
      * необходимыми для неё реквизитами и т.п.);
      * amount - сумма в центах */
     virtual void make_payment_out(gbp::ns_api::e_payment_system ps, const std::string& description, ns_api::money amount) = 0;

     // запрос на выдачу приза по акции "случайный приз на депозит"
     virtual void get_random_prize_item() = 0;

     // перевод денег от одного юзера к другому
     virtual void send_money_to_user(const std::string& receiver_nickname, ns_api::money money_) = 0;
     // запрос массива аватарок по ID пользователей
	 virtual void get_avatars(const std::vector<gbp_i64>& user_ids) = 0;
     // билеты юзера
     virtual void get_tickets() = 0;
     // смена адреса (страна, почтовый индекс, адрес)
     virtual void update_address(
      gbp_u16 country
      , const std::string& city
      , const std::string& mail_index
      , const std::string& address) = 0;
     // смена номера телефона
     virtual void update_phone_number(const std::string& phone_number) = 0;
     // инфа по vip-системе
     virtual void get_vip_system_info() = 0;

     // получить "бездепозитный бонус"
     virtual void get_nodeposit_bonus() = 0;

    };

    struct lobby_desktop
    {
     virtual ~lobby_desktop() { }

     // юзер устанавливает свой язык: 'ru', 'en'...
     virtual void set_user_lang(const std::string& lang) = 0;

     /**
      * юзер логинится в лобби. В случае успешности операции сервер
      * заполняет структуры user_context и user_info данными и передаёт user_info в лобби */
     virtual void login(
      const std::string& nick
      , const std::string& password
      , const std::string& hardware_key) = 0;
    };

    struct lobby_mobile
    {
     virtual ~lobby_mobile() { }

     // чтобы не было дисконнекта
     virtual void mobile_ping() = 0;

     /**
      * авторизация для мобайла
      * если token пустой, то пароль не должен быть пустым;
      * если token не пустой, то пароль должен быть пустым; */
     virtual void mobile_login(
      const std::string& nick
      , const std::string& password
      , const std::string& hardware_key
      , const std::string& token) = 0;
    };

   } // ns_request
  } // ns_lobby

  namespace ns_bots
  {
   namespace ns_request
   {
    struct authorization
    {
     virtual ~authorization() { }

     virtual void bot_login(const gbp::ns_api::ns_bots::bot_info& info) = 0;
    };
   } // ns_request
  } // ns_bots

 } // ns_api
} // namespace gbp