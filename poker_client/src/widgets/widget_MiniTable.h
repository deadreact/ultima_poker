#pragma once

#include <QButtonGroup>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>
#include <basewidgets/types.h>

#include <core/money.h>

namespace Ui {
class MiniTable;
class miniTableItem;
}



namespace ns_widget
{

    class MinitableItem : public QStackedWidget
    {
        Q_OBJECT
        Q_PROPERTY(int buttonOffset READ buttonOffset WRITE moveButton NOTIFY buttonMoved)
        Q_PROPERTY(QString nickname READ nickname WRITE setNickname)
        Q_PROPERTY(Money stack READ stack WRITE setStack)

        Ui::miniTableItem *m_ui;
        std::map<int, std::pair<QPoint, int>> m_positions;
    public:
        MinitableItem(QWidget* parent=0);
        QAbstractButton *buttonItem() const;
        int buttonOffset() const;
        int buttonFreeSpace() const;
        QString nickname() const;
        Money stack() const;
        bool isBusy() const;
        void setPosition(int id);
        void savePosition(int id, const std::pair<QPoint, int> &pos);
        inline void savePosition(int id) {
            savePosition(id, std::make_pair(pos(), buttonOffset()));
        }

    public slots:
        void moveButton(int offset);
        inline void setBusy(bool busy = true) { setCurrentIndex(busy ? 0 : 1); }
        inline void setFree(bool free = true) { setCurrentIndex(free ? 1 : 0); }
        void setNickname(const QString &nickname);
        void setStack(Money stack);
    signals:
        void buttonClicked();
        void buttonMoved(int buttonOffset);
    };

    class MiniTable : public QWidget
    {
        Q_OBJECT
        Q_PROPERTY(GameId gameId READ gameId WRITE setGameId NOTIFY gameIdChanged)
        Q_PROPERTY(int seatsCount READ seatsCount WRITE setSeatsCount)

        Ui::MiniTable *m_ui;
        GameId m_gameId;
        int m_seatsCount;
        QVector<MinitableItem*> m_items;
        QButtonGroup m_buttonGroup;

    public:
        explicit MiniTable(QWidget *parent = 0);

        const GameId &gameId() const;
        int seatsCount() const;
        MinitableItem* item(int idx) const;
    signals:
        void gameIdChanged(const GameId& gameId);
        void requestTakeASit(int pos);
    public slots:
        void setGameId(const GameId& gameId);
        void setSeatsCount(int count);

        void onIndexChanged(const QModelIndex& index);
        void init();


    protected:
        void updateCashesView(const QModelIndex &index);
        void updateTournamentView();
    };
} // namespace ns_widget
