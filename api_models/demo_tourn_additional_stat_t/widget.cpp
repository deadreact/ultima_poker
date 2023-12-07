#include "widget.h"
#include "ui_widget.h"
#include <model_tourn_additional_stat_t.hpp>
#include <iostream>

#ifndef Q_MOC_RUN
    #include <api/lobby_stat/types.h>
#endif //Q_MOC_RUN
#include <QDebug>
#include <model_tournament_stat_t.hpp>
#include <model_tournament_user_info_t.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::Widget)
{
    m_ui->setupUi(this);


    ns_model::tourn_additional_stat_t* m = new ns_model::tourn_additional_stat_t(-1);
    m->updateData(createRandomItem(rand()%101));

    m_ui->treeView->setModel(m);
    m_ui->treeView_2->setModel(m);
    m_ui->treeView_3->setModel(m);
    m_ui->tableView->setModel(m);
    m_ui->tableView_2->setModel(m);
    m_ui->tableView_3->setModel(m);


    qDebug() << m->index(0, 0, m->index(1, 0));


    ns_model::tournament_user_info_t* pm = new ns_model::tournament_user_info_t;
    pm->setSourceModel(new ns_model::tournament_stat_t);

    m_ui->tableView_4->setModel(pm->sourceModel());
    m_ui->tableView_5->setModel(pm);

    connect(m_ui->tableView_4, (void(TableView::*)(const QModelIndex&))&TableView::currentChanged, this, [this, pm](const QModelIndex& idx)
    {
        try {
            pm->setId(idx.data(UserType::RowItemIdRole).value<qint64>());
        } catch (std::out_of_range& ex) {
            qDebug() << ex.what() << __FILE__ << __LINE__ << __FUNCTION__;
        }
    });

    games_stat_t d;
    d.second.resize(11);
    for (auto& item:d.second) {
        item._users.resize(rand()%20);
    }
    pm->sourceModel()->updateData(d);
}

Widget::~Widget()
{
    delete m_ui;
}

void Widget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Widget::on_tableView_currentChanged(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    m_ui->tableView_2->setRootIndex(index);
    m_ui->tableView_2->setVisible(index.model()->hasChildren(index));
}

void Widget::on_tableView_2_currentChanged(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    m_ui->tableView_3->setRootIndex(index);
    m_ui->tableView_3->setVisible(index.model()->hasChildren(index));
}

void Widget::on_tableView_4_currentChanged(const QModelIndex &)
{

}
