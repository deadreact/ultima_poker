#pragma once

//#include <QHeaderView>
//#include <QTableView>
//#include <qsortfilterproxymodel.h>
//#include <models/model_GamesStatistic.h>


//#include "defs.h"

//namespace ns_model {
//    class CashTables;
//    class Tournaments;
//    class cash_stat_t;
//    class tournament_stat_t;

//}

//class HorizontalHeaderView : public QHeaderView {
//    Q_OBJECT
//public:
//    HorizontalHeaderView(QWidget* parent = nullptr) : QHeaderView(Qt::Horizontal, parent) {}
//};

//namespace Ui {
//class StatisticTableView;
//}

//class StatisticTableView : public QTableView
//{
//    Q_OBJECT
//protected:
//    Ui::StatisticTableView *m_ui;

//    gbp_i64 m_savedID;
//signals:
//    void currentIndexChanged(const QModelIndex&);
//    void modelUpdated(quintptr sharedDataID);
//public:
//    explicit StatisticTableView(QWidget *parent = nullptr);
//    virtual ~StatisticTableView();
//    virtual void init();

//    int rowCount() const;
//    int columnCount() const;
////    bool isReady() const;

//protected slots:
//    virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

//    // QWidget interface
//protected:
//    virtual void showEvent(QShowEvent *e) override;
//};


//class CashTableView : public StatisticTableView
//{
//    Q_OBJECT

//    QScopedPointer<ns_model::CashTables> m_model;
//public:
//    CashTableView(QWidget* parent = nullptr);

//    void setSourceModel(ns_model::cash_stat_t* m);
//    virtual void init() override;

//    games_stat_t::first_type::value_type item() const;
//};


//class TournTableView : public StatisticTableView
//{
//    Q_OBJECT

//    QScopedPointer<ns_model::Tournaments> m_model;
//public:
//    TournTableView(QWidget* parent = nullptr);

//    void setSourceModel(ns_model::tournament_stat_t* m);
//    virtual void init() override;
//};
