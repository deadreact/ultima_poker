#pragma once
#include <QWidget>
#include <qtableview.h>

namespace Ui {
class Widget;
}

class TableView : public QTableView
{
    Q_OBJECT
public:
    TableView(QWidget* parent = nullptr)
        : QTableView(parent)
    {}
    virtual ~TableView() {}
    Q_SIGNAL void currentChanged(const QModelIndex&);
protected:
    void currentChanged(const QModelIndex& curr, const QModelIndex& prev) {
        QTableView::currentChanged(curr, prev);
        emit currentChanged(curr);
    }
};

class Widget : public QWidget
{
    Q_OBJECT

private:
    Ui::Widget *m_ui;
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_tableView_currentChanged(const QModelIndex &index);
    void on_tableView_2_currentChanged(const QModelIndex &index);
    void on_tableView_4_currentChanged(const QModelIndex &);
};
