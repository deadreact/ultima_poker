#include <precompiled.h>
#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "ui_page_MainScreen.h"
#include <dialogs/dialog_account.h>
#include <dialogs/dialog_gametable.h>
#include <dialogs/dialog_okmessage.h>
#include <dialogs/menubar/dialog_mytournaments.h>
#include <dialogs/menubar/dialog_settings.h>
#include <dialogs/menubar/dialog_vipsystem.h>
#include <pages/page_MainScreen.h>
#include <tools/stylesheeteditor.hpp>
#include "widget_LanguageItem.h"
#include "widget_PlayerInfo.h"
#include <core/money.h>
//#include <db/Database.h>
//#include <db/SharedDataService.h>
#include <descriptors/descriptor_GameTable.h>
#include <dialogs/buyin/dialog_CashBuyIn.h>
#include <dialogs/dialog_authorization.h>
//#include <game/cashhandler.h>
#include <helpers/helper_statistic.h>
#include <PokerApplication.h>
#include <QDateTime>
#include <QFileDialog>
//#include "StatisticTableView.h"
#include <basewidgets/models/model_timezone.h>
#include <api_models/model_user_info_t.hpp>
#include <helpers/helper_enum.h>
#include <common.h>

#ifndef Q_MOC_RUN
    #include <rpc/rpc_common.h>
    #include <rpc/rpc_lobby.h>
#endif // Q_MOC_RUN
#include <tools/sequentalactionqueue.h>

Q_DECLARE_METATYPE(games_stat_t::first_type::value_type)

namespace
{
    gbp_i64 currentId(TableView* tblView) {
        return tblView->currentIndex().data(UserType::RowItemIdRole).value<gbp_i64>();
    }
    QVariant currentItem(TableView* tblView) {
        return tblView->currentIndex().data(UserType::RowDataRole);
    }
} //namespace





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_gameManager(nullptr)
    , m_ui(new Ui::MainWindow)
    , m_localizer()
    , m_accountDialog(nullptr)
    , m_vipSystemDialog(nullptr)
    , m_settingsDialog(nullptr)
    , m_myTournamentsDialog(nullptr)
#ifndef NO_REGISTRATION
    , m_regDialog(nullptr)
#endif // !NO_REGISTRATION
    , m_currentPopUp(nullptr)
    , m_bttnGroup_language(new QButtonGroup(this))
    , m_modelTimeZone(new ns_model::Timezone)
{
    m_ui->setupUi(this);
}

void MainWindow::init()
{
    m_localizer.init(this);
    m_ui->main_window->init();
    page()->stackedWidget->setCurrentIndex(0);


    registerNetworkReplyHandlers(&pApp->client());
    m_gameManager = pApp->gameManager();
    m_gameManager->registerServerEvents();

    connect(page()->bttn_login, &QAbstractButton::clicked, this, &MainWindow::openLoginDialog);
    connect(page()->bttn_logon, &QAbstractButton::clicked, this, &MainWindow::openRegistrationDialog);

    m_ui->popup_space->hide();

    createLanguageMenu();
    createTimezoneMenu();
    setupAnimations();

    auto on_miss_func = [this]{
        if (m_currentPopUp && !m_currentPopUp->rect().contains(m_currentPopUp->mapFromGlobal(QCursor::pos()))) {
            m_ui->bttn_cap->click();
        }
    };

    connect(m_ui->popup_space, &EntityWidget::pressed, this, on_miss_func);

//    connect(m_ui->main_window->cashTablesPage(), &ns_page::CashTables::openTableRequested, this, &MainWindow::openCurrentCash);
//    connect(page()->bttn_open_tbl, &QPushButton::clicked, this, &MainWindow::openCurrentCash);
//    connect(page()->minitbl_container, SIGNAL(requestTakeASit(int)), this, SLOT(joinCurrentCash(int)));

    connect(m_ui->popup_space, &EntityWidget::shown, m_ui->main_window, &ns_page::MainScreen::enableBlur);
    connect(m_ui->popup_space, &EntityWidget::hidden, m_ui->main_window, &ns_page::MainScreen::disableBlur);

    m_ui->bar->setMask(QPixmap(":textures/menubarMask.png").createMaskFromColor(Qt::black));

    m_ui->curveLine->setAttribute(Qt::WA_TransparentForMouseEvents);

    m_ui->bttn_account->resetLogic<MainMenubarLogic>();

#ifndef ONLY_FULLY_COMPLETED_CONTENT
    m_ui->bttn_tournaments->resetLogic<MainMenubarLogic>();
    m_ui->bttn_promotions->resetLogic<MainMenubarLogic>();
    m_ui->bttn_vip_system->resetLogic<MainMenubarLogic>();
    m_ui->bttn_settings->resetLogic<MainMenubarLogic>();
    m_ui->bttn_support->resetLogic<MainMenubarLogic>();
    m_ui->bttn_news->resetLogic<MainMenubarLogic>();
    page()->bttn_cash->setEnabled(true);
#else
    m_ui->bttn_tournaments->setDisabled(true);
    m_ui->icon_tournaments->setProperty("opacity", 0.5);
    m_ui->bttn_promotions->setDisabled(true);
    m_ui->icon_promotions->setProperty("opacity", 0.5);
    m_ui->bttn_vip_system->setDisabled(true);
    m_ui->icon_vip_system->setProperty("opacity", 0.5);
    m_ui->bttn_settings->setDisabled(true);
    m_ui->icon_settings->setProperty("opacity", 0.5);
    m_ui->bttn_support->setDisabled(true);
    m_ui->icon_support->setProperty("opacity", 0.5);
    m_ui->bttn_news->setDisabled(true);
    m_ui->icon_news->setProperty("opacity", 0.5);
#endif
    m_ui->bttnGroup_menubar->setId(m_ui->clock_widget, 0);
    m_ui->bttnGroup_menubar->setId(m_ui->bttn_account, 1);
    m_ui->bttnGroup_menubar->setId(m_ui->bttn_tournaments, 2);
    m_ui->bttnGroup_menubar->setId(m_ui->bttn_promotions, 3);
    m_ui->bttnGroup_menubar->setId(m_ui->bttn_vip_system, 4);
    m_ui->bttnGroup_menubar->setId(m_ui->bttn_settings, 5);
    m_ui->bttnGroup_menubar->setId(m_ui->bttn_support, 6);
    m_ui->bttnGroup_menubar->setId(m_ui->bttn_news, 7);
    m_ui->bttnGroup_menubar->setId(m_ui->bttn_language, 8);

    connect(m_ui->bttnGroup_menubar, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onMenubarBttnClicked(QAbstractButton*)));

    m_ui->short_date->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_ui->short_date_shadow->setAttribute(Qt::WA_TransparentForMouseEvents);

//    QGraphicsBlurEffect* eff = new QGraphicsBlurEffect(this);
//    m_ui->short_date_shadow->setGraphicsEffect(eff);
//    eff->setBlurRadius(3);

//    SharedDataService &sharedDataService = pApp->db().sharedDataService();
//    quintptr id = sharedDataService.singleIdForType<GameTableSettingsData>();
//    GameTableSettingsData* data = sharedDataService.getData<GameTableSettingsData>(id);

//    gbp_i64 playerID = -1;
//    // if (pApp->db().autoLogin()) {
//    //     pApp->db().getLoginUserId(playerID);
//    // }

//    data->restoreSelectionState(playerID);
}

void MainWindow::onMenubarBttnClicked(QAbstractButton* bttn)
{
    if (bttn != nullptr)
    {
        if (bttn == m_ui->bttn_cap) {
            bttn->move(-100, -100);
        }
        else
        {
            m_ui->bttn_cap->setFixedSize(bttn->size());
            m_ui->bttn_cap->move(bttn->pos());
        }
    }
}

void MainWindow::onMenubarBttnEntered(QWidget *wgt)
{
    QPixmap pixmap1(wgt->size());
    pixmap1.fill(Qt::transparent);
    QPainter p;

    p.begin(&pixmap1);
    wgt->render(&p);
    p.end();

    QPixmap pixmap2 = pixmap1.copy(0, 2, pixmap1.width(), pixmap2.height() - 2);
    QImage img = pixmap2.toImage();
    uchar* end = img.bits() + img.byteCount();
    for (uchar* it = img.bits(); it != end; it++) {
        *it -= 44;
    }

}
void MainWindow::onMenubarBttnLeaved(QWidget* wgt)
{
    if (QGraphicsEffect* eff = wgt->graphicsEffect()) {
        eff->setEnabled(false);
    }
}

//class LogOut : public Action {
//public:
//    LogOut(QObject* parent = nullptr)
//        : Action("log_out", 0, parent)
//    {}
//    virtual void execute() override {
//        rpc_lobby *rpcLobby = pApp->client().rpc<rpc_lobby>();
//        QTime time = QTime::currentTime();
//        qDebug() << "request logout";
//        rpcLobby->logout([this, time](ns_lobby::e_status status){
//            std::cout << "reply logout " << status << " " << time.msecsTo(QTime::currentTime()) << std::endl;
//            emit done(ns_helpers::isOk(status));
//        });
//    }
//};

void MainWindow::on_bttn_news_clicked()
{
//    SequentalActionQueue *q = new SequentalActionQueue(this);
//    q->append(new Action("action_10_ms", 10, q));
//    q->append(new Action("action_100_ms", 100, q));
//    q->append(new Action("action_500_ms", 500, q));
////    q->append(new LogOut(q));
//    q->append(new Action("action_1400_ms", 1400, q));

//    q->execute();
//    connect(q, &SequentalActionQueue::done, q, &SequentalActionQueue::deleteLater);
}


void MainWindow::setupAnimations()
{
    addPopUpAnimation(m_ui->languagePopUp);
    addPopUpAnimation(m_ui->timezonePopUp);

    connect(m_ui->bttnGroup_menubar, static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked), this, &MainWindow::triggerPopUp);
}

void MainWindow::registerNetworkReplyHandlers(const NetworkClient *client)
{
    const reply_lobby::emitter_t *emitter_auth   = client->emitter<reply_lobby>();
    const reply_lobby::emitter_t *emitter_avatar = client->emitter<reply_lobby>();

    connect(client, &NetworkClient::loggedIn, this, &MainWindow::onLogin, Qt::UniqueConnection);

    connect(emitter_auth,   &reply_lobby::emitter_t::logout,            this, &MainWindow::logout);
    connect(emitter_avatar, &reply_lobby::emitter_t::get_avatars, this, &MainWindow::get_avatars);



}

void MainWindow::addPopUpAnimation(QWidget *target, QPoint offset)
{
    if (!target->property("animation").isValid())
    {
        QPropertyAnimation* anim = new QPropertyAnimation(target, "pos", target->parentWidget());
        anim->setStartValue(target->pos());
        anim->setEndValue(target->pos() + offset);
        anim->setDuration(350);
        anim->setDirection(QAbstractAnimation::Backward);
        anim->setEasingCurve(QEasingCurve::InCirc);
        target->setProperty("animation", QVariant::fromValue(anim));
        connect(anim, &QPropertyAnimation::finished, this, [this]{if (m_currentPopUp == nullptr) m_ui->popup_space->hide();});
    }
}

bool MainWindow::isUserLoggedIn() const
{
    return pApp->client().userId() != -1;
}
#define STR(s) #s
#define PROFILE(code) (code); //printTime(__FILELINE__, STR(code))

void MainWindow::onLogin(const gbp::ns_api::ns_lobby::user_info_t& info)
{
    ns_model::user_info_t* userInfoModel = modelsDB().getUserInfo();

    page()->stackedWidget->setCurrentIndex(1);

    sha1_t hash = sha1_t::fromStdString(info._hash_avatar);
    QPixmap pixmap = pApp->db().getAvatarPixmap(hash);
    if (pixmap.isNull())
    {
//        request_lobby* avatar_api = pApp->client().request<request_lobby>();
//        avatar_api->get_avatar(info._user_id);

        rpc_lobby *rpcLobby = pApp->client().rpc<rpc_lobby>();

        rpcLobby->user_avatar([this, hash](ns_lobby::e_status result, const std::string &data){
            if (ns_helpers::isOk(result)) {
                sha1_t newHash = pApp->db().addAvatar(base64_t::fromStdString(data));
                //qDebug() << hash;
                //qDebug() << newHash;
                QPixmap pixmap = pApp->db().getAvatarPixmap(newHash);
                m_ui->main_window->setAvatar(pixmap);
            }
        });

    }
    else
    {
    }
    m_ui->main_window->setAvatar(pixmap);

    userInfoModel->registerDisplayWidget(page()->bttn_nickname, ns_model::user_info_t::field::_nickname);

//    page()->bttn_nickname->setText(info._nickname.c_str());
    connect(page()->bttn_nickname, &QPushButton::clicked, m_ui->bttn_account, &FramedButton::click);
    connect(page()->avatar, &ns_widget::ProfilePicture::pressed, m_ui->bttn_account, &FramedButton::click);
    connect(page()->bttn_status, &QPushButton::clicked, m_ui->bttn_vip_system, &FramedButton::click);
    Money m = Money(info._money);
    page()->bttn_accountMoney->setText(m.toString(Money::DisplayType::Full));/*setText(Money(info._money).toString());*/

    int x = 5;
    int y = 88;

    static QTime t = QTime::currentTime();
    auto printTime = [&](const char* f, int l, const char* funcinfo){
        qDebug("   Loc: [%s:%i] %s", f, l, funcinfo);
        qDebug() << (t.msecsTo(QTime::currentTime()));
        t = QTime::currentTime();
    };


    t = QTime::currentTime();
    PROFILE(m_accountDialog = new ns_dialog::Account(m_ui->popup_space));
    PROFILE(m_vipSystemDialog = new ns_dialog::VipSystem(m_ui->popup_space));
    PROFILE(m_settingsDialog = new ns_dialog::Settings(m_ui->popup_space));
    PROFILE(m_myTournamentsDialog = new ns_dialog::MyTournaments(m_ui->popup_space));
    auto fn_create = [&] {
        PROFILE(m_accountDialog->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint));
        PROFILE(m_vipSystemDialog->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint));
        PROFILE(m_settingsDialog->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint));
        PROFILE(m_myTournamentsDialog->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint));
        PROFILE(m_accountDialog->init());
        PROFILE(m_vipSystemDialog->init());
        PROFILE(m_settingsDialog->init());
        PROFILE(m_myTournamentsDialog->init());

        PROFILE(connect(m_accountDialog, &ns_dialog::Account::rejected, m_ui->bttn_cap, &QPushButton::click));
        PROFILE(connect(m_accountDialog, &ns_dialog::Account::avatarChanged, m_ui->main_window, &ns_page::MainScreen::setAvatar));
        PROFILE(connect(m_vipSystemDialog, &ns_dialog::VipSystem::rejected, m_ui->bttn_cap, &QPushButton::click));
        PROFILE(connect(m_settingsDialog, &ns_dialog::Settings::rejected, m_ui->bttn_cap, &QPushButton::click));
        PROFILE(connect(m_myTournamentsDialog, &ns_dialog::MyTournaments::rejected, m_ui->bttn_cap, &QPushButton::click));

        PROFILE(m_accountDialog->move(x - m_accountDialog->width(), y));
        PROFILE(m_vipSystemDialog->move(x - m_vipSystemDialog->width(), y));
        PROFILE(m_settingsDialog->move(x - m_settingsDialog->width(), y));
        PROFILE(m_myTournamentsDialog->move(x - m_myTournamentsDialog->width(), y));
        PROFILE(addPopUpAnimation(m_accountDialog, QPoint(m_accountDialog->width(), 0)));
        PROFILE(addPopUpAnimation(m_vipSystemDialog, QPoint(m_vipSystemDialog->width(), 0)));
        PROFILE(addPopUpAnimation(m_settingsDialog, QPoint(m_settingsDialog->width(), 0)));
        PROFILE(addPopUpAnimation(m_myTournamentsDialog, QPoint(m_myTournamentsDialog->width(), 0)));
    };
    fn_create();
//    QFuture<void> res = QtConcurrent::run(fn_create);

#ifndef NO_REGISTRATION
    if (m_regDialog != nullptr)
    {
        delete m_regDialog;
        m_regDialog = nullptr;
    }
#endif // !NO_REGISTRATION
}

void MainWindow::requestLogout() {
    rpc_object<rpc_lobby>()->logout();
}

void MainWindow::get_avatars(ns_lobby::e_status result, const std::unordered_map<gbp_i64, std::string> &avatars)
{
    if (result == ns_lobby::e_status::ok)
    {
        auto it = avatars.find( pApp->client().userId() );

        if (it != avatars.end())
        {
            sha1_t hash = pApp->db().addAvatar(base64_t::fromStdString(it->second));
            modelsDB().getUserInfo()->setData(modelsDB().getUserInfo()->indexOf(ns_model::user_info_t::field::_hash_avatar), hash);
            page()->avatar->setIcon(ns_util::fromBase64(base64_t::fromStdString(it->second)));
        }
    }
}

void MainWindow::triggerPopUp(QAbstractButton* bttn)
{
    if (m_authorize_canceled) return;

    if (bttn != m_ui->bttn_cap) {
     m_ui->popup_space->show();
    }

    // свернуть
    if (m_currentPopUp != nullptr)
    {
        QPropertyAnimation* animation = qvariant_cast<QPropertyAnimation*>(m_currentPopUp->property("animation"));
        if (animation != nullptr)
        {
            if (animation->state() == QAbstractAnimation::Running) {
                animation->stop();
            }
            animation->setDirection(QAbstractAnimation::Backward);
            animation->setEasingCurve(QEasingCurve::InCirc);
            animation->start();
            m_currentPopUp = nullptr;
        }
    }

    if (bttn == m_ui->bttn_language) {
        m_currentPopUp = m_ui->languagePopUp;
    } else if (bttn == m_ui->clock_widget) {
        m_currentPopUp = m_ui->timezonePopUp;
    } else if (bttn == m_ui->bttn_account) {
        m_currentPopUp = m_accountDialog;
    } else if (bttn == m_ui->bttn_vip_system) {
        m_currentPopUp = m_vipSystemDialog;
    } else if (bttn == m_ui->bttn_settings) {
        m_currentPopUp = m_settingsDialog;
    } else if (bttn == m_ui->bttn_tournaments) {
        m_currentPopUp = m_myTournamentsDialog;
    }

    // развернуть
    if (m_currentPopUp != nullptr)
    {
        QPropertyAnimation* animation = qvariant_cast<QPropertyAnimation*>(m_currentPopUp->property("animation"));
        if (animation != nullptr)
        {
            if (animation->state() == QAbstractAnimation::Running) {
                animation->stop();
            }
            animation->setDirection(QAbstractAnimation::Forward);
            animation->setEasingCurve(QEasingCurve::OutCirc);
            animation->start();            
        }
    }
}

void MainWindow::logout(ns_lobby::e_status result)
{
    if (ns_helpers::isOk(result)) {
        page()->stackedWidget->setCurrentIndex(0);
        if (m_accountDialog != nullptr) {
            delete m_accountDialog;
            m_accountDialog = nullptr;
        }
        if (m_vipSystemDialog != nullptr) {
            delete m_vipSystemDialog;
            m_vipSystemDialog = nullptr;
        }
        if (m_settingsDialog != nullptr) {
            delete m_settingsDialog;
            m_settingsDialog = nullptr;
        }
        if (m_myTournamentsDialog != nullptr) {
            delete m_myTournamentsDialog;
            m_myTournamentsDialog = nullptr;
        }
    }
}

bool MainWindow::openLoginDialog() const
{
    ns_dialog::Authorization dialog((QWidget*)this);
    dialog.init();
    return (dialog.exec() == QDialog::Accepted);
}

void MainWindow::requestDebug() {
    ns_tools::EditStyleSheet(this);
}

MainWindow::~MainWindow()
{
	delete m_bttnGroup_language;
    delete m_ui;
}

bool MainWindow::mustLogin() const {
    return isUserLoggedIn() || openLoginDialog();
}

Ui::MainScreen *MainWindow::page() {
    return m_ui->main_window->ui();
}

const Ui::MainScreen *MainWindow::page() const {
    return m_ui->main_window->ui();
}

TableView *MainWindow::statisticTable() const {
    return m_ui->main_window->statisticTable();
}

QTimeZone MainWindow::currentTimeZone() const {
    return QTimeZone(m_ui->comboBox->currentData().toByteArray());
}

Localizer* MainWindow::localizer()
{
    return &m_localizer;
}

void MainWindow::openRegistrationDialog() {
#ifndef NO_REGISTRATION
    if (m_regDialog == nullptr) {
        m_regDialog = new ns_dialog::Registration(this);
        m_regDialog->init();
    }
    m_regDialog->raise();
    m_regDialog->activateWindow();
    m_regDialog->open();
#endif // !NO_REGISTRATION
}

void MainWindow::openAccountDialog()
{
    m_authorize_canceled = false;

    if (!isUserLoggedIn()) {
        if (!openLoginDialog()) {
            m_ui->bttn_cap->click();
            m_ui->popup_space->hide();
            m_authorize_canceled = true;
            return;
        }
    }
}

void MainWindow::openVipSystemDialog()
{
    m_authorize_canceled = false;

    if (!isUserLoggedIn()) {
        if (!openLoginDialog()) {
            m_ui->bttn_cap->click();
            m_ui->popup_space->hide();
            m_authorize_canceled = true;
            return;
        }
    }

}

void MainWindow::openSettingsDialog()
{
    m_authorize_canceled = false;

    if (!isUserLoggedIn()) {
        if (!openLoginDialog()) {
            m_ui->bttn_cap->click();
            m_ui->popup_space->hide();
            m_authorize_canceled = true;
            return;
        }
    }
}

void MainWindow::openMyTournamentsDialog()
{
    m_authorize_canceled = false;

    if (!isUserLoggedIn()) {
        if (!openLoginDialog()) {
            m_ui->bttn_cap->click();
            m_ui->popup_space->hide();
            m_authorize_canceled = true;
            return;
        }
    }
}

void MainWindow::changeEvent(QEvent* event)
{
    if(event != nullptr)
    {
        if (event->type() == QEvent::LanguageChange)
        {
            m_ui->retranslateUi(this);
            page()->retranslateUi(this);
            onUiRetranslated();
        }
        else if (event->type() == QEvent::LocaleChange)
        {
            QString locale = QLocale::system().name();
            locale.truncate(locale.lastIndexOf('_'));
            m_localizer.loadLanguage(locale);
        }
    }
    QMainWindow::changeEvent(event);
}


void MainWindow::createLanguageMenu()
{
    QList<QAction* > actions = m_localizer.languageGroup()->actions();
    for (QAction* lang: actions)
    {
        ns_widget::LanguageItem* widget = new ns_widget::LanguageItem(lang, m_ui->languagePopUp);
//        widget->assignLocale(QLocale(lang->data().toString()));
        QGraphicsDropShadowEffect* eff = new QGraphicsDropShadowEffect(this);
        widget->button()->setGraphicsEffect(eff);
        eff->setEnabled(lang->isChecked());

        eff->setOffset(2., 4.);
        eff->setBlurRadius(22.);
        int n = 50;
        eff->setColor(QColor(n, n, n, 255 - n));
        m_bttnGroup_language->addButton(widget, widget->language());


        m_ui->layoutLanguageItems->insertWidget(1, widget, 1, Qt::AlignLeft | Qt::AlignVCenter);

//        connect(widget->button(), &QAbstractButton::toggled, this, &MainWindow::triggerLanguageChanged);
        connect(m_localizer.languageGroup(), &QActionGroup::triggered, this, &MainWindow::slotLanguageChanged);


        widget->setChecked(lang->isChecked());
        widget->setIconSize(lang->isChecked() ? QSize(48, 48) : QSize(32, 32));
//        widget->setContentHeight(lang->isChecked() ? 66 : 46);
        widget->setBackgroundColor(lang->isChecked() ? QColor(69, 90, 100) : QColor(0, 0, 0, 0));
        widget->indicator()->setVisible(lang->isChecked());

        widget->setFixedHeight(56);

        static const int msec = 160;

        auto animateText = [this](QLabel* widget, int to, QEasingCurve curve){
            QPropertyAnimation* animText = new QPropertyAnimation(widget, "indent", this);
            animText->setStartValue(widget->indent());
            animText->setEndValue(to);
            animText->setDuration(msec);
            animText->setEasingCurve(curve);
            animText->start(QAbstractAnimation::DeleteWhenStopped);
        };
        auto animateIcon = [this](QAbstractButton* widget, int newSize, QEasingCurve curve){
            QPropertyAnimation* animIcon = new QPropertyAnimation(widget, "iconSize", this);
            animIcon->setStartValue(widget->iconSize());
            animIcon->setEndValue(QSize(newSize, newSize));
            animIcon->setDuration(msec);
            animIcon->setEasingCurve(curve);
            animIcon->start(QAbstractAnimation::DeleteWhenStopped);
        };


        static const QColor selCol(69, 90, 100, 255);
        connect(widget, &ns_widget::LanguageItem::entered, widget, [animateIcon, animateText, widget]{
            if (!widget->isChecked())
            {
                widget->setBackgroundColor(selCol);
//                widget->setStyleSheet("QPushButton {background-color: rgba(69, 90, 100, 255);");
                animateIcon(widget->button(), 48, QEasingCurve::Linear);
                widget->button()->graphicsEffect()->setEnabled(true);
                animateText(widget->textLabel(), 16, widget->animation()->easingCurve());
            }
        });
        connect(widget, &ns_widget::LanguageItem::leaved, widget, [animateIcon, animateText, widget]{
            if (!widget->isChecked())
            {
                widget->setBackgroundColor(Qt::transparent);
//                widget->setStyleSheet("QPushButton {background-color: rgba(69, 90, 100, 0);");
                animateIcon(widget->button(), 32, QEasingCurve::Linear);
                widget->button()->graphicsEffect()->setEnabled(false);
                animateText(widget->textLabel(), 0, widget->animation()->easingCurve());
//                widget->adjustSize();
            }
        });
        connect(widget, &QAbstractButton::toggled, widget, [animateIcon, widget, animateText](bool toggle){
            if (!toggle)
            {
                widget->setBackgroundColor(Qt::transparent);
//                widget->setStyleSheet("QPushButton {background-color: rgba(69, 90, 100, 0);");
                animateIcon(widget->button(), 32, QEasingCurve::Linear);
                widget->button()->graphicsEffect()->setEnabled(false);
                animateText(widget->textLabel(), 0, widget->animation()->easingCurve());
                widget->adjustSize();
            }
            else
            {
                widget->setBackgroundColor(selCol);
//                widget->setStyleSheet("QPushButton {background-color: rgba(69, 90, 100, 255);}");
                animateIcon(widget->button(), 48, QEasingCurve::Linear);
                widget->button()->graphicsEffect()->setEnabled(true);
                animateText(widget->textLabel(), 16, widget->animation()->easingCurve());
                widget->adjustSize();
            }
        });
        m_ui->scrollContent_lang->adjustSize();



//        connect(m_bttnGroup_language, &QButtonGroup::buttonToggled, this, [this]{

//        });
    }
    QString file = ":/textures/menubarMask.png";
    if (!file.isNull() && !file.isEmpty()) {
        QPixmap maskPix(file);
        QPixmap pix(m_ui->languageBG->size());
        pix.fill(Qt::transparent);
        QPainter p;

        p.begin(&pix);
        p.drawPixmap(pix.width() - maskPix.width(), 0, maskPix.width(), maskPix.height(), maskPix);
        p.end();

        QBitmap bitmap = pix.createMaskFromColor(Qt::black);
        m_ui->languageBG->setMask(bitmap);
        m_ui->timezoneBG->setMask(bitmap);
    }
    m_ui->curveLine_language->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_ui->curveLine_timezone->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_ui->curveLine_language->raise();
    m_ui->curveLine_timezone->raise();


    triggerLanguageChanged();
    m_ui->scrollContent_lang->setMaximumHeight(m_ui->scrollContent_lang->height() + 20);
}

void MainWindow::createTimezoneMenu()
{
    m_ui->comboBox->setModel(getModelTimeZone());
    connect(m_ui->comboBox->view(), &QAbstractItemView::activated, m_ui->comboBox->model(), &ns_model::Timezone::setCurrent);

    m_ui->clock_widget->setTimezoneId(m_ui->comboBox->model()->currentId());
    m_ui->clock_widget->update();
    updateTimeDate();
    connect(m_ui->comboBox, (void(QComboBox::*)(int))&QComboBox::currentIndexChanged, this, [this](int row)
    {
        QByteArray timezoneId =  m_ui->comboBox->currentTimeZoneId();
        m_ui->clock_widget->setTimezoneId(timezoneId);
        m_ui->comboBox->model()->switchTimezone(row);
        //m_ui->bttn_cap->click();
    });
    connect(m_ui->comboBox->model(), &ns_model::Timezone::currentTimeZoneChanged, m_ui->comboBox, &TimeZoneBox::setCurrentIndex);
    connect(m_ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTimeDate()));
}

void MainWindow::slotLanguageChanged(QAction* action)
{
    m_localizer.changeLanguage(action);
    QString lang = action->data().toString();

    setLocale(QLocale(lang));
    //qDebug() << "!!!!-- " << lang << locale();
}


void MainWindow::updateTimeDate()
{
    QDateTime dateTime = m_ui->comboBox->currentDateTime();
    m_ui->msg_time->setText(dateTime.time().toString("hh:mm:ss"));
    m_ui->msg_date->setText(dateTime.date().toString(Qt::DateFormat::DefaultLocaleLongDate).replace(", ", ",\n").toUpper());
    m_ui->short_date->setText(dateTime.date().toString(Qt::DateFormat::RFC2822Date).toUpper());
    m_ui->short_date_shadow->setText(m_ui->short_date->text());
    m_ui->clock_widget->setTimezoneId(m_ui->comboBox->currentTimeZoneId());
}

void MainWindow::triggerLanguageChanged()
{
    QAbstractButton* currentWidget = m_bttnGroup_language->checkedButton(); //m_ui->listWidget->itemWidget(m_ui->listWidget->currentItem());
    ns_widget::LanguageItem* w = qobject_cast<ns_widget::LanguageItem*>(currentWidget);
    if (w)
    {
        w->trigger();
        m_ui->bttn_language->setIcon(w->icon());
    }
    m_ui->bttn_cap->click();
}

void MainWindow::onUiRetranslated()
{
    int h = 0;
    int total = 0;
    static const int clockHeight = 87;
    static const int langHeight = 87;
    m_ui->bttnGroup_menubar->button(0)->setFixedHeight(clockHeight);
    m_ui->bttnGroup_menubar->button(8)->setFixedHeight(langHeight);
    int heights[8];
    for (int i = 1; i < 8; i++)
    {
        QAbstractButton* bttn = m_ui->bttnGroup_menubar->button(i);
        FramedEntity* icon = bttn->findChild<FramedEntity*>(bttn->objectName().replace("bttn_", "icon_"));
        h = (nullptr != icon) ? icon->height() : 0;
        h += bttn->fontMetrics().size(Qt::TextWordWrap, tr(bttn->text().toLatin1())).height();
        heights[i] = h;
        total += h;
    }
    int freeSpace = m_ui->bar->height() - clockHeight - langHeight - total - 9;

    int y = clockHeight + 1;
    for (int i = 1; i < 8; i++)
    {
        QAbstractButton* bttn = m_ui->bttnGroup_menubar->button(i);
        int delta = freeSpace / (8-i);
        freeSpace -= delta;
        bttn->setFixedHeight(heights[i] + delta);
        bttn->move(0, y);
        y += bttn->height();
    }
    m_ui->bttnGroup_menubar->button(8)->move(0, m_ui->bar->height() - langHeight + 1);

    triggerLanguageChanged();
}


void MainWindow::closeEvent(QCloseEvent *e)
{
    QMainWindow::closeEvent(e);
    if (e->isAccepted()) {
        emit closed();
    }
}
