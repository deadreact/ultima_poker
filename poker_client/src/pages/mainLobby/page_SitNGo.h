#pragma once

#include "page_MainLobbyBase.h"

namespace Ui {
class SitNGo;
}
namespace ns_dialog {
    class LobbyTournament;
}
namespace ns_model {
class Tournaments;
}

class QAbstractItemModel;

namespace ns_page
{
    class SitNGo : public MainLobbyBase
    {
        Q_OBJECT
    private:
        Ui::SitNGo *m_ui;
        QMap<gbp_i64, ns_dialog::LobbyTournament*> m_openedLobbies;
    public:
        explicit SitNGo(QWidget *parent = nullptr);
        virtual ~SitNGo();

        virtual void init() override;
        virtual void updateTexts();
        virtual TableView *tableView() const override;
    protected:
        virtual void changeEvent(QEvent *e) override;

        // MainLobbyBase interface
    protected:
        virtual void updateFilters() override;
    protected slots:
        virtual void onItemActivated(const QModelIndex& index) override;
        virtual void onCurrentChanged(const QModelIndex& index) override;
        virtual void disableFilters(bool disabled) override;

        // MainLobbyBase interface
    public:
        virtual void initTableView() override;
    };

} //namespace ns_page
