#include <precompiled.h>
#include "TimelineHandler.h"
#include <QTimerEvent>
#include <QDateTime>

TimelineHandler::TimelineHandler(QObject *parent)
    : QObject(parent)
    , m_playerID(-1)
    , m_timeMark(0)
    , m_interval(30, 100, 100)
    , m_timerId(-1)
{

}

TimelineHandler::~TimelineHandler()
{
}

void TimelineHandler::reset(gbp_i64 playerID, gbp_i64 time_sh, gbp_i64 time_main, gbp_i64 time_add)
{
    if (m_playerID != -1) {
        finish();
    }
    m_playerID = playerID;
    auto offset = QDateTime::currentDateTime().toTime_t();
    time_sh += offset;
    time_main += time_sh;
    time_add += time_main;
    m_timeMark.set(time_sh, time_main, time_add);
}

void TimelineHandler::restart(int interval_sh, int interval_main, int interval_add)
{
    m_interval.set(interval_sh, interval_main, interval_add);
    start();
}

void TimelineHandler::start()
{
    killTimers();
    m_clocks(0) = clock();
    m_timerId(0) = startTimer(m_interval(0), Qt::PreciseTimer);
    emit started(m_playerID);
}

void TimelineHandler::finish()
{
    killTimers();
    m_timeMark.set(0, 0, 0);
    gbp_i64 id = m_playerID;
    m_playerID = -1;
    emit finished(id);
}

const Triple<int>& TimelineHandler::interval() const {
    return m_interval;
}

gbp_i64 TimelineHandler::durationFull() const {
    return std::max(0ll, QDateTime::currentDateTime().msecsTo(QDateTime::fromTime_t(m_timeMark.a())));
}

gbp_i64 TimelineHandler::durationMain() const {
    return std::max(0ll, QDateTime::currentDateTime().msecsTo(QDateTime::fromTime_t(m_timeMark.m())));
}

gbp_i64 TimelineHandler::durationShort() const {
    return std::max(0ll, QDateTime::currentDateTime().msecsTo(QDateTime::fromTime_t(m_timeMark.s())));
}

const Triple<gbp_i64>& TimelineHandler::timeMark() const {
    return m_timeMark;
}

void TimelineHandler::timerEvent(QTimerEvent *e)
{
    clock_t point = clock();
    TimeLineEvent ev;
    ev.handler = this;
    ev.playerID = m_playerID;
    if (e->timerId() == m_timerId.s())
    {
        ev.type = TimeLineEvent::s;
        ev.deltaTime = point - m_clocks.s();
        emit timeout(ev);
        m_clocks.s() = point;
        if (durationShort() == 0) {
            killTimer(e->timerId());
            m_timerId(0) = -1;
            if (durationMain() > 0) {
                m_clocks(1) = clock();
                m_timerId(1) = startTimer(m_interval.m(), Qt::PreciseTimer);
            } else if (durationFull() > 0) {
                m_clocks(2) = clock();
                m_timerId(2) = startTimer(m_interval.a(), Qt::PreciseTimer);
            } else {
                finish();
            }
        }

    }
    else if (e->timerId() == m_timerId.m())
    {
        ev.type = TimeLineEvent::m;
        ev.deltaTime = point - m_clocks.m();
        emit timeout(ev);
        m_clocks.m() = point;
        if (durationMain() == 0) {
            killTimer(e->timerId());
            m_timerId(1) = -1;
            if (durationFull() > 0) {
                m_clocks(2) = clock();
                m_timerId(2) = startTimer(m_interval.m(), Qt::PreciseTimer);
            } else {
                finish();
            }
        }

    }
    else if (e->timerId() == m_timerId.a())
    {
        ev.type = TimeLineEvent::a;
        ev.deltaTime = point - m_clocks.a();
        emit timeout(ev);
        m_clocks.a() = point;
        if (durationFull() == 0) {
            killTimer(e->timerId());
            m_timerId(2) = -1;
            finish();
        }
    }
    QObject::timerEvent(e);
}

void TimelineHandler::killTimers()
{
    if (m_timerId(0) != -1) {
        killTimer(m_timerId(0));
        m_timerId(0) = -1;
    }
    if (m_timerId(1) != -1) {
        killTimer(m_timerId(1));
        m_timerId(1) = -1;
    }
    if (m_timerId(2) != -1) {
        killTimer(m_timerId(2));
        m_timerId(2) = -1;
    }
}
