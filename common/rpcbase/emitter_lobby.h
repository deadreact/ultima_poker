#pragma once

#include <QObject>
#include <api/lobby/reply.h>
#include "defs.h"

//! @namespace ns_lobby
namespace ns_lobby
{
    //! @class emitter_t
    //! @brief класс - набор сигналов-ответов от сервака
    class emitter_t : public QObject
    {
        Q_OBJECT
    public:
        explicit emitter_t(QObject *parent = nullptr);
    signals:
        void hardware_checking(     e_status status) const;
        void nick_free(             e_status status) const;
        void email_free(            e_status status) const;
        void register_user(         e_status status) const;
        void partner_code_valid(    e_status status) const;
        void restore_password_step1(e_status status) const;
        void restore_password_step2(e_status status) const;
        void restore_password_step3(e_status status) const;
        void change_password(       e_status status) const;
        void logout(                e_status status) const;
        void change_avatar(         e_status status) const;
        void change_email_step1(    e_status status) const;
        void change_email_step2(    e_status status) const;
        void make_payment_out(      e_status status) const;
        void send_money_to_user(    e_status status) const;
        void update_address(        e_status status) const;
        void update_phone_number(   e_status status) const;
        void set_user_lang(         e_status status) const;
        void get_nodeposit_bonus(   e_status status) const;
        void get_partner_code(      e_status status, const std::string &partner_code) const;
        void activation(            e_status status, const user_info_t &u_info) const;
        void login(                 e_status status, const user_info_t &u_info) const;
        void get_random_prize_item( e_status status, const random_prize_item &info) const;
        void get_tickets(           e_status status, const std::vector<user_ticket>& tickets) const;
        void get_avatars(           e_status status, const std::unordered_map<gbp_i64, std::string> &avatars) const;
        void get_vip_system_info(   e_status status, const vip_system_info& info) const;
    };
} // namespace ns_lobby
