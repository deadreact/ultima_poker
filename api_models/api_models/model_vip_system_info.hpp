#pragma once

#include "api_models_global.h"
#include <QAbstractListModel>
#include "../poker_client/src/core/money.h"

#ifndef Q_MOC_RUN
    #include <api/api_common.h>
    #include <api/lobby/types.h>
#endif // !Q_MOC_RUN

Q_DECLARE_METATYPE(gbp::ns_api::ns_lobby::vip_system_info)
Q_DECLARE_METATYPE(gbp::ns_api::ns_lobby::vip_system_info::bonus_item)

namespace ns_model
{
    class API_MODEL_DLL vip_system_info : public QAbstractListModel
    {
        Q_OBJECT

    public:
        enum class field : int {
            _current_vip_status = 0,
            _current_rake,
            _current_status_money_left,
            _rakeback_current_percent,
            _rakeback_current,
            _rakeback_current_month,
            _rakeback_last_month,
            _rakeback_all_time,
            _bonuses,

            count
        };
        Q_ENUM(field)

        enum class bonus_item : int {
            _type = 0,
            _datetime,
            _rakeback_money,
            _tickets_count,
            _rakeback_period,
            _status_bonus_month,

            count
        };
        Q_ENUM(bonus_item)

    private:
         QScopedPointer<gbp::ns_api::ns_lobby::vip_system_info> m_data;
         QMap<QString, QMap<int, QVariant>> m_dynamicData;

         static QString indexToKey(const QModelIndex& idx);
         QModelIndex keyToIndex(const QString &key) const;

    public:
        explicit vip_system_info(QObject *parent = nullptr);
        virtual ~vip_system_info();

        void resetData(gbp::ns_api::ns_lobby::vip_system_info *newData);

        virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        virtual QVariant additionalData(const QModelIndex& index, int role = Qt::DisplayRole) const;
        virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        QString text(field f) const;
        QString text(int row, bonus_item f) const;

        gbp_i64 money(field f) const;

        gbp::ns_vip_system::e_bonus bonusTypeAtRow(int row) const;

        virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

        QModelIndex bonusItemIndex() const;
        virtual bool hasChildren(const QModelIndex &parent) const override;
        virtual QModelIndex parent(const QModelIndex &child) const override;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

        // QAbstractItemModel interface
    public:
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        virtual QMap<int, QVariant> itemData(const QModelIndex &index) const override;
        virtual bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles) override;
        virtual bool insertRows(int row, int count, const QModelIndex &parent) override;
        virtual bool removeRows(int row, int count, const QModelIndex &parent) override;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

        bool isBonusItem(const QModelIndex& index) const;
    };

    namespace ns_test
    {
        gbp::ns_api::ns_lobby::vip_system_info* createRandomData();
    }

} //namespace ns_model
