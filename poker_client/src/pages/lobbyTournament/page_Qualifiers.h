#pragma once

#include "page_LobbyBase.h"

#include <models/lobbyTournament/model_QualifiersTable.h>

namespace Ui {
class Qualifiers;
}

namespace ns_page
{
    class Qualifiers : public LobbyBase
    {
        Q_OBJECT
        Q_PROPERTY(int headerHeight READ headerHeight)
    private:
        Ui::Qualifiers *m_ui;
        ns_model::QualifiersTable* m_model;
    public:
        explicit Qualifiers(QWidget *parent = nullptr);
        virtual ~Qualifiers();

        virtual QAbstractItemView *view() const override;
        virtual QAbstractItemModel *model() const override;
        virtual QModelIndex rootIndex() const override;

        int headerHeight() const;
    protected:
        virtual void internalInit() override;
    };

} //namespace ns_page
