#include "model_transformproxy.h"
#include <QTimer>
#include <tools/hackindex_p.hpp>

//QModelIndex createIndex(int row, int column, void* ptr, const QAbstractItemModel* m) {
//    return HackIndex(row, column, ptr, m);
//}

namespace ns_model
{
    TransformProxy::TransformProxy(QObject *parent)
        : QIdentityProxyModel(parent)
        , m_transposed(false)
    {}

    TransformProxy::TransformProxy(QAbstractItemModel *sourceModel, QObject *parent)
        : QIdentityProxyModel(parent)
        , m_transposed(false)
    {
        QTimer::singleShot(0, [this, sourceModel] { setSourceModel(sourceModel); });
    }

    QVariant TransformProxy::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (m_transposed == (orientation == Qt::Horizontal)) {
            return QIdentityProxyModel::headerData(section, Qt::Vertical, role);
        }
        return QIdentityProxyModel::headerData(section, Qt::Horizontal, role);
    }

    QModelIndex TransformProxy::index(int row, int column, const QModelIndex &parent) const {
        Q_ASSERT(!parent.isValid() || parent.model() == this);
        const QAbstractItemModel* sModel = sourceModel();
        if (!sModel || !hasIndex(row, column, parent)) {
            return QModelIndex();
        }
        const QModelIndex sourceParent = mapToSource(parent);
        const QModelIndex sourceIndex = sModel->index(row, column, sourceParent);
        if (!sourceIndex.isValid()) {
            return QModelIndex();
        }
        return mapFromSource(sourceIndex);
    }
    
    QModelIndex TransformProxy::parent(const QModelIndex &child) const
    {
        return QIdentityProxyModel::parent(child);
    }

    int TransformProxy::rowCount(const QModelIndex &parent) const {
        if (!isTransposed()) {
            return QIdentityProxyModel::rowCount(parent);
        }
        Q_ASSERT(!parent.isValid() || parent.model() == this);
        const QAbstractItemModel* sModel = sourceModel();
        if (!sModel) {
            return 0;
        }
        return sModel->columnCount(mapToSource(parent));
    }

    int TransformProxy::columnCount(const QModelIndex &parent) const {
        if (!isTransposed()) {
            return QIdentityProxyModel::columnCount(parent);
        }
        Q_ASSERT(!parent.isValid() || parent.model() == this);
        const QAbstractItemModel* sModel = sourceModel();
        if (!sModel) {
            return 0;
        }
        return sModel->rowCount(mapToSource(parent));
    }

    void TransformProxy::setTransposed(bool transposed)
    {
        if (m_transposed != transposed) {
            toggleTranspose();
        }
    }

    void TransformProxy::toggleTranspose()
    {
        beginResetModel();
        resetInternalData();
        m_transposed = !m_transposed;
        endResetModel();
    }

    bool TransformProxy::isTransposed() const {
        return m_transposed;
    }

} //namespace ns_model



QModelIndex ns_model::TransformProxy::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!isTransposed()) {
        return QIdentityProxyModel::mapToSource(proxyIndex);
    }
    const QAbstractItemModel* sModel = sourceModel();
    if (!sModel || !proxyIndex.isValid()) {
        return QModelIndex();
    }
    Q_ASSERT(proxyIndex.model() == this);
    return HackIndex(proxyIndex.column(), proxyIndex.row(), proxyIndex.internalPointer(), sModel);
}

QModelIndex ns_model::TransformProxy::mapFromSource(const QModelIndex &sourceIndex) const
{
    if (!isTransposed()) {
        return QIdentityProxyModel::mapFromSource(sourceIndex);
    }
    const QAbstractItemModel* sModel = sourceModel();
    if (!sModel || !sourceIndex.isValid()) {
        return QModelIndex();
    }
    Q_ASSERT(sourceIndex.model() == sModel);
    return createIndex(sourceIndex.column(), sourceIndex.row(), sourceIndex.internalPointer());
}


bool ns_model::TransformProxy::insertRows(int row, int count, const QModelIndex &parent)
{
    if (isTransposed()) {
        return QIdentityProxyModel::insertColumns(row, count, parent);
    }
    return QIdentityProxyModel::insertRows(row, count, parent);
}

bool ns_model::TransformProxy::insertColumns(int column, int count, const QModelIndex &parent)
{
    if (isTransposed()) {
        return QIdentityProxyModel::insertRows(column, count, parent);
    }
    return QIdentityProxyModel::insertColumns(column, count, parent);
}

bool ns_model::TransformProxy::removeRows(int row, int count, const QModelIndex &parent)
{
    if (isTransposed()) {
        return QIdentityProxyModel::removeColumns(row, count, parent);
    }
    return QIdentityProxyModel::removeRows(row, count, parent);
}

bool ns_model::TransformProxy::removeColumns(int column, int count, const QModelIndex &parent)
{
    if (isTransposed()) {
        return QIdentityProxyModel::removeRows(column, count, parent);
    }
    return QIdentityProxyModel::removeColumns(column, count, parent);
}

bool ns_model::TransformProxy::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    if (isTransposed()) {
        return QIdentityProxyModel::moveColumns(sourceParent, sourceRow, count, destinationParent, destinationChild);
    }
    return QIdentityProxyModel::moveRows(sourceParent, sourceRow, count, destinationParent, destinationChild);
}

bool ns_model::TransformProxy::moveColumns(const QModelIndex &sourceParent, int sourceColumn, int count, const QModelIndex &destinationParent, int destinationChild)
{
    if (isTransposed()) {
        return QIdentityProxyModel::moveRows(sourceParent, sourceColumn, count, destinationParent, destinationChild);
    }
    return QIdentityProxyModel::moveColumns(sourceParent, sourceColumn, count, destinationParent, destinationChild);
}
