#include <precompiled.h>
#include "PokerApplication.h"
#include <qresource.h>
#ifndef Q_MOC_RUN
    #include <rpc/network_client.h>
#endif // !Q_MOC_RUN
#include <db/Database.h>
#include <db/modeldatabase.h>
#include <utils/IconProvider.h>
#include <utils/logger.h>
#include "widgets/MainWindow.h"
#include <utils/helper_debug.h>
#include <game/NotificationHandler.h>
#include <models/model_GamesStatistic.h>
#include <qfontdatabase.h>
#include <settingsmanager.h>
#include <qstringbuilder.h>

#define REGISTER_METATYPE(type) qRegisterMetaType<type>(#type)

struct PokerApplicationData
{
    Logger m_logger;
    IconProvider m_iconProvider;
    NetworkClient m_client;
    SharedDB m_db;
    QScopedPointer<MainWindow> m_mainWnd;
    QScopedPointer<GameManager> m_gameManager;
    QScopedPointer<DebugHelper> m_debugHelper;
    QScopedPointer<NotificationHandler> m_notificationHandler;
    QScopedPointer<SettingsMgr> m_settingsManager;

    PokerApplicationData(PokerApplication* app)
        : m_logger(app)
        , m_iconProvider()
        , m_client()
        , m_db(nullptr)
        , m_mainWnd(nullptr)
        , m_gameManager(nullptr)
        , m_debugHelper(new DebugHelper)
        , m_notificationHandler(nullptr)
        , m_settingsManager(new SettingsMgr)
    {}
    ~PokerApplicationData()
    {

    }
};

PokerApplication::PokerApplication(int &argc, char **argv)
    : QApplication(argc, argv)
    , m_dPtr(nullptr)
{

}

PokerApplication::~PokerApplication()
{
//    qDebug() << "~PokerApplication()";
//    m_dPtr->m_client.reset();
}

template <typename T, int splitval = 100>
QVariant splitLinearInterpolator(const T &t0, const T &t1, qreal t) {
    T t01 = t0 % splitval;
    T t00 = t0 / splitval;
    T t11 = t1 % splitval;
    T t10 = t1 / splitval;
    return QVariant::fromValue(T(t01 * (1.0 - t) + t11 * t) + T(t00 * (1.0 - t) + t10 * t) * splitval);
}

template <typename T>
QVariant linearInterpolator(const T &t0, const T &t1, qreal t) {
    return QVariant::fromValue(T(t0 * (1.0 - t) + t1 * t));
}
template <typename T>
void qRegisterLinearInterpolator() {
    qRegisterAnimationInterpolator<T>(linearInterpolator<T>);
}
template <typename T, int splitvalue = 100>
void qRegisterSplitLinearInterpolator() {
    qRegisterAnimationInterpolator<T>(splitLinearInterpolator<T, splitvalue>);
}


void PokerApplication::registerMetaTypes()
{
    QMetaType::registerTypedef("gbp_i8", QMetaType::SChar);
    QMetaType::registerTypedef("gbp_u8", QMetaType::UChar);
    QMetaType::registerTypedef("gbp_i16", QMetaType::Short);
    QMetaType::registerTypedef("gbp_u16", QMetaType::UShort);
    QMetaType::registerTypedef("gbp_i32", QMetaType::Int);
    QMetaType::registerTypedef("gbp_u32", QMetaType::UInt);
    QMetaType::registerTypedef("gbp_i64", QMetaType::LongLong);
    QMetaType::registerTypedef("gbp_u64", QMetaType::ULongLong);

    qRegisterLinearInterpolator<gbp_i64>();
    qRegisterLinearInterpolator<gbp_u64>();
}

bool PokerApplication::initData()
{
    m_dPtr.reset(new PokerApplicationData(this));
    QString resourcePath = applicationDirPath() % QStringLiteral("/bin/resources.dat");

    if (!QResource::registerResource(resourcePath)) {
        Logger::error("Ошибкамана!! дата нема, гиде она? ни скулайт ни хуя!!", "Тут в мєйн", __LINE__);
        qCritical("   Loc: [%s:%i]", __FILELINE__);
        qFatal("Can not load resource %s", qPrintable(resourcePath.toLatin1()));
        return false;
    }
//    m_dPtr->m_client.reset(new NetworkClient);

    resourcePath = applicationDirPath() % QStringLiteral("/bin/resources.db");
    if (!QDir(resourcePath).exists()) {
        QString defaultResourcePath = applicationDirPath() % QStringLiteral("/../resources/default_resources.db");
        QFile f(defaultResourcePath);
        f.copy(resourcePath);
    }
    m_dPtr->m_db.initDB("", resourcePath);

    if (!m_dPtr->m_client.init()) return { false };

    modelsDB().init();
    m_dPtr->m_notificationHandler.reset(new NotificationHandler);

//    connect(client, &NetworkClient::loggedIn, this, [this](const gbp::ns_api::ns_lobby::user_info_t& userInfo){
//        ns_model::Interface::setUserId(userInfo._user_id);
//    });
//    connect(client, &NetworkClient::loggedOut, this, [this]{
//        ns_model::Interface::setUserId(-1);
//    });

    QDir dir(":/fonts/Open_Sans/");
    for (const QString& filename: dir.entryList(QStringList() << "*.ttf")) {
        QString path = dir.absoluteFilePath(filename);
        int id = QFontDatabase::addApplicationFont(path);
        if (id == -1) {
            Logger::warning(path % " not found", __FILELINE__);
        } else {
            Logger::info(path % " font added", __FILELINE__);
        }
    }

    // host info
    _host_info = gbp::get_host_info();

    m_dPtr->m_mainWnd.reset(new MainWindow);
    m_dPtr->m_gameManager.reset(new GameManager);
    m_dPtr->m_mainWnd->init();
    m_dPtr->m_mainWnd->show();

    return true;
}

Logger &PokerApplication::logger() const
{
    return m_dPtr->m_logger;
}

NetworkClient &PokerApplication::client() const {
    return m_dPtr->m_client;
}

SharedDB &PokerApplication::db() const {
    return m_dPtr->m_db;
}

MainWindow *PokerApplication::mainWindow() const {
    return m_dPtr->m_mainWnd.data();
}

DebugHelper *PokerApplication::debugHelper() const {
    return m_dPtr->m_debugHelper.data();
}

SettingsMgr *PokerApplication::settingsManager() const
{
    return m_dPtr->m_settingsManager.data();
}

GameManager *PokerApplication::gameManager() const
{
    return m_dPtr->m_gameManager.data();
}

IconProvider &PokerApplication::iconProvider() const {
    return m_dPtr->m_iconProvider;
}

void PokerApplication::expandMainLobby(bool expand)
{
    if (expand) {
        mainWindow()->showNormal();
    } else {
        mainWindow()->showMinimized();
    }
}

#ifdef ENABLE_VISUAL_DEBUGGING
    QString PokerApplication::cheatRequest(const QString& /*msg*/)
    {
        return "unknoun command";
    }

#endif //ENABLE_VISUAL_DEBUGGING
