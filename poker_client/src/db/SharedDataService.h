#pragma once

#include <QObject>
#include <map>
#include "GameTableSettingsData.h"
#ifndef Q_MOC_RUN
    // #include <api/lobby_stat/types.h>
#endif // !Q_MOC_RUN

//#include <memory>
#include <functional>

class SharedDataService;
typedef std::pair<gbp_i64/*userId*/, std::map<gbp_i64/*tableId*/, int/*user pos in queue*/>> waiter_t;


SharedDataService& shdService();

class SharedDataService
{
private:
    QObject* m_qobject; // предпочел композицию
    std::map<quintptr, void*> m_dataMap;
    std::map<quintptr, std::function<void()> > m_deallocators;

    void logWarning(const QString& msg, const char* file, int line);
public:
    explicit SharedDataService(QObject *parent = nullptr);
    virtual ~SharedDataService();

    template <typename T>
    quintptr singleIdForType() {
        static const quintptr id = createSharedData<T>();
        return id;
    }

    template <typename T>
    quintptr createSharedData()
    {
        T* sharedData = new T;
        quintptr id = quintptr(sharedData);
        m_dataMap.insert(std::make_pair(id, sharedData));
        QString msg = QString("Shared Data created with id = %0, allocated %1 bytes.").arg(id).arg(sizeof(T));

        logWarning(msg, __FILE__, __LINE__);
        m_qobject->connect(m_qobject, &QObject::destroyed, [sharedData]{ delete sharedData; });

        return id;
    }

    template <typename T>
    T* getData(quintptr id) const {
        auto it = m_dataMap.find(id);
        if (it == m_dataMap.end()) {
            return nullptr;
        }
        return static_cast<T*>(it->second);
    }

    template <typename T>
    void updateData(quintptr id, const T& data) const {
        if (T* oldData = getData<T>(id)) {
            *oldData = data;
        }
    }

    template <typename T>
    T* getSingleDataForType() {
        return getData<T>(singleIdForType<T>());
    }

    template <typename T>
    quintptr updateSingleData(const T& data) {
        quintptr id = singleIdForType<T>();
        updateData(id, data);
        return id;
    }

    template <typename T>
    void eraseData(quintptr id) {
        std::map<quintptr, void*>::iterator it = m_dataMap.find(id);
        if (it != m_dataMap.end()) {
            T* concreteData = static_cast<T*>(it->second);
            delete concreteData;
            m_dataMap.erase(it);
        }
    }
};
