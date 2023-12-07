#pragma once

#include "page_LobbyBase.h"

namespace Ui {
class LobbyTables;
}

namespace ns_page
{

class LobbyTables : public LobbyBase
{
    Q_OBJECT
private:
    Ui::LobbyTables *m_ui;
public:
    explicit LobbyTables(QWidget *parent = nullptr);
    virtual ~LobbyTables();


    virtual QAbstractItemView *view() const override;
    virtual QAbstractItemModel* model() const override;
    virtual QModelIndex rootIndex() const override;
protected:
    virtual QAbstractProxyModel* createProxyModel() const override;
    virtual void internalInit() override;
};
} //namespace ns_page

