#pragma once
#include <interface/initable.h>
#include <interface/identifiable.h>
#include <tools/types_fwd.hpp>
#include <game/gamehandlerinterface.hpp>
#include <tools/types_fwd.hpp>
struct QTextBrowser;

class UiElement
{
    Q_GADGET
public:
//    enum Type
//    {
//        ChatBrowser,
//        ButtonGuestBet,
//        ButtonWaitBB
//    };

    struct ChatBrowserType { using Type = QTextBrowser*; };
    static const ChatBrowserType ChatBrowser;
};


class UserControls : public InitableWidget
{
    Q_OBJECT
private:
   struct Impl;
   Impl* m_impl;
public:
    UserControls(QWidget *parent = nullptr);
    virtual ~UserControls();

//    const GameId& id() const;
    //! @returns internal uniqueID, which is never overriden, even after it's unregistered
    int registerExternalControl(QWidget* w);
    QWidget* unregisterExternalControl(int uniqueID);
    QWidget* usrControl(int uniqueID) const;

    template <typename UiElementType>
    typename UiElementType::Type uiElement(UiElementType) const;

    void setPredefinedInfo(gbp_i64 defaultBet, gbp_i64 currentBet);

private slots:
    // slots by items names
    void on_bttnGrp_gameActions_buttonClicked(QAbstractButton* bttn);
    void on_bttn_joinQueue_clicked();
    void on_bttn_unjoinQueue_clicked();
private:
    void registerCourseActionTrigger(QAbstractButton* bttn, e_user_action id, gbp_i64 moneyMin = 0, gbp_i64 range = 0);
    void registerAutoActionTrigger(QAbstractButton* bttn, const AutoAction& autoAction);
    void onModelDataChange(const QModelIndex& index, int role = UserType::OriginDataRole);
public:
    virtual void init() override;
protected:
    virtual void changeEvent(QEvent *e) override;
    virtual void childEvent(QChildEvent *e) override;
public:
    void timelineEvent(const TimeLineEvent& ev);
};

//--------------------------------------
