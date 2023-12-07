#pragma once

#include "panel_Description.h"


namespace Ui {
class SitNGoDescription;
}

class QSortFilterProxyModel;
namespace ns_model {
class tournament_stat_t;
}
class SitNGoDescription : public Description
{
    Q_OBJECT
private:
    Ui::SitNGoDescription *m_ui;
    QSortFilterProxyModel* m_model;
public:
    explicit SitNGoDescription(QWidget *parent = 0);
    virtual ~SitNGoDescription();

public:
    virtual QTableView *playersList() const override;
    virtual bool isPlayersFetched() const override;

    virtual void onCurrentIdChanged() override;
    virtual void init() override;
    virtual QModelIndex currentIndex() const override;
protected slots:

protected:
    virtual void initLeft() override;
    virtual void initCentral() override;
    virtual void initPlayers() override;
    virtual void initTexts() override {}

    virtual void updateLeft() override;
    virtual void updateCentral() override;
    virtual void updatePlayers() override;
    virtual void updateTexts() override;
    ns_model::tournament_stat_t *sourceModel() const;
protected:
    virtual void changeEvent(QEvent *e) override;

};
