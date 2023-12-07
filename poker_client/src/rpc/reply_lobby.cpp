#include <precompiled.h>
#ifndef Q_MOC_RUN
    #include "reply_lobby.h"
    #include "network_client.h"
#endif // Q_MOC_RUN
#include "utils/logger.h"

/*static */const std::string classname_common = "gbp::ns_api::ns_lobby::ns_reply::lobby_common";
/*static */const std::string classname_desktop = "gbp::ns_api::ns_lobby::ns_reply::lobby_desktop";

//emitter_lobby::emitter_lobby(QObject *parent): QObject(parent) {}

reply_lobby::reply_lobby(NetworkClient * const client)
    : m_reg_common(client->create_registry(classname_common))
    , m_reg_desktop(client->create_registry(classname_desktop))
    , m_isInitialized(false)
{

    qRegisterMetaType<e_lobby>("e_lobby");
    qRegisterMetaType<e_lobby>("reply_lobby::e_lobby");
    qRegisterMetaType<e_lobby>("gbp::ns_api::ns_lobby::e_lobby");

    init();
}



const reply_lobby::emitter_t *reply_lobby::emitter() const
{
    static const emitter_t emitter;
    return &emitter;
}

void reply_lobby::init()
{
    if (!m_isInitialized)
    {
         m_reg_common->on("hardware_checking",      this, &reply_lobby::hardware_checking);
         m_reg_common->on("nick_free",              this, &reply_lobby::nick_free);
         m_reg_common->on("email_free",             this, &reply_lobby::email_free);
         m_reg_common->on("get_nodeposit_bonus",    this, &reply_lobby::get_nodeposit_bonus);
         m_reg_common->on("restore_password_step1", this, &reply_lobby::restore_password_step1);
         m_reg_common->on("restore_password_step2", this, &reply_lobby::restore_password_step2);
         m_reg_common->on("restore_password_step3", this, &reply_lobby::restore_password_step3);
         m_reg_common->on("change_password",        this, &reply_lobby::change_password);
         m_reg_common->on("logout",                 this, &reply_lobby::logout);
         m_reg_common->on("change_avatar",          this, &reply_lobby::change_avatar);
         m_reg_common->on("change_email_step1",     this, &reply_lobby::change_email_step1);
         m_reg_common->on("change_email_step2",     this, &reply_lobby::change_email_step2);
         m_reg_common->on("make_payment_out",       this, &reply_lobby::make_payment_out);
         m_reg_common->on("get_random_prize_item",  this, &reply_lobby::get_random_prize_item);
         m_reg_common->on("send_money_to_user",     this, &reply_lobby::send_money_to_user);
         m_reg_common->on("get_avatars",            this, &reply_lobby::get_avatars);
         m_reg_common->on("update_address",         this, &reply_lobby::update_address);
         m_reg_common->on("update_phone_number",    this, &reply_lobby::update_phone_number);
         m_reg_common->on("get_vip_system_info",    this, &reply_lobby::get_vip_system_info);
         m_reg_common->on("get_tickets",            this, &reply_lobby::get_tickets);
         m_reg_desktop->on("login",                 this, &reply_lobby::login);
         m_reg_desktop->on("set_user_lang",         this, &reply_lobby::set_user_lang);


        m_isInitialized = true;
    }
}

void reply_lobby::get_tickets(reply_lobby::e_lobby result, const std::vector<gbp::ns_api::ns_lobby::user_ticket> &tickets) {
    emitter()->get_tickets(result, tickets);
}
void reply_lobby::hardware_checking(reply_lobby::e_lobby result) {
    emit emitter()->hardware_checking(result);
}

void reply_lobby::nick_free(e_lobby result) {
    emit emitter()->nick_free(result);
}

void reply_lobby::email_free(e_lobby result) {
    emit emitter()->email_free(result);
}

void reply_lobby::get_nodeposit_bonus(e_lobby result)
{
    emit emitter()->get_nodeposit_bonus(result);
}

//void reply_lobby::partner_code_valid(e_lobby result)
//{
//    emit emitter()->partner_code_valid(result);
//}

//void reply_lobby::get_partner_code(e_lobby result, const std::string &partner_code)
//{
//    emit emitter()->get_partner_code(result, partner_code);
//}

//void reply_lobby::activation(e_lobby result, const gbp::ns_api::ns_lobby::user_info_t &u_info)
//{
//    emit emitter()->activation(result, u_info);
//}

void reply_lobby::login(e_lobby result, const gbp::ns_api::ns_lobby::user_info_t &u_info)
{
    emit emitter()->login(result, u_info);
}

void reply_lobby::restore_password_step1(e_lobby result)
{
    emit emitter()->restore_password_step1(result);
}

void reply_lobby::restore_password_step2(e_lobby result)
{
    emit emitter()->restore_password_step2(result);
}

void reply_lobby::restore_password_step3(e_lobby result)
{
    emit emitter()->restore_password_step3(result);
}

void reply_lobby::change_password(e_lobby result)
{
    emit emitter()->change_password(result);
}

void reply_lobby::logout(e_lobby result)
{
    emit emitter()->logout(result);
}

void reply_lobby::change_avatar(e_lobby result)
{
    emit emitter()->change_avatar(result);
}

void reply_lobby::change_email_step1(e_lobby result)
{
    emit emitter()->change_email_step1(result);
}

void reply_lobby::change_email_step2(e_lobby result)
{
    emit emitter()->change_email_step2(result);
}

void reply_lobby::make_payment_out(e_lobby result)
{
    emit emitter()->make_payment_out(result);
}

void reply_lobby::get_random_prize_item(e_lobby result, const gbp::ns_api::ns_special_offers::random_prize_item &info)
{
    emit emitter()->get_random_prize_item(result, info);
}

void reply_lobby::send_money_to_user(e_lobby result)
{
    emit emitter()->send_money_to_user(result);
}

void reply_lobby::get_avatars(e_lobby result, const std::unordered_map<gbp_i64, std::string> &avatars)
{
    emit emitter()->get_avatars(result, avatars);
}

void reply_lobby::update_address(e_lobby result)
{
    emit emitter()->update_address(result);
}

void reply_lobby::update_phone_number(e_lobby result)
{
    emit emitter()->update_phone_number(result);
}

void reply_lobby::set_user_lang(reply_lobby::e_lobby result)
{
    emit emitter()->set_user_lang(result);
}

void reply_lobby::get_vip_system_info(reply_lobby::e_lobby result, const gbp::ns_api::ns_lobby::vip_system_info &info)
{
    emit emitter()->get_vip_system_info(result, info);
}
