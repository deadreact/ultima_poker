#pragma once

#include <QFrame>
#include <QPushButton>
#include <qcombobox.h>
#include <utils/IconProvider.h>
#include <basewidgets/types.h>
#include <QTableView>

class TableView : public QTableView
{
    Q_OBJECT
public:
    TableView(QWidget* parent = nullptr);
    Q_SIGNAL void currentChanged(const QModelIndex& index);
protected:
    virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;
};

namespace ns_model {
    class ApiBase;
}
class QSortFilterProxyModel;

typedef QList<UserType::EFilters> FiltersList;

namespace ns_page
{
    class MainLobbyBase : public QFrame
    {
        Q_OBJECT
    protected:
        const UserType::FilterFlags m_mask;
        UserType::FilterFlags m_filter;

        gbp_i64 m_currentId;
    public:
        explicit MainLobbyBase(UserType::FilterFlags mask, QWidget *parent = nullptr);
        virtual ~MainLobbyBase();

        virtual void init();
        virtual void initTableView() = 0;
        virtual TableView* tableView() const = 0;
        virtual void initFilterBox(QComboBox* box, const QStringList& txtlst, const FiltersList& filters);

        QSortFilterProxyModel* proxyModel() const;
        QAbstractItemModel* sourceModel() const;

        void resetFilter();
        void setFilter(UserType::EFilters mask);
        void setFilter(UserType::FilterFlags mask);
    protected:
        virtual void updateFilters() = 0;
    protected slots:
        virtual void onItemActivated(const QModelIndex&) = 0;
        virtual void onCurrentChanged(const QModelIndex&) = 0;
        virtual void disableFilters(bool disable) = 0;
    };

}// namespace page

