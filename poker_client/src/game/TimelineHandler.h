#pragma once

#include <QObject>
#include <time.h>

class TimelineHandler;

template <typename T>
class Triple
{
    union
    {
        struct
        {
            T _s;
            T _m;
            T _a;
        };
        T m_[3];
    };
public:
    Triple(): _s(0), _m(0), _a(0) {}
    Triple(const T& val): _s(val), _m(val), _a(val) {}
    Triple(const T& s, const T& m, const T& a): _s(s), _m(m), _a(a) {}

    inline const T& s() const { return _s; }
    inline const T& m() const { return _m; }
    inline const T& a() const { return _a; }
    inline T& s() { return _s; }
    inline T& m() { return _m; }
    inline T& a() { return _a; }

    inline const T& operator()(int idx) const { return m_[idx]; }
    inline T& operator()(int idx) { return m_[idx]; }

    inline void set(const T& s, const T& m, const T& a) {
        _s = s; _m = m; _a = a;
    }
};

struct TimeLineEvent
{
    typedef gbp_u8 TimeType;
    constexpr static const TimeType s = 0;
    constexpr static const TimeType m = 1;
    constexpr static const TimeType a = 2;

    const TimelineHandler* handler;
    gbp_i64 playerID;
    int deltaTime;
    TimeType type;

    QString toString() const {
        return QString("playerID: %0, deltaTime: %1, type: %2").arg(playerID).arg(deltaTime).arg(type == 0 ? "short" : (type == 1 ? "main" : "additional"));
    }
};

Q_DECLARE_METATYPE(TimeLineEvent)


class TimelineHandler : public QObject
{
    Q_OBJECT

signals:
    void timeout(const TimeLineEvent&);
    void started(gbp_i64 playerID);
    void finished(gbp_i64 playerID);
private:
    gbp_i64 m_playerID;
    Triple<gbp_i64/*sec*/> m_timeMark;
    Triple<int/*msec*/> m_interval;
    Triple<int/*id*/> m_timerId;
    Triple<clock_t> m_clocks;
public:
    explicit TimelineHandler(QObject *parent = nullptr);
    virtual ~TimelineHandler();

    gbp_i64 playerID() const { return m_playerID; }

    void reset(gbp_i64 playerID, gbp_i64 time_sh, gbp_i64 time_main, gbp_i64 time_add);
    void restart(int interval_sh, int interval_main, int interval_add);

    void start();
    void finish();

    gbp_i64 durationFull() const;
    gbp_i64 durationMain() const;
    gbp_i64 durationShort() const;

    const Triple<gbp_i64>& timeMark() const;
    const Triple<gbp_i32>& interval() const;

protected:
    virtual void timerEvent(QTimerEvent *e) override;
    void killTimers();
};

