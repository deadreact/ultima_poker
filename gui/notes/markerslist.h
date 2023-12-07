#pragma once

#include "notes_global.h"
#include <QWidget>
#include <qlistview.h>
#include <qtableview.h>

namespace Ui {
class MarkersList;
}
namespace ns_model {
class Markers;
}

class QIdentityProxyModel;

class NOTES_DLL MarkersListView : public QListView
{
    Q_OBJECT
public:
    MarkersListView(QWidget* parent = nullptr): QListView(parent) {}
protected slots:
    virtual void currentChanged(const QModelIndex &current, const QModelIndex& previous) override;
signals:
    void currentChanged(const QModelIndex &current);
};

class NOTES_DLL MarkersTableView : public QTableView
{
    Q_OBJECT
public:
    MarkersTableView(QWidget* parent = nullptr): QTableView(parent) {}
protected slots:
    virtual void currentChanged(const QModelIndex &current, const QModelIndex& previous) override;
signals:
    void currentChanged(const QModelIndex &current);
};


class NOTES_DLL MarkersList : public QWidget
{
    Q_OBJECT

private:
    Ui::MarkersList *m_ui;
    QScopedPointer<QIdentityProxyModel> m_model;
public:
    explicit MarkersList(QWidget *parent = nullptr);
    virtual ~MarkersList();

    virtual void init();
    ns_model::Markers* markersModel() const;
    void resetMarkersModel(ns_model::Markers* m);
protected:
    void changeEvent(QEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;
private slots:
    void on_bttn_up_clicked();
    void on_bttn_down_clicked();
    void on_listView_markers_currentChanged(const QModelIndex& idx);
public slots:
    void setCurrent(const QModelIndex& idx);
signals:
    void currentChanged(const QModelIndex& idx);
};

