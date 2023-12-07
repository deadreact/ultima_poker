#include <precompiled.h>
#include <qstringlistmodel.h>
#include "model_modelcontainer.h"



bool operator ==(const InternalData& d1, const InternalData& d2)
{
    return d1.first == d2.first && d1.second.size() == d2.second.size() && d1.second == d2.second;
}

void* insertInternalData(const InternalData& data, QList<InternalData>& lst)
{
    for (InternalData& existingData: lst) {
        if (existingData == data) {
            return &existingData;
        }
    }
    lst.push_back(data);
    return &lst.last();
}


namespace ns_model
{
    ModelContainer::ModelContainer(QObject *parent)
        : QIdentityProxyModel(parent)
    {}



 
    void ModelContainer::on_sourceRowsAboutToBeInserted(const QModelIndex& parent, int first, int last) {
        beginInsertRows(mapFromSource(parent), first, last);
    }
    void ModelContainer::on_sourceRowsInserted(const QModelIndex&, int, int) {
        endInsertRows();
    }
    void ModelContainer::on_sourceRowsAboutToBeRemoved(const QModelIndex& parent, int first, int last) {
        beginRemoveRows(mapFromSource(parent), first, last);
    }
    void ModelContainer::on_sourceRowsRemoved(const QModelIndex&, int, int) {
        endRemoveRows();
    }
    void ModelContainer::on_sourceRowsAboutToBeMoved(const QModelIndex& sourceParent, int sourceFirst, int sourceLast, const QModelIndex& destinationParent, int destinationRow) {
        beginMoveRows(mapFromSource(sourceParent), sourceFirst, sourceLast, mapFromSource(destinationParent), destinationRow);
    }
    void ModelContainer::on_sourceRowsMoved(const QModelIndex&, int, int count,QModelIndex,int) {
        endMoveRows();
    }
    void ModelContainer::on_sourceColumnsAboutToBeInserted(const QModelIndex& parent, int first, int last) {
        beginInsertColumns(mapFromSource(parent), first, last);
    }
    void ModelContainer::on_sourceColumnsInserted(const QModelIndex&, int, int) {
        endInsertColumns();
    }
    void ModelContainer::on_sourceColumnsAboutToBeRemoved(const QModelIndex& parent, int first, int last) {
        beginRemoveColumns(mapFromSource(parent), first, last);
    }
    void ModelContainer::on_sourceColumnsRemoved(const QModelIndex&, int, int) {
        endRemoveColumns();
    }
    void ModelContainer::on_sourceColumnsAboutToBeMoved(const QModelIndex& sourceParent, int sourceFirst, int sourceLast, const QModelIndex& destinationParent, int destinationRow) {
        beginMoveColumns(mapFromSource(sourceParent), sourceFirst, sourceLast, mapFromSource(destinationParent), destinationRow);
    }
    void ModelContainer::on_sourceColumnsMoved(const QModelIndex&, int, int,QModelIndex,int) {
        endMoveColumns();
    }
    void ModelContainer::on_sourceDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles) {
        emit dataChanged(mapFromSource(topLeft), mapFromSource(bottomRight), roles);
    }
    void ModelContainer::on_sourceHeaderDataChanged(Qt::Orientation orientation, int first, int last) {
        emit headerDataChanged(orientation, first, last);
    }

    void ModelContainer::on_sourceModelAboutToBeReset() {
        beginResetModel();
    }
    void ModelContainer::on_sourceModelReset() {
        endResetModel();
    }

} //namespace ns_model


QModelIndex ns_model::ModelContainer::index(int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        if (column != 0 || row >= m_models.size()) {
            return QModelIndex();
        }
        return createIndex(row, column, nullptr);
    }
    InternalData* internalData =  static_cast<InternalData*>(parent.internalPointer());
    if (internalData == nullptr) {
        if (column != 0) {
            return QModelIndex();
        }
        InternalData d;
        d.first = m_models[row];

        return createIndex(row, column, insertInternalData(d, m_internal));
    }
    InternalData d(*internalData);
    d.second.push_back(QPoint(parent.row(), parent.column()));

    return createIndex(row, column, insertInternalData(d, m_internal));
}

QModelIndex ns_model::ModelContainer::indexOf(const QAbstractItemModel *m, const QList<QPoint>& parentInfo) const
{
    if (!m_models.contains((QAbstractItemModel*)m)) {
        return QModelIndex();
    }
    if (parentInfo.isEmpty()) {
        return index(m_models.indexOf((QAbstractItemModel*)m), 0);
    }
    QList<QPoint> info = parentInfo;
    QPoint coord = info.takeLast();
    return index(coord.x(), coord.y(), indexOf(m, info));
}

void ns_model::ModelContainer::push_back(QAbstractItemModel *model)
{
    if (insertRow(m_models.size())) {
        connectTo(model);
        m_models.push_back(model);
    }
}

void ns_model::ModelContainer::pop_back()
{
    if (m_models.isEmpty()) {
        return;
    }
    beginRemoveRows(QModelIndex(), m_models.size()-1, m_models.size()-1);
    const QAbstractItemModel* model = m_models.takeLast();
    disconnectFrom(model);
    for (auto it = m_internal.begin(); it != m_internal.end(); it++) {
        if (it->first == model) {
            m_internal.erase(it);
        }
    }
    endInsertRows();
}

void ns_model::ModelContainer::connectTo(QAbstractItemModel *model)
{
    if (model == nullptr) {
        return;
    }
    connect(model, &QAbstractItemModel::dataChanged,              this, &ModelContainer::on_sourceDataChanged);
    connect(model, &QAbstractItemModel::headerDataChanged,        this, &ModelContainer::on_sourceHeaderDataChanged);
    connect(model, &QAbstractItemModel::rowsAboutToBeInserted,    this, &ModelContainer::on_sourceRowsAboutToBeInserted);
    connect(model, &QAbstractItemModel::rowsInserted,             this, &ModelContainer::on_sourceRowsInserted);
    connect(model, &QAbstractItemModel::rowsAboutToBeRemoved,     this, &ModelContainer::on_sourceRowsAboutToBeRemoved);
    connect(model, &QAbstractItemModel::rowsRemoved,              this, &ModelContainer::on_sourceRowsRemoved);
    connect(model, &QAbstractItemModel::columnsAboutToBeInserted, this, &ModelContainer::on_sourceColumnsAboutToBeInserted);
    connect(model, &QAbstractItemModel::columnsInserted,          this, &ModelContainer::on_sourceColumnsInserted);
    connect(model, &QAbstractItemModel::columnsAboutToBeRemoved,  this, &ModelContainer::on_sourceColumnsAboutToBeRemoved);
    connect(model, &QAbstractItemModel::columnsRemoved,           this, &ModelContainer::on_sourceColumnsRemoved);
    connect(model, &QAbstractItemModel::modelAboutToBeReset,      this, &ModelContainer::on_sourceModelAboutToBeReset);
    connect(model, &QAbstractItemModel::modelReset,               this, &ModelContainer::on_sourceModelReset);
    connect(model, &QAbstractItemModel::rowsAboutToBeMoved,       this, &ModelContainer::on_sourceRowsAboutToBeMoved);
    connect(model, &QAbstractItemModel::rowsMoved,                this, &ModelContainer::on_sourceRowsMoved);
    connect(model, &QAbstractItemModel::columnsAboutToBeMoved,    this, &ModelContainer::on_sourceColumnsAboutToBeMoved);  
    connect(model, &QAbstractItemModel::columnsMoved,             this, &ModelContainer::on_sourceColumnsMoved);

}

void ns_model::ModelContainer::disconnectFrom(const QAbstractItemModel *model)
{
    if (model == nullptr) {
        return;
    }
    disconnect(model, &QAbstractItemModel::dataChanged,              this, &ModelContainer::on_sourceDataChanged);
    disconnect(model, &QAbstractItemModel::headerDataChanged,        this, &ModelContainer::on_sourceHeaderDataChanged);
    disconnect(model, &QAbstractItemModel::rowsAboutToBeInserted,    this, &ModelContainer::on_sourceRowsAboutToBeInserted);
    disconnect(model, &QAbstractItemModel::rowsInserted,             this, &ModelContainer::on_sourceRowsInserted);
    disconnect(model, &QAbstractItemModel::rowsAboutToBeRemoved,     this, &ModelContainer::on_sourceRowsAboutToBeRemoved);
    disconnect(model, &QAbstractItemModel::rowsRemoved,              this, &ModelContainer::on_sourceRowsRemoved);
    disconnect(model, &QAbstractItemModel::columnsAboutToBeInserted, this, &ModelContainer::on_sourceColumnsAboutToBeInserted);
    disconnect(model, &QAbstractItemModel::columnsInserted,          this, &ModelContainer::on_sourceColumnsInserted);
    disconnect(model, &QAbstractItemModel::columnsAboutToBeRemoved,  this, &ModelContainer::on_sourceColumnsAboutToBeRemoved);
    disconnect(model, &QAbstractItemModel::columnsRemoved,           this, &ModelContainer::on_sourceColumnsRemoved);
    disconnect(model, &QAbstractItemModel::modelAboutToBeReset,      this, &ModelContainer::on_sourceModelAboutToBeReset);
    disconnect(model, &QAbstractItemModel::modelReset,               this, &ModelContainer::on_sourceModelReset);
    disconnect(model, &QAbstractItemModel::rowsAboutToBeMoved,       this, &ModelContainer::on_sourceRowsAboutToBeMoved);
    disconnect(model, &QAbstractItemModel::rowsMoved,                this, &ModelContainer::on_sourceRowsMoved);
    disconnect(model, &QAbstractItemModel::columnsAboutToBeMoved,    this, &ModelContainer::on_sourceColumnsAboutToBeMoved);
    disconnect(model, &QAbstractItemModel::columnsMoved,             this, &ModelContainer::on_sourceColumnsMoved);
}

QModelIndex ns_model::ModelContainer::parent(const QModelIndex &child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }
    if (child.internalPointer() == nullptr) {
        return QModelIndex();
    }
    InternalData* internalData = static_cast<InternalData*>(child.internalPointer());
    const QAbstractItemModel* model = internalData->first;
    QList<QPoint> parentInfo = internalData->second;

    return indexOf(model, parentInfo);
}

bool ns_model::ModelContainer::hasChildren(const QModelIndex &parent) const
{
    if (!parent.isValid() || (!parent.parent().isValid() && parent.internalPointer() == nullptr)) {
        return true;
    }

    InternalData* internalData = static_cast<InternalData*>(parent.internalPointer());
    const QAbstractItemModel* model = internalData->first;
    if (!m_models.contains((QAbstractItemModel*)model)) {
        return false;
    }
    return model->hasChildren(mapToSource(parent));
}


int ns_model::ModelContainer::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return m_models.size();
    }
    if (!parent.parent().isValid()) {
        const QAbstractItemModel* m = m_models.at(parent.row());
        return m->rowCount();
    }
    QModelIndex idx = mapToSource(parent);
    return idx.model()->rowCount(idx.parent());
}

int ns_model::ModelContainer::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return 1;
    }
    if (!parent.parent().isValid()) {
        const QAbstractItemModel* m = m_models.at(parent.row());
        return m->columnCount();
    }
    QModelIndex idx = mapToSource(parent);
    return idx.model()->rowCount(idx.parent());
}

void ns_model::ModelContainer::setSourceModel(QAbstractItemModel *sourceModel)
{
    if (!m_models.contains(sourceModel)) {
        beginInsertRows(QModelIndex(), m_models.size(), m_models.size());
        m_models.push_back(sourceModel);
        endInsertRows();
    }
}

QModelIndex ns_model::ModelContainer::mapToSource(const QModelIndex &proxyIndex) const
{
    if (proxyIndex.model() != this) {
        return QModelIndex();
    }
    if (!proxyIndex.isValid() || (proxyIndex.internalPointer() == nullptr)) {
        return QModelIndex();
    }

    InternalData* internalData = static_cast<InternalData*>(proxyIndex.internalPointer());
    const QAbstractItemModel* model = internalData->first;
    if (model == nullptr || !m_models.contains((QAbstractItemModel*)model)) {
        return QModelIndex();
    }
    QList<QPoint> parentInfo = internalData->second;
    if (parentInfo.isEmpty()) {
        return model->index(proxyIndex.row(), proxyIndex.column());
    }
    return model->index(proxyIndex.row(), proxyIndex.column(), mapToSource(indexOf(model, parentInfo)));
}

QModelIndex ns_model::ModelContainer::mapFromSource(const QModelIndex &sourceIndex) const
{
    if (!sourceIndex.isValid()) {
        return QModelIndex();
    }
    if (!m_models.contains((QAbstractItemModel*)sourceIndex.model())) {
        return QModelIndex();
    }
    if (!sourceIndex.parent().isValid()) {
        return indexOf(sourceIndex.model());
    }
    QList<QPoint> parentInfo;
    QModelIndex sourceParent = sourceIndex;
    while (sourceParent.isValid()) {
        parentInfo.push_back(QPoint(sourceParent.row(), sourceParent.column()));
        sourceParent = sourceParent.parent();
    }
    return indexOf(sourceIndex.model(), parentInfo);
}

bool ns_model::ModelContainer::insertRows(int row, int count, const QModelIndex &parent)
{
    if (!parent.isValid())
    {
        static QStringListModel capModel;
        beginInsertRows(parent, row, row + count - 1);
        for (;count > 0; row++, count--) {
            m_models.insert(row, &capModel);
        }
        endInsertRows();
        return true;
    }
    if (parent.internalPointer() == nullptr) {
        if (parent.column() != 0 || m_models.size() >= parent.row()) {
            return false;
        }
        return m_models[parent.row()]->insertRows(row, count, mapToSource(parent));
    }
    return static_cast<InternalData*>(parent.internalPointer())->first->insertRows(row, count, mapToSource(parent));
}

QVariant ns_model::ModelContainer::data(const QModelIndex &proxyIndex, int role) const
{
    if (!proxyIndex.isValid()) {
        return QVariant();
    }
    QModelIndex sourceIndex = mapToSource(proxyIndex);
    if (sourceIndex.isValid()) {
        return sourceIndex.data(role);
    }
    return m_models.at(proxyIndex.row())->metaObject()->className();
}
