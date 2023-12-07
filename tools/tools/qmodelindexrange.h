#pragma once

#include "tools_global.h"
#include "../../common/api/gbp_int.hpp"
#include <qabstractitemmodel.h>
#include <qrect.h>

using QModelIndexPair = QPair<QModelIndex, QModelIndex>;

class TOOLS_DLL QModelIndexRange : public QModelIndexPair
{
public:
    struct iterator;
public:
    QModelIndexRange();
    QModelIndexRange(const QModelIndex& single);
    QModelIndexRange(const QModelIndex& topLeft, const QModelIndex& bottomRight);

    static QModelIndexRange rowRange(const QModelIndex& parent, int row, int firstColumn, int lastColumn);
    static QModelIndexRange rowRange(const QAbstractItemModel* m, int row, int firstColumn, int lastColumn);
    static QModelIndexRange columnRange(const QModelIndex& parent, int column, int firstRow, int lastRow);
    static QModelIndexRange columnRange(const QAbstractItemModel* m, int column, int firstRow, int lastRow);
    static QModelIndexRange parentRange(const QModelIndex& parent);

    QModelIndex parent() const;
    bool isValid() const;
    bool isEmpty() const;
    inline bool empty() const;
    int rowCount() const;
    int columnCount() const;
    bool contains(const QModelIndex& index) const;
    bool contains(int row, int column) const;
    inline int indexCount() const {
        return rowCount() * columnCount();
    }
    inline QSize size() const;
    inline QRect area() const ;
    inline const QAbstractItemModel* model() const;

    iterator begin() const;
    iterator last() const;
    iterator end() const;
};

struct TOOLS_DLL QModelIndexRange::iterator {
private:
    const QModelIndexRange* owner;
    int row, column;
public:
    iterator(const QModelIndexRange* owner): owner(owner), row(owner->first.row()), column(owner->first.column()) {}
    iterator(const QModelIndexRange* owner, int row, int column): owner(owner), row(row), column(column) {}
    iterator(const iterator& other): owner(other.owner), row(other.row), column(other.column) {}
    iterator& operator=(const iterator& other) {
        owner = other.owner;
        row = other.row;
        column = other.column;
        return *this;
    }
    QModelIndex operator*() {
        if (!owner || !owner->isValid()) {
            return QModelIndex();
        }
        return owner->parent().model()->index(row, column, owner->parent());
    }
    bool operator==(const iterator& other) const { return row == other.row && column == other.column && owner == other.owner; }
    bool operator!=(const iterator& other) const { return !operator==(other); }

    inline iterator& operator++() {
        return operator+=(1);
    }
    inline iterator& operator--() {
        return operator-=(1);
    }
    inline iterator& operator++(int) {
        return operator++();
    }
    inline iterator& operator--(int) {
        return operator--();
    }
    inline iterator& operator+=(int n) {
        row += n;
        while (q_check_ptr(owner)->second.row() < row) {
            row -= owner->rowCount();
            column++;
        }
        return *this;
    }
    inline iterator& operator-=(int n) {
        row -= n;
        while (q_check_ptr(owner)->first.row() > row) {
            row += owner->rowCount();
            column--;
        }
        return *this;
    }
    iterator operator+(int n) const {
        iterator it = *this;
        it += n;
        return it;
    }
    iterator operator-(int n) const {
        iterator it = *this;
        it -= n;
        return it;
    }
};


// **************************** INLINE *********************************** //
bool QModelIndexRange::empty() const {
    return isEmpty();
}
inline QSize QModelIndexRange::size() const {
    return QSize(rowCount(), columnCount());
}
inline QRect QModelIndexRange::area() const {
    return QRect(first.row(), first.column(), second.row(), second.column());
}
inline const QAbstractItemModel* QModelIndexRange::model() const {
    return first.model();
}
