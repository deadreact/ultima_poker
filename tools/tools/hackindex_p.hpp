#pragma once

#include <qabstractitemmodel.h>

class HackIndex;

struct hack_cast
{
    union
    {
        const QModelIndex* modelIndex;
        const HackIndex* hackIndex;
    };

    hack_cast(const HackIndex& hackIndex): hackIndex(&hackIndex) {}
};

class HackIndex
{
public:
    constexpr inline static HackIndex prototype(int arow, int acolumn, quintptr id) {
        return HackIndex(arow, acolumn, id, nullptr);
    }
    constexpr inline static HackIndex fromPrototype(const HackIndex& prototype, const QAbstractItemModel* amodel) {
        return HackIndex(prototype.row(), prototype.column(), prototype.internalId(), amodel);
    }
public:
    constexpr inline HackIndex() noexcept : r(-1), c(-1), i(0), m(nullptr) {}
    inline HackIndex(int arow, int acolumn, void *ptr, const QAbstractItemModel *amodel) noexcept
        : r(arow), c(acolumn), i(reinterpret_cast<quintptr>(ptr)), m(amodel) {}
    constexpr inline HackIndex(int arow, int acolumn, quintptr id, const QAbstractItemModel *amodel) noexcept
        : r(arow), c(acolumn), i(id), m(amodel) {}
    int r, c;
    quintptr i;
    const QAbstractItemModel *m;
public:
    constexpr inline HackIndex(const QModelIndex& index) noexcept : r(index.row()), c(index.column()), i(index.internalId()), m(index.model()) {}

    operator QModelIndex() const {
        hack_cast hack(*this);
        return *hack.modelIndex;
    }

    Q_DECL_CONSTEXPR inline int row() const Q_DECL_NOTHROW { return r; }
    Q_DECL_CONSTEXPR inline int column() const Q_DECL_NOTHROW { return c; }
    Q_DECL_CONSTEXPR inline quintptr internalId() const Q_DECL_NOTHROW { return i; }
    inline void *internalPointer() const Q_DECL_NOTHROW { return reinterpret_cast<void*>(i); }
    inline HackIndex parent() const { return m ? HackIndex(m->parent(*this)) : HackIndex(); }
public:
    constexpr HackIndex& with(const QAbstractItemModel *amodel) {
        m = amodel;
        return *this;
    }
};
