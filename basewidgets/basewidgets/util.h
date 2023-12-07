#pragma once

#include "basewidgets_global.h"
#include <QObject>
#include <QByteArray>
#include <QPixmap>
#include <QVector3D>
#include <cmath>
#include "types.h"
#include <QMetaEnum>
#include <QTime>
#include <QWidget>
#include <set>
#ifndef M_PI
#define M_PI 3.1415926
#endif // M_PI

#include "../../api_models/api_models/api_util.hpp"

namespace ns_util
{
    template <typename EnumType>
    QString enumToString(EnumType enumValue)
    {
        return QMetaEnum::fromType<EnumType>().valueToKey(underlying_cast(enumValue));
    }

    QString BASEWIDGETS_DLL shortMonthNativeName(int month);
    QString BASEWIDGETS_DLL longMonthNativeName(int month);
    // monday == 1
    QString BASEWIDGETS_DLL shortDayOfWeekNativeName(int dayOfWeek);
    QString BASEWIDGETS_DLL longDayOfWeekNativeName(int dayOfWeek);

//    template <typename T>
//    auto rad2deg(T rad) -> decltype((rad * T(180)) / M_PI)  {
//        return (rad * T(180)) / M_PI;
//    }
//    template <typename T>
//    auto deg2rad(T deg) -> decltype((deg * M_PI) / T(180))  {
//        return (deg * M_PI) / T(180);
//    }

    template <typename T>
    void validatePeriodic(T minVal, T maxVal, T& val) {
        T val1 = val - minVal;
        T maxFromZero = maxVal - minVal;
        if (maxFromZero != 0) {
            auto k = val1 / maxFromZero;
            val1 -= k*maxFromZero;
            val1 = (val1 + maxFromZero) % maxFromZero;
            val = val1 + minVal;
        }
    }

//    QRect BASEWIDGETS_DLL square(const QRect& rc);


    QPixmap BASEWIDGETS_DLL fromBase64(const base64_t &data);
    base64_t BASEWIDGETS_DLL toBase64(const QPixmap &pix);

    template <typename T>
    sha1_t toSha1(const T&);

    template <>
    sha1_t BASEWIDGETS_DLL toSha1(const QString&);
    template <>
    sha1_t BASEWIDGETS_DLL toSha1(const QPixmap&);


    void BASEWIDGETS_DLL setBlendingOption(E::Blending blending, QPainter& p);
    int BASEWIDGETS_DLL getPasswordWeight(const std::string &pass);
//    void BASEWIDGETS_DLL sincosf(float __x, float *p_sin, float *p_cos);

//    template <typename T>
//    std::set<T> range(T from, T to)
//    {
//        std::set<T> resultRange;
//        for (T value = from; value != to; ++value) {
//            resultRange.insert(value);
//        }
//        return resultRange;
//    }
//    template <typename T>
//    QSet<T> qRange(T from, T to)
//    {
//        QSet<T> resultRange;
//        for (T value = from; value != to; ++value) {
//            resultRange.insert(value);
//        }
//        return resultRange;
//    }

//    template <typename EnumType>
//    QSet<typename std::underlying_type<EnumType>::type> enumValues()
//    {
//        QSet<typename std::underlying_type<EnumType>::type> range;
//        QMetaEnum metaEnum = QMetaEnum::fromType<EnumType>();
//        int end = metaEnum.keyCount();
//        if (QLatin1String(metaEnum.key(end - 1)) == "End") {
//            --end;
//        }
//        for (int i = 0; i < end; i++) {
//            range.insert(metaEnum.value(i));
//        }
//        return range;
//    }

//    template <typename T>
//    T* findParent(QWidget* w)
//    {
//        for (QWidget* p = w; p != nullptr; p = p->parentWidget()) {
//            if (T* concreteParent = dynamic_cast<T*>(p)) {
//                return concreteParent;
//            }
//        }
//        return nullptr;
//    }

//    template <typename T>
//    const GameId& inheritedID(QWidget* w)
//    {
//        if (T* p = findParent<T>(w)) {
//            return p->id();
//        }
//        return GameId::invalidID();
//    }
}

//inline const QPixmap *ns_util::PixmapCache::cached(const QPixmap &pixmap) {
//    return cached(toBase64(pixmap), &pixmap);
//}

