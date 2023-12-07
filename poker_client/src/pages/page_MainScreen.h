#pragma once

#include <QWidget>
#include "widgets/MainWindow.h"

namespace Ui {
class MainScreen;
}
namespace ns_widget {
class PlayerInfo;
}
class QTableView;

namespace ns_page
{
    class CashTables;
    class Tournaments;
    class SitNGo;

    class MainScreen : public QWidget
    {
        Q_OBJECT
    private:
        Ui::MainScreen *m_ui;
        QPropertyAnimation* m_animation;
//        QGraphicsBlurEffect* m_blurEffect;
        ns_widget::PlayerInfo* m_players[9];
    public:
        explicit MainScreen(QWidget *parent = 0);
        virtual ~MainScreen();
        virtual void init();

        Ui::MainScreen* ui() const;
        TableView *statisticTable() const;

        CashTables* cashTablesPage() const;
        Tournaments* tournamentsPage() const;
        SitNGo* sitNGoPage() const;
    protected:
        void changeEvent(QEvent *e);
    protected slots:
        void toggleAnimationDirection();
        void updateTexts() const;
        void onTableItemDoubleClicked(const QModelIndex& modelIndex);
        void onLogoutActionTriggered();
    public slots:
        void enableBlur();
        void disableBlur();
        void toggleTableView();
        void setAvatar(const QIcon& icon);
    };

}// namespace page

