#pragma once

#include <qbytearraylist.h>
#include <qstring.h>
#include "gbp_registrator.hpp"

struct ModelDataIndex
{
    const ModelDataIndex* m_parent;
    QByteArray m_key;
    QList<ModelDataIndex*> m_children;

    ModelDataIndex(const QByteArray& key, const ModelDataIndex* parent = nullptr);
    ~ModelDataIndex();

    void clearChildren();

    const ModelDataIndex* parent() const;
    const QByteArray& key() const;
    const QList<ModelDataIndex*>& children() const;
    bool isRoot() const;
    int childrenCount() const;

    QString traceDown(int* rowsNeeded = nullptr) const;
    QString traceUp() const;
    QString traceTotal() const;
};
