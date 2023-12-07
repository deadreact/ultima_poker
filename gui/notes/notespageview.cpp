//#include <precompiled.h>
#include "notespageview.h"
#include <qpropertyanimation.h>
#include "markerslisteditor.h"
#include "ui_notespageview.h"
#include "delegates.h"

NotesPageView::NotesPageView(QWidget *parent)
    : QWidget(parent)
    , NotesViewInterface()
    , m_ui(new Ui::NotesPageView)
    , m_useAnimation(false)
    , m_normalizedProgress(0)
{
    m_ui->setupUi(this);
}

NotesPageView::~NotesPageView()
{
    delete m_ui;
}

void NotesPageView::init()
{
    NotesViewInterface::init();

    m_ui->notesListEdit->setHeightForEditing(418);
    m_ui->notesListEdit->setHeightForDisplaying(418);
    m_ui->notesListEdit->setTextEditorAvailableHeight(192, 192);
    m_ui->notesListEdit->setHideOnEditingFinished(true);

    DelegateForPageView* delegate = new DelegateForPageView;

    m_ui->notesList->view()->setItemDelegate(delegate);
    connect(delegate, &DelegateForPageView::editingStarted, this, [this]{
        showNoteEditor(true);
    });
    connect(delegate, &DelegateForPageView::editingFinished, this, [this]{
        showNoteEditor(false);
    });

    connect(m_ui->markers, &MarkersList::currentChanged, m_ui->notesListEdit, &NotesListEditor::setCurrentMarker);
    connect(m_ui->notesListEdit, &NotesListEditor::currentMarkerChanged, m_ui->markers, &MarkersList::setCurrent);
    connect(m_ui->notesListEdit, &NotesListEditor::editingFinished, m_ui->notesListEdit, &NotesListEditor::hide);

//    connect(m_ui->notesList, &NotesList::currentChanged, m_ui->notesListEdit, &NotesListEditor::displayNote);

    showNoteEditor(false);
    m_ui->notesListEdit->setParentPainting(E::IgnoreParentPainting);


    m_useAnimation = true;
    m_initialized = true;
}

void NotesPageView::changeEvent(QEvent *e)
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

MarkersList *NotesPageView::markersList() const {
    return m_ui->markers;
}

NotesList *NotesPageView::notesList() const {
    return m_ui->notesList;
}

MarkersListEditor *NotesPageView::markersListEditor() const {
    return findChild<MarkersListEditor *>();
}

NotesListEditor *NotesPageView::notesListEditor() const {
    return m_ui->notesListEdit;
}

void NotesPageView::resetMarkersList(MarkersList *wgt)
{

}

void NotesPageView::resetNotesList(NotesList *wgt)
{
}

void NotesPageView::showNoteEditor(bool show)
{
    if (m_ui->notesListEdit->isHidden() != show) {
        return;
    }
    m_ui->notesListEdit->setRespondable(show);


    if (m_useAnimation)
    {
        m_ui->notesListEdit->setVisible(true);
        m_normalizedProgress = 0;
        QPropertyAnimation* anim = new QPropertyAnimation(this, "normalizedProgress");
        anim->setStartValue(0);
        anim->setEndValue(1);
        anim->setEasingCurve(QEasingCurve::InSine);
        anim->setDuration(100);
        anim->setDirection(show ? QAbstractAnimation::Backward : QAbstractAnimation::Forward);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
        if (!show) {
            connect(anim, &QPropertyAnimation::finished, m_ui->notesListEdit, &NotesListEditor::hide);
        }
    }
    else
    {
        m_ui->notesListEdit->setVisible(show);
        m_ui->subpage_colorMarks->setFixedWidth(show ? 320 : 476);
        m_ui->subpage_notesAboutPlayers->setFixedWidth(show ? 640 : 476);
    }
}

void NotesPageView::setNormalizedProgress(qreal t)
{
    if (!qFuzzyCompare(t, m_normalizedProgress)) {
        m_normalizedProgress = t;

        m_ui->notesListEdit->setFixedWidth(320*(1. - t) + 0*t);
        m_ui->subpage_colorMarks->setFixedWidth(320*(1. - t) + 476*t);
        m_ui->subpage_notesAboutPlayers->setFixedWidth(640*(1. - t) + 476*t);
    }
}
