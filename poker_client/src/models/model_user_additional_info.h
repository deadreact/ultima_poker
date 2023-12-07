#pragma once

#include <api_models/model_user_info_t.hpp>
#include <api_models/model_io.hpp>
#include <qstandarditemmodel.h>

struct CategoryItem : public QStandardItem
{
    CategoryItem(const QString& name, int rows);
    virtual ~CategoryItem();
    virtual void read(QDataStream& stream) = 0;
    virtual void write(QDataStream& stream) const = 0;
    virtual void load();
    virtual void save() const;

    virtual CategoryItem* init(QStandardItem* parent) = 0;
    virtual QStandardItem* field(const QString& name) const = 0;
    QVariant fieldData(const QString& name, int role) const;
    void setField(const QString& name, const QVariant& value, int role = Qt::EditRole);

    virtual QString fieldName(QStandardItem* child) const = 0;
    virtual bool contains(QStandardItem* child) const;

    inline Qt::CheckState fieldCheckState(const QString& name) const {
        return qvariant_cast<Qt::CheckState>(fieldData(name, Qt::CheckStateRole));
    }
    inline QString fieldText(const QString& name) const {
        return fieldData(name, Qt::DisplayRole).toString();
    }
    inline QString fieldEditText(const QString& name) const {
        return fieldData(name, Qt::EditRole).toString();
    }

    const QString& name() const;
private:
    const QString m_name;
public:
    virtual QVariant data(int role) const override;
    virtual void setData(const QVariant &value, int role) override;
};

namespace ns_model {
    class UserInfo : public QStandardItemModel, public IOModel
    {
        Q_OBJECT
        struct PrivateData;
        PrivateData* m_data;

    public:
        enum class field
        {
            waiter_pos
        };
        Q_ENUM(field)
    public:
        UserInfo(QObject* parent = nullptr);
        virtual ~UserInfo();

        virtual void init();
        QStandardItem* getItem(const QString& itemKey, const QString& category = "common");
        CategoryItem* category(const QString& categoryName) const;
    public:
        virtual bool read(QIODevice *device) override;
        virtual bool write(QIODevice *device) const override;
        bool backup(const QString&) const;
        bool restore(const QString&);

        bool saveCategory(const QString& categoryName) const;
        bool restoreCategory(const QString& categoryName);

        QStringList categories() const;
        bool isInitialized() const;

        void printAll();
    public slots:
        void onUserInfoAboutToBeReset();
        void onUserInfoReset();
        void onUserInfoDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);

    };
} // namespace ns_model
