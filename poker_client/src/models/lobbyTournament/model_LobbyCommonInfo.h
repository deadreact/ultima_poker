#pragma once

#include "model_LobbyBase.h"

namespace ns_model
{
    class tourn_additional_stat_t;

    class LobbyCommonInfo : public LobbyBase
    {
        Q_OBJECT
    public:
        LobbyCommonInfo(QObject *parent = nullptr);

        QVariant data(const QModelIndex &modelIndex, int role = Qt::DisplayRole) const override;


        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual bool isRowDisplayed(int index) const override;

        tourn_additional_stat_t* additional_statistic() const;
    protected:
        QModelIndex rebuysCount() const;
        QModelIndex addonsCount() const;

        virtual void onReset() override;
    };

} // namespace ns_model
