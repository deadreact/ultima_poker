#pragma once

#include "abstractgamehandler.h"

class GameTable;
class TableOver;
class QAbstractItemDelegate;

// карты, банки, ставки, дилер
class TableSurfaceViewHandler : public GameViewHandler
{
    Q_OBJECT

private:
    QModelIndexRangeList m_indexFilter;
public:
    enum class ItemType : gbp_i8 {
        Unknown = -1,
        Card,
        Pot,
        Bet,
        Dealer
    };
    Q_ENUM(ItemType)


    ItemType type(const QModelIndex& index) const;
    ItemType type(const QModelIndexRange& range) const;
//    TableOver* widget() const;

public:
    explicit TableSurfaceViewHandler(const GameId& gameID, QObject *parent = nullptr);
    virtual ~TableSurfaceViewHandler();

    virtual bool indexAffectsView(const QModelIndex &index) const override;
public slots:
    virtual void initialize() override;
    virtual void timelineEvent(const TimeLineEvent &ev) override;
    virtual void onModelAboutToBeReset() override;
    virtual void onModelReset() override;
    virtual void onModelDataChanged(const QModelIndexRange &range) override;
    virtual void onModelAboutToInsertItems(const QModelIndex &parent) override;
    virtual void onModelInsertItems(const QModelIndexRangeList &items) override;
    virtual void onModelAboutToRemoveItems(const QModelIndexRangeList &items) override;
    virtual void onModelRemoveItems(const QModelIndex &parent) override;

//    bool assignWidget(QWidget* widget, const QModelIndex &index);

    void assign(GameTable* gameTable);
public:
    QAbstractItemDelegate* getCardDelegate() const;
protected:
//    void connectPots(const QModelIndexRange &range);
public:
    virtual void openTable() override;
    virtual void closeTable() override;
};
