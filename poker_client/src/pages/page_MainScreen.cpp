#include <precompiled.h>
#include "page_MainScreen.h"
#include "ui_page_MainScreen.h"
#include "widgets/StatisticTableView.h"
#include "models/model_PlayersListModel.h"
#include <QMetaMethod>
#include <QScrollBar>
#include <QTableView>
#include <QTimer>
#include <styles/shadowedtextstyle.h>

#include <dialogs/lobby/dialog_LobbyTournament.h>
#include "../gui/cashier/cashier/cashierdialog.h"

#include <models/model_GamesStatistic.h>
#include <api_models/api_models.hpp>
#include <db/modeldatabase.h>


#ifndef Q_MOC_RUN
    #include <rpc/rpc_common.h>
    #include <rpc/rpc_lobby.h>
#endif // !Q_MOC_RUN

template <typename T>
T reflectedValue(T value);

template <>
QAbstractAnimation::Direction reflectedValue(QAbstractAnimation::Direction value) {
    return (value == QAbstractAnimation::Forward) ? QAbstractAnimation::Backward : QAbstractAnimation::Forward;
}
template <>
QEasingCurve::Type reflectedValue(QEasingCurve::Type value) {
    switch (value) {
        case QEasingCurve::InBack:  return QEasingCurve::OutBack;
        case QEasingCurve::OutBack: return QEasingCurve::InBack;
        default:                    return QEasingCurve::Linear;
    }
}

namespace ns_page
{
    MainScreen::MainScreen(QWidget *parent)
        : QWidget(parent)
        , m_ui(new Ui::MainScreen)
        , m_animation(nullptr)
        {
            m_ui->setupUi(this);
        }

    MainScreen::~MainScreen()
    {
        delete m_animation;
        delete m_ui;
    }

    void MainScreen::init()
    {
        ns_model::user_info_t* usrModel = modelsDB().getUserInfo();
//        connect(&netClient(), &NetworkClient::loggedIn, this, [this, usrModel] {
//        });
        ns_model::registerDisplayWidget(usrModel, m_ui->bttn_nickname, ns_model::user_info_t::field::_nickname);
        ns_model::registerDisplayWidget(usrModel, m_ui->bttn_accountMoney, ns_model::user_info_t::field::_money);
        ns_model::registerDisplayWidget(usrModel, m_ui->bttn_status, ns_model::user_info_t::field::_vip_status);

        m_ui->page_logged->addAction(m_ui->actionLogout);
        connect(m_ui->bttn_cash, &QAbstractButton::clicked, m_ui->actionOpenCashier, &QAction::trigger);

        connect(m_ui->actionOpenCashier, &QAction::triggered, this, [this]{
            CashierDialog* cashier = new CashierDialog(pApp->mainWindow());
            cashier->setAttribute(Qt::WA_DeleteOnClose);
            cashier->setUserInfoModel(modelsDB().getUserInfo());
            cashier->setRpcObject(rpc_object<rpc_lobby>());
            cashier->init();

            cashier->setAvatar(m_ui->avatar->icon());
            cashier->open();
        });

        QList<QAbstractButton*> buttons = findChildren<QAbstractButton*>(QRegularExpression(".+"));
        for (QAbstractButton* bttn : buttons) {
//        for (QAbstractButton* bttn : m_ui->buttonGroup_types->buttons()) {
            setTextDropShadowStyle(bttn, QPoint(0, 1), QColor(0x191919));
        }
        QList<QLabel*> lst = findChildren<QLabel*>(QRegularExpression(".+"));
        for (QLabel* label : lst) {
            setTextDropShadowStyle(label, QPoint(0, -1), QColor(0, 0, 0, 255));
        }

//        ns_model::CashTables* modelCash = new ns_model::CashTables;
//        m_ui->tab_cashTables->setModel(modelCash);
//        m_ui->tab_tournaments->setModel(modelTourn);
//        m_ui->tab_sitNGo->setModel(modelTourn);

//        m_ui->tab_cashTables->setModel();
        m_ui->tab_cashTables->init();
        m_ui->tab_tournaments->init();
        m_ui->tabWidget->setTabEnabled(2, false);
        m_ui->tabWidget->setTabEnabled(3, false);
#ifdef ONLY_FULLY_COMPLETED_CONTENT
        m_ui->tabWidget->setTabEnabled(1, false);
        m_ui->tabWidget->setTabEnabled(4, false);
#endif // ONLY_FULLY_COMPLETED_CONTENT

        m_ui->tab_spinNGo->init();
        m_ui->tab_sitNGo->init();
//        m_ui->tabWidget->setTabEnabled(3, false);
//        m_ui->tabWidget->setTabEnabled(4, false);

    }

    Ui::MainScreen *MainScreen::ui() const {
        return m_ui;
    }

    TableView *MainScreen::statisticTable() const
    {
        if (MainLobbyBase* lobby = qobject_cast<MainLobbyBase*>(m_ui->tabWidget->currentWidget())) {
            return lobby->tableView();
        }
        return nullptr;
    }

    CashTables *MainScreen::cashTablesPage() const {
        return m_ui->tab_cashTables;
    }

    Tournaments *MainScreen::tournamentsPage() const {
        return m_ui->tab_tournaments;
    }

    SitNGo *MainScreen::sitNGoPage() const {
        return m_ui->tab_sitNGo;
    }

    void MainScreen::updateTexts() const {
        m_ui->tab_cashTables->updateTexts();
    }

    void MainScreen::onTableItemDoubleClicked(const QModelIndex &/*modelIndex*/)
    {

    }

    void MainScreen::enableBlur() {
//        m_blurEffect->setEnabled(true);
    }

    void MainScreen::disableBlur() {
//        m_blurEffect->setEnabled(false);
    }

    void MainScreen::changeEvent(QEvent *e)
    {
        if (e != nullptr && e->type() == QEvent::LanguageChange) {
            m_ui->retranslateUi(this);
            updateTexts();
        }
        QWidget::changeEvent(e);
    }

    void MainScreen::toggleAnimationDirection()
    {
        m_animation->setDirection(reflectedValue(m_animation->direction()));
        m_animation->setEasingCurve(reflectedValue(m_animation->easingCurve().type()));
    }

    void MainScreen::toggleTableView()
    {
        if (m_animation->state() == QAbstractAnimation::Running)
        {
            m_animation->stop();
        }
        toggleAnimationDirection();
        m_animation->start();
    }

    void MainScreen::setAvatar(const QIcon &icon) {
        m_ui->avatar->setAvatar(Avatar(icon));
    }

    void MainScreen::onLogoutActionTriggered() {
        rpc_object<rpc_lobby>()->logout();
    }

} // namespace page
