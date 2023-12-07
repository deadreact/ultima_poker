#pragma once

namespace gbp {
namespace ns_api {
namespace ns_lobby {
namespace ns_request {
/******************* request ***********************/
struct lobby_common;
struct lobby_desktop;
/**************************************************/
}
namespace ns_reply {
/******************** reply ***********************/
struct lobby_common;
struct lobby_desktop;
/**************************************************/
}
}
}
}
/******************* emitter **********************/
#include "emitter_lobby.h"
/**************************************************/


//! @namespace ns_lobby
namespace ns_lobby
{

    //! @class rpc_object
    /** @brief базовый класс для работы с api/lobby на qt.
     *         В каждом методе в качестве последнего (необязательного) аргумента передается Callback, который \b единоразово вызовется, когда придет ответ от сервера
     *         Использует более родные для qt типы параметров
     */
    struct rpc_object
    {
        //! @note QString    - имена, названия, описания
        //! @note QByteArray - коды, пароли, а также BLOB
    public:
        //! @pure request_common
        virtual gbp::ns_api::ns_lobby::ns_request::lobby_common*  request_common() const = 0;
        //! @pure request_desktop
        virtual gbp::ns_api::ns_lobby::ns_request::lobby_desktop* request_desktop() const = 0;
        //! @pure emitter
        virtual const emitter_t* emitter() const = 0;

        static QString classname_common();
        static QString classname_desktop();
    public:
        virtual void login(const QString &nick, const QByteArray &password, const std::string &hardware_key, on_login callback);
        void login(const QString &nick, const QByteArray &password, const std::string &hardware_key, on_common_reply callback = on_common_reply());
        virtual void logout(on_common_reply callback = on_common_reply());

        virtual void nick_free(             const QString &nick, on_common_reply callback = on_common_reply());
        virtual void email_free(            const QString &email, on_common_reply callback = on_common_reply());

        virtual void set_user_lang(         const QString &lang, on_common_reply callback = on_common_reply());
        virtual void hardware_checking(     const std::string &hardware_key, on_common_reply callback = on_common_reply());

        virtual void get_nodeposit_bonus(on_common_reply callback = on_common_reply());

        virtual void get_random_prize_item(on_get_random_prize_item callback = on_get_random_prize_item());

        virtual void update_address(gbp_u16 country
                                  , const QString &city
                                  , const QString &mail_index
                                  , const QString &address
                                  , on_common_reply callback = on_common_reply());
        virtual void update_phone_number(   const std::string &phone_number, on_common_reply callback = on_common_reply());
        inline  void update_phone_number(   const QString &phone_number, on_common_reply callback = on_common_reply());
        virtual void change_avatar(         const QPixmap &new_avatar, on_common_reply callback = on_common_reply());
        virtual void change_password(       const QByteArray &current_pass, const QByteArray &new_pass, on_common_reply callback = on_common_reply());
        virtual void restore_password(      const QString &email_or_login, on_common_reply callback = on_common_reply());
        virtual void restore_password(      const QString &email_or_login, const QByteArray &key, on_common_reply callback = on_common_reply());
        virtual void restore_password(      const QString &email_or_login, const QByteArray &password, const QByteArray &retyped_password, const QByteArray &key, on_common_reply callback = on_common_reply());
        virtual void change_email_step1(    const QString &new_email, on_common_reply callback = on_common_reply());
        virtual void change_email_step2(    const QByteArray &key, on_common_reply callback = on_common_reply());

        virtual void make_payment_out(      e_payment_system ps, const QString &description, gbp_i64 amount, on_common_reply callback = on_common_reply());
        virtual void send_money_to_user(    const QString &receiver_nickname, gbp_i64 amount, on_common_reply callback = on_common_reply());
        virtual void get_avatars(           const std::vector<gbp_i64> &user_ids, on_get_avatars callback = on_get_avatars());
        virtual void get_tickets(           on_get_tickets callback = on_get_tickets());
        virtual void get_vip_system_info(   on_get_vip_system_info callback = on_get_vip_system_info());

    };
} //namespace ns_lobby

void ns_lobby::rpc_object::update_phone_number(const QString &phone_number, on_common_reply callback) {
    update_phone_number(phone_number.toStdString(), callback);
}
