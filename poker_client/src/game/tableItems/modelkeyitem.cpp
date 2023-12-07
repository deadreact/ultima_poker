#include <precompiled.h>
#include <qabstractproxymodel.h>
#include <api_models/model_apibase.hpp>
#include "modelkeyitem.h"

QModelIndex ModelKeyItem::index(const QAbstractItemModel *model) const
{
    if (const ns_model::ApiBase* m = qobject_cast<const ns_model::ApiBase*>(model)) {
        return m->keyToIndex(modelKey());
    }
    if (const QAbstractProxyModel* m = qobject_cast<const QAbstractProxyModel*>(model)) {
        return index(m->sourceModel());
    }
    return QModelIndex();
}
