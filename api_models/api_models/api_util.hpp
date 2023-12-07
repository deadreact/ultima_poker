#pragma once

#include <qstring.h>
#include <functional>
#include <QMetaEnum>
#include <qabstractproxymodel.h>
#include "../../common/api/gbp_int.hpp"

template <typename EnumType>
constexpr typename std::underlying_type<EnumType>::type underlying_cast(EnumType enumValue) {
    return static_cast<typename std::underlying_type<EnumType>::type>(enumValue);
}

namespace gbp {
namespace ns_api {
    struct birth_date;
}
}


QModelIndex mapToModel(const QAbstractItemModel* model, const QModelIndex& index);

template <typename T>
const T* qmodelindexmodel_cast(const QModelIndex& index)
{
    if (!index.isValid()) {
        return nullptr;
    }
    if (const T* m = qobject_cast<const T*>(index.model())) {
        return m;
    }
    if (const QAbstractProxyModel* m = qobject_cast<const QAbstractProxyModel*>(index.model())) {
        return qmodelindexmodel_cast<T>(m->mapToSource(index));
    }
    return nullptr;
}

namespace ns_util
{
    QString traceModelIndex(const QModelIndex& index);
    QString traceModelIndexWIthData(const QModelIndex& index);
    QString printItemInfo(const QModelIndex& index);

    QDate toQDate(const gbp::ns_api::birth_date& bdate);
    gbp::ns_api::birth_date toApiBirthDate(const QDate& date);
    gbp::ns_api::birth_date apiDate(qint16 year, qint16 month, qint16 day);

    template <typename EnumType>
    QString toString(EnumType e);

    namespace
    {
        template <typename EnumType>
        inline QString enumToString(EnumType enumValue)
        {
            return QMetaEnum::fromType<EnumType>().valueToKeys(underlying_cast(enumValue)).split('|').last();
        }
    }

    template <typename container_t, typename value_t = typename container_t::value_type, typename int_t = typename container_t::size_type>
    void push_empty(container_t& vec, int_t count = 1) {
        for ( ;count > 0; --count) {
            vec.push_back(value_t());
        }
    }

    QString cardToString(gbp_i8 cardIndex);

    template <typename TContainer>
    QStringList toStringList(const TContainer& container) {
        QStringList result;
        for (const typename TContainer::value_type& value: container) {
            result << QString("%0").arg(value);
        }
        return result;
    }
    template <typename TContainer>
    QStringList toStringList(const TContainer& container, std::function<QString(const typename TContainer::value_type&)> conv_func) {
        QStringList result;
        for (const typename TContainer::value_type& value: container) {
            result << conv_func(value);
        }
        return result;
    }
}

