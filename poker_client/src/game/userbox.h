#pragma once

#include <QStackedWidget>
#include <qpropertyanimation.h>
#include <qqueue.h>
#include <core/money.h>
#include <interface/identifiable.h>
#include <models/model_CardsDeck.h>
#include "tableItems/modelkeyitem.h"

#ifndef Q_MOC_RUN
    #include "rpc/network_client.h"
#endif // Q_MOC_RUN

class QAbstractButton;
class FramedEntity;
struct UserboxData;
class TablePlayer;
class CardWidget;
class BetItem;
struct TimeLineEvent;
class QPropertyAnimation;

namespace
{
    using e_user_action = gbp::ns_api::ns_table::e_user_action;
}

class Userbox : public QStackedWidget, public ModelKeyItem
{
    Q_OBJECT

    Q_PROPERTY(gbp_i64 playerId READ playerId)
    Q_PROPERTY(QIcon avatar READ avatar WRITE setAvatar)
    Q_PROPERTY(QString nickname READ nickname WRITE setNickname)
    Q_PROPERTY(int cardsCount READ cardsCount WRITE setCardsCount NOTIFY cardsCountChanged)

    Q_PROPERTY(gbp_i64 stack READ stack WRITE setStack NOTIFY stackChanged)
    Q_PROPERTY(gbp_i64 roundBet READ roundBet)
    Q_PROPERTY(gbp_i64 distributionBet READ distributionBet)
    Q_PROPERTY(gbp_i64 winnings READ winnings WRITE setWinnings)
    Q_PROPERTY(gbp_i64 totalMoney READ totalMoney)

    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
    Q_PROPERTY(bool isRightToLeft READ isRightToLeft WRITE setIsRightToLeft)
private:
    constexpr static const char* sm_modelKey[] = {"[%0,0[4,0,[0,0,0x0]]]", "[%0,14[4,0,[0,0,0x0]]]"};
private:
    UserboxData* m_dPtr;
    const int m_seatIndex;
    QMap<QByteArray/*key-name*/, QQueue<QPropertyAnimation*>> m_animQueue;
public:
    explicit Userbox(const GameId& gameID, int seatIndex, QWidget *parent = 0);
    virtual ~Userbox();

    void updateModelData();
    bool updateModelData(const QModelIndex& index, int role = UserType::OriginDataRole);



    virtual void init();
    int seatIndex() const;
    bool isPlayerReady() const;
    gbp_i64 playerId() const;
    void reserve(gbp_i64 playerID);
//    void setPlayer(TablePlayer *player);
    void free();

    QIcon avatar() const;
    QString nickname() const;
    gbp_i64 stack() const;
    gbp_i64 roundBet() const;
    gbp_i64 distributionBet() const;
    gbp_i64 totalMoney() const;
    gbp_i64 winnings() const;
    int cardsCount() const;
    void setCard(int index, gbp_i8 card);
    void setCard(int index, const char* repr); // example "♠10" or "♦Q", hex example "e299 a353" ♣5
    gbp_i8 card(int index) const;
    CardWidget *findCard(gbp_i8 cardId) const;

    QAbstractButton* buttonTakeASit() const;

    void setIsRightToLeft(bool);
    bool isRightToLeft() const;

    ns_model::CardsDeck* deckModel() const;
    BetItem *betItem() const;
    void setBetItem(BetItem* betItem);

    qreal opacity() const;
    void setOpacity(qreal newOpacity);


public slots:
    void setAvatar(const QIcon &avatar);
    void setNickname(QString name);
    void setStack(gbp_i64 stack);
    void gainMoney(gbp_i64 m);
    void setWinnings(gbp_i64 winnings);
    void setCardsCount(int count);
    void clear();

    virtual void timelineEvent(const TimeLineEvent& ev);

    void setHorizontalOffset(int offset);
    int horizontalOffset() const;
public:
    void displayAction(GameAction action, int durationMsec = 1000);
    void setDeckModel(ns_model::CardsDeck* model);

    void onSettingsChanged(QAction *settings);
    void setGlowWidget(QWidget *glow);
    QWidget* takeGlowWidget();
    QAction* actionAddNote() const;
    QAction* actionEditNote() const;
private slots:
    //! @brief коннектятся к модели данных table_state_t на сигнал
    //! dataChanged(const QModelIndex& topLeft, const QModelIndex&  bottomRight, const QVector<int>& roles)
    //! @returns 'true' - enable animation, 'false' - disable animation
    bool updateStack(const QVariant& value, int role = UserType::OriginDataRole);
    bool updateStartStack(const QVariant& value, int role = UserType::OriginDataRole);
    bool updateNickname(const QVariant& value, int role = UserType::OriginDataRole);
    bool updateBounty(const QVariant& value, int role = UserType::OriginDataRole);
    bool updateAvatar(const QVariant& value, int role = UserType::OriginDataRole);
    bool updateIsDealer(const QVariant& value, int role = UserType::OriginDataRole);
    bool updateIsCurrent(const QVariant& value, int role = UserType::OriginDataRole);
    bool updateVipStatus(const QVariant& value, int role = UserType::OriginDataRole);
    bool updateState(const QVariant& value, int role = UserType::OriginDataRole);
    bool updateRoundBet(const QVariant& value, int role = UserType::OriginDataRole);
    bool updateCards(const QVariant& value, int role = UserType::OriginDataRole);

    void displayUserInfoPage();

    void onModelReset();
//    void onStatusChanged(const QMetaProperty& hint);
//    void onMoneyChanged(const QMetaProperty& hint);
signals:
    void stackChanged(int stack);
    void cardsCountChanged(int cardsCount);
    void opacityChanged(qreal);
protected:
    virtual void actionEvent(QActionEvent *e) override;
//    virtual void playerAddEvent(const TablePlayer *pl);
//    virtual void playerChangeEvent(const TablePlayer *pl);
//    virtual void playerRemoveEvent(const TablePlayer *pl);

    // ModelKeyItem interface
public:
    virtual QByteArray modelKey() const override;
    virtual void updateData(const QAbstractItemModel *model) override;
    virtual void enqueueAnimation(QObject* target
                                , const QByteArray& property
                                , int duration
                                , const QVariant& startValue
                                , const QVariant& endValue
                                , QEasingCurve::Type ease = QEasingCurve::Linear
                                , const QByteArray& queueName = QByteArray() // default
                                , bool run = true);
};
