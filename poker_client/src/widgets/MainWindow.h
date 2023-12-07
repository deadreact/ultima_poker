#pragma once

#include <QMainWindow>
#include <tools/types_fwd.hpp>
#include <widgets/GLWidget.h>
#include <QTranslator>
#include <QStackedWidget>
#include <QTimeZone>
#include <QQueue>
#include <QParallelAnimationGroup>
#include "utils/localizer.h"
#include <interface/NetworkHandlers.h>
#include <game/GameManager.h>
#include <basewidgets/models/model_timezone.h>
#include <unordered_map>

#ifndef NO_REGISTRATION
#include <dialogs/dialog_registration.h>
#endif // !NO_REGISTRATION




class QLabel;
class QButtonGroup;
class QAbstractButton;
namespace Ui {
class MainWindow;
class MainScreen;
}
namespace ns_dialog
{
    class Account;
    class MyTournaments;
    class Settings;
    class VipSystem;
}

class QTableView;
class GameTable;
class TableView;

class MainWindow : public QMainWindow, protected IReplyHandler
{
    Q_OBJECT
private:
    GameManager* m_gameManager;
private:
    Ui::MainWindow *m_ui;
    Localizer m_localizer;
    QMap<int, QPropertyAnimation*> m_popupAnimations;
    bool m_authorize_canceled = false;

    ns_dialog::Account* m_accountDialog;
    ns_dialog::VipSystem* m_vipSystemDialog;
    ns_dialog::Settings* m_settingsDialog;
    ns_dialog::MyTournaments* m_myTournamentsDialog;
#ifndef NO_REGISTRATION
    ns_dialog::Registration* m_regDialog;
#endif // !NO_REGISTRATION
    QWidget* m_currentPopUp;
    QButtonGroup* m_bttnGroup_language;

    QScopedPointer<ns_model::Timezone> m_modelTimeZone;
signals:
    void closed();
private slots:
    void requestDebug();

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    ns_model::Timezone* getModelTimeZone() const {
        return m_modelTimeZone.data();
    }

    bool mustLogin() const;
//-------------------- initializations -------------------
    void init();
    void setupAnimations();
    virtual void registerNetworkReplyHandlers(const NetworkClient* client) override;
//-------------------- --------------- -------------------

    bool isUserLoggedIn() const;
    Ui::MainScreen* page();
    const Ui::MainScreen* page() const;

    TableView *statisticTable() const;

    QTimeZone currentTimeZone() const;
    Localizer* localizer();
public slots:
    void openRegistrationDialog();
    bool openLoginDialog() const;
private slots:
    void onLogin(const gbp::ns_api::ns_lobby::user_info_t &info);
    void requestLogout();
    void logout(ns_lobby::e_status result);

    void openAccountDialog();
    void openVipSystemDialog();
    void openSettingsDialog();
    void openMyTournamentsDialog();
//    GameTable *openGameTable(const GameId& id) const;

    void get_avatars(ns_lobby::e_status result, const std::unordered_map<gbp_i64, std::string>& avatars);

    void triggerPopUp(QAbstractButton *bttn=nullptr);
    void triggerLanguageChanged();

    void onUiRetranslated();
protected:
    virtual void changeEvent(QEvent *e) override;
    virtual void closeEvent(QCloseEvent *e) override;
    void addPopUpAnimation(QWidget* target, QPoint offset = QPoint(270, 0));
protected slots:
    void slotLanguageChanged(QAction* action);
    void updateTimeDate();
    void onMenubarBttnClicked(QAbstractButton *bttn);
    void onMenubarBttnEntered(QWidget *wgt);
    void onMenubarBttnLeaved(QWidget *wgt);


    void on_bttn_news_clicked();
private:
    void createLanguageMenu();
    void createTimezoneMenu();
};

