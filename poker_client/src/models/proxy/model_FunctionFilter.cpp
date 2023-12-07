#include <precompiled.h>
#include "model_FunctionFilter.h"

namespace ns_model
{
    FunctionFilter::FunctionFilter(QObject *parent)
        : QSortFilterProxyModel(parent)
        , m_filterKeyRow(0)
    {}

    void FunctionFilter::setHandlerForRow(handler_t&& handler)
    {
        m_rowFilter = std::move(handler);
        invalidate();
    }

    void FunctionFilter::setHandlerForRow(handler_t &&handler, int filterKeyColumn)
    {
        setFilterKeyColumn(filterKeyColumn);
        setHandlerForRow(std::move(handler));
    }

    void FunctionFilter::setHandlerForColumn(handler_t&& handler)
    {
        m_columnFilter = std::move(handler);
        invalidate();
    }

    void FunctionFilter::setHandlerForColumn(handler_t &&handler, int filterKeyRow)
    {
        setFilterKeyRow(filterKeyRow);
        setHandlerForColumn(std::move(handler));
    }

    void FunctionFilter::setFilterKeyRow(int key)
    {
        if (m_filterKeyRow != key) {
            m_filterKeyRow = key;
            invalidate();
        }
    }

    int FunctionFilter::filterKeyRow() const
    {
        return m_filterKeyRow;
    }

    bool FunctionFilter::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
    {
        if (m_rowFilter)
        {
            QModelIndex modelIndex = sourceModel()->index(source_row, filterKeyColumn(), source_parent);
            return m_rowFilter(modelIndex);
        }
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }

    bool FunctionFilter::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
    {
        if (m_columnFilter)
        {
            QModelIndex modelIndex = sourceModel()->index(filterKeyRow(), source_column, source_parent);
            return m_columnFilter(modelIndex);
        }
        return QSortFilterProxyModel::filterAcceptsColumn(source_column, source_parent);
    }

} //namespace ns_model



