#pragma once


#include "page_LobbyBase.h"

namespace Ui {
class LobbyPrizes;
}

namespace ns_page
{
    class LobbyPrizes : public LobbyBase
    {
        Q_OBJECT
    private:
        Ui::LobbyPrizes *m_ui;
    public:
        explicit LobbyPrizes(QWidget *parent = nullptr);
        virtual ~LobbyPrizes();


        virtual QAbstractItemView *view() const override;
        virtual QAbstractItemModel* model() const override;
        virtual QModelIndex rootIndex() const override;
    protected:
        virtual void internalInit() override;
        virtual QAbstractProxyModel* createProxyModel() const override;
    };

} //namespace ns_page
