#include "algorithm.hpp"

//QModelIndexRange::QModelIndexRange()
//    : QModelIndexPair(QModelIndex(), QModelIndex())
//{}

//QModelIndexRange::QModelIndexRange(const QModelIndex &topLeft, const QModelIndex &bottomRight)
//    : QModelIndexPair(qMin(topLeft, bottomRight), qMax(topLeft, bottomRight))
//{
////    Q_ASSERT(topLeft.parent() == bottomRight.parent());
//    if (!(topLeft.parent() == bottomRight.parent())) {
//        qDebug() << "catch";
//    }
//}



//QModelIndexRange::QModelIndexRange(const QModelIndex &single)
//    : QModelIndexPair(single, single)
//{}

//QModelIndexRange QModelIndexRange::rowRange(const QAbstractItemModel* m, int row, int firstColumn, int lastColumn)
//{
//    if (m == nullptr) {
//        return QModelIndexRange();
//    }
//    if (firstColumn > lastColumn) {
//        std::swap(firstColumn, lastColumn);
//    }
//    if (row < 0 || row >= m->rowCount() || firstColumn < 0 || lastColumn >= m->columnCount()) {
//        return QModelIndexRange();
//    }
//    return QModelIndexRange(m->index(row, firstColumn), m->index(row, lastColumn));
//}
//QModelIndexRange QModelIndexRange::rowRange(const QModelIndex &parent, int row, int firstColumn, int lastColumn)
//{
//    if (!parent.isValid() || !parent.model()->hasChildren(parent)) {
//        return QModelIndexRange();
//    }
//    if (firstColumn > lastColumn) {
//        std::swap(firstColumn, lastColumn);
//    }
//    if (row < 0 || row >= parent.model()->rowCount(parent) || firstColumn < 0 || lastColumn >= parent.model()->columnCount(parent)) {
//        return QModelIndexRange();
//    }
//    return QModelIndexRange(parent.model()->index(row, firstColumn, parent), parent.model()->index(row, lastColumn, parent));
//}

//QModelIndexRange QModelIndexRange::columnRange(const QModelIndex &parent, int column, int firstRow, int lastRow)
//{
//    if (!parent.isValid() || !parent.model()->hasChildren(parent)) {
//        return QModelIndexRange();
//    }
//    if (firstRow > lastRow) {
//        std::swap(firstRow, lastRow);
//    }

//    if (column < 0 || column >= parent.model()->rowCount(parent) || firstRow < 0 || lastRow >= parent.model()->columnCount(parent)) {
//        return QModelIndexRange();
//    }
//    return QModelIndexRange(parent.model()->index(column, firstRow, parent), parent.model()->index(column, lastRow, parent));
//}

//QModelIndexRange QModelIndexRange::columnRange(const QAbstractItemModel *m, int column, int firstRow, int lastRow)
//{
//    if (m == nullptr) {
//        return QModelIndexRange();
//    }
//    if (firstRow > lastRow) {
//        std::swap(firstRow, lastRow);
//    }
//    if (column < 0 || column >= m->rowCount() || firstRow < 0 || lastRow >= m->columnCount()) {
//        return QModelIndexRange();
//    }
//    return QModelIndexRange(m->index(column, firstRow), m->index(column, lastRow));
//}

//QModelIndexRange QModelIndexRange::parentRange(const QModelIndex &parent)
//{
//    if (!parent.isValid() || !parent.model()->hasChildren(parent) || (parent.flags() & Qt::ItemNeverHasChildren)) {
//        return QModelIndexRange();
//    }
//    const QAbstractItemModel* m = parent.model();
//    return QModelIndexRange(m->index(0, 0, parent), m->index(m->rowCount(parent)-1, m->columnCount(parent)-1));
//}



//bool QModelIndexRange::isValid() const {
//    return first.isValid() && second.isValid() && (first < second || first == second) && first.parent() == second.parent();
//}

//bool QModelIndexRange::isEmpty() const
//{
//    return begin() == end();
//}

//int QModelIndexRange::rowCount() const {
//    return second.row() - first.row() + 1;
//}

//int QModelIndexRange::columnCount() const {
//    return second.column() - first.column() + 1;
//}

//bool QModelIndexRange::contains(const QModelIndex &index) const {
//    if (!isValid()) {
//        return false;
//    }
//    return (index.parent() == parent()) && ((first == index) || (first < index && (index < second || index == second)));
//}

//bool QModelIndexRange::contains(int row, int column) const {
//    if (!isValid()) {
//        return false;
//    }
//    return first.row() <= row && row <= second.row() && first.column() <= column && column <= second.column();
//}

//QModelIndexRange::iterator QModelIndexRange::begin() const {
//    return iterator(this);
//}

//QModelIndexRange::iterator QModelIndexRange::last() const {
//    return iterator(this, second.row(), second.column());
//}

//QModelIndexRange::iterator QModelIndexRange::end() const {
//    return last() + 1;
//}

//QModelIndex QModelIndexRange::parent() const {
//    return first.parent();
//}

namespace ns_algo
{
    namespace ns_detail
    {
        ns_model::LogModel* LogModelKeeper::m_logModel = nullptr;

        void LogModelKeeper::resetModel(ns_model::LogModel *model) {
            m_logModel = model;
        }


        void LogModelKeeper::log(ns_model::LogMsgItem::MsgType type, const QString &msg, const char *file, int line, const char *func, const QStringList &args) {
            if (!m_logModel) {
                return;
            }
            m_logModel->newMsg(msg, type);
            if (file != nullptr) {
                QString fileline = QStringLiteral("source_link:") % file % QStringLiteral(":") % QString::number(line);
                *m_logModel << QUrl(fileline);

            }
            if (func != nullptr) {
                if (args.isEmpty()) {
                    *m_logModel << (file % QStringLiteral("()"));
                } else {
                    *m_logModel << (file % QStringLiteral("(") % args.join(", ") % QStringLiteral(")"));
                }
            }
        }


        void LogModelKeeper::warning(const QString &msg, const char *file, int line, const char *func, const QStringList &args) {
            log(ns_model::LogMsgItem::MsgWarning, msg, q_check_ptr(file), line, func, args);
        }


        void LogModelKeeper::error(const QString &msg, const char *file, int line, const char *func, const QStringList &args) {
            log(ns_model::LogMsgItem::MsgCritical, msg, q_check_ptr(file), line, func, args);
        }


        void LogModelKeeper::info(const QString &msg, const char *file, int line, const char *func, const QStringList &args) {
            log(ns_model::LogMsgItem::MsgInfo, msg, file, line, func, args);
        }
    } //namespace ns_detail


} // namespace ns_algo

