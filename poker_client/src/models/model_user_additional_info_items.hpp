#pragma once

#include "model_user_additional_info.h"

// enum class guest_bttn : gbp_i8 {
//       hidden         = 0x00
//     , make_guest_bet = 0x01
//     , wait_bb        = 0x02
// };

//enum class optiontype : gbp_u32
//{
//      num_limit  = 0x01000000
//    , action     = 0x02000000
//    , pending    = 0x04000000
//    , feature    = 0x08000000
//    , control    = 0x10000000
//    , appearance = 0x20000000
//    , state      = 0x40000000
//    , effect     = 0x80000000
//};

//template <int n>
//struct seat {
//    enum
//    {
//        pos = seat<n-1>::pos + 1
//    };
//};
//template <>
//struct seat<0> {
//    enum
//    {
//        pos = 0,
//    };
//};

class Controls {
    Q_GADGET
public:

    enum class UserControl
    {
        // viewer spec
          UC_OnSit_0 = 0
        , UC_OnSit_1
        , UC_OnSit_2
        , UC_OnSit_3
        , UC_OnSit_4
        , UC_OnSit_5
        , UC_OnSit_6
        , UC_OnSit_7
        , UC_OnSit_8
        // frozen
        , UC_WaitBB
        , UC_SetGuestBet
        // sitout (anticipant)
        , UC_BackToGame

        , UC_JoinQueue
        // waiter
        , UC_UnjoinQueue

        , UC_AlwaysFold
        , UC_SitOut
        , UC_SitOutOnBB
        // user is the current player
        , UC_CourseFold
        , UC_CourseCheck
        , UC_CourseBet
        , UC_CourseCall
        , UC_CourseRaise
        , UC_CourseAllIn

        , UC_AutoFold
        , UC_AutoCheck
        , UC_AutoCheckFold
        , UC_AutoCheckCallAny
        , UC_AutoCall
        , UC_AutoBet
        , UC_AutoBetRaiseAny
        , UC_AutoRaise
        , UC_AutoRaiseAny

        , UC_ShowHand
        , UC_HideHand
        // menu
        , UC_BuyChips
        , UC_Deposit
        , UC_Lobby

        , UC_Statistic
        , UC_Settings
        , UC_Sound
        , UC_ArrangeWindowsNear
        , UC_ArrangeWindowsCascade
        , UC_ArrangeWindowsSingle
        , UC_Replay
        // tournaments spec on break
        , UC_SingleRebuy
        , UC_DoubleRebuy
        // chat && layout
        , UC_SmileSelect
        , UC_ChatMsg
        , UC_ShowHideChat
        , UC_ShowHideLeftBottomChecks
        // window controls
        , UC_Quit
        , UC_Minimize
        , UC_Maximize
        // complex control (slider/spinbox/buttons)
        , UC_BetController
        , UC_Chat
        , UC_ActionOnUserbox

        , UC_Count
    };
    Q_ENUM(UserControl)

    template <UserControl control>
    QWidget* visualTrigger(const GameId& gameID);

    enum class GameplayElement
    {
          GE_DisplayDistributionNo
        , GE_TimeBank
        , GE_MsgJoinQueue
        , GE_MsgUnjoinQueue
    };
    Q_ENUM(GameplayElement)

    struct ControlOption
    {
        //! Что влияет на изменения елемента
        enum class ChangeReason
        {
            Server = 0x01,
            User   = 0x02,
            Data   = 0x04,
            Time   = 0x08,

            Other  = 0xf0
        };
        ChangeReason changeReason;

        //! На что влияют изменения елемента
        enum class Effect
        {
            RequestToServer,
            ReplyFromServer, // уточнение что реквест ожидающий reply
            ChangesView,
            ChangesData,
            TriggerEvents
        };
        Effect effect;



        bool isTrigger;
        bool isVisual;
        bool hasState;
        bool isComplex;
        bool hasExclusiveEffect;
    };
};

//enum class bet_control
//{
//      min_amount      = num_limit | 0x01
//    , max_amount      = num_limit | 0x02
//    , range           = num_limit | min_amount | max_amount
//    , fixed_amount    = num_limit | 0x04 | range
//    , zero_amount     = num_limit | 0x08 | fixed_amount
//    , invalid_amount  = num_limit | 0x10
//    , negative_amount = num_limit | 0x20 | invalid_amount
//};

//enum class course : gbp_i8 {
//      fold       = 0x0100 | zero_amount
//    , check      = 0x0200 | zero_amount
//    , bet        = 0x0400 | range
//    , call       = 0x0800 | fixed_amount
//    , raise      = bet | call
//    , all_in     = 0x1000 | call
//    , blind      = 0x2000 | fixed_amount
//    , guest_bet  = 0x4000 | blind
//};

//enum class autoactions : gbp_i8 {
//      autoAction     = pending | action
//    , autoFold
//    , autoCheck
//    , autoCheckFold
//    , autoCheckCallAny
//    , autoCall
//    , autoBet
//    , autoBetRaiseAny
//    , autoRaise
//    , autoRaiseAny
//};


//struct waiters_info
//{
//    gbp_i16 quantity;
//    gbp_i16 user_pos; // -1 если не в очереди
//};



//class GamesCategoryItem : public CategoryItem
//{
//    QMap<QString, QStandardItem*> itemByName;
//public:
//    GamesCategoryItem();

//    virtual void read(QDataStream &in) override;
//    virtual void write(QDataStream &out) const override;
//    virtual QStandardItem *init(QStandardItem *parent) override;
//    virtual QStandardItem *field(const QString &name) const override;
//    virtual QString fieldName(QStandardItem *child) const override;
//};

