#include "sequentalactionqueue.h"
#include <qdebug.h>
#include <iostream>
#include <qapplication.h>

#define PRINT_DEBUG \
    QTimer::singleShot(0, [&]{ \
        if (!this) return; \
        setObjectName(metaObject()->className()); \
        qDebug() << ".............construct(" << this->objectName() << ")"; \
        connect(this, &Action::destroyed, [&]{ \
            qDebug() << ".............destruct(" << this->objectName() << ")"; \
        }); \
        connect(this, &Action::done, [&](bool result){ \
            if (result) { \
                qDebug() << ".............accepted(" << this->objectName() << ")"; \
            } else { \
                qDebug() << ".............rejected(" << this->objectName() << ")"; \
            } \
        }); \
    })

namespace ns_action
{
    Action::Action(QObject* parent)
        : QObject(parent)
        , m_execStarts(ExecutionStarts::Default)
    //    , m_msec(0)
    {
        PRINT_DEBUG;
    }
    Action::Action(const QString& objectName, QObject* parent)
        : QObject(parent)
        , m_execStarts(ExecutionStarts::Default)
    {
        setObjectName(objectName);
        PRINT_DEBUG;
    }

    Action::~Action() {
    }


    SequentalActionQueue::~SequentalActionQueue()
    {
        clear();
    }

    void SequentalActionQueue::clear() {
        qDeleteAll(m_queue);
        m_queue.clear();
    }

    QSharedPointer<Action> SequentalActionQueue::currentAction() const {
        return m_currentAction;
    }

    SequentalActionQueue::SequentalActionQueue(QObject *parent)
        : Action("sequentalActionQueue", parent)
        , m_currentAction(nullptr)
        , m_lastResult(false)
    {}

    void SequentalActionQueue::append(Action *action)
    {
        Q_ASSERT(!m_queue.contains(action));
        m_queue.enqueue(action);
        connect(action, &Action::done, this, &SequentalActionQueue::on_action_done);
    }

    void SequentalActionQueue::initialize()
    {/*DO NOTHING*/}

    void SequentalActionQueue::execute() {
        m_lastResult = true;
        dequeue_next();
        call_next();
    //    QTimer::singleShot(0, this, &SequentalActionQueue::call_next);
    }

    void SequentalActionQueue::call_next() {
        if (m_currentAction->execStarts() == ExecutionStarts::Schreduled) {
            QTimer::singleShot(0, m_currentAction.data(), &Action::execute);
        } else {
            m_currentAction->execute();
        }
    }

    void SequentalActionQueue::dequeue_next()
    {
        if (!m_currentAction.isNull()) {
            disconnect(m_currentAction.data());
        }
        m_currentAction.reset(m_queue.dequeue());
    }

    void SequentalActionQueue::on_action_done(bool status) {
        m_lastResult = status;
        if (!m_lastResult) {
            emit done(false);
            return;
        }
        emit actionDone(m_currentAction, status);
        if (m_queue.isEmpty()) {
            m_currentAction.reset();
            emit done(m_lastResult);
        } else {
            dequeue_next();
            call_next();
        }
    }

} //namespace ns_action

