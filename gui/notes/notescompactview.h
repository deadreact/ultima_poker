#pragma once

#include "notes_global.h"
#include <QDialog>
#include "notesview.h"

namespace Ui {
class NotesCompactView;
}

class NOTES_DLL NotesCompactView : public QDialog, public NotesViewInterface
{
    Q_OBJECT
private:
    Ui::NotesCompactView *m_ui;
    QAction* m_addNoteAction;
public:
    explicit NotesCompactView(QWidget *parent = nullptr);
    virtual ~NotesCompactView();

    virtual void init();
    void addNote(const QString& nickname);
    void getNotes(const QStringList& nicknames, QMap<QString, QPair<QColor, QString>>& info) const;
protected:
    void changeEvent(QEvent *e);

private slots:
    void on_notesListEdit_editingStarted();
    void on_notesListEdit_editingFinished();

    // NotesViewInterface interface
public:
    virtual MarkersList *markersList() const override;
    virtual NotesList *notesList() const override;
    virtual MarkersListEditor *markersListEditor() const override;
    virtual NotesListEditor *notesListEditor() const override;

    virtual void resetMarkersList(MarkersList *wgt) override;
    virtual void resetNotesList(NotesList *wgt) override;
};

