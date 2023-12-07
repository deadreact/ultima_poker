#pragma once

#include <core/money.h>
#include <basewidgets/entitywidget.h>
#include <qqueue.h>
#include "modelkeyitem.h"

namespace Ui {
class PotItem;
}
class QPropertyAnimation;

class PotItem : public EntityWidget, public ModelKeyItem
{
    Q_OBJECT
    Q_PROPERTY(bool mirrored READ mirrored WRITE setMirrored)
    Q_PROPERTY(gbp_i64 money READ money WRITE setMoney RESET clear)
    Q_PROPERTY(QByteArray modelKey READ modelKey)
    Q_PROPERTY(gbp_i8 modelKeyNumber MEMBER m_modelKeyNumber WRITE setModelKeyNumber)
private:
//    constexpr static const char* sm_modelKey1 = "table_state._all_pots.%0";
    constexpr static const char* sm_modelKey = "[%0,0,[7,0,[0,0,0x0]]]";

    Ui::PotItem *m_ui;
    gbp_i8 m_modelKeyNumber;
    gbp_i64 m_money;

    QQueue<QPropertyAnimation*> m_animQueue;
public:
    explicit PotItem(QWidget *parent = 0);
    virtual ~PotItem();

    virtual QByteArray modelKey() const override;
    virtual void updateData(const QAbstractItemModel* model) override;

    PotItem* clone() const;
    bool mirrored() const;
    gbp_i64 money() const;
    void clear();
public slots:
    void setMirrored(bool mirrored);
    void setMoney(gbp_i64 money);
    void setModelKeyNumber(gbp_i8 n);

protected:
    virtual void changeEvent(QEvent *e) override;
};

