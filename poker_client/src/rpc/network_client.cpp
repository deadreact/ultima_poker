#include <precompiled.h>
#include "network_client.h"
#include <qhostaddress.h>
#include <QApplication>
#include <QtConcurrent/QtConcurrent>
#include <utils/logger.h>
#include <dialogs/dialog_onerror.h>
#include <dialogs/dialog_server_not_available.h>

#ifndef Q_MOC_RUN
    #include <api/api_common.h>
    #include "request_statistic.h"
    #include "reply_statistic.h"
    #include "reply_lobby.h"
    #include "request_lobby.h"

    #include "table/reply.h"
    #include "table/request.h"
    #include "table/server_reply.h"
    #include "table/server_request.h"
    #include "server_request_lobby.h"
    #include <api/tools/sha1.h>

    #include "table/rpc_table.h"
    #include "rpc_lobby.h"
    #include "rpc_statistic.h"
#endif // Q_MOC_RUN

#include <PokerApplication.h>
#include <db/SharedDataService.h>
#include <api_models/model_user_info_t.hpp>
#include <widgets/MainWindow.h>
#include <table/rpc_table.h>
#include <notes/model_io.h>
#include <db/modeldatabase.h>
#include <QMessageBox>

NetworkClient &netClient()
{
    return pApp->client();
}
// --------------- API DATA ------------

struct ApiData
{
    request_statistic       m_request_lobby_stat;
    reply_statistic         m_reply_lobby_stat;
    request_lobby            m_request_lobby;
    reply_lobby              m_reply_lobby;

    request_table_cash       m_request_table_cash;
    reply_table_cash         m_reply_table_cash;

    request_table_tournament m_request_table_tournament;
    reply_table_tournament   m_reply_table_tournament;

    ns_server_request::table_cash m_server_request_table_cash;
    ns_server_reply::table_cash   m_server_reply_table_cash;

    ns_server_request::table_tournament m_server_request_table_tournament;

    ns_server_request::special_offers m_server_request_special_offers;
    ns_server_request::lobby m_server_request_lobby;

    ApiData(NetworkClient* const client)
        : m_request_lobby_stat(client)
        , m_reply_lobby_stat(client)
        , m_request_lobby(client)
        , m_reply_lobby(client)
        , m_request_table_cash(client)
        , m_reply_table_cash(client)
        , m_request_table_tournament(client)
        , m_reply_table_tournament(client)
        , m_server_request_table_cash(client)
        , m_server_reply_table_cash(client)
        , m_server_request_table_tournament(client)
        , m_server_request_special_offers(client)
        , m_server_request_lobby(client)
    {}
};

struct RpcData {
    rpc_statistic m_rpc_statistic;
    rpc_table     m_rpc_table;
    rpc_lobby     m_rpc_lobby;

    RpcData(NetworkClient* const client)
        : m_rpc_statistic(client)
        , m_rpc_table(client)
        , m_rpc_lobby(client)
    {}
};


// --------------- REGISRY -----------------------

template <>
request_lobby* NetworkClient::request() const
{
    Logger::msg("Get request_lobby from Network client", __FILE__, __LINE__);
    return &m_apiData->m_request_lobby;
}

template <>
reply_lobby* NetworkClient::reply() const
{
    Logger::msg("Get reply_lobby from Network client", __FILE__, __LINE__);
    return &m_apiData->m_reply_lobby;
}

template <>
request_statistic* NetworkClient::request() const
{
    Logger::msg("Get request_lobby_stat from Network client", __FILE__, __LINE__);
    return &m_apiData->m_request_lobby_stat;
}

template <>
reply_statistic* NetworkClient::reply() const
{
    Logger::msg("Get reply_lobby_stat from Network client", __FILE__, __LINE__);
    return &m_apiData->m_reply_lobby_stat;
}

template <>
request_table_cash* NetworkClient::request() const
{
    Logger::msg("Get request_table_cash from Network client", __FILE__, __LINE__);
    return &m_apiData->m_request_table_cash;
}

template <>
reply_table_cash* NetworkClient::reply() const
{
    Logger::msg("Get reply_table_cash from Network client", __FILE__, __LINE__);
    return &m_apiData->m_reply_table_cash;
}

template <>
request_table_tournament* NetworkClient::request() const
{
    Logger::msg("Get request_table_tournament from Network client", __FILE__, __LINE__);
    return &m_apiData->m_request_table_tournament;
}

template <>
reply_table_tournament* NetworkClient::reply() const
{
    Logger::msg("Get reply_table_tournament from Network client", __FILE__, __LINE__);
    return &m_apiData->m_reply_table_tournament;
}

// -------------------------------------------------
template <>
ns_server_request::table_cash* NetworkClient::server_request() const
{
    Logger::msg("Get ns_server_request::table_cash from Network client", __FILE__, __LINE__);
    return &m_apiData->m_server_request_table_cash;
}

template <>
ns_server_request::table_tournament* NetworkClient::server_request() const
{
//    gbp::ns_api::ns_game::
    Logger::msg("Get ns_server_request::table_cash from Network client", __FILE__, __LINE__);
    return &m_apiData->m_server_request_table_tournament;
}

template <>
ns_server_request::special_offers* NetworkClient::server_request() const
{
    Logger::msg("Get ns_server_request::special_offers from Network client", __FILE__, __LINE__);
    return &m_apiData->m_server_request_special_offers;
}

template <>
ns_server_request::lobby* NetworkClient::server_request() const
{
    Logger::msg("Get ns_server_request::lobby from Network client", __FILE__, __LINE__);
    return &m_apiData->m_server_request_lobby;
}

template <>
ns_server_reply::table_cash* NetworkClient::server_reply() const
{
    Logger::msg("Get ns_server_reply::table_cash from Network client", __FILE__, __LINE__);
    return &m_apiData->m_server_reply_table_cash;
}

// ----------------------------------------------------------- RPC DATA ------------------------

// template <> rpc_table* NetworkClient::rpc() const {
//     if (m_rpcData.isNull()) {
//         Logger::error("Rpc data is null", __FILELINE__);
//         qFatal("Rpc data is null");
//     }
//     return &m_rpcData->m_rpc_tournament;
// }

template <> rpc_table* NetworkClient::rpc() const {
    if (m_rpcData.isNull()) {
        Logger::error("Rpc data is null", __FILELINE__);
        qFatal("Rpc data is null");
    }
    return &m_rpcData->m_rpc_table;
}

template <> rpc_lobby* NetworkClient::rpc() const {
    if (m_rpcData.isNull()) {
        Logger::error("Rpc data is null", __FILELINE__);
        qFatal("Rpc data is null");
    }
    return &m_rpcData->m_rpc_lobby;
}

template <> rpc_statistic* NetworkClient::rpc() const {
    if (m_rpcData.isNull()) {
        Logger::error("Rpc data is null", __FILELINE__);
        qFatal("Rpc data is null");
    }
    return &m_rpcData->m_rpc_statistic;
}

// ----------------------------------------------------------- NetworkClient ------------------------

NetworkClient::NetworkClient(QObject *parent)
    : QObject(parent)
    , m_worker()
    , m_netio(m_worker
            , gbp::net::netio::client // | gbp::net::netio::test_inactivity
            , gbp::net::netio::k_inactivity_timeout
            , gbp::net::netio::k_max_body_size
            , gbp::net::netio::k_compress_if_greater
            , [this](gbp::net::impl_holder holder, const char *fl, int ec, const char *emsg)
              { onError(std::move(holder), fl, ec, emsg); }
            , gbp::net::on_read_handler()
            , gbp::net::on_write_handler()
            , gbp::net::on_incoming_handler()
            , [this](gbp::net::impl_holder holder = gbp::net::impl_holder{})
            {
                Q_UNUSED(holder)
                //qDebug() << "STOP";
            })
    , m_connSetting(readConnectionSetting())
    , m_apiData(new ApiData(this))
    , m_rpcData(nullptr) // init()
//    , m_userdata(new ns_model::user_info_t)
    , m_serverRespondTimeoutMsec(10000)
    , m_connected(false)
    , m_finished(false)
    , m_reconnectDialog(nullptr)
{
//    m_netio.set_incoming_handler([this](gbp::net::impl_holder, const char* cls, const char* func, const char*, size_t){
//        std::cout << "incoming_handler: " << cls << "." << func << std::endl;
//    });
}

NetworkClient::~NetworkClient()
{
    //qDebug() << "~NetworkClient()";
//    qDebug() << parent();
//    m_apiData->m_request_lobby.logout();
    m_finished = true;
    if (m_statRequestJob.isRunning()) {
        m_statRequestJob.waitForFinished();
    }
    if (m_netio.started()) {
        m_netio.stop();
    }
    //qDebug() << "OLOLO";
    m_apiData.reset();
}

bool NetworkClient::init()
{
    if (m_connected) {
        return true;
    }
    m_rpcData.reset(new RpcData(this));
    Logger::info(m_connSetting.m_info, __FILE__, __LINE__);
    gbp::net::impl_holder holder{};

    int ec = 0;
    std::string emsg;

    m_netio.connect(m_connSetting.m_ip.toLatin1(), m_connSetting.m_port, &ec, &emsg);
    if (ec)
    {
     DialogServerNotAvailable d(pApp->mainWindow());
     d.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
     d.exec();

     return false;
    }
    else {
     m_netio.stop();
    }

    //qDebug() << "About to async_connect";
    m_netio.async_connect(
         m_connSetting.m_ip.toLatin1(), m_connSetting.m_port
        ,[this]
         (gbp::net::impl_holder holder, const char* /*fl*/, int ec, const char *emsg)
         { handleConnect(std::move(holder), ec, emsg); }
        ,std::move(holder)
    );

    return true;
}

void NetworkClient::handleConnect(gbp::net::impl_holder holder, int errcode, const char *errmsg)
{
    //qDebug() << "Handle connect: ec=" << errcode << ", emsg=" << errmsg;
    if (errcode != 0) {
        onError(std::move(holder), QString("file: NetworkClient.cpp, line: %0").arg(__LINE__).toLatin1(), errcode, errmsg);
    } else {
        onConnected();
    }
}

void NetworkClient::onConnected()
{
    connect(m_apiData->m_reply_lobby.emitter(), &reply_lobby::emitter_t::login,  this, &NetworkClient::onLogin , Qt::UniqueConnection);
    connect(m_apiData->m_reply_lobby.emitter(), &reply_lobby::emitter_t::logout, this, &NetworkClient::onLogout, Qt::UniqueConnection);
    Logger::event("NetClient Inited", __FILE__, __LINE__);
    Logger::msg("Connected", __FILE__, __LINE__);

    m_netio.async_handshake(
         [this]
         (gbp::net::impl_holder holder, const char *fileline, int ec, const char *emsg)
         { handleHandShake(std::move(holder), fileline, ec, emsg);}
    );
}

void NetworkClient::handleHandShake(gbp::net::impl_holder holder, const char *fileline, int errcode, const char *errmsg)
{
    if (errcode != 0) {
        onError(std::move(holder), fileline, errcode, errmsg);
    } else {
        start(std::move(holder));
    }
}

void NetworkClient::start(gbp::net::impl_holder holder)
{
    m_netio.start(std::move(holder));

    m_statRequestJob = QtConcurrent::run(
    [this]
    {
        //! это нужно для того что-бы не ждать 3 секунды завершения потока
        static const int request_frequency_ms = 3000;
        static const int max_wait_for_finished = 100;
        static const int msleep_duration = std::min(request_frequency_ms, max_wait_for_finished);
        int msleep_counter = 0;

        connect(this, &NetworkClient::disconnected, QThread::currentThread(), &QThread::quit);
        forever {
            if ( this->m_finished ) {
                return;
            }
            if ( !m_netio.started() ) {
                return;
            }
            if (msleep_counter < request_frequency_ms) {
                msleep_counter += msleep_duration;
                QThread::currentThread()->msleep(msleep_duration);
                continue;
            }
            m_apiData->m_request_lobby_stat.games_statistic();
            msleep_counter = 0;
        }
    });

    Logger::msg("Netio initialized succsessfully!!", __FILE__, __LINE__);

    m_connected = true;
    if (m_reconnectDialog) {
        m_reconnectDialog->deleteLater();
        m_reconnectDialog = nullptr;
    }
    m_apiData->m_request_lobby_stat.games_statistic();

    emit connected();
}

gbp_i64 NetworkClient::userId() const {
    return modelsDB().getUserInfo()->userId();
}

gbp::net::object_registry_ptr NetworkClient::create_registry(const std::string &name)
{
    return m_netio.create_registry(name);
}

bool NetworkClient::reconnect()
{
    if (m_connected == false) {
        int errcode = 0;
        std::string errmsg;
        m_netio.connect(m_connSetting.m_ip.toLatin1(), m_connSetting.m_port, &errcode, &errmsg);
        std::cout << errcode << " " << errmsg << std::endl;
        if (errcode != 0) {
            return false;
        }
        m_netio.handshake(&errcode, &errmsg);
        std::cout << errcode << " " << errmsg << std::endl;
        if (errcode != 0) {
            return false;
        }
        gbp::net::impl_holder holder{};
        start(std::move(holder));
    }
    return m_connected;
}

void NetworkClient::onLogin(gbp::ns_api::ns_lobby::ns_reply::e_lobby result, const user_info_t &info)
{
    if (!ns_helpers::isOk(result)) {
        return;
    }
//    Q_ASSERT(m_userdata->nativeData() == nullptr);

//    m_userdata->resetData(new user_info_t(info));
    // легче сделать екстра-запрос и обновить всю статистику, чем учесть все нюансы
    m_apiData->m_request_lobby_stat.games_statistic();

    waiter_t *waiterInfo = pApp->db().sharedDataService().getSingleDataForType<waiter_t>();
    *waiterInfo = std::make_pair(info._user_id, std::map<gbp_i64, int>());

    emit loggedIn(info);
}

void NetworkClient::onLogout(gbp::ns_api::ns_lobby::ns_reply::e_lobby result)
{
    if (!ns_helpers::isOk(result)) {
        return;
    }
//    Q_ASSERT(m_userdata->nativeData() != nullptr);

//    gbp_i64 userId = m_userdata->nativeData()->_user_id;
//    m_userdata->resetData(nullptr);
    // легче сделать екстра-запрос и обновить всю статистику, чем учесть все нюансы
    m_apiData->m_request_lobby_stat.games_statistic();

    emit loggedOut(modelsDB().lastUserId());
}

void NetworkClient::onError(gbp::net::impl_holder, const char* fileline, int errcode, const char *errmsg)
{
    if (errcode != 0) {
        m_connected = false;

        emit disconnected();

        if (!m_reconnectDialog) {
            m_reconnectDialog = new OnError(pApp->mainWindow());

            m_reconnectDialog->setWindowModality(Qt::ApplicationModal);
            m_reconnectDialog->setAttribute(Qt::WA_DeleteOnClose);
            m_reconnectDialog->open();
            init();
        }

        QString line = QRegularExpression("\\(\\d+\\)$").match(fileline).captured();
        int lineInt = QRegularExpression("\\d+").match(line).captured().toInt();
        QString errStr = QString("Network error %0: %1").arg(errcode).arg(errmsg);
        Logger::error(errStr, QString(fileline).replace(line, "").toLatin1(), lineInt);
    } else {
        Logger::error("Netio error with code 0!!", __FILE__, __LINE__);
    }

}

ConnectionSetting NetworkClient::readConnectionSetting()
{
    QString configFilePath = QApplication::applicationDirPath() % QStringLiteral("/config.ini");
    if (QFile::exists(configFilePath))
    {
        QSettings settings(configFilePath, QSettings::IniFormat);
        settings.beginGroup("main");
        QString version_section = settings.value("current_version").toString();
        settings.endGroup();

        if (!version_section.isEmpty() && settings.childGroups().contains(version_section))
        {
            settings.beginGroup(version_section);
            QString ip = settings.value("server_ip", "127.0.0.1").toString();
            quint16 port = settings.value("server_port", 44500).value<quint16>();
            settings.endGroup();
            return ConnectionSetting(ip, port, QString("IP %0, port %1, read succsessfully").arg(ip).arg(port));
        }
    }
    return ConnectionSetting::defaultSetting();
}

const ConnectionSetting &ConnectionSetting::defaultSetting() {
    static const ConnectionSetting cs("93.190.143.130", 44550, "1-st test server");
    return cs;
}

