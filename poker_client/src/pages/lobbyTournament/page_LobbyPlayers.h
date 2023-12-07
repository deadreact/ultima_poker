#pragma once

#include "page_LobbyBase.h"

namespace Ui {
class LobbyPlayers;
}
class QSortFilterProxyModel;

namespace ns_model {
class players_list;
}

namespace ns_page
{
    class LobbyPlayers : public LobbyBase
    {
        Q_OBJECT
    private:
        Ui::LobbyPlayers *m_ui;
//        QSortFilterProxyModel* m_proxyModel;
    public:
        explicit LobbyPlayers(QWidget *parent = nullptr);
        virtual ~LobbyPlayers();

        virtual QAbstractProxyModel* createProxyModel() const override;

        virtual QAbstractItemView *view() const override;
        virtual QAbstractItemModel* model() const override;
        virtual QModelIndex rootIndex() const override;
    public slots:
        void filterPlayers(const QString &fixedString);
    protected:
        virtual void internalInit() override;
        virtual void showEvent(QShowEvent *e) override;
        virtual void adjustHeader() override;
    private:
        ns_model::players_list* playersListModel() const;
        QSortFilterProxyModel* proxyModel() const;
    };

} //namespace ns_page
