#pragma once

#include "notes_global.h"
#include <QWidget>
#include "notesview.h"

namespace Ui {
class NotesPageView;
}

class NOTES_DLL NotesPageView : public QWidget, public NotesViewInterface
{
    Q_OBJECT
    Q_PROPERTY(qreal normalizedProgress MEMBER m_normalizedProgress WRITE setNormalizedProgress)
private:
    Ui::NotesPageView *m_ui;
    bool m_useAnimation;
    qreal m_normalizedProgress;
public:
    explicit NotesPageView(QWidget *parent = nullptr);
    virtual ~NotesPageView();

    virtual void init() override;
protected:
    void changeEvent(QEvent *e);
public:
    virtual MarkersList *markersList() const override;
    virtual NotesList *notesList() const override;
    virtual MarkersListEditor *markersListEditor() const override;
    virtual NotesListEditor *notesListEditor() const override;

    virtual void resetMarkersList(MarkersList *wgt) override;
    virtual void resetNotesList(NotesList *wgt) override;
public slots:
    void showNoteEditor(bool show);
protected slots:
    void setNormalizedProgress(qreal t);
};

