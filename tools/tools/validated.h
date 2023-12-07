#pragma once

#include "tools_global.h"
#include "types_fwd.hpp"
#include <QDebug>

struct CountingObject
{
    gbp_i32 m_objCreated;
    gbp_i32 m_objDeleted;
    gbp_i64 m_memAllocated;
    gbp_i64 m_memDeallocated;

    CountingObject()
        : m_objCreated(0)
        , m_objDeleted(0)
        , m_memAllocated(0)
        , m_memDeallocated(0)
    {}

    ~CountingObject();
};

template <typename T>
class Validated
{
    using type = T;
    static CountingObject data;
public:
    Validated() {
        data.m_objCreated++;
        data.m_memAllocated += sizeof(T);
//        qDebug() << "currentObjectsCount:" << currentObjectsCount();
//        qDebug() << "currentMemoryUsage:" << currentMemoryUsage();
    }

    virtual ~Validated();

    static gbp_i32 currentObjectsCount() {
        return data.m_objCreated - data.m_objDeleted;
    }
    static gbp_i64 currentMemoryUsage() {
        return data.m_memAllocated - data.m_memDeallocated;
    }
};

template<typename T>
Validated<T>::~Validated() {
    data.m_objDeleted++;
    data.m_memDeallocated += sizeof(T);
    //        qDebug() << "currentObjectsCount:" << currentObjectsCount();
    //        qDebug() << "currentMemoryUsage:" << currentMemoryUsage();
}


