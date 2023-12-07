#include "model_apibase.hpp"
#include <QRegularExpression>
#include <basewidgets/types.h>
#include <QDebug>

#ifndef Q_MOC_RUN
    #include <api/lobby_stat/reply.h>
    #include <api/lobby_stat/types.h>
#endif // !Q_MOC_RUN

#include <QFile>

//! %0 - row, %1 - column, %2 - (0x0 invalid parent|[...] - parent index)
const char* index_to_key_pattern = "[%0,%1,%2]";
const char* key_to_index_pattern = "^\\[(\\d+), ?(\\d+), ?(0x0|.+)\\]$";

Q_DECLARE_METATYPE(gbp::ns_api::ns_statistic::cash_stat_t)

namespace ns_model
{

}

namespace ns_model
{
    QByteArray ApiBase::indexToKey(int row, int column, const QModelIndex &parent) const
    {
        static const QString pattern(index_to_key_pattern);
        return pattern.arg(row).arg(column).arg(qPrintable(indexToKey(parent))).toLocal8Bit();
    }

    QByteArray ApiBase::indexToKey(const QModelIndex &idx) const
    {
        if (idx.isValid()) {
            return indexToKey(idx.row(), idx.column(), idx.parent());
        }
        return "0x0";
    }

    QByteArray ApiBase::anyIndexToKey(const QModelIndex &idx)
    {
        if (const ApiBase* m = qobject_cast<const ApiBase*>(idx.model())) {
            return m->indexToKey(idx);
        }
        return "0x0";
    }

    QModelIndex ApiBase::keyToIndex(const QByteArray &key) const
    {
        if (key == QByteArrayLiteral("0x0")) {
            return QModelIndex();
        }
        QRegularExpression re(key_to_index_pattern);
        QStringList captured = re.match(key).capturedTexts();
        if(captured.isEmpty()) {
            return QModelIndex();
        }

        int row  = captured.at(1).toInt();
        int col  = captured.at(2).toInt();
        QByteArray parentKey = captured.at(3).toLocal8Bit();

        return index(row, col, keyToIndex(parentKey));
    }


    ApiBase::ApiBase(QObject *parent)
        : QAbstractItemModel(parent)
        , IOModel(this)
        , m_logModel(nullptr)
    {
    }

    ApiBase::~ApiBase()
    {
        //qDebug() << "~ApiBase()" << metaObject()->className() << (void*)(this);
        qDeleteAll(m_internal);
    }

    QModelIndex ApiBase::index(int row, int column, const QModelIndex &parent) const
    {
        QByteArray key = indexToKey(row, column, parent);
        auto it = m_internal.find(key);
        if (it == m_internal.end()) {
            it = m_internal.insert(key, new QByteArray(key));
        }
        return createIndex(row, column, it.value());
    }

    QModelIndex ApiBase::parent(const QModelIndex &index) const
    {
        if (index.isValid()) {
            if (QByteArray* key = static_cast<QByteArray*>(index.internalPointer())) {
                QRegularExpression re(key_to_index_pattern);
                return keyToIndex(re.match(*key).captured(3).toLatin1());
            }
        }
        return QModelIndex();
    }

    QVariant ApiBase::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid()) {
            return QVariant();
        }
        if (role == UserType::InternalIdRole) {
            return indexToKey(index);
        }
        if (isItemOrientedData()) {
            return itemData(index).value(role);
        }
        return QVariant();
    }

    bool ApiBase::backup(const QString &filename) const
    {
        QFile f(getPath() + filename);
        return write(&f);
    }

    bool ApiBase::restore(const QString &filename)
    {
        QFile f(getPath() + filename);
        return read(&f);
    }

    gbp_i64 ApiBase::idAt(int row, const QModelIndex& parent) const {
        return idAt(index(row, 0, parent));
    }

    gbp_i64 ApiBase::idAt(const QModelIndex &idx) const {
        if (!idx.isValid()) {
            return -1;
        }
        return idx.data(UserType::RowItemIdRole).value<gbp_i64>();
    }

    int ApiBase::rowsCapacity(const QModelIndex &/*parent*/) const {
        return 1000;
    }

    int ApiBase::columnsCapacity(const QModelIndex &parent) const {
        return columnCount(parent);
    }

} //namespace ns_model


