#pragma once

#include <QAbstractTableModel>
#include <basewidgets/types.h>
#include <QDate>


enum class EBonus : gbp_i8
{
    rakeback = 0,
    status
};
struct RowItemData
{
    QDate accrualDate;             // период 1 неделя date - date + 7days
    struct {
        EBonus bonusType;
        bool isMoneyPrize;
    } desc;
    // у rakeback есть период(7 дней), у status - статус
    union {
        QDate rakebackPeriodStartDate;
        E::VipStatus achievedStatus;
    };
    // приз или деньги или билеты
    union {
        gbp_i64 money;
        int ticketsCount;
    };

    RowItemData()
        : accrualDate(QDate::currentDate())
        , desc{EBonus::rakeback, true}
        , rakebackPeriodStartDate(QDate::currentDate())
        , money(0)
    {}

    static RowItemData RakebackBonus(const QDate& periodStartDate, const QDate& accuralDate, gbp_i64 prizeMoney) {
        RowItemData d;
        d.desc.bonusType = EBonus::rakeback;
        d.desc.isMoneyPrize = true;
        d.rakebackPeriodStartDate = periodStartDate;
        d.accrualDate = accuralDate;
        d.money = prizeMoney;

        return d;
    }
    static RowItemData RakebackBonus(const QDate& periodStartDate, const QDate& accuralDate, int prizeTickets) {
        RowItemData d;
        d.desc.bonusType = EBonus::rakeback;
        d.desc.isMoneyPrize = false;
        d.rakebackPeriodStartDate = periodStartDate;
        d.accrualDate = accuralDate;
        d.ticketsCount = prizeTickets;

        return d;
    }

    static RowItemData StatusBonus(E::VipStatus achievedStatus, const QDate& accuralDate, gbp_i64 prizeMoney) {
        RowItemData d;
        d.desc.bonusType = EBonus::status;
        d.desc.isMoneyPrize = true;
        d.achievedStatus = achievedStatus;
        d.accrualDate = accuralDate;
        d.money = prizeMoney;

        return d;
    }
    static RowItemData StatusBonus(E::VipStatus achievedStatus, const QDate& accuralDate, int prizeTickets) {
        RowItemData d;
        d.desc.bonusType = EBonus::status;
        d.desc.isMoneyPrize = false;
        d.achievedStatus = achievedStatus;
        d.accrualDate = accuralDate;
        d.ticketsCount = prizeTickets;

        return d;
    }

};


namespace ns_model
{
    class vip_system_info;

    class RakebackHistory : public QAbstractTableModel
    {
        Q_OBJECT
    public:

    private:
//        QList<RowItemData> m_data;

        vip_system_info* m_sourceModel;
    public:
        explicit RakebackHistory(QObject *parent = nullptr);

        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        void refresh();

        virtual void setSourceModel(vip_system_info* model);

        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    protected slots:
        void onSourceDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);
        void onSourceRowsAboutToBeInserted(const QModelIndex& parent,int first, int last);
        void onSourceRowsInserted(const QModelIndex& parent, int first, int last);
        void onSourceRowsAboutToBeRemoved(const QModelIndex& parent,int first, int last);
        void onSourceRowsRemoved(const QModelIndex& parent, int first, int last);
    };
} //namespace ns_model

