//#include <precompiled.h>
#include "model_notes.h"
#include "model_markers.h"
#include "noteslist.h"
#include "ui_noteslist.h"
#include <QDebug>
#include <iostream>
#include <qfile.h>
#include <qitemdelegate.h>
#include <qmessagebox.h>
#include <qpainter.h>
#include <qsortfilterproxymodel.h>
#include <basewidgets/types.h>
#include "delegates.h"
#include "noteslisteditor.h"
#include <QScrollBar>
#include <QMetaEnum>
#include <qevent.h>
#include <QPropertyAnimation>

NotesTableView::NotesTableView(QWidget *parent)
    : QTableView(parent)
{
    connect(this, &NotesTableView::clicked, this, [this](const QModelIndex& idx){
        QRect rectDel(width() - 40, 1, 20, 14);
        QRect rectEdit(width() - 64, 1, 20, 14);
        if (idx.column() == 1) {
            QPoint localPos = viewport()->mapFromGlobal(QCursor::pos()) - QPoint(horizontalScrollBar()->isVisible() ? horizontalScrollBar()->width() : 0, 16*idx.row());
            if (rectEdit.contains(localPos)) {
                QModelIndex idx2 = idx.sibling(idx.row(), 0);
                setCurrentIndex(idx2);
                edit(idx2);
            } else if (rectDel.contains(localPos)) {
                if (QMessageBox::question(this, tr("@title_delete_note"), tr("@question_confirm_delete_note")) == QMessageBox::Yes) {
                    model()->removeRow(idx.row());
                }
            }
        }
    });
}

void NotesTableView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QTableView::currentChanged(current, previous);
    emit currentChanged(current);
}


// --------------------------------------------------------------

NotesList::NotesList(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::NotesList)
    , m_proxyModel(new QSortFilterProxyModel)
{
    m_ui->setupUi(this);

    connect(m_ui->tableView, SIGNAL(currentChanged(QModelIndex)), this, SIGNAL(currentChanged(QModelIndex)));

//    m_ui->tableView->horizontalHeader()->resizeSection(0, 160);
//    m_ui->tableView->horizontalHeader()->resizeSection(1, 100);
//    m_ui->tableView->horizontalHeader()->resizeSection(2, 30);
//    m_ui->tableView->horizontalHeader()->resizeSection(3, 30);


    m_ui->tableView->horizontalHeader()->setFixedHeight(24);
    m_ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_ui->tableView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    m_ui->tableView->setModel(m_proxyModel.data());
    m_proxyModel->setFilterKeyColumn(0);
    m_proxyModel->setFilterRole(Qt::DisplayRole);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
}

NotesList::~NotesList()
{
    delete m_ui;
}

QAbstractItemView *NotesList::view() const {
    return m_ui->tableView;
}

ns_model::Notes *NotesList::notesModel() const {
    return qobject_cast<ns_model::Notes*>(m_proxyModel->sourceModel());
}

void NotesList::resetNotesModel(ns_model::Notes *model) {
    if (m_proxyModel->sourceModel() != model) {
        m_proxyModel->setSourceModel(model);
    }
}

void NotesList::init()
{
    ns_model::Notes* m = notesModel();

    Q_ASSERT(m != nullptr);

    connect(m_ui->input_search, &QLineEdit::textChanged, m_proxyModel.data(), &QSortFilterProxyModel::setFilterFixedString);

    if (!m->read())
    {
        auto handler = [this, m](bool isOk, ns_model::Notes::Item data) {
            if (isOk) {
    //            std::cout << data.toString().toStdString() << std::endl;
                ns_model::Notes::ItemEditData d;
                uint id = m->sourceModel()->index(rand() % m->sourceModel()->rowCount(), 0).data(UserType::RowItemIdRole).value<uint>();
                if (m->sourceModel()->indexOf(id).isValid()) {
                    d.markerId = id;
                }
                auto info = QMetaEnum::fromType<QEvent::Type>();
                d.note = QString("%0::%1::%2").arg(info.scope()).arg(info.name()).arg(data.nickname);
                m->setData(m->indexOf(data.nickname), QVariant::fromValue(d), Qt::EditRole);
            } else {
                //qDebug() << "Not OK!";
            }
        };

        for (int i = 0; i < 12; i++)
        {
            m->createNote(QMetaEnum::fromType<QEvent::Type>().key(i), m->sourceModel()->rowItem(rand()%m->sourceModel()->rowCount()).id, handler);
        }
        m->createNote("nickname", m->sourceModel()->rowItem(rand()%m->sourceModel()->rowCount()).id, handler);

    }
    m_ui->tableView->addAction(m_ui->actioneditItem);
    m_ui->tableView->horizontalHeader()->resizeSection(0, 190);

}

void NotesList::changeEvent(QEvent *e)
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

void NotesList::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    //qDebug() << e->size();
    m_ui->tableView->setFixedWidth(e->size().width());
    m_ui->tableView->horizontalHeader()->resizeSection(0, 190);
    m_ui->tableView->horizontalHeader()->resizeSection(1, e->size().width() - 192);
}
