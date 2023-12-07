#pragma once

#include "page_MainLobbyBase.h"
#include <QPushButton>
#include <qcombobox.h>
#include <utils/IconProvider.h>

namespace Ui {
    class Tournaments;
}
namespace ns_dialog {
    class LobbyTournament;
}

class TournTableView;

namespace ns_model {
    class ApiBase;
}

namespace ns_page
{
    class Tournaments : public MainLobbyBase
    {
        Q_OBJECT
    private:
        Ui::Tournaments *m_ui;
        QMap<gbp_i64, ns_dialog::LobbyTournament*> m_openedLobbies;
    public:
        explicit Tournaments(QWidget *parent = nullptr);
        virtual ~Tournaments();

        virtual void init() override;
        virtual void initTableView() override;
        virtual TableView *tableView() const override;

        void updateTexts();

//        QString filterName(UserType::EFilters f) const;
//        QStringList filterNames(UserType::FilterFlags flags);
    protected:
        virtual void changeEvent(QEvent *e) override;
        virtual void showEvent(QShowEvent *e) override;
        virtual void updateFilters() override;
    protected slots:
        virtual void onItemActivated(const QModelIndex& index) override;
        virtual void onCurrentChanged(const QModelIndex& index) override;
        virtual void disableFilters(bool disabled) override;

    };

}// namespace page

