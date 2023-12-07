#pragma once

#ifndef Q_MOC_RUN
    #include <api/lobby/request.h>
#endif // Q_MOC_RUN

class NetworkClient;

struct request_lobby : gbp::ns_api::ns_lobby::ns_request::lobby_common, gbp::ns_api::ns_lobby::ns_request::lobby_desktop
{
private:
    NetworkClient& m_client;
    static const std::string classname_common() { return "gbp::ns_api::ns_lobby::ns_request::lobby_common"; }
    static const std::string classname_desktop() { return "gbp::ns_api::ns_lobby::ns_request::lobby_desktop"; }
public:
    explicit request_lobby(NetworkClient * const client);


    // lobby_desktop interface
public:
    virtual void hardware_checking(const std::string& hardware_key) override;

    virtual void login(const std::string &nick, const std::string &password, const std::string &hardware_key = "") override;
    virtual void logout() override;

    virtual void nick_free(const std::string &nick) override;
    virtual void email_free(const std::string &email) override;
    virtual void get_nodeposit_bonus() override;
    virtual void make_payment_out(gbp::ns_api::e_payment_system ps, const std::string &description, gbp::ns_api::money amount) override;
    virtual void send_money_to_user(const std::string &receiver_nickname, gbp::ns_api::money money_) override;

    virtual void change_email_step1(const std::string &new_email) override;
    virtual void change_email_step2(const std::string &key) override;
    virtual void change_password(const std::string &current_pass, const std::string &new_pass) override;
    virtual void restore_password_step1(const std::string &email_or_login) override;
    virtual void restore_password_step2(const std::string &email_or_login, const std::string &key) override;
    virtual void restore_password_step3(const std::string &email_or_login, const std::string &password, const std::string &retyped_password, const std::string &key) override;
    virtual void update_address(gbp_u16 country, const std::string &city, const std::string &mail_index, const std::string &address) override;
    virtual void update_phone_number(const std::string &phone_number) override;
    virtual void change_avatar(const std::string &new_avatar) override;
    virtual void get_avatars(const std::vector<gbp_i64> &user_ids) override;
    inline void get_avatar(gbp_i64 user_id);

    virtual void set_user_lang(const std::string &lang) override;
    virtual void get_random_prize_item() override;

    virtual void get_vip_system_info() override;
    virtual void get_tickets() override;
};

inline void request_lobby::get_avatar(gbp_i64 user_id) {
    std::vector<gbp_i64> ids;
    ids.push_back(user_id);
    get_avatars(ids);
}
