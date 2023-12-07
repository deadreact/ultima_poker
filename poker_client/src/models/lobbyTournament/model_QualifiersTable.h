#pragma once

#include "model_LobbyBase.h"
#include <tools/item_reference.hpp>

namespace ns_model
{

    class QualifiersTable : public LobbyBase
    {
        Q_OBJECT
    private:
        QModelIndex m_sourceRootIndex;
    public:
        explicit QualifiersTable(QObject *parent = 0);
        virtual ~QualifiersTable();


        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        TournamentRef rowData(int idx) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal, int role = Qt::DisplayRole) const override;
    protected:
        virtual void onReset() override;
    };

} //namespace ns_model
