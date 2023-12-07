#pragma once

#include <QSortFilterProxyModel>
#include <functional>

namespace ns_model
{
    class FunctionFilter : public QSortFilterProxyModel
    {
        Q_OBJECT

        Q_PROPERTY(int filterKeyRow READ filterKeyRow WRITE setFilterKeyRow)
    public:
        using handler_t = std::function<bool(const QModelIndex& sourceIndex)>;
    private:
        handler_t m_rowFilter;
        handler_t m_columnFilter;
        int m_filterKeyRow;
    public:
        explicit FunctionFilter(QObject *parent = 0);

        virtual void setHandlerForRow(handler_t&& handler);
        virtual void setHandlerForRow(handler_t&& handler, int filterKeyColumn);

        virtual void setHandlerForColumn(handler_t &&handler);
        virtual void setHandlerForColumn(handler_t &&handler, int filterKeyRow);

        void setFilterKeyRow(int key);
        int filterKeyRow() const;
    protected:
        virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
        virtual bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const override;
    };

} //namespace ns_model
