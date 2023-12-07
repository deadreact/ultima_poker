#pragma once
#include "single_actions.h"

namespace ns_action
{
    class UpdateWaiters;
    /**
     * @brief Тут должны быть непосредственные действия текущего юзера, например клик по кнопке, или комб. клавиш ...
     *        Если действие не меняет общие данные и не обращается к серваку, то нахуй!
     *        Если есть клиент-серверное взаимодействие, emit done(true) должен вызываться только после удачного реплая,
     *        emit done(false) после неудачного реплая, или на крайняк по таймауту.
     *        ВСЕ (сука) данные, которые как либо учавствуют, должны быть ПОЛНОЦЕННО (блять) обработаны
     */
    class UserBase : public SequentalActionQueue
    {
        Q_OBJECT
    protected:
        QWidget* m_activeWindow;
        GameId m_gameID;
        bool m_loginRequired;
    public:
        explicit UserBase(const GameId& gameID, QWidget* activeWindow, QObject* parent = nullptr, bool loginRequired = true);
        virtual ~UserBase();

        virtual void initialize() override;
        virtual void execute() override;

        gbp_i64 userId() const;
    };

    class UserJoiningQueue : public UserBase
    {
        Q_OBJECT
//        static
//        UpdateWaiters* m_updWaiters;
    public:
        explicit UserJoiningQueue(gbp_i64 cashId, QObject* parent = nullptr);
        virtual ~UserJoiningQueue();
        virtual void initialize() override;
        virtual void execute() override;

//        void subscribe();
    };




} //namespace ns_action
