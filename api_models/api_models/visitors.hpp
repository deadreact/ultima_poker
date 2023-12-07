#pragma once
#include <qvariant.h>
#include <sstream>
#include <iostream>
#include <qvector.h>
#include "modeldataindex.h"
#include "../../common/api/gbp_int.hpp"

struct Converter {
private:
    template <typename T>
    struct Reader {
        T data;
        template <typename TVal>
        void operator ()(const char* key, const TVal& value) {
            data.insert(QByteArray(key), QVariant::fromValue(value));
        }
    };



    template <typename T>
    struct Writer {
        const T& data;
        mutable QByteArrayList modifiedFields;
        Writer(T& data): data(data) {}

        template <typename TVal>
        void operator ()(const char* key, TVal& value) const {
            QByteArray name(key);
            auto it = data.find(name);
            if (it != data.end()) {
                QVariant v = it.value();
                if (v.canConvert<TVal>()) {
                    TVal newValue = qvariant_cast<TVal>(it.value());
                    if (value != newValue) {
                        value = newValue;
                        modifiedFields << name;
                    }
                }
            }
        }
    };

public:
    template <typename VarType, typename ApiType>
    VarType toVariants(const ApiType& data) {
        Reader<VarType> reader;
        data.apply(reader);
        return reader.data;
    }
    template <typename ApiType>
    QVariantList toVariantList(const ApiType& data);

    template <typename VarType, typename ApiType>
    QByteArrayList fromVariants(ApiType& obj, const VarType& data) {
        Writer<VarType> writer(data);
        obj.apply(writer);
        return writer.modifiedFields;
    }

};

template <>
struct Converter::Reader<QVariantList> {
    QVariantList data;
    template <typename TVal>
    void operator ()(const char* /*key*/, const TVal& value) {
        data << QVariant::fromValue(value);
    }
};

template <typename ApiType>
QVariantList Converter::toVariantList(const ApiType& data) {
    Reader<QVariantList> reader;
    data.apply(reader);
    return reader.data;
}


struct Comparator {
    mutable QMap<QByteArray, QPair<QVariant, QVariant>> m_changes;

    template <typename T>
    void operator()(const char* key, const T& first, const T& second) const {
        m_changes.insert(key, qMakePair(QVariant::fromValue(first), QVariant::fromValue(second)));
    }

    template <typename T>
    bool compare(const T& data1, const T& data2) const {
        m_changes.clear();
        return data1.compare(data2, *this);
    }

    const QMap<QByteArray, QPair<QVariant, QVariant>>& changes() const {
        return m_changes;
    }
};


template <typename Type, bool isGBP = GbpRegistrator<Type>::is_registered >
struct ModelDataSetter
{
    ModelDataIndex* const m_dataIndex;

    ModelDataSetter(ModelDataIndex* index)
        : m_dataIndex(index)
    {
//        std::cout << "created unregistered" << index->traceUp().toStdString() << std::endl;
    }
    int setData(Type& target, const Type& source) {
        target = source;
        return 1;
    }
    int setData(Type& target, const QVariant& source) {
        if (!source.canConvert<Type>()) {
            return 0;
        }
        return setData(target, qvariant_cast<Type>(source));
    }
};

template <typename Type>
struct ModelDataSetter<Type, true>
{
    ModelDataIndex* const m_dataIndex;

    ModelDataSetter(ModelDataIndex* index)
        : m_dataIndex(index)
    {
//        std::cout << "created registered" << index->traceUp().toStdString() << std::endl;
    }

    template <typename MemberType>
    void operator()(const char* key, MemberType& first, const MemberType& second) {
        ModelDataIndex* child = new ModelDataIndex(key, m_dataIndex);
        ModelDataSetter<MemberType> childSetter(child);
        childSetter.setData(first, second);
        m_dataIndex->m_children << child;
    }

    int setData(Type& target, const Type& source) {
        m_dataIndex->clearChildren();
        if (!target.compare(source, *this)) {
            return 0;
        }
        return m_dataIndex->childrenCount();
    }

    int setData(Type& target, const QVariant& source) {
        if (!source.canConvert<Type>()) {
            return 0;
        }
        return setData(target, qvariant_cast<Type>(source));
    }

    QByteArrayList changes() const {
        QByteArrayList lst;
        for (ModelDataIndex* child: m_dataIndex->children()) {
            lst << child->key();
        }
        return lst;
    }
};

template <typename T>
struct PropertyIndex {
    T obj;
    QVector<QByteArray> keys;
    QMap<QByteArray, int> indices;
    QVector<std::ptrdiff_t> offsets;
//    QMap<QByteArray, int> map;

    PropertyIndex()
        : obj()
    {
        obj.apply(*this);
    }
    PropertyIndex(T&& _obj)
        : obj(std::move(_obj))
    {
        obj.apply(*this);
    }

    template <typename PropertyType>
    void operator()(const char* name, const PropertyType& value) {
//        std::cout << "index of property \"" << name << "\": " << indices.size() << std::endl;
        keys.push_back(name);
        indices.insert(name, indices.size());
        offsets.push_back(reinterpret_cast<const gbp_u8*>(&value) - reinterpret_cast<const gbp_u8*>(&obj));
//        qDebug() << QStringLiteral("offset of") << qPrintable(name) << QStringLiteral("is") << offsets.back();
    }

    int count() const {
        return keys.size();
    }
    QByteArray keyAt(int index) const {
        return keys.at(index);
    }

    bool contains(const QByteArray& key) const {
        return indices.contains(key);
    }
    int indexOf(const QByteArray& key) const {
        if (contains(key)) {
            return indices.value(key);
        }
        return -1;
    }
    template <typename PropertyType>
    PropertyType* value(const QByteArray& key, T& data) {
        if (!indices.contains(key)) {
            return nullptr;
        }
        return reinterpret_cast<PropertyType*>(reinterpret_cast<gbp_u8*>(&data) + offsets.at(indexOf(key)));
    }
    template <typename PropertyType>
    const PropertyType* value(const QByteArray& key, const T& data) const {
        if (!indices.contains(key)) {
            return nullptr;
        }
        return reinterpret_cast<const PropertyType*>(reinterpret_cast<const gbp_u8*>(&data) + offsets.at(indexOf(key)));
    }

    template <typename PropertyType>
    PropertyType* value(int index, T& data) const {
        if (index >= 0 && index < offsets.size()) {
            return reinterpret_cast<const PropertyType*>(reinterpret_cast<const gbp_u8*>(&data) + offsets.at(index));
        }
        return nullptr;
    }
    template <typename PropertyType>
    const PropertyType* value(int index, const T& data) const {
        if (index >= 0 && index < offsets.size()) {
            return reinterpret_cast<const PropertyType*>(reinterpret_cast<const gbp_u8*>(&data) + offsets.at(index));
        }
        return nullptr;
    }

//    QVariant variantValue(int index, const T& data) {

//    }

//    QMap<QByteArray, QVariant> values(const T& data) const
//    {
//        QMap<QByteArray, QVariant> valuesMap;
//        for (const QByteArray& key: keys) {
//            valuesMap.insert(key, QVariant::fromValue(*value(key, data)));
//        }
//        return valuesMap;
//    }
};

constexpr static const char* teststyle = "<p style=\" background-color: #37474f; font-size:12px; font-weight:500;\"><span style=\" color:#f8f8f2;\">&lt;"
                        "</span><span style=\" color:#f92672;\">property </span><span style=\" color:#a6e22e;\">"
                        "key </span><span style=\" color:#f8f8f2;\">= </span><span style=\" color:#e6db74;\">"
                        "&quot;%0&quot;</span><span style=\" color:#f8f8f2;\">,</span><span style=\" color:#a6e22e;\">"
                        "value =</span><span style=\" color:#ae81ff;\">%1</span><span style=\" color:#f8f8f2;\">&gt;</span></p>";

template <typename TargetType>
struct Printer
{
    TargetType& target;
    QString pattern;

    Printer(TargetType& target, const QString& pattern = QString("key: %0, value: %1;")): target(target), pattern(pattern) {}

    template <typename T>
    void operator()(const char* key, const T& value) const {
        print(key, value);
    }
    template <typename T>
    void print(const char* key, const T& value) const {
        std::ostringstream os;
        os << value;
        target << pattern.arg(key).arg(os.str().c_str());
    }

    template <typename T>
    Printer& operator<<(const T& data) {
        data.apply(*this);
        return *this;
    }
    template <typename TargetTp>
    static Printer<TargetTp> printTo(TargetTp& target, const QString& pattern = QString("key: %0, value: %1;")) {
        Printer<TargetTp> printer(target, pattern);
        return printer;
    }
};

template <typename TargetType>
Printer<TargetType> printTo(TargetType& target, const QString& pattern = QString(teststyle)) {
    return Printer<TargetType>::printTo(target, pattern);
}
