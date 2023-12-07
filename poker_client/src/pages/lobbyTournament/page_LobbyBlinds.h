#pragma once

#include "page_LobbyBase.h"

namespace Ui {
class LobbyBlinds;
}
class QIdentityProxyModel;
namespace ns_model {
class tourn_additional_stat_t;
} //namespace ns_model
namespace ns_page
{
    class LobbyBlinds : public LobbyBase
    {
        Q_OBJECT
    private:
        Ui::LobbyBlinds *m_ui;
//        QScopedPointer<QIdentityProxyModel> m_proxyModel;
    public:
        explicit LobbyBlinds(QWidget *parent = nullptr);
        virtual ~LobbyBlinds();


        virtual QAbstractItemView* view() const override;
        virtual QAbstractItemModel* model() const override;
        virtual QModelIndex rootIndex() const override;

//        ns_model::tourn_additional_stat_t *sourceModel() const;
    protected:
        virtual QAbstractProxyModel* createProxyModel() const override;
        virtual void internalInit() override;
    };

} //namespace ns_page
