#pragma once

#ifndef Q_MOC_RUN
    #include <rpcbase/emitter_lobby.h>
    #include <api/lobby/reply.h>
    #include <netio/object_registry.hpp>
#endif // Q_MOC_RUN

class NetworkClient;

struct reply_lobby : gbp::ns_api::ns_lobby::ns_reply::lobby_common, gbp::ns_api::ns_lobby::ns_reply::lobby_desktop
{
public:
    typedef gbp::ns_api::ns_lobby::ns_reply::e_lobby e_lobby;
    typedef ns_lobby::emitter_t emitter_t;
    const emitter_t *emitter() const;
private:
    gbp::net::object_registry_ptr m_reg_common;
    gbp::net::object_registry_ptr m_reg_desktop;
    bool m_isInitialized;
    void init();
public:
    explicit reply_lobby(NetworkClient* const client);

    virtual void get_tickets(e_lobby result, const std::vector<gbp::ns_api::ns_lobby::user_ticket>& tickets);
    virtual void hardware_checking(e_lobby result) override;

    virtual void login(e_lobby result, const gbp::ns_api::ns_lobby::user_info_t &u_info) override;
    virtual void logout(e_lobby result) override;

    virtual void nick_free(e_lobby result) override;
    virtual void email_free(e_lobby result) override;

    virtual void get_nodeposit_bonus(e_lobby status) override;
//    virtual void activation(e_lobby result, const gbp::ns_api::ns_lobby::user_info_t &u_info) override;
//    virtual void register_user(e_lobby result) override;

//    virtual void partner_code_valid(e_lobby result) override;
//    virtual void get_partner_code(e_lobby result, const std::string &partner_code) override;

    virtual void restore_password_step1(e_lobby result) override;
    virtual void restore_password_step2(e_lobby result) override;
    virtual void restore_password_step3(e_lobby result) override;
    virtual void change_password(e_lobby result) override;
    virtual void change_avatar(e_lobby result) override;
    virtual void change_email_step1(e_lobby result) override;
    virtual void change_email_step2(e_lobby result) override;
    virtual void get_avatars(e_lobby result, const std::unordered_map<gbp_i64, std::string> &avatars) override;
    virtual void update_address(e_lobby result) override;
    virtual void update_phone_number(e_lobby result) override;

    virtual void make_payment_out(e_lobby result) override;
    virtual void send_money_to_user(e_lobby result) override;

    virtual void get_random_prize_item(e_lobby result, const gbp::ns_api::ns_special_offers::random_prize_item &info) override;
    virtual void set_user_lang(e_lobby result) override;

    virtual void get_vip_system_info(e_lobby result, const gbp::ns_api::ns_lobby::vip_system_info& info) override;
};

//namespace ns_reply_lobby {
//    struct e_lobby {
//        e_lobby(reply_lobby::e_lobby val): val(val) {}
//        reply_lobby::e_lobby val;
//        static const reply_lobby::e_lobby ok = reply_lobby::e_lobby::ok;
//        operator reply_lobby::e_lobby() { return val; }
//    };
//}

//class emitter_lobby : public QObject
//{
//    Q_OBJECT
//public:
//    explicit emitter_lobby(QObject *parent = 0);
//signals:
//    void get_tickets(reply_lobby::e_lobby result, const std::vector<gbp::ns_api::ns_lobby::user_ticket>& tickets) const;
//    void hardware_checking(reply_lobby::e_lobby result) const;
//    void nick_free(reply_lobby::e_lobby result) const;
//    void email_free(reply_lobby::e_lobby result) const;
//    void register_user(reply_lobby::e_lobby result) const;
//    void partner_code_valid(reply_lobby::e_lobby result) const;
//    void get_partner_code(reply_lobby::e_lobby result, const std::string &partner_code) const;
//    void activation(reply_lobby::e_lobby result, const gbp::ns_api::ns_lobby::user_info_t &u_info) const;
//    void login(reply_lobby::e_lobby result, const gbp::ns_api::ns_lobby::user_info_t &u_info) const;
//    void restore_password_step1(reply_lobby::e_lobby result) const;
//    void restore_password_step2(reply_lobby::e_lobby result) const;
//    void restore_password_step3(reply_lobby::e_lobby result) const;
//    void change_password(reply_lobby::e_lobby result) const;
//    void logout(reply_lobby::e_lobby result) const;
//    void change_avatar(reply_lobby::e_lobby result) const;
//    void change_email_step1(reply_lobby::e_lobby result) const;
//    void change_email_step2(reply_lobby::e_lobby result) const;
//    void make_payment_out(reply_lobby::e_lobby result) const;
//    void get_random_prize_item(reply_lobby::e_lobby result, const gbp::ns_api::ns_special_offers::random_prize_item &info) const;
//    void send_money_to_user(reply_lobby::e_lobby result) const;
//    void get_avatars(reply_lobby::e_lobby result, const std::unordered_map<gbp_i64, std::string> &avatars) const;
//    void update_address(reply_lobby::e_lobby result) const;
//    void update_phone_number(reply_lobby::e_lobby result) const;
//    void set_user_lang(reply_lobby::e_lobby result) const;
//    void get_vip_system_info(reply_lobby::e_lobby result, const gbp::ns_api::ns_lobby::vip_system_info& info) const;
//};

Q_DECLARE_METATYPE(reply_lobby::e_lobby)
