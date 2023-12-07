//#include <precompiled.h>
#include "notescompactview.h"
#include "markerslisteditor.h"
#include "model_notes.h"
#include "ui_notescompactview.h"
#include "model_markers.h"
#include <QDebug>
#include <QStandardPaths>
#include <qdir.h>
#include <qitemdelegate.h>
#include <qpainter.h>
#include <qpropertyanimation.h>
#include <delegates.h>
#include <qabstractproxymodel.h>




// ----------------------------------------------------------------------------

NotesCompactView::NotesCompactView(QWidget *parent)
    : QDialog(parent)
    , NotesViewInterface()
    , m_ui(new Ui::NotesCompactView)
    , m_addNoteAction(new QAction("add&NoteAction", this))
{
    m_ui->setupUi(this);
}

NotesCompactView::~NotesCompactView()
{
    delete m_ui;
}

void NotesCompactView::init()
{
    NotesViewInterface::init();

    m_ui->notesList->view()->setItemDelegate(new DelegateForCompactView);
    m_ui->notesListEdit->setFixedWidth(width());

    connect(m_ui->tab_markers, &MarkersList::currentChanged, m_ui->notesListEdit, &NotesListEditor::setCurrentMarker);
    connect(m_ui->notesListEdit, &NotesListEditor::currentMarkerChanged, m_ui->tab_markers, &MarkersList::setCurrent);

    connect(m_ui->notesList, &NotesList::currentChanged, m_ui->notesListEdit, &NotesListEditor::displayNote);
    m_ui->notesListEdit->setRespondable(false);

    addAction(m_addNoteAction);
    connect(m_addNoteAction, &QAction::triggered, this, [this]{
        QString nickname = m_addNoteAction->data().toString();
        QModelIndex idx = m_ui->notesList->notesModel()->indexOf(nickname);
        //qDebug() << idx;
        if (!idx.isValid()) {
            idx = m_ui->notesList->notesModel()->createNote(nickname, -1);
        }
        if (idx.isValid()) {
            if (QAbstractProxyModel* m = qobject_cast<QAbstractProxyModel*>(m_ui->notesList->view()->model())) {
                m_ui->notesList->view()->edit(m->mapFromSource(idx));
            }
        }
    });

    m_initialized = true;
}

void NotesCompactView::addNote(const QString &nickname)
{
    m_addNoteAction->setData(nickname);
    m_addNoteAction->trigger();
}

void NotesCompactView::getNotes(const QStringList &nicknames, QMap<QString, QPair<QColor, QString> > &info) const
{
    if (QAbstractProxyModel* m = qobject_cast<QAbstractProxyModel*>(m_ui->notesList->view()->model()))
    {
        info.clear();

        for (const QString& nickname: nicknames)
        {
            ns_model::Notes* model = m_ui->notesList->notesModel();
             QModelIndex idx = model->indexOf(nickname);
             if (idx.isValid()) {
                 info.insert(nickname, qMakePair(model->markerColor(idx), model->note(idx)));
             }
        }
    }
}

void NotesCompactView::changeEvent(QEvent *e)
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

void NotesCompactView::on_notesListEdit_editingStarted()
{
//    m_ui->notesListEdit->setFixedHeight(154);
}
void NotesCompactView::on_notesListEdit_editingFinished()
{
//    if (m_ui->notesListEdit->text())
}


MarkersList *NotesCompactView::markersList() const {
    return m_ui->tab_markers;
}

NotesList *NotesCompactView::notesList() const {
    return m_ui->notesList;
}

MarkersListEditor *NotesCompactView::markersListEditor() const {
    return findChild<MarkersListEditor *>();
}

NotesListEditor *NotesCompactView::notesListEditor() const {
    return m_ui->notesListEdit;
}

void NotesCompactView::resetMarkersList(MarkersList *wgt)
{
}

void NotesCompactView::resetNotesList(NotesList *wgt)
{
}
