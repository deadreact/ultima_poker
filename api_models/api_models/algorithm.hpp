#pragma once

#include "defs_metatype.hpp"
#include <tools/qmodelindexrange.h>
#include "model_apibase.hpp"
#include "api_util.hpp"
#include "model_log.h"
#include <qurl.h>

#define __FILELINE__ __FILE__, __LINE__
#define REQUIRES_COND(condition) if (condition) {} else { Log::error(QStringLiteral(#condition), __FILELINE__, __FUNCTION__); }

//using QModelIndexPair = QPair<QModelIndex, QModelIndex>;

//class QModelIndexRange : public QModelIndexPair
//{
//public:
//    struct iterator;
//public:
//    QModelIndexRange();
//    QModelIndexRange(const QModelIndex& single);
//    QModelIndexRange(const QModelIndex& topLeft, const QModelIndex& bottomRight);

//    static QModelIndexRange rowRange(const QModelIndex& parent, int row, int firstColumn, int lastColumn);
//    static QModelIndexRange rowRange(const QAbstractItemModel* m, int row, int firstColumn, int lastColumn);
//    static QModelIndexRange columnRange(const QModelIndex& parent, int column, int firstRow, int lastRow);
//    static QModelIndexRange columnRange(const QAbstractItemModel* m, int column, int firstRow, int lastRow);
//    static QModelIndexRange parentRange(const QModelIndex& parent);

//    QModelIndex parent() const;
//    bool isValid() const;
//    bool isEmpty() const;
//    inline bool empty() const {
//        return isEmpty();
//    }
//    int rowCount() const;
//    int columnCount() const;
//    bool contains(const QModelIndex& index) const;
//    bool contains(int row, int column) const;
//    inline int indexCount() const {
//        return rowCount() * columnCount();
//    }
//    inline QSize size() const {
//        return QSize(rowCount(), columnCount());
//    }
//    inline QRect area() const {
//        return QRect(first.row(), first.column(), second.row(), second.column());
//    }

//    const QAbstractItemModel* model() const {
//        return first.model();
//    }

//    iterator begin() const;
//    iterator last() const;
//    iterator end() const;
//};

//struct QModelIndexRange::iterator {
//private:
//    const QModelIndexRange* owner;
//    int row, column;
//public:
//    iterator(const QModelIndexRange* owner): owner(owner), row(owner->first.row()), column(owner->first.column()) {}
//    iterator(const QModelIndexRange* owner, int row, int column): owner(owner), row(row), column(column) {}
//    iterator(const iterator& other): owner(other.owner), row(other.row), column(other.column) {}
//    iterator& operator=(const iterator& other) {
//        owner = other.owner;
//        row = other.row;
//        column = other.column;
//        return *this;
//    }
//    QModelIndex operator*() {
//        if (!owner || !owner->isValid()) {
//            return QModelIndex();
//        }
//        return owner->parent().model()->index(row, column, owner->parent());
//    }
//    bool operator==(const iterator& other) const { return row == other.row && column == other.column && owner == other.owner; }
//    bool operator!=(const iterator& other) const { return !operator==(other); }

//    iterator& operator++() {
//        return operator+=(1);
//    }
//    iterator& operator--() {
//        return operator-=(1);
//    }
//    iterator& operator++(int) {
//        return operator++();
//    }
//    iterator& operator--(int) {
//        return operator--();
//    }
//    iterator& operator+=(int n) {
//        row += n;
//        while (q_check_ptr(owner)->second.row() < row) {
//            row -= owner->rowCount();
//            column++;
//        }
//        return *this;
//    }
//    iterator& operator-=(int n) {
//        row -= n;
//        while (q_check_ptr(owner)->first.row() > row) {
//            row += owner->rowCount();
//            column--;
//        }
//        return *this;
//    }
//    iterator operator+(int n) const {
//        iterator it = *this;
//        it += n;
//        return it;
//    }
//    iterator operator-(int n) const {
//        iterator it = *this;
//        it -= n;
//        return it;
//    }
//};

namespace ns_algo
{
    namespace ns_detail
    {   
        struct LogModelKeeper {
            static ns_model::LogModel* m_logModel;
            static void resetModel(ns_model::LogModel* model = nullptr);

            static void log(ns_model::LogMsgItem::MsgType type, const QString& msg, const char* file, int line, const char* func, const QStringList& args = QStringList());
            static void warning(const QString& msg, const char* file, int line, const char* func = nullptr, const QStringList& args = QStringList());
            static void error(const QString& msg, const char* file, int line, const char* func = nullptr, const QStringList& args = QStringList());
            static void info(const QString& msg, const char* file = nullptr, int line = 0, const char* func = nullptr, const QStringList& args = QStringList());
        };
    } // ns_detail
    /******************************************************************************/
    using Log = ns_detail::LogModelKeeper;
    /******************************************************************************/

    template <typename container_t, typename value_t = typename container_t::value_type>
    bool insert(ns_model::ApiBase* m, container_t& container, int row, int count, const QModelIndex& parent = QModelIndex())
    {
        QModelIndex p = m->parentOf<typename container_t::value_type>();
        p = (p.isValid()) ? p : parent;
        int size = container.size();
        REQUIRES_COND(size == q_check_ptr(m)->rowCount(p));
        REQUIRES_COND(row > size || count < 1 || row < 0);
        REQUIRES_COND(size + count > m->rowsCapacity(p));

        Log::info(QString("beginInsertRows(parent: %0, first: %1, last: %2)").arg(ns_util::traceModelIndex(parent)).arg(row).arg(row + count - 1));
        m->beginInsertRows(parent, row, row + count - 1);
        if (row == size) {
            container.resize(container.size() + count);
        } else {
            container.insert(container.begin()+row, count, value_t());
        }
        m->endInsertRows();
        return true;
    }
//    template <typename container_t, typename value_t = typename container_t::value_type>
//    bool removeRowsOf(container_t& container, int row, int count, const QModelIndex& parent = QModelIndex())
//    {
//        int size = container.size();
//        if (m_logModel && size != static_cast<int>(rowCount(parent))) {
//            m_logModel->newMsg(QString("ASSERT(size == static_cast<int_t>(rowCount(parent)))"), LogMsgItem::MsgFatal)
//            << size << rowCount(parent) << MAKE_FILELINE(":");
//        }
//        if (row < 0 || count < 1 || row + count > size) {
//            m_logModel->newMsg(QString("ASSERT(!(row < 0 || count < 1 || row + count > size)), row: %0, count: %1, size: %2").arg(row).arg(count).arg(size), LogMsgItem::MsgFatal)
//                                << MAKE_FILELINE(":");
//            return false;
//        }

//        if (m_logModel) {
//            m_logModel->newMsg(QString("beginRemoveRows(parent: %0, first: %1, last: %2)").arg(ns_util::traceModelIndex(parent)).arg(row).arg(row + count - 1))
//                        << MAKE_FILELINE(":");
//        }
//        beginRemoveRows(parent, row, row + count - 1);
//        if (row + count == size) {
//            container.resize(container.size() - count);
//        } else {
//            container.erase(container.begin()+row, container.begin()+row + count);
//        }
//        endRemoveRows();
//        return true;
//    }
} // ns_algo

