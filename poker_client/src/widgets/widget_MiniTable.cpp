#include <precompiled.h>
#include "widget_MiniTable.h"
#include <basewidgets/util.h>
#include "StatisticTableView.h"
#include "ui_widget_minitable.h"
#include "ui_widget_minitableItem.h"

#include <api/lobby_stat/types.h>
Q_DECLARE_METATYPE(gbp::ns_api::ns_statistic::cash_stat_t)

namespace ns_widget
{
    MinitableItem::MinitableItem(QWidget *parent)
        : QStackedWidget(parent)
        , m_ui(new Ui::miniTableItem)
    {
        m_ui->setupUi(this);
        connect(m_ui->bttn_seat, SIGNAL(clicked(bool)), this, SIGNAL(buttonClicked()));
    }

    QAbstractButton *MinitableItem::buttonItem() const
    {
        return m_ui->bttn_seat;
    }

    int MinitableItem::buttonOffset() const {
        return m_ui->bttn_seat->x() - buttonFreeSpace();
    }

    void MinitableItem::moveButton(int offset)
    {
        int space = buttonFreeSpace();
        offset = qBound(1 - space, offset, space - 1);
        if (buttonOffset() == offset)
            return;
        m_ui->bttn_seat->move(offset + buttonFreeSpace(), m_ui->bttn_seat->y());
        emit buttonMoved(offset);
    }

    int MinitableItem::buttonFreeSpace() const {
        return (m_ui->free->width() - m_ui->bttn_seat->width())/2;
    }

    QString MinitableItem::nickname() const {
        return m_ui->nick->text();
    }

    Money MinitableItem::stack() const
    {
        return Money::fromString(m_ui->money->text());
    }

    bool MinitableItem::isBusy() const
    {
        return currentWidget() == m_ui->busy;
    }

    void MinitableItem::setPosition(int id)
    {
        auto it = m_positions.find(id);
        if (it != m_positions.end()) {
            move(it->second.first);
            moveButton(it->second.second);
        }
    }

    void MinitableItem::savePosition(int id, const std::pair<QPoint, int>& pos)
    {
        m_positions[id] = pos;
    }
    void MinitableItem::setNickname(const QString& nickname) {
        return m_ui->nick->setText(nickname);
    }

    void MinitableItem::setStack(Money stack)
    {
        m_ui->money->setText(stack.toString());
    }


    MiniTable::MiniTable(QWidget *parent)
        : QWidget(parent)
        , m_ui(new Ui::MiniTable)
        , m_gameId(GameId())
        , m_seatsCount(0)
        , m_buttonGroup(this)
    {
        m_ui->setupUi(this);

        for (int i = 0; i < 9; i++) {
            if (MinitableItem* item = findChild<MinitableItem*>(QString("item_%0").arg(i)))
            {
                m_buttonGroup.addButton(item->buttonItem(), i);
                m_items.push_back(item);
            }
        }
        connect(&m_buttonGroup, SIGNAL(buttonClicked(int)), this, SIGNAL(requestTakeASit(int)));
    }

    void MiniTable::init()
    {
        char posProperty[] = "pos_players_?\0";
        char offProperty[] = "buttonOffset_?\0";
        for (auto i : {2, 6, 9}) {
            posProperty[12] = i + 48;
            offProperty[13] = i + 48;

            for (int j = 0; j < 9; j++) {
                MinitableItem* item = m_items[j];
                QVariant currPos = item->property(posProperty);
                QVariant currOffset = item->property(offProperty);
                if (currPos.isValid() && currPos.type() == QVariant::Point) {
                    item->savePosition(i, std::make_pair(currPos.toPoint(), currOffset.toInt()));
                }
            }
        }
    }

    void MiniTable::updateCashesView(const QModelIndex& index)
    {
        if (!index.isValid()) {
            return;
        }

        gbp::ns_api::ns_statistic::cash_stat_t data = index.data(UserType::RowDataRole).value<gbp::ns_api::ns_statistic::cash_stat_t>();

        setGameId(data._id);
        setSeatsCount(data._players_count.second);

        if (data._wait_players > 0) {
            m_ui->waiters_count->setVisible(true);
            m_ui->waiters_count->setNum((int)data._wait_players);
        } else {
            m_ui->waiters_count->setVisible(false);
        }

        for (int i = 0; i < data._players_count.second; i++) {
            m_items[i]->setFree();
        }

        for (gbp::ns_api::ns_statistic::player_t &info : data._players) {
            MinitableItem* item = m_items[info._pos];
            item->setBusy();
            item->setNickname(info._nick.c_str());
            item->setStack(info._money);
        }
    }

    void MiniTable::updateTournamentView()
    {

    }

    const GameId& MiniTable::gameId() const
    {
        return m_gameId;
    }

    int MiniTable::seatsCount() const {
        return m_seatsCount;
    }

    MinitableItem *MiniTable::item(int idx) const
    {
        if (m_items.size() > idx && idx >= 0) {
            return  m_items.at(idx);
        }
        return nullptr;
    }


    void MiniTable::setGameId(const GameId& gameId)
    {
        if (m_gameId == gameId)
            return;

        m_gameId = gameId;
        emit gameIdChanged(gameId);
    }

    void MiniTable::setSeatsCount(int count)
    {
        if (m_seatsCount == count)
            return;
        while (count > m_items.size()) {
            MinitableItem* mtItem = new MinitableItem(this);
            mtItem->move(114, m_items.size()*2);
            mtItem->show();
            m_items.push_back(mtItem);
        }
        for (int i = 0; i < m_items.size(); i++) {
            m_items[i]->setVisible(i < count);
            m_items[i]->setPosition(count);
        }
        m_seatsCount = count;
    }

    void MiniTable::onIndexChanged(const QModelIndex & index)
    {
        if (m_gameId.m_tournamentID == -1) {
            updateCashesView(index);
        } else {
            updateTournamentView();
        }
    }

} //namespace ns_widget
