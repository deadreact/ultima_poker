#include <precompiled.h>
#include "request_lobby.h"
#ifndef Q_MOC_RUN
    #include "network_client.h"
#endif // Q_MOC_RUN

request_lobby::request_lobby(NetworkClient * const client)
    : m_client(*client)
{}

void request_lobby::hardware_checking(const std::string &hardware_key)
{
    m_client.call_server(classname_common() + ".hardware_checking", hardware_key);
}


void request_lobby::login(const std::string &nick, const std::string &password, const std::string &hardware_key)
{
    m_client.call_server(classname_desktop() + ".login", nick, password, hardware_key);
}

void request_lobby::nick_free(const std::string &nick)
{
	m_client.call_server(classname_common() + ".nick_free", nick);
}

void request_lobby::email_free(const std::string &email)
{
    m_client.call_server(classname_common() + ".email_free", email);
}

void request_lobby::get_nodeposit_bonus()
{
    m_client.call_server(classname_common() + ".get_nodeposit_bonus");
}

//void request_lobby::register_user(const gbp::ns_api::ns_lobby::user_register_info_t &ui)
//{
//	m_client.call_server(classname_common() + ".register_user", ui);
//}

//void request_lobby::partner_code_valid(const std::string &partner_code)
//{
//	m_client.call_server(classname_common() + ".partner_code_valid", partner_code);
//}

//void request_lobby::get_partner_code(gbp_i64 partner_code_id)
//{
//	m_client.call_server(classname_common() + ".get_partner_code", partner_code_id);
//}

//void request_lobby::activation(const std::string &activation_code)
//{
//	m_client.call_server(classname_common() + ".activation", activation_code);
//}

void request_lobby::restore_password_step1(const std::string &email_or_login)
{
	m_client.call_server(classname_common() + ".restore_password_step1", email_or_login);
}

void request_lobby::restore_password_step2(const std::string &email_or_login, const std::string &key)
{
	m_client.call_server(classname_common() + ".restore_password_step2", email_or_login, key);
}

void request_lobby::restore_password_step3(const std::string &email_or_login, const std::string &password, const std::string &retyped_password, const std::string &key)
{
	m_client.call_server(classname_common() + ".restore_password_step3", email_or_login, password, retyped_password, key);
}

void request_lobby::change_password(const std::string &current_pass, const std::string &new_pass)
{
	m_client.call_server(classname_common() + ".change_password", current_pass, new_pass);
}

void request_lobby::logout()
{
	m_client.call_server(classname_common() + ".logout");
}

void request_lobby::change_avatar(const std::string &new_avatar)
{
	m_client.call_server(classname_common() + ".change_avatar", new_avatar);
}

void request_lobby::change_email_step1(const std::string &new_email)
{
	m_client.call_server(classname_common() + ".change_email_step1", new_email);
}

void request_lobby::change_email_step2(const std::string &key)
{
	m_client.call_server(classname_common() + ".change_email_step2", key);
}

void request_lobby::make_payment_out(gbp::ns_api::e_payment_system ps, const std::string &description, gbp::ns_api::money amount)
{
	m_client.call_server(classname_common() + ".make_payment_out", ps, description, amount);
}

void request_lobby::get_random_prize_item()
{
	m_client.call_server(classname_common() + ".get_random_prize_item");
}

void request_lobby::send_money_to_user(const std::string &receiver_nickname, gbp::ns_api::money money_)
{
	m_client.call_server(classname_common() + ".send_money_to_user", receiver_nickname, money_);
}

void request_lobby::get_avatars(const std::vector<gbp_i64> &user_ids)
{
	m_client.call_server(classname_common() + ".get_avatars", user_ids);
}

void request_lobby::update_address(gbp_u16 country, const std::string &city, const std::string &mail_index, const std::string &address)
{
    m_client.call_server(classname_common() + ".update_address", country, city, mail_index, address);
}

void request_lobby::update_phone_number(const std::string &phone_number)
{
	m_client.call_server(classname_common() + ".update_phone_number", phone_number);
}

void request_lobby::set_user_lang(const std::string &lang)
{
    m_client.call_server(classname_desktop() + ".set_user_lang", lang);
}

void request_lobby::get_vip_system_info() {
    m_client.call_server(classname_common() + ".get_vip_system_info");
}

void request_lobby::get_tickets() {
    m_client.call_server(classname_common() + ".get_tickets");
}
