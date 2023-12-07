#pragma once

#include "TimelineHandler.h"

#include <QAction>
#include <api_models/api_util.hpp>

#include <core/money.h>
#include <interface/initable.h>
#include <interface/identifiable.h>
#include <queue>
#include <db/GameTableSettingsData.h>
#include <PokerApplication.h>
#include <dialogs/buyin/dialog_CashBuyIn.h>
#include <tools/types_fwd.hpp>

class QTextBrowser;
namespace ns_dialog {
    class CashBuyIn;
}
namespace ns_descriptor {
 struct CashBuyIn;
}

class TableOver;
class TablePlayer;
class QAbstractButton;
class QLabel;

namespace
{
    using e_game_limit            = gbp::ns_api::ns_game::e_game_limit;
    using e_user_action           = gbp::ns_api::ns_table::e_user_action;
    using action_info_t           = gbp::ns_api::ns_table::action_info_t;
    using available_user_course_t = gbp::ns_api::ns_table::available_user_course_t;
    using money_info_t            = gbp::ns_api::ns_table::money_info_t;
    using user_info_t             = gbp::ns_api::ns_lobby::user_info_t;
}

struct ActionFlag
{
    static const uint fold;
    static const uint check;
    static const uint bet;
    static const uint call;
    static const uint raise;
    static const uint all_in;
    static const uint blind;
    static const uint guest_bet;
    static const uint last_flag;
};

struct PlayerData;

class Player : public InitableWidget, public CurrentGamePlayerIdObject
{
    Q_OBJECT
    Q_PROPERTY(bool loggedIn           READ isLoggedIn)
    Q_PROPERTY(long long playerId      READ playerId)
    Q_PROPERTY(GameId gameId           READ gameId)
    Q_PROPERTY(bool sitsOnTable        READ isSitsOnTable)
    Q_PROPERTY(int seatIndex           READ seatIndex)

    Q_PROPERTY(bool  inGame             READ isInGame)
    Q_PROPERTY(bool  current            READ isCurrent)
    Q_PROPERTY(bool  dealer             READ isDealer)
    Q_PROPERTY(bool  viewer             READ isViewer)
    Q_PROPERTY(bool  waiter             READ isWaiter)
    Q_PROPERTY(short waitersCount       READ waitersCount)
    Q_PROPERTY(short waiterPos          READ waiterPos)
    Q_PROPERTY(bool  canPredefineCourse READ canPredefineCourse)
    Q_PROPERTY(Money roundBet           READ roundBet)
    Q_PROPERTY(Money distributionBet    READ distributionBet)
    Q_PROPERTY(Money gameMoney          READ gameMoney)

    Q_PROPERTY(GameAction triggeredAction    READ triggeredAction)
    Q_PROPERTY(GameActions courseActions     READ courseActions     WRITE setCourseActions)
    Q_PROPERTY(GameActions predefinedActions READ predefinedActions WRITE setPredefinedActions)

//    Q_ENUMS(E::PlayerState)
public:
//    typedef E::PlayerState state_t;

    constexpr static const GameAction noAction() { return std::make_pair(e_user_action(-1), 0); }
private:
    PlayerData* m_dPtr;
    int m_rev;
public slots:
    void setTablePlayer(TablePlayer* );
    void onLogin(TablePlayer* loggedInPlayer);
    void onLogout();
    void setViewer(bool enabled = true);
    void setDealerIndex(int dealerIndex);

    void updateGameControls(const QMetaProperty& hint = QMetaProperty());

    void onStatusChanged(const QMetaProperty& hint);
    void onMoneyChanged(const QMetaProperty& hint);
    void onIndexChanged(const QMetaProperty& hint);
public:
    Player(QWidget *parent);
    Player(const GameId& gameId, QWidget *parent = 0);
    virtual ~Player();

    bool isLoggedIn() const;
    int  seatIndex() const;
    bool isSitsOnTable() const;
    // play current distribution
    bool isInGame() const;
    // make course
    bool isCurrent() const;
    bool isDealer() const;

    bool isViewer() const;
    bool isWaiter() const;
    gbp_i16  waitersCount() const;
    gbp_i16  waiterPos() const;

    Money roundBet() const;
    Money distributionBet() const;
    Money gameMoney() const;
    //! @brief размер стека на начало раздачи
    Money totalGameMoney() const;

//    gbp_i64 takeMoney();

    void triggerCourseAction();
    const GameAction& triggeredAction() const;
    GameAction takeTriggeredAction();

    GameActions courseActions() const;
    GameActions predefinedActions() const;
    void setCourseActions(const GameActions&);
    void setPredefinedActions(const GameActions&);

    bool canPredefineCourse() const;
public:
    void showHandButtons();
    void setCurrentActivePlayer( const std::vector<available_user_course_t> &actions
                               , const action_info_t &action_info
                               , Money commonPot);
    void setPredefinedInfo(Money defaultBet, Money currentBet);

    void toViewer(bool distributionsReason = false, bool rechargeReason = false);
    QString takeChatMessage();

    QAbstractButton* getControl(const QString& name) const;

    // подтверждение сервака
    void setGuestBlindChecked(bool checked);
    void setSitOutFromNextDistribution(bool checked);
    void setShowHand() {}
private:
//    template <typename ...Args>
//    void request(const char* what, Args ...args);
public:

    void setRound(int index, const QString& name);

    bool askBlind(gbp_i64 blind, bool isBB = false);

    void enableGuestBlind();
    void disableGuestBlind();

    void addSitOnTableTrigger(QAbstractButton* trigger, gbp_i8 seatIndex = -1);

    virtual void init() override;
    QTextBrowser* chatBrowser() const;
    QWidget* handButtons() const;
    QLabel* distributionNumber() const;

    void setWaitersInfo(gbp_i16 count, gbp_i16 seatIndex);
signals:
    void lobbyOpenRequested();
    void gameSettingsChanged();
    void courseActionTriggered(const GameAction&);
public slots:
    void toggleChatVisibility(bool);
    void showSmiles();
    void hideSmiles();

    void insertSmileToChat(QModelIndex index);
    void activateGameActions();
    void enableMask();

    virtual void timelineEvent(const TimeLineEvent& e);
private slots:
    void clearPredefinedActions();
    void sendMsg();
    void setPlayersCourse(int index);
    void toggleGuestBlind(bool guestBlind);
    void resetControls();
    void onSettingsChanged(QAction *settings);
protected:
    bool loginIfNeeded();
    virtual void changeEvent(QEvent *e) override;
    virtual void childEvent(QChildEvent *e) override;
    virtual void actionEvent(QActionEvent *e) override;
    virtual void playerAddEvent(const TablePlayer *pl);
    virtual void playerChangeEvent(const TablePlayer *pl);
    virtual void playerRemoveEvent(const TablePlayer *pl);
//private slots:
//    void on_bttnGrp_gameActions_buttonClicked(QAbstractButton* button);
//    void on_bttnGrp_autoActions_buttonToggled(QAbstractButton* button, bool);
//    void on_bttnGrp_guestButtons_buttonClicked(QAbstractButton* button);
};
//template <typename ...Args>
//void Player::request(const char *what, Args ...args) {
//    m_request->request(what, tableId(), args...);
//}


//--------------------------------------
