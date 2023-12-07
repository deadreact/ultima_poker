#pragma once

#include "tools_global.h"
#include "types_fwd.hpp"
#include <QObject>
#include <QQueue>
#include <functional>
#include <QSharedPointer>
#include <qtimer.h>

/**
 * @brief The Action class
 */
namespace ns_action
{
    class TOOLS_DLL Action : public QObject
    {
        Q_OBJECT
    public:
        enum class ExecutionStarts : gbp_i8
        {
            Schreduled = 0
          , Immediately
          , Default = Schreduled
        };
        Q_ENUM(ExecutionStarts)
    protected:

        ExecutionStarts m_execStarts;
    signals:
        void done(bool);
    public:
        Action(QObject* parent = nullptr);
        Action(const QString& objectName, QObject* parent = nullptr);
        virtual ~Action();
        virtual void execute() = 0;
        inline ExecutionStarts execStarts() const { return m_execStarts; }
    };

    /**
     * @brief The SequentalActionQueue class
     */
    class TOOLS_DLL SequentalActionQueue : public Action
    {
        Q_OBJECT
    private:
        QQueue<Action*> m_queue;
        QSharedPointer<Action> m_currentAction;
        bool m_lastResult;
    public:
        explicit SequentalActionQueue(QObject *parent = nullptr);
        virtual ~SequentalActionQueue();

        void clear();
        QSharedPointer<Action> currentAction() const;
        //! @brief adds action to queue
        //! @param action - should be allocated on heap
        //!        before execute() call it will be dequeued from action queue to m_currentAction, replacing previous one
        void append(Action* action);

        inline SequentalActionQueue& operator<<(Action* action) {
            append(action);
            return *this;
        }

        //! @brief Overload to set initial logic
        virtual void initialize();
        virtual void execute() override;
    private slots:
        void call_next();
        void dequeue_next();
        void on_action_done(bool status);
    signals:
        void actionDone(QSharedPointer<Action>, bool);
    };

} //namespace ns_action

