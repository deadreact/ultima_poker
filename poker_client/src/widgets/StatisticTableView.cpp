#include <precompiled.h>
//#include <qtimer.h>
#include "StatisticTableView.h"
//#include "ui_StatisticTableView.h"
//#include <db/Database.h>
//#include <models/model_GamesStatistic.h>
//#include <QScrollBar>
//#include <QLabel>
//#include <delegates/delegate_GamesStatisticItem.h>
//#include <api_models/api_util.hpp>
//#include <api_models/model_cash_stat_t.hpp>
//#include <api_models/model_tournament_stat_t.hpp>

//#ifndef Q_MOC_RUN
//    #include <api/lobby_stat/types.h>
//#endif // !Q_MOC_RUN

//StatisticTableView::StatisticTableView(QWidget *parent)
//    : QTableView(parent)
//    , m_ui(new Ui::StatisticTableView)
////    , m_rootModel(nullptr)
//{
//    m_ui->setupUi(this);
//}

//StatisticTableView::~StatisticTableView()
//{
//    delete m_ui;
//}

//void StatisticTableView::init()
//{
////    setVerticalScrollBar(m_ui->scrollbar);
////    horizontalHeader()->setObjectName(objectName() + "_horizontalHeader");

////    horizontalHeader()->setFixedHeight(24);
////    m_ui->scrollbar->setParent(m_ui->scrollbar_container);

////    m_proxyModel->setSourceModel(m_rootModel);
////    m_proxyModel->setSortRole(UserType::SortRole);
////    setModel(m_proxyModel);


////    connect(model(), &QAbstractItemModel::modelReset, this, [this]{
////        if (!currentIndex().isValid()) {
////            setCurrentIndex(model()->index(0, 0));
////        }
////    });

////#ifdef ENABLE_VISUAL_DEBUGGING
////    connect(m_proxyModel, &QAbstractItemModel::dataChanged, this, [this](const QModelIndex& indexFrom, const QModelIndex& indexTo, const QVector<int>& roles){
////        update(visualRect(/*m_proxyModel->mapFromSource*/(indexFrom)));

////        QLabel *w = new QLabel(viewport());
////        w->setStyleSheet("QLabel { background-color: rgba(200, 100, 100, 170); border: 2px outset #a9d2f0; color: rgba(200, 200, 144, 252); font: 11px \"Open Sans Semibold\"; }");
////        QRect rc = visualRect(indexFrom).united(visualRect(indexTo));
////        QStringList lst;

////        for (int role: roles) {
////            lst << ((role >= Qt::UserRole) ? ns_util::enumToString(UserType::ERoles(role)) : ns_util::enumToString(Qt::ItemDataRole(role)));
////        }
////        w->setWordWrap(true);
////        w->setText(lst.join(", "));
////        w->setGeometry(rc);
////        w->raise();
////        w->show();
////        QTimer::singleShot(1000, [w]{w->deleteLater();});
////    });
////#endif //ENABLE_VISUAL_DEBUGGING

////    ns_delegate::GamesStatisticItem* itemDelegate = new ns_delegate::GamesStatisticItem(22, this);

////    this->setItemDelegateForColumn(0, itemDelegate);

//}

////ns_model::GamesStatistic *StatisticTableView::rootModel() const
////{
////    return m_rootModel;
////}

////QSortFilterProxyModel *StatisticTableView::proxyModel() const
////{
////    return m_model;
////}

////bool StatisticTableView::isReady() const {
////    return true;//rootModel() && rootModel()->isReady();
////}

////void StatisticTableView::setRootModel(ns_model::GamesStatistic *model)
////{
////    m_rootModel = model;
////}
////void StatisticTableView::setProxyModel(QSortFilterProxyModel *model)
////{
////    m_model = model;
////}

//void StatisticTableView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
//{
//    QTableView::currentChanged(current, previous);

//    emit currentIndexChanged(current);
//}

//int StatisticTableView::rowCount() const
//{
//    return model()->rowCount();
//}

//int StatisticTableView::columnCount() const
//{
//    return model()->columnCount();
//}

//void StatisticTableView::showEvent(QShowEvent *e)
//{
//    if (!currentIndex().isValid())
//    {
//        setCurrentIndex(model()->index(0, 0));
//    }
//    QTableView::showEvent(e);
//}

////=================================================================================
////=================================================================================

//CashTableView::CashTableView(QWidget *parent)
//    : StatisticTableView(parent)
//    , m_model(new ns_model::CashTables)
//{
//    setModel(m_model.data());
//}

//void CashTableView::setSourceModel(ns_model::cash_stat_t *m)
//{
//    m_model->setSourceModel(m);
//}

//void CashTableView::init()
//{

//}

//games_stat_t::first_type::value_type CashTableView::item() const
//{
//    return qvariant_cast<games_stat_t::first_type::value_type>(currentIndex().data(UserType::RowDataRole));
//}

////=================================================================================
////=================================================================================

//TournTableView::TournTableView(QWidget *parent)
//    : StatisticTableView(parent)
//    , m_model(new ns_model::Tournaments)
//{
//    setModel(m_model.data());
//}

//void TournTableView::setSourceModel(ns_model::tournament_stat_t *m) {
//    m_model->setSourceModel(m);
//}

//void TournTableView::init()
//{

//}
