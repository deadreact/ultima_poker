#pragma once

#include "utils/Utils.h"
#include <helpers/helper_enum.h>

#include <qfuture.h>

#include <netio/object_registry.hpp>
#include <netio/netio.hpp>

#include <tools/types_fwd.hpp>

namespace
{
    using user_info_t = gbp::ns_api::ns_lobby::user_info_t;
}

struct ApiData;
struct RpcData;

struct ConnectionSetting
{
    QString m_ip;
    int m_port;
    QString m_info;
    ConnectionSetting(const QString& ip, int port, const QString& info = QString())
        : m_ip(ip)
        , m_port(port)
        , m_info(info)
    {}


    static const ConnectionSetting& defaultSetting();
};

class NetworkClient;
//-----------------------------------------------------------------------------------
NetworkClient& netClient();
//-----------------------------------------------------------------------------------
namespace ns_model
{
    class user_info_t;
}
class OnError;

class NetworkClient : public QObject
{
    Q_OBJECT
private:
    gbp::net::worker m_worker;
    gbp::net::netio m_netio;
    QFuture<void> m_statRequestJob;
    ConnectionSetting m_connSetting;
    QScopedPointer<ApiData> m_apiData;
    QScopedPointer<RpcData> m_rpcData;
//    QScopedPointer<user_info_t> m_userdata;
//    QScopedPointer<ns_model::user_info_t> m_userdata;
    QDateTime m_lastServerRespond;
    int m_serverRespondTimeoutMsec;
    bool m_connected;
    bool m_finished;

    OnError *m_reconnectDialog;
public:
    explicit NetworkClient(QObject* parent = nullptr);
    virtual ~NetworkClient();

    virtual bool init();
    virtual void start(gbp::net::impl_holder holder);
    virtual bool reconnect();

    template <typename T> T* request() const;
    template <typename T> T* reply() const;
    template <typename T> T* server_request() const;
    template <typename T> T* server_reply() const;

    template <typename T> const typename  T::emitter_t* emitter() const {
        return reply<T>()->emitter();
    }

    template <typename T> T* rpc() const;

    gbp_i64 userId() const;

    void updateEmail(const std::string& email);
    void updatePhone(const std::string& phone);
    void updateAddress(const gbp_u16 country_id, const char *city, const char *index, const char *address);
    void updateAvatarHash(const sha1_t &hash);

    template <typename ...Args>
    void call_server(const char* api, const Args&... args);

    template <typename ...Args>
    void call_server(const std::string& api, const Args&... args);

    gbp::net::object_registry_ptr create_registry(const std::string& name);

    void onError(gbp::net::impl_holder, const char *fileline, int errcode, const char *errmsg);
    void handleConnect(gbp::net::impl_holder holder, int errcode, const char *errmsg);
    void handleHandShake(gbp::net::impl_holder holder, const char *fileline, int errcode, const char *errmsg);

private:
    ConnectionSetting readConnectionSetting();
signals:
    void loggedIn(const user_info_t& userInfo);
    void loggedOut(long long userId);
    void connected();
    void disconnected();
public:
    inline bool isConnected() const { return m_connected; }
private slots:
    virtual void onLogin(gbp::ns_api::ns_lobby::ns_reply::e_lobby result, const user_info_t& info);
    virtual void onLogout(gbp::ns_api::ns_lobby::ns_reply::e_lobby result);
    virtual void onConnected();
signals:
    void respondTimeout(int msecs);
};



/*-------- inline ---------- */

template <typename ...Args>
void NetworkClient::call_server(const char* api, const Args&... args) {
    if (m_connected && !m_finished) {
        m_netio.call2(api, args...);
    }
}
template <typename ...Args>
void NetworkClient::call_server(const std::string& api, const Args&... args) {
    if (m_connected && !m_finished) {
        call_server(api.c_str(), args...);
    }
}
