#pragma once
#include "accessor.hpp"
#include "tools_global.h"
#ifdef Q_OS_WIN
#define FUNC_TOOLS_DLL TOOLS_DLL
#else
#define FUNC_TOOLS_DLL
#endif

#include <functional>
#include <qmath.h>
#include <qmetaobject.h>
#include <set>
#include "gameid.h"

template <typename EnumType>
inline QSet<typename std::underlying_type<EnumType>::type> enumValues()
{
    QSet<typename std::underlying_type<EnumType>::type> range;
    QMetaEnum metaEnum = QMetaEnum::fromType<EnumType>();
    int end = metaEnum.keyCount();
    if (QLatin1String(metaEnum.key(end - 1)) == "End") {
        --end;
    }
    for (int i = 0; i < end; i++) {
        range.insert(metaEnum.value(i));
    }
    return range;
}

template <typename T1, typename T2>
inline T1* as(T2* obj) {
    return dynamic_cast<T1*>(obj);
}
template <typename T>
T* get_parent(T* child);
template <>
QObject* FUNC_TOOLS_DLL get_parent(QObject* child);
template <>
QWidget* FUNC_TOOLS_DLL get_parent(QWidget* child);

template <typename T>
struct IdGetter {
    using id_type = gbp_i64;
    constexpr inline static id_type invalidID() {
        return -1;
    }

    constexpr inline static const id_type& get_id(const T& obj) {
        return obj._id;
    }

    template <typename Container> using const_iterator = typename Container::const_iterator;
    template <typename Container> using iterator = typename Container::iterator;

    template <typename Container>
    static const_iterator<Container>
    find_id(const Container& items
          , const typename std::enable_if<std::is_same<typename Container::value_type, T>::value, id_type>::type& id)
    {
        for (auto it = items.begin(); it != items.end(); it++) {
            if (get_id(*it) == id) {
                return it;
            }
        }
        return items.end();
    }

    template <typename Container>
    static iterator<Container>
    find_id(Container& items
          , const typename std::enable_if<std::is_same<typename Container::value_type, T>::value, id_type>::type& id)
    {
        for (auto it = items.begin(); it != items.end(); it++) {
            if (get_id(*it) == id) {
                return it;
            }
        }
        return items.end();
    }
};

template <typename T>
inline Accessor<typename IdGetter<T>::id_type> get_id(const T* obj) {
    return accessor((obj) ? IdGetter<T>::get_id(*obj) : IdGetter<T>::invalidID());
}


namespace ns_util
{
    void FUNC_TOOLS_DLL sincosf(float __x, float *p_sin, float *p_cos);

    template <typename T>
    inline T* findParent(QWidget* w);

    template <typename T>
    inline const typename IdGetter<T>::id_type& inheritedID(QWidget* w);

    //! для виджетов, созданных на форме, используя стандартный конструктор
    //! пример: GameId id = ns_util::findIdFromParent(&GameTable::id, this);
    template <typename Fn, typename R = typename QtPrivate::FunctionPointer<Fn>::ReturnType>
    typename std::remove_reference<R>::type findIdFromParent(Fn idGetter, QWidget* invoker)
    {
        using Object = typename QtPrivate::FunctionPointer<Fn>::Object;
        if (Object* obj = findParent<Object>(invoker)) {
            auto func = std::bind(idGetter, obj);
            return func();
        }
        return typename std::remove_reference<R>::type();
    }

    template <typename T>
    inline std::set<T> range(T from, T to);
    template <typename T>
    inline QSet<T> qRange(T from, T to);

} //namespace ns_util

template <typename T>
inline T* ns_util::findParent(QWidget* w)
{
    for (QWidget* p = w; p != nullptr; p = get_parent(p)) {
        if (T* concreteParent = as<T>(p)) {
            return concreteParent;
        }
    }
    return nullptr;
}

template <typename T>
inline const typename IdGetter<T>::id_type& ns_util::inheritedID(QWidget* w)
{
    return get_id(findParent<T>(w)).ref();
}
/**
 * @brief range of items of type T, [from, to)
 */
template <typename T>
inline std::set<T> ns_util::range(T from, T to)
{
    std::set<T> resultRange;
    for (T value = from; value != to; ++value) {
        resultRange.insert(value);
    }
    return resultRange;
}

template <typename T>
inline QSet<T> ns_util::qRange(T from, T to)
{
    QSet<T> resultRange;
    for (T value = from; value != to; ++value) {
        resultRange.insert(value);
    }
    return resultRange;
}
