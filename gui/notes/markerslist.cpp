//#include <precompiled.h>
#include "delegates.h"
#include "markerslist.h"
#include "ui_markerslist.h"
#include <QDebug>
#include <qevent.h>
#include <qfileinfo.h>
#include <qidentityproxymodel.h>

void MarkersListView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QListView::currentChanged(current, previous);
    emit currentChanged(current);

}

void MarkersTableView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QTableView::currentChanged(current, previous);
    emit currentChanged(current);
}

// -------------------------------------

MarkersList::MarkersList(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::MarkersList)
    , m_model(new QIdentityProxyModel)
{
    m_ui->setupUi(this);

//    connect(m_ui->listView_markers, SIGNAL(currentChanged(QModelIndex)), this, SIGNAL(currentChanged(QModelIndex)));
}

MarkersList::~MarkersList()
{
    delete m_ui;
}

void MarkersList::init()
{   
//    resetMarkersModel(new ns_model::Markers);
    m_ui->listView_markers->setModel(m_model.data());
    m_ui->listView_markers->horizontalHeader()->resizeSection(1, 16);
    m_ui->listView_markers->horizontalHeader()->resizeSection(2, 54);
    m_ui->listView_markers->horizontalHeader()->resizeSection(0, m_ui->listView_markers->width() - 70);

    addAction(m_ui->actionsave);
    addAction(m_ui->actionload);

    EditorDelegate* editorDelegate = new EditorDelegate;
    m_ui->listView_markers->setItemDelegateForColumn(0, editorDelegate);
    m_ui->listView_markers->setItemDelegateForColumn(1, new Delegate);
    m_ui->listView_markers->setItemDelegateForColumn(2, new Delegate);

    connect(m_model.data(), &ns_model::Markers::rowsAboutToBeInserted, editorDelegate, &EditorDelegate::reset);
    connect(m_model.data(), &ns_model::Markers::rowsAboutToBeRemoved, editorDelegate, &EditorDelegate::reset);
    connect(m_model.data(), &ns_model::Markers::rowsAboutToBeMoved, editorDelegate, &EditorDelegate::reset);
    connect(m_model.data(), &ns_model::Markers::modelReset, editorDelegate, &EditorDelegate::reset);
    connect(m_model.data(), &ns_model::Markers::dataChanged, editorDelegate, &EditorDelegate::reset);

    connect(m_ui->listView_markers, &QTableView::clicked, this, [this](const QModelIndex& idx) {
        if (idx.column() == 1) {
            const QModelIndex index = idx.model()->index(idx.row(), 0);
            m_ui->listView_markers->setCurrentIndex(index);
            m_ui->listView_markers->edit(index);
        } else if (idx.column() == 2) {
            m_ui->listView_markers->model()->removeRow(idx.row());
        }
    });

    connect(m_ui->bttn_newMark, &QPushButton::clicked, this, [this] {
        QAbstractItemModel* m = m_ui->listView_markers->model();
        if (m->insertRow(1)) {
            const QModelIndex modelIndex = m->index(1, 0);
            m_ui->listView_markers->setCurrentIndex(modelIndex);
            m_ui->listView_markers->edit(modelIndex);
        }
    });

    auto updBttnNewMarkIsEnabled = [this] { m_ui->bttn_newMark->setEnabled(m_model->rowCount() < ns_model::Markers::sm_maximumItems); };

    connect(m_model.data(), &ns_model::Markers::rowsInserted, this, updBttnNewMarkIsEnabled);
    connect(m_model.data(), &ns_model::Markers::rowsRemoved,  this, updBttnNewMarkIsEnabled);

}

ns_model::Markers *MarkersList::markersModel() const
{
    return qobject_cast<ns_model::Markers*>(m_model->sourceModel());
}

void MarkersList::resetMarkersModel(ns_model::Markers *m)
{
    if (m_model->sourceModel() != m)
    {
//        if (auto oldModel = markersModel())
//        {
//            disconnect(m_ui->actionsave, &QAction::triggered, oldModel, (void(ns_model::Markers::*)())&ns_model::Markers::save);
//            disconnect(m_ui->actionload, &QAction::triggered, oldModel, (void(ns_model::Markers::*)())&ns_model::Markers::load);
//        }
        m_model->setSourceModel(m);
//        if (m != nullptr)
//        {
//            connect(m_ui->actionsave, &QAction::triggered, m, (void(ns_model::Markers::*)())&ns_model::Markers::save);
//            connect(m_ui->actionload, &QAction::triggered, m, (void(ns_model::Markers::*)())&ns_model::Markers::load);
//        }
    }
}

void MarkersList::changeEvent(QEvent *e)
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

void MarkersList::resizeEvent(QResizeEvent *e)
{
    int widthDiff = e->size().width() - e->oldSize().width();

    if (widthDiff != 0) {
//        m_ui->listView_markers->horizontalHeader()->resizeSection(0, m_ui->listView_markers->horizontalHeader()->sectionSize(0) + widthDiff);
        m_ui->listView_markers->horizontalHeader()->resizeSection(0, e->size().width() - 70);
    }
    QWidget::resizeEvent(e);
}


void MarkersList::on_bttn_up_clicked()
{
    QModelIndex currIdx = m_model->mapToSource(m_ui->listView_markers->currentIndex());
    if (currIdx.isValid() && currIdx.row() > 0) {
        int row = currIdx.row();
        if (m_model->sourceModel()->moveRow(currIdx.parent(), currIdx.row(), currIdx.parent(), currIdx.row()-1)) {
            m_ui->bttn_up->setEnabled(row > 2);
            m_ui->bttn_down->setEnabled(true);
        }
    }
}

void MarkersList::on_bttn_down_clicked()
{
    QModelIndex currIdx = m_model->mapToSource(m_ui->listView_markers->currentIndex());
    if (currIdx.isValid() && currIdx.row() < m_model->sourceModel()->rowCount() - 1) {
        int row = currIdx.row();
        if (m_model->sourceModel()->moveRow(currIdx.parent(), currIdx.row()+1, currIdx.parent(), currIdx.row()) ) {
            m_ui->bttn_up->setEnabled(true);
            m_ui->bttn_down->setEnabled(currIdx.model()->rowCount(currIdx.parent()) - row > 2);
        }
    }
}

void MarkersList::on_listView_markers_currentChanged(const QModelIndex &idx)
{
    m_ui->bttn_up->setEnabled(idx.isValid() && idx.row() > 1);
    m_ui->bttn_down->setEnabled(idx.isValid() && (idx.model()->rowCount(idx.parent()) - idx.row() > 1));
    emit currentChanged(idx);
}

void MarkersList::setCurrent(const QModelIndex &idx)
{
    m_ui->listView_markers->setCurrentIndex(idx);
}





