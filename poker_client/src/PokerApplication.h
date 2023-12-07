#pragma once
#include <qapplication.h>
#include <qmainwindow.h>
#include <hostinfo/hostinfo.h>

class PokerApplication;
class Logger;
class IconProvider;
class NetworkClient;
class SharedDB;
class SettingsMgr;
class GameManager;
class DebugHelper;
struct PokerApplicationData;
namespace ns_model {
class ApiBase;
}

class MainWindow;

class PokerApplication : public QApplication
{
    Q_OBJECT
private:
    QScopedPointer<PokerApplicationData> m_dPtr;
signals:
    void timezoneChanged(const QByteArray& timezoneId);
public:
    PokerApplication(int &argc, char **argv);
    virtual ~PokerApplication();

    void registerMetaTypes();
    bool initData();
    Logger& logger() const;
    IconProvider& iconProvider() const;
    NetworkClient& client() const;
    SharedDB& db() const;
    MainWindow *mainWindow() const;
    ns_model::ApiBase *statisticModelCash() const;
    ns_model::ApiBase *statisticModelTournament() const;
    DebugHelper* debugHelper() const;
    SettingsMgr* settingsManager() const;
    GameManager* gameManager() const;
    gbp::hostinfo _host_info;

public:
    void expandMainLobby(bool expand = true);

#ifdef ENABLE_VISUAL_DEBUGGING
    QString cheatRequest(const QString& msg);
#endif //ENABLE_VISUAL_DEBUGGING
};


#define pApp (static_cast<PokerApplication *>(QCoreApplication::instance()))
#define DEBUG (*pApp->debugHelper())
