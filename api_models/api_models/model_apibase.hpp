#pragma once

#include <tools/types_fwd.hpp>
#include <tools/math_tools.hpp>

#include <QAbstractItemModel>
#include <QSet>
#include <basewidgets/types.h>
#include <functional>
#include <iostream>
#include "model_io.hpp"
#include <QDebug>
#include <qfile.h>
#include "visitors.hpp"
#include "model_log.h"
#include "api_util.hpp"
#include <qstringbuilder.h>

#define MAKE_FILELINE __FILE__ % QStringLiteral(":") % QString::number(__LINE__)

//template <typename T>
//struct IdGetter
//{
//    using type = IdGetter<T>;
//    using id_type = gbp_i64;

//    static const id_type invalidId = -1;

//    static id_type get_id(const T& v) {
//        return v._id;
//    }


//    using iterator       = typename std::vector<T>::iterator;
//    using const_iterator = typename std::vector<T>::const_iterator;

//    static const_iterator find_id(const std::vector<T>& items, id_type id)
//    {
//        for (auto it = items.begin(); it != items.end(); it++) {
//            if (get_id(*it) == id) {
//                return it;
//            }
//        }
//        return items.end();
//    }

//    static iterator find_id(std::vector<T>& items, id_type id)
//    {
//        for (auto it = items.begin(); it != items.end(); it++) {
//            if (get_id(*it) == id) {
//                return it;
//            }
//        }
//        return items.end();
//    }
//};

template <>
struct IdGetter<gbp_i8>
{
    using type = IdGetter<gbp_i8>;
    using id_type = gbp_i8;
    static const id_type& get_id(const gbp_i8& v) {
        return v;
    }
};
template <>
struct IdGetter<gbp_i64>
{
    using type = IdGetter<gbp_i64>;
    using id_type = gbp_i64;
    static const id_type& get_id(const gbp_i64& v) {
        return v;
    }
};

//template <typename ItemType, typename IdType = gbp_i64>
//IdType get_id(const ItemType& v) {
//    return IdGetter<ItemType>::get_id(v);
//}


template <typename T1>
struct id_comparator {
    const T1 &value;
    id_comparator(const T1& value): value(value) {}
    template <typename T2>
    bool id(const T2& id) {
        return value._id == id;
    }
};
template <>
struct id_comparator<gbp_i8> {
    gbp_i8 value;
    id_comparator(const gbp_i8& value): value(value) {}

    bool id(const gbp_i8& id) {
        return value == id;
    }
};
template <>
struct id_comparator<gbp_i64> {
    gbp_i64 value;
    id_comparator(const gbp_i64& value): value(value) {}

    bool id(const gbp_i64& id) {
        return value == id;
    }
};

template <typename T>
id_comparator<T> cmp(const T& v) {
    return id_comparator<T>(v);
}


template <typename ContainerType, typename ItemType = typename ContainerType::value_type, typename IdType = gbp_i64>
typename ContainerType::iterator find_id(ContainerType& vec, IdType id) {
    for (auto it = vec.begin(); it != vec.end(); it++) {
        if (cmp(*it).id(id)) {
            return it;
        }
    }
    return vec.end();
}
template <typename ContainerType, typename ItemType = typename ContainerType::value_type, typename IdType = gbp_i64>
typename ContainerType::const_iterator find_id(const ContainerType& vec, IdType id) {
    for (auto it = vec.begin(); it != vec.end(); it++) {
        if (cmp(*it).id(id)) {
            return it;
        }
    }
    return vec.end();
}


namespace ns_model
{
    class ApiBase : public QAbstractItemModel, public IOModel
    {
        Q_OBJECT
    protected:
        LogModel* m_logModel;
    public:
        void resetLogModel(LogModel* logModel = nullptr) {
            m_logModel = logModel;
        }
    public:
        template <typename value_t> using on_get_id = std::function<gbp_i64(const value_t&)>;
    protected:
        mutable QMap<QByteArray/*key*/, QByteArray*/*internal data*/> m_internal;
        QList<const void*> m_waitForInserting;
    public:
        explicit ApiBase(QObject *parent = nullptr);
        virtual ~ApiBase();


        virtual QByteArray indexToKey(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QByteArray indexToKey(const QModelIndex& idx) const;

        static QByteArray anyIndexToKey(const QModelIndex& idx);

        virtual QModelIndex keyToIndex(const QByteArray& key) const;
        virtual bool isItemOrientedData() const = 0;

        virtual int rowById(gbp_i64 id, const void* data = nullptr, const QModelIndex& parent = QModelIndex()) const = 0;
        virtual int propertyIndex(const QByteArray &name, const QModelIndex& parent = QModelIndex()) const = 0;

        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        virtual QModelIndex parent(const QModelIndex &index) const override;

        template <typename T>
        QModelIndex parentOf(const T& data = T()) const {
            Q_UNUSED(data)
            return QModelIndex();
        }

        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        bool backup(const QString& filename) const;
        bool restore(const QString& filename);

        gbp_i64 idAt(int row, const QModelIndex& parent = QModelIndex()) const;
        gbp_i64 idAt(const QModelIndex& index) const;

        virtual bool removeItems(const QList<gbp_i64>& ids, const QModelIndex& parent = QModelIndex()) = 0;
        virtual bool insertItems(const QList<gbp_i64>& ids, const QModelIndex& parent = QModelIndex()) = 0;

        inline bool removeItem(gbp_i64 id, const QModelIndex& parent = QModelIndex()) {
            return removeItems(QList<gbp_i64>() << id, parent);
        }
        inline bool insertItem(gbp_i64 id, const QModelIndex& parent = QModelIndex()) {
            return insertItems(QList<gbp_i64>() << id, parent);
        }

        template <typename container_t, typename value_t = typename container_t::value_type>
        bool insertRowsTo(container_t& container, int row, int count, const QModelIndex& parent = QModelIndex())
        {
            int size = container.size();
            if (m_logModel && size != rowCount(parent)) {
                m_logModel->newMsg(QString("ASSERT(size == static_cast<int_t>(rowCount(parent)))"), LogMsgItem::MsgFatal)
                << size << rowCount(parent) << MAKE_FILELINE;
            }

            if (row > size || count < 1 || row < 0) {
                m_logModel->newMsg(QString("ASSERT(!(row > size || count < 1 || row < 0)), row: %0, count: %1, size: %2").arg(row).arg(count).arg(size), LogMsgItem::MsgFatal)
                                    << MAKE_FILELINE;
                return false;
            }
            if (size + count > rowsCapacity(parent)) {
                return false;
            }
            if (m_logModel) {
                m_logModel->newMsg(QString("beginInsertRows(parent: %0, first: %1, last: %2)").arg(ns_util::traceModelIndex(parent)).arg(row).arg(row + count - 1))
                            << MAKE_FILELINE;
            }
            beginInsertRows(parent, row, row + count - 1);
            if (row == size) {
                container.resize(container.size() + count);
            } else {
                container.insert(container.begin()+row, count, value_t());
            }
            endInsertRows();
            return true;
        }
        template <typename container_t, typename value_t = typename container_t::value_type>
        bool removeRowsOf(container_t& container, int row, int count, const QModelIndex& parent = QModelIndex())
        {
            int size = container.size();
            if (m_logModel && size != static_cast<int>(rowCount(parent))) {
                m_logModel->newMsg(QString("ASSERT(size == static_cast<int_t>(rowCount(parent)))"), LogMsgItem::MsgFatal)
                << size << rowCount(parent) << MAKE_FILELINE;
            }
            if (row < 0 || count < 1 || row + count > size) {
                m_logModel->newMsg(QString("ASSERT(!(row < 0 || count < 1 || row + count > size)), row: %0, count: %1, size: %2").arg(row).arg(count).arg(size), LogMsgItem::MsgFatal)
                                    << MAKE_FILELINE;
                return false;
            }

            if (m_logModel) {
                m_logModel->newMsg(QString("beginRemoveRows(parent: %0, first: %1, last: %2)").arg(ns_util::traceModelIndex(parent)).arg(row).arg(row + count - 1))
                            << MAKE_FILELINE;
            }
            beginRemoveRows(parent, row, row + count - 1);
            if (row + count == size) {
                container.resize(container.size() - count);
            } else {
                container.erase(container.begin()+row, container.begin()+row + count);
            }
            endRemoveRows();
            return true;
        }
        virtual int rowsCapacity(const QModelIndex& parent = QModelIndex()) const;
        virtual int columnsCapacity(const QModelIndex& parent = QModelIndex()) const;

    protected:

        template <typename T>
        T takeWaitingForInsertItem() {
            if (m_waitForInserting.empty()) {
                return T();
            }
            if (const T* item = static_cast<const T*>(m_waitForInserting.takeFirst())) {
                return T(*item);
            }
            return T();
        }

        virtual QModelIndex indexOf(gbp_i64 /*id*/) const { return QModelIndex(); }

    public:
        template <typename container_t, typename value_t = typename container_t::value_type, typename IdGetter = IdGetter<value_t>>
        static void updateItems(ApiBase& model, container_t& oldItems, const container_t &newItems, const QModelIndex& parent = QModelIndex())
        {
            if (oldItems.empty()) {
                if (newItems.empty()) {
                    return;
                }
                if (!parent.isValid())
                {
                    //! 0. Reset
                    //!    no need to analyze empty data
                    model.beginResetModel();
                    model.resetInternalData();
                    oldItems = newItems;
                    model.endResetModel();
                }
                else
                {
                    if (model.m_logModel) {
                        model.m_logModel->newMsg(QString("beginInsertRows(parent: %0, first: %1, last: %2)").arg(ns_util::traceModelIndex(parent)).arg(0).arg(newItems.size()-1), LogMsgItem::MsgInfo)
                        << MAKE_FILELINE;
                    }

                    model.beginInsertRows(parent, 0, newItems.size()-1);
                    oldItems = newItems;
                    model.endInsertRows();
                }
            }
            if (newItems.empty())
            {
                if (model.m_logModel) {
                    model.m_logModel->newMsg(QString("beginInsertRows(parent: %0, first: %1, last: %2), resize(0)").arg(ns_util::traceModelIndex(parent)).arg(0).arg(oldItems.size()-1), LogMsgItem::MsgInfo)
                    << MAKE_FILELINE;
                }
                model.beginRemoveRows(parent, 0, model.rowCount(parent)-1);
                oldItems.resize(0);
                model.endRemoveRows();
                return;
            }

            for (typename container_t::size_type row = 0; row < newItems.size(); ++row)
            {
                typename IdGetter::id_type id = IdGetter::get_id(newItems[row]);
                if (IdGetter::get_id(oldItems[row]) != id)
                {
                    typename container_t::iterator it = IdGetter::find_id(oldItems, id);
                    typename container_t::iterator targetIt = oldItems.begin() + row;
                    if (it == oldItems.end())
                    {
                        if (model.m_logModel) {
                            model.m_logModel->newMsg(QString("beginInsertRows(parent: %0, first: %1, last: %2)")
                                             .arg(ns_util::traceModelIndex(parent)).arg(row).arg(row), LogMsgItem::MsgInfo) << MAKE_FILELINE;
                        }
                        model.beginInsertRows(parent, row, row);
                        oldItems.insert(targetIt, 1, newItems[row]);
                        model.endInsertRows();
                    }
                    else
                    {
                        typename container_t::size_type sourceRow = it - oldItems.begin();
                        if (model.m_logModel) {
                            model.m_logModel->newMsg(QString("beginMoveRows(parent: %0, from row: %1, to row: %2)")
                                             .arg(ns_util::traceModelIndex(parent)).arg(sourceRow).arg(row), LogMsgItem::MsgInfo) << MAKE_FILELINE;
                        }
                        model.beginMoveRows(parent, sourceRow, sourceRow, parent, row);
                        while (sourceRow != row) {
                            std::swap(oldItems.at(sourceRow), oldItems.at(sourceRow - 1));
                            sourceRow--;
                        }
                        model.endMoveRows();
                    }
                }
            }

            if (oldItems.size() > newItems.size())
            {
                if (model.m_logModel) {
                    model.m_logModel->newMsg(QString("beginRemoveRows(parent: %0, first: %1, last: %2)").arg(ns_util::traceModelIndex(parent)).arg(newItems.size()).arg(oldItems.size() - 1))
                                << MAKE_FILELINE;
                }
                model.beginRemoveRows(parent, newItems.size(), oldItems.size() - 1);
                oldItems.resize(newItems.size());
                model.endRemoveRows();
            }



            for (typename container_t::size_type row = 0; row < newItems.size(); ++row)
            {
//                updateRowItem(model, oldItems, newItems, i, parent);
                if (model.m_logModel) {
                    model.m_logModel->newMsg(QString("before updateRowData(parent: %0, row: %1)").arg(ns_util::traceModelIndex(parent)).arg(row))
                                << MAKE_FILELINE;
                }
                QList<int> changes = model.updateRowData(parent, row, oldItems, newItems);
                if (model.m_logModel) {
                    model.m_logModel->newMsg(QString("changed columns count: %0, columns[%1]").arg(changes.size()).arg(ns_util::toStringList(changes).join("|")))
                                << MAKE_FILELINE;
                }
//                for (int col: changes) {
//                    QModelIndex index = model.index(row, col, parent);
//                    emit model.dataChanged(index, index);
//                }
            }

        }

        // **************************************************

        template <typename container_t, typename value_t = typename container_t::value_type>
        static void updatePrimitives(ApiBase& model, container_t& oldItems, const container_t &newItems, const QModelIndex& parent = QModelIndex())
        {
            int newSize = newItems.size();

            if (oldItems.empty())
            {
                if (newItems.empty()) {
                    return;
                }
                if (!parent.isValid())
                {
                    if (model.m_logModel) {
                        model.m_logModel->newMsg(QString("beginResetModel"), LogMsgItem::MsgInfo);
                    }
                    model.beginResetModel();
                    model.resetInternalData();
                    oldItems = newItems;
                    model.endResetModel();
                    if (model.m_logModel) {
                        model.m_logModel->newMsg(QString("endResetModel"), LogMsgItem::MsgInfo);
                    }
                }
                else
                {
                    if (model.m_logModel) {
                        model.m_logModel->newMsg(QString("beginInsertRows(parent:%0, first:%1, last:%2)").arg(ns_util::traceModelIndex(parent)).arg(0).arg(newSize-1))
                        << MAKE_FILELINE;
                    }
                    model.beginInsertRows(parent, 0, newSize - 1);
                    oldItems = newItems;
                    model.endInsertRows();
                    if (model.m_logModel) {
                        model.m_logModel->newMsg("endInsertRows()") << MAKE_FILELINE;
                    }
                }
            }
            if (newItems.empty())
            {
                if (model.m_logModel) {
                    model.m_logModel->newMsg(QString("beginInsertRows(parent: %0, first: %1, last: %2), resize(0)").arg(ns_util::traceModelIndex(parent)).arg(0).arg(oldItems.size()-1), LogMsgItem::MsgInfo)
                    << MAKE_FILELINE;
                }
                model.beginRemoveRows(parent, 0, oldItems.size()-1);
                oldItems.resize(0);
                model.endRemoveRows();
                return;
            }

            if (oldItems.size() != newItems.size())
            {
                if (oldItems.size() > newItems.size()) {
                    model.removeRowsOf(oldItems, newItems.size(), oldItems.size() - newItems.size(), parent);
                } else {
                    model.insertRowsTo(oldItems, oldItems.size(), newItems.size() - oldItems.size(), parent);
                }
            }

            if (oldItems.size() != newItems.size()) {
                //qDebug() << "NU ETO PIZDEC!!!";
                //qDebug() << oldItems.size() << newItems.size();
            }

            QList<int> changedRows;
            for (int row = 0; row < model.rowCount(parent); row++)
            {
                value_t& oldItem = oldItems[row];
                value_t  newItem = newItems[row];
                if (oldItem != newItem) {
                    changedRows << row;
                    oldItem = newItem;
                }
            }
            if (changedRows.empty()) {
                return;
            }
            QModelIndex changedIndex = model.index(changedRows.first(), 0, parent);
            if (changedRows.size() == 1) {
                emit model.dataChanged(changedIndex, changedIndex);
                return;
            }

            int expectedRowToJoin = changedIndex.row() + 1;
            for (int i = 1; i < changedRows.size(); i++) {
                if (changedRows.at(i) == expectedRowToJoin) {
                    expectedRowToJoin++;
                } else {
                    emit model.dataChanged(changedIndex, changedIndex.sibling(expectedRowToJoin - 1, 0));
                    changedIndex = changedIndex.sibling(changedRows.at(i), 0);
                    expectedRowToJoin = changedIndex.row() + 1;
                }
            }
            if (changedIndex.isValid()) {
                emit model.dataChanged(changedIndex, changedIndex.sibling(expectedRowToJoin - 1, 0));
            }
        }


        template <typename value_t>
        QList<int> updateRowData(const QModelIndex& parent, value_t& target, const value_t& source)
        {
            ModelDataIndex mdIndex("index");
            ModelDataSetter<value_t> dSetter(&mdIndex);

            dSetter.setData(target, source);
            static const PropertyIndex<value_t> pIdx;
            QList<int> lst;
            for (ModelDataIndex* mdIdx: mdIndex.children()) {
                lst << pIdx.indexOf(mdIdx->key());
            }
            return lst;
        }

        template <typename container_t>
        QList<int> updateRowData(const QModelIndex& parent, int row, container_t& target, const container_t& source)
        {
            Q_ASSERT(!parent.isValid() || parent.model() == this);
            const typename container_t::value_type& sourceRowData = source.at(row);
            typename container_t::value_type& targetRowData       = target.at(row);
            QList<int> lst = updateRowData(parent, targetRowData, sourceRowData);
            for (int col: lst)
            {
                QModelIndex modelIndex = index(row, col, parent);
                emit dataChanged(modelIndex, modelIndex);
            }
            return lst;
        }

        template <typename container_t>
        static void updateRowItem(ApiBase& model, container_t& oldItems, const container_t &newItems, typename container_t::size_type row, const QModelIndex& parent = QModelIndex());
    };


} //namespace ns_model



template <typename container_t>
void ns_model::ApiBase::updateRowItem(ApiBase& model, container_t& oldItems, const container_t &newItems, typename container_t::size_type row, const QModelIndex& parent) {
    using ItemType = typename container_t::value_type;
    Q_ASSERT(oldItems.size() == newItems.size() && oldItems.size() > row);
    ItemType& oldItem = oldItems.at(row);
    const ItemType& newItem = newItems.at(row);

    QModelIndex index = model.index(row, 0, parent);
    Q_ASSERT(model.columnCount(parent) > 0);
    if (model.columnCount(parent) == 1) {
        if (oldItem != newItem) {
            oldItem = newItem;
            emit model.dataChanged(index, index);
        }
        return;
    }

    static const PropertyIndex<ItemType> propIdx;
    Comparator comparator;
    if (comparator.compare(oldItem, newItem)) {
        const QMap<QByteArray, QPair<QVariant, QVariant>>& changedItems = comparator.changes();
        for (const QByteArray& key: changedItems.keys()) {
            QModelIndex modelIndex = model.index(row, propIdx.indexOf(key), parent);
            model.setData(modelIndex, changedItems.value(key).second, UserType::OriginDataRole);
        }
    }
}


