#pragma once

#include <QTableView>
#include <tools/types_fwd.hpp>

namespace ns_model {
    class tournament_stat_t;
}

class QActionGroup;

class Description : public QFrame
{
    Q_OBJECT
public:
    enum ActionName
    {
        action_get_info,
        action_register,
        action_unregister,
        action_back_to_game
    };
    Q_ENUM(ActionName)
protected:
    QActionGroup *m_actionGroup;
    QMap<ActionName, QAction*> m_actions;
    gbp_i64 m_currentId;
private:
    QAction *createAction(ActionName name);
signals:
    void openLobbyRequested(const QModelIndex&);
public:
    Description(QWidget *parent = nullptr);
    virtual ~Description();

    virtual QTableView* playersList() const = 0;
    virtual bool isPlayersFetched() const = 0;

    virtual QModelIndex currentIndex() const = 0;

    gbp_i64 currentId() const;
    void setCurrentId(gbp_i64 id);
    virtual void onCurrentIdChanged() = 0;

    virtual QVariant rootData() const;
public slots:
    virtual void openCurrentLobby();
    void setCurrentIndex(const QModelIndex& idx);
protected slots:
    virtual void onActionTriggered(QAction* action);

    virtual void onPlayersFetched() const {}
protected:
    virtual void initLeft() = 0;
    virtual void initCentral() = 0;
    virtual void initPlayers() = 0;
    virtual void initTexts() = 0;   // Тени, переводы, сокращения и т. д.


    virtual void updateLeft() = 0;
    virtual void updateCentral() = 0;
    virtual void updatePlayers() = 0;
    virtual void updateTexts() = 0;

    QAction* action(ActionName name) const {
        return m_actions.value(name);
    }
public:
    virtual void init();
    virtual void initActions();

};
