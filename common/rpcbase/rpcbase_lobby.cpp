#include "rpcbase_lobby.h"

#include <api/lobby/request.h>
#include <api/lobby/reply.h>
#include <api/lobby/types.h>

#include <QBuffer>
#include <QDebug>
#include <QtGui/QPixmap>
#include <QMap>
#include <unordered_map>

namespace
{
    void killConnection(QMetaObject::Connection* conn) {
        if (conn != nullptr) {
            QObject::disconnect(*conn);
            delete conn;
        }
    }
    std::string toBase64(const QPixmap &pix)
    {
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        pix.save(&buffer, "PNG");

        return byteArray.toBase64().toStdString();
    }
}

namespace ns_lobby
{
QString rpc_object::classname_common()  { return QLatin1String("gbp::ns_api::ns_lobby::ns_request::lobby_common"); }
QString rpc_object::classname_desktop() { return QLatin1String("gbp::ns_api::ns_lobby::ns_request::lobby_desktop"); }

void rpc_object::login(const QString &nick, const QByteArray &password, const std::string &hardware_key, on_login callback)
{
        QMetaObject::Connection* conn = new QMetaObject::Connection;
        auto callback2 = [callback, conn](e_status status, const user_info_t& info) {
           killConnection(conn);
           callback(status, info);
        };
        *conn = QObject::connect(emitter(), &emitter_t::login, callback2);
        request_desktop()->login(nick.toStdString(), password.toStdString(), hardware_key);
}

void rpc_object::login(const QString &nick, const QByteArray &password, const std::string &hardware_key, on_common_reply callback)
{
    if (callback)
    {
        QMetaObject::Connection* conn = new QMetaObject::Connection;
        auto callback2 = [callback, conn](e_status status, const user_info_t&) {
            killConnection(conn);
            callback(status);
        };
        *conn = QObject::connect(emitter(), &emitter_t::login, callback2);
    }
    request_desktop()->login(nick.toStdString(), password.toStdString(), hardware_key);
}

    void rpc_object::get_avatars(const std::vector<gbp_i64> &ids, on_get_avatars callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status, const std::unordered_map<gbp_i64, std::string>& avatars) {
                killConnection(conn);
                QHash<gbp_i64, QByteArray> hashmap;
                for (const auto& it: avatars) {
                    hashmap.insert(it.first, QByteArray::fromStdString(it.second));
                }
                callback(status, hashmap);
            };
            *conn = QObject::connect(emitter(), &emitter_t::get_avatars, callback2);
        }
        std::vector<gbp_i64> ids2;
        for (auto id: ids) {
            ids2.push_back(id);
        }
        request_common()->get_avatars(ids2);
    }

    void rpc_object::nick_free(const QString &nickname, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status) {
                killConnection(conn);
                callback(status);
            };
            *conn = QObject::connect(emitter(), &emitter_t::nick_free, callback2);
        }
        request_common()->nick_free(nickname.toStdString());
    }

    void rpc_object::email_free(const QString &email, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status) {
                killConnection(conn);
                callback(status);
            };
            *conn = QObject::connect(emitter(), &emitter_t::email_free, callback2);
        }
        request_common()->email_free(email.toStdString());
    }

    void rpc_object::get_random_prize_item(on_get_random_prize_item callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status, const random_prize_item& item) {
                killConnection(conn);
                callback(status, item);
            };
            *conn = QObject::connect(emitter(), &emitter_t::get_random_prize_item, callback2);
        }
        request_common()->get_random_prize_item();
    }
    void rpc_object::update_address(gbp_u16 country, const QString &city, const QString &mail_index, const QString &address, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status) {
                killConnection(conn);
                callback(status);
            };
            *conn = QObject::connect(emitter(), &emitter_t::update_address, callback2);
        }
        request_common()->update_address(country, city.toStdString(), mail_index.toStdString(), address.toStdString());
    }
    void rpc_object::update_phone_number(const std::string &phone_number, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status) {
                killConnection(conn);
                callback(status);
            };
            *conn = QObject::connect(emitter(), &emitter_t::update_phone_number, callback2);
        }
        request_common()->update_phone_number(phone_number);
    }

    void rpc_object::make_payment_out(e_payment_system ps, const QString &desc, gbp_i64 amount, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status) {
                killConnection(conn);
                callback(status);
            };
            *conn = QObject::connect(emitter(), &emitter_t::make_payment_out, callback2);
        }
        request_common()->make_payment_out(ps, desc.toStdString(), amount);
    }

    void rpc_object::send_money_to_user(const QString &nickname, gbp_i64 amount, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status) {
                killConnection(conn);
                callback(status);
            };
            *conn = QObject::connect(emitter(), &emitter_t::send_money_to_user, callback2);
        }
        request_common()->send_money_to_user(nickname.toStdString(), amount);
    }

    void rpc_object::get_tickets(on_get_tickets callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status, const std::vector<user_ticket>& tickets) {
                killConnection(conn);
                callback(status, tickets);
            };
            *conn = QObject::connect(emitter(), &emitter_t::get_tickets, callback2);
        }
        request_common()->get_tickets();
    }

    void rpc_object::set_user_lang(const QString &lang, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status) {
                killConnection(conn);
                callback(status);
            };
            *conn = QObject::connect(emitter(), &emitter_t::set_user_lang, callback2);
        }
        request_desktop()->set_user_lang(lang.toStdString());
    }

    void rpc_object::hardware_checking(const std::string &hardware_key, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status) {
                killConnection(conn);
                callback(status);
            };
            *conn = QObject::connect(emitter(), &emitter_t::hardware_checking, callback2);
        }
        request_common()->hardware_checking(hardware_key);
    }

    void rpc_object::get_nodeposit_bonus(on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status) {
                killConnection(conn);
                callback(status);
            };
            *conn = QObject::connect(emitter(), &emitter_t::get_nodeposit_bonus, callback2);
        }
        request_common()->get_nodeposit_bonus();
    }

    void rpc_object::get_vip_system_info(on_get_vip_system_info callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status, const gbp::ns_api::ns_lobby::vip_system_info& info) {
                killConnection(conn);
                callback(status, info);
            };
            *conn = QObject::connect(emitter(), &emitter_t::get_vip_system_info, callback2);
        }
        request_common()->get_vip_system_info();
    }

    void rpc_object::logout(on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status) {
                killConnection(conn);
                callback(status);
            };
            *conn = QObject::connect(emitter(), &emitter_t::logout, callback2);
        }
        request_common()->logout();
    }

    void rpc_object::change_email_step1(const QString &new_email, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status) {
                killConnection(conn);
                callback(status);
            };
            *conn = QObject::connect(emitter(), &emitter_t::change_email_step1, callback2);
        }
        request_common()->change_email_step1(new_email.toStdString());
    }

    void rpc_object::change_email_step2(const QByteArray &key, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status) {
                killConnection(conn);
                callback(status);
            };
            *conn = QObject::connect(emitter(), &emitter_t::change_email_step2, callback2);
        }
        request_common()->change_email_step2(key.toStdString());
    }

    void rpc_object::change_password(const QByteArray &current_pass, const QByteArray &new_pass, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status) {
                killConnection(conn);
                callback(status);
            };
            *conn = QObject::connect(emitter(), &emitter_t::change_password, callback2);
        }
        request_common()->change_password(current_pass.toStdString(), new_pass.toStdString());
    }

    void rpc_object::change_avatar(const QPixmap& avatar, on_common_reply callback) {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status) {
                killConnection(conn);
                callback(status);
            };
            *conn = QObject::connect(emitter(), &emitter_t::change_avatar, callback2);
        }

        request_common()->change_avatar(::toBase64(avatar));
    }


    void rpc_object::restore_password(const QString &email_or_login, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status) {
                killConnection(conn);
                callback(status);
            };
            *conn = QObject::connect(emitter(), &emitter_t::restore_password_step1, callback2);
        }
        request_common()->restore_password_step1(email_or_login.toStdString());
    }

    void rpc_object::restore_password(const QString &email_or_login, const QByteArray &key, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status) {
                killConnection(conn);
                callback(status);
            };
            *conn = QObject::connect(emitter(), &emitter_t::restore_password_step2, callback2);
        }
        request_common()->restore_password_step2(email_or_login.toStdString(), key.toStdString());

    }

    void rpc_object::restore_password(const QString &email_or_login, const QByteArray &password, const QByteArray &retyped_password, const QByteArray &key, on_common_reply callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](e_status status) {
                killConnection(conn);
                callback(status);
            };
            *conn = QObject::connect(emitter(), &emitter_t::restore_password_step3, callback2);
        }
        request_common()->restore_password_step3(email_or_login.toStdString(), password.toStdString(), retyped_password.toStdString(), key.toStdString());

    }

} //ns_lobby

