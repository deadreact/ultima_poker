#include <QtWidgets/QLabel>
#include "precompiled.h"
#include <qsortfilterproxymodel.h>
#include <testform.h>
#include "widgets/MainWindow.h"

#include <QTimer>
#include <core/money.h>
#include "utils/CommonPainter.h"
#include <styles/colorizedstyle.h>
#include <core/colortransform.h>
#include <game/tableItems/panel_BetControls.h>
#include <dialogs/buyin/dialog_TournamentBuyIn.h>

#include <api_models/api_models.hpp>
#include <api_models/model_cash_stat_t.hpp>
#include <api_models/model_tournament_stat_t.hpp>
#include <api_models/model_transformproxy.h>

#include <qlistview.h>
#include <qtableview.h>
#include <qtreeview.h>
#include <qcolumnview.h>
#include <QHeaderView>
#include <api_models/model_user_info_t.hpp>
#include <models/model_GamesStatistic.h>
#include <models/model_VipStatusInfo.h>
#include <models/model_cashtables.h>
#include <qpushbutton.h>
#include <iostream>
#include <qdesktopwidget.h>
#include <db/modeldatabase.h>
#include <api_models/model_tournament_stat_t.hpp>
#include <api_models/model_tournament_user_info_t.h>
#include <QWindow>
#include <QScreen>
#include <tools/sequentalactionqueue.h>

void testListView()
{
    QTableView* v = new QTableView;
    v->setWindowFlags(Qt::Window);
    v->setModel(new ns_model::VipStatusInfo);
    v->show();
    v->raise();

#ifdef ENABLE_VISUAL_DEBUGGING
    netClient().connect(&netClient(), &NetworkClient::loggedIn, []{
        QListView* lst = new QListView(pApp->mainWindow());
        lst->setWindowFlags(Qt::Window);
        QAbstractListModel* m = modelsDB().getUserInfo();
        lst->setModel(m);
        lst->showNormal();
        lst->raise();
    });
#endif // ENABLE_VISUAL_DEBUGGING
}

void testNativeModel(QWidget* parent = nullptr)
{
    using CurrentView = QTableView;
    CurrentView* view = new CurrentView(parent);
    ns_model::tournament_stat_t* model = new ns_model::tournament_stat_t(view);
    ns_model::TransformProxy* proxyModel = new ns_model::TransformProxy;
    proxyModel->setSourceModel(model);

    view->setModel(proxyModel);
    model->init();

    view->connect(view, &CurrentView::activated, [view](const QModelIndex& idx) { if (view->model()->hasChildren(idx)) view->setRootIndex(idx); });

//    view->setWordWrap(true);
    view->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    view->setWindowFlags(Qt::Window);
    view->verticalHeader()->setDefaultSectionSize(16);
    view->verticalHeader()->setVisible(true);
    view->horizontalHeader()->setVisible(true);

    QPushButton *bttn = new QPushButton;
    view ->setIndexWidget(model->index(0, 0), bttn);
    bttn->connect(bttn, &QPushButton::clicked, [proxyModel]{ proxyModel->toggleTranspose(); });
    view->show();
    view->raise();
    bttn->show();
    bttn->raise();
    view->setStyleSheet("QAbstractItemView, QHeaderView { font: 550 12px \"Open Sans\"; color: #fafafa; }"
                        "QAbstractItemView::item::disabled { color: rgba(250, 250, 250, 150); background-color: gray}");
    view->activateWindow();
}

void testCashNativeModel(QWidget* parent = nullptr)
{

    using CurrentView = QTableView;
    CurrentView* view = new CurrentView(parent);
    ns_model::cash_stat_t* model = new ns_model::cash_stat_t(view);
    ns_model::TransformProxy* proxyModel = new ns_model::TransformProxy;
    proxyModel->setSourceModel(model);

    view->setModel(proxyModel);
    model->init();

    view->connect(view, &CurrentView::activated, [view](const QModelIndex& idx) { if (view->model()->hasChildren(idx)) view->setRootIndex(idx); });

//    view->setWordWrap(true);
    view->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    view->setWindowFlags(Qt::Window);
    view->verticalHeader()->setDefaultSectionSize(16);
    view->verticalHeader()->setVisible(true);
    view->horizontalHeader()->setVisible(true);

    QPushButton *bttn = new QPushButton;
    view ->setIndexWidget(model->index(0, 0), bttn);
    bttn->connect(bttn, &QPushButton::clicked, [proxyModel]{ proxyModel->toggleTranspose(); });
    view->show();
    view->raise();
    bttn->show();
    bttn->raise();
    view->setStyleSheet("QAbstractItemView, QHeaderView { font: 550 12px \"Open Sans\"; color: #fafafa; }"
                        "QAbstractItemView::item::disabled { color: rgba(250, 250, 250, 150); background-color: gray}");
    view->activateWindow();
}

void testBuyIn(QWidget *parent = nullptr)
{
    ns_descriptor::TournamentBuyIn desc("Это тестовый бай-ин", "Бай-ин это тестовый", 1110, 155, 10, -1, false);

    ns_dialog::TournamentBuyIn* buyIn = new ns_dialog::TournamentBuyIn(GameId(1, 1), parent);
    buyIn->initWithDescriptor(&desc);
//    buyIn->show();
//    buyIn->raise();
    buyIn->exec();
}

void enableEffects(QApplication& app)
{
    app.setEffectEnabled(Qt::UI_AnimateMenu, true);
    app.setEffectEnabled(Qt::UI_FadeMenu, true);
    app.setEffectEnabled(Qt::UI_AnimateCombo, true);
    app.setEffectEnabled(Qt::UI_AnimateTooltip, true);
    app.setEffectEnabled(Qt::UI_FadeTooltip, true);
    app.setEffectEnabled(Qt::UI_AnimateToolBox, true);
}

void applyStyle(QMainWindow& w)
{
    ns_utildb::appendSavedStyle("tableview", &w);
}

int main(int argc, char *argv[])
{
    PokerApplication app(argc, argv);

    app.addLibraryPath(QDir::current().absolutePath() + "/lib");

    if (app.arguments().contains("--version")) {
        std::cout << __DATE__ << "-" << __TIME__ << std::endl;
        return 0;
    }

    app.setApplicationVersion("0.0.0.0.1");
    app.registerMetaTypes();

    if (!app.initData()) {
        return -1;
    }

    MainWindow* mainWnd = app.mainWindow();

    enableEffects(app);
    applyStyle(*mainWnd);

    mainWnd->connect(app.mainWindow(), &MainWindow::destroyed, &app, &PokerApplication::quit);
    app.setOrganizationName("Ultima Poker");
    app.setApplicationName("Ultima Poker");

//    QDesktopWidget* desktop = app.desktop();

#ifdef DEBUG_WINDOW
    TestForm* tstForm = new TestForm;
    tstForm->setWindowFlags(Qt::Window);
    tstForm->open();
    tstForm->setGeometry(app.screens().last()->geometry().adjusted(100, 100, -100, -100));
#endif

    mainWnd->move(app.screens().first()->geometry().center() - mainWnd->rect().center());

#ifdef DEBUG_WINDOW
    mainWnd->connect(mainWnd, &MainWindow::closed, tstForm, &TestForm::close);
#endif

//    QFile f(".clang_complete");
//    f.open(QIODevice::WriteOnly);
//    f.close();


    return app.exec();
}

