#pragma once
#include <QWidget>
#include "models/model_PlayersListModel.h"
#include "panel_Description.h"
#include <QTableView>
#include <rpcbase/defs.h>

namespace Ui {
class TournDescription;
}
namespace ns_model {
class tournament_stat_t;
}
namespace ns_page {
    class Qualifiers;
}

class QSortFilterProxyModel;

class TournDescription : public Description
{
    Q_OBJECT
private:
    using ProxyModelType = QSortFilterProxyModel;
    Ui::TournDescription* m_ui;
    ProxyModelType *m_model;
    QMap<gbp_i64, ns_page::Qualifiers*> m_openedQualifiers;
public:
    TournDescription(QWidget* parent = nullptr);
    virtual ~TournDescription();

//    virtual ns_model::tournament_stat_t *model() const override;
//    void setModel(ns_model::tournament_stat_t *model);

    virtual void init() override;
    virtual QModelIndex currentIndex() const override;

    virtual void onCurrentIdChanged() override;
public slots:
    virtual void openCurrentQualifiers();
protected:
    virtual QTableView *playersList() const override;
    virtual bool isPlayersFetched() const override;
    virtual void changeEvent(QEvent *e) override;

    virtual void initLeft() override;
    virtual void initCentral() override;
    virtual void initPlayers() override;
    virtual void initTexts() override;

    virtual void updateLeft() override;
    virtual void updateCentral() override;
    virtual void updatePlayers() override;
    virtual void updateTexts() override;

    void updateButtons(const games_stat_t::second_type::value_type &val);

    ns_model::tournament_stat_t* sourceModel() const;
};

