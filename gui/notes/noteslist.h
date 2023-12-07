#pragma once

#include "notes_global.h"
#include <QWidget>
#include <QTableView>
#include <qheaderview.h>

namespace Ui {
class NotesList;
}
namespace ns_model {
class Notes;
}
class QSortFilterProxyModel;

class NOTES_DLL NotesTableView : public QTableView
{
    Q_OBJECT
public:
    NotesTableView(QWidget* parent = nullptr);
public slots:
    inline void editCurrent() { edit(currentIndex().sibling(currentIndex().row(), 0)); }
protected slots:
    virtual void currentChanged(const QModelIndex &current, const QModelIndex& previous) override;
signals:
    void currentChanged(const QModelIndex &current);
};



class NOTES_DLL NotesList : public QWidget
{
    Q_OBJECT
private:
    Ui::NotesList *m_ui;
    QScopedPointer<QSortFilterProxyModel> m_proxyModel;
public:
    explicit NotesList(QWidget *parent = nullptr);
    virtual ~NotesList();

    QAbstractItemView* view() const;

    ns_model::Notes* notesModel() const;
    void resetNotesModel(ns_model::Notes* notesModel);
    virtual void init();
protected:
    virtual void changeEvent(QEvent *e) override;
    virtual void resizeEvent(QResizeEvent *e) override;
signals:
    void currentChanged(const QModelIndex&);

};

