#pragma once

#include <basewidgets/entitywidget.h>
#include <core/money.h>
#include <qqueue.h>
#include "modelkeyitem.h"

namespace Ui {
class BetItem;
}

class QPropertyAnimation;

class BetItem : public EntityWidget, public ModelKeyItem
{
    Q_OBJECT
    Q_PROPERTY(gbp_i64 money READ money WRITE setMoney NOTIFY moneyChanged RESET clear)
    Q_PROPERTY(QByteArray modelKey READ modelKey)
    Q_PROPERTY(gbp_i8 modelKeyNumber MEMBER m_modelKeyNumber WRITE setModelKeyNumber)
private:
//    constexpr static const char* sm_modelKey = "table_state._players.%0._round_bet";
    constexpr static const char* sm_modelKey = "[%0,11,[4,0,[0,0,0x0]]]";
public:
private:
    Ui::BetItem *m_ui;
    gbp_i8 m_modelKeyNumber;
    gbp_i64 m_money;
    QQueue<QPropertyAnimation*> m_animQueue;
public:
    explicit BetItem(QWidget *parent = nullptr);
    explicit BetItem(gbp_i8 n, QWidget *parent = nullptr);
    virtual ~BetItem();

    virtual QByteArray modelKey() const override;
    Q_SLOT virtual void updateData(const QAbstractItemModel* model) override;

    gbp_i64 money() const;

signals:
    void moneyChanged(gbp_i64 money);
public slots:
    void setMoney(gbp_i64 money);
    void clear();
    void setModelKeyNumber(gbp_i8 n);

protected:
    virtual void moveEvent(QMoveEvent *e) override;
};
