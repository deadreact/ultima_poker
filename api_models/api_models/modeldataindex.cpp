#include "modeldataindex.h"
#include <iostream>


ModelDataIndex::ModelDataIndex(const QByteArray &key, const ModelDataIndex *parent)
    : m_parent(parent)
    , m_key(key)
{
//    std::cout << "created " << traceUp().toStdString() << std::endl;
}

ModelDataIndex::~ModelDataIndex()
{
    clearChildren();
}

void ModelDataIndex::clearChildren()
{
    if (!m_children.empty()) {
        qDeleteAll(m_children);
    }
    m_children.clear();
}

const ModelDataIndex *ModelDataIndex::parent() const {
    return m_parent;
}

const QByteArray &ModelDataIndex::key() const {
    return m_key;
}

const QList<ModelDataIndex *> &ModelDataIndex::children() const {
    return m_children;
}

bool ModelDataIndex::isRoot() const {
    return m_parent == nullptr;
}

int ModelDataIndex::childrenCount() const {
    return m_children.size();
}

//root_|_key1_|_key1_|_key1-*
//     |      |      |_key2_|_key-*
//     |      |_key2_
//     |      |_key3_|_key1-*
//     |             |_key2-*
//     |_key2-*


QString ModelDataIndex::traceDown(int *rowsNeeded) const {
    if (m_children.empty()) {
        if (rowsNeeded) {
            *rowsNeeded = 1;
        }
        return m_key + "-*";
    }
    QStringList lst;
    int childRowsNeeded = 0;
    int totalRowsNeeded = 0;
    for (ModelDataIndex* child: m_children) {
        lst << child->traceDown(&childRowsNeeded);
        totalRowsNeeded += childRowsNeeded;
    }
    if (rowsNeeded) {
        *rowsNeeded = totalRowsNeeded;
    }
    int offset = m_key.size() + 3;
    QString prefix = QString(" ").repeated(offset - 2) + "| ";
    return m_key + "_|_" + lst.join("\n" + prefix);
}
QString ModelDataIndex::traceUp() const {
    if (isRoot()) {
        return "ROOT";
    }
    return m_parent->traceUp() + "-->" + m_key;
}
QString ModelDataIndex::traceTotal() const {
    return traceUp() + "_|_" + traceDown();
}
