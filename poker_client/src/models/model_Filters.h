#pragma once

#include <QStandardItemModel>
#include <QAbstractProxyModel>
#include <basewidgets/types.h>
#include <qsortfilterproxymodel.h>

namespace ns_model
{

    class FiltersItem : public QStandardItem
    {
    private:
        UserType::FilterFlags m_filter;
        UserType::FilterFlags m_mask;
        bool m_isCheckedByUser;
    public:
        FiltersItem(UserType::FilterFlags mask, const QString& text);
        FiltersItem(const FiltersItem& item);
        virtual QStandardItem *clone() const override;
        FiltersItem* child(int row, int column = 0) const { return dynamic_cast<FiltersItem*>(QStandardItem::child(row, column)); }
        FiltersItem* parent() const { return dynamic_cast<FiltersItem*>(QStandardItem::parent()); }

        UserType::FilterFlags filter() const { return data(UserType::FilterRole).value<UserType::FilterFlags>(); }
        void setFilter(UserType::FilterFlags f) {
            setData(QVariant::fromValue(f), UserType::FilterRole);
        }

        virtual QVariant data(int role) const override
        {
            if (role == Qt::DisplayRole) {
                return QString("(%0/%1)").arg(QString::number(filter(), 16)).arg(QString::number(m_mask, 16));
            }
            return QStandardItem::data(role);
        }
        virtual void setData(const QVariant& value, int role) override
        {
            QStandardItem::setData(value, role);
            if (m_isCheckedByUser) {
                if (role == Qt::CheckStateRole) {
                    onUserCheckStateChanged();
                } else if (role == UserType::FilterRole) {
                    onUserFilterChanged();
                }
            }
            if (role == Qt::CheckStateRole) {
                if (checkState() == Qt::Checked) {
                    setFilter(m_mask);
                } else if (checkState() == Qt::Unchecked) {
                    setFilter(0x0);
                } else {
                    updateFilterFromChildren();
                }
            }
        }

        void onUserCheckStateChanged() {
            if (hasChildren()) {
                for (int i = 0; i < rowCount(); i++) {
                    child(i)->updateCheckStateFromParent();
                }
            }
            FiltersItem* root = this;
            for (;root->parent() != nullptr; root = root->parent());
            root->updateCheckStateFromChildren();
        }

        void onUserFilterChanged() {
            if (hasChildren()) {
                for (int i = 0; i < rowCount(); i++) {
                    child(i)->updateFilterFromParent();
                }
            }
            FiltersItem* root = this;
            for (;root->parent() != nullptr; root = root->parent());
            root->updateFilterFromChildren();
        }

        void updateCheckStateFromParent() {
            if (FiltersItem* p = parent()) {
                if (p->checkState() != Qt::PartiallyChecked) {
                    m_isCheckedByUser = false;
                    setCheckState(p->checkState());
                    m_isCheckedByUser = true;
                    if (hasChildren()) {
                        for (int i = 0; i < rowCount(); i++) {
                            child(i)->updateCheckStateFromParent();
                        }
                    }
                }
            }
        }
        void updateFilterFromParent() {
            if (FiltersItem* p = parent()) {
                m_isCheckedByUser = false;
                setFilter(p->filter() & filter());
                m_isCheckedByUser = true;
                if (hasChildren()) {
                    for (int i = 0; i < rowCount(); i++) {
                        child(i)->updateFilterFromParent();
                    }
                }
            }
        }

        void updateCheckStateFromChildren() {
            if (hasChildren()) {
                bool childChecked = false;
                bool childUnckecked = false;
                for (int i = 0; i < rowCount(); i++) {
                    child(i)->updateCheckStateFromChildren();
                    if (child(i)->checkState() == Qt::Unchecked) {
                        childUnckecked = true;
                    } else if (child(i)->checkState() == Qt::Checked) {
                        childChecked = true;
                    } else {
                        childUnckecked = true;
                        childChecked = true;
                    }
                }
                m_isCheckedByUser = false;
                if (childChecked) {
                    setCheckState(childUnckecked ? Qt::PartiallyChecked : Qt::Checked);
                } else {
                    setCheckState(Qt::Unchecked);
                }
                m_isCheckedByUser = true;
            }
        }

        void updateFilterFromChildren() {
            if (hasChildren()) {
                ::UserType::FilterFlags mask;
                for (int i = 0; i < rowCount(); i++) {
                    child(i)->updateFilterFromChildren();
                    mask |= child(i)->filter();
                }
                m_isCheckedByUser = false;
                setFilter(mask);
                m_isCheckedByUser = true;
            }
        }
    };

    class Filters : public QStandardItemModel
    {
        Q_OBJECT
    private:
        uint m_checkState;
        FiltersItem* m_rootItem;
    public:
        explicit Filters(const QStringList &names, QObject *parent = nullptr);

        FiltersItem* rootItem() const;
        void updateFilter();
        // QAbstractItemModel interface
    public:
    };


    class FilterBox : public QSortFilterProxyModel
    {
        Q_OBJECT

        const QList<UserType::FilterFlags> m_items;
        UserType::FilterFlags m_mask;
    public:
        FilterBox(int page, int boxIndex, Filters* sourceModel, QObject* parent = nullptr);

        static QList<UserType::FilterFlags> getInitialData(int page, int boxIndex);

        UserType::FilterFlags mask() const { return m_mask; }

        Qt::CheckState checkState(UserType::FilterFlags f) const;
    protected:
        virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    };




} // namespace ns_model
