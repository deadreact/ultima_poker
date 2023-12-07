#pragma once

#include <qbytearray.h>
#include <qvariant.h>
#include <tools/types_fwd.hpp>

class QAbstractItemModel;

//! упрощенный ItemView-елемент игрового стола
struct ModelKeyItem {
    virtual ~ModelKeyItem() {}
    virtual QByteArray modelKey() const = 0;
    virtual void updateData(const QAbstractItemModel* model) = 0;

    virtual QModelIndex index(const QAbstractItemModel* model) const;
//    template <typename T>
//    T value(const ns_model::table_state_t* model) const {
//        return qvariant_cast<T>(index(model).data(UserType::OriginDataRole));
//    }
};
