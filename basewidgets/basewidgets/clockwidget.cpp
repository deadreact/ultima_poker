#include "clockwidget.h"

#include <QTimerEvent>
#include <QTimeZone>
#include <QRegularExpression>
#include <QDebug>
#include <QTimer>


QTime operator+(const QTime& t1, const QTime& t2)
{
    int s = t1.second() + t2.second();
    int m = (t1.minute() + t2.minute()) + s / 60;
    int h = (t1.hour() + t2.hour()) + m / 60;
    return QTime(h%24, m%60, s%60);
}

ClockWidget::ClockWidget(QWidget *parent)
    : QPushButton(parent)
    , m_timer()
    , m_offset()
    , m_format("hh~:mm")
    , m_timezoneId(QTimeZone::systemTimeZoneId())
{
//    QTimer::singleShot(0, Qt::VeryCoarseTimer, [this]{
//        if (!restoreTimeZone(m_timezoneId)) {
//            m_timezoneId = QTimeZone::systemTimeZoneId();
//        }
//    });
    m_timer.start(500, this);
}

ClockWidget::~ClockWidget()
{}

QTime ClockWidget::time() const {
    QTime currTime = QDateTime::currentDateTime().toTimeZone(QTimeZone(m_timezoneId)).time();
    return currTime + offset();
}

const QString& ClockWidget::format() const {
    return m_format;
}

const QByteArray &ClockWidget::timezoneId() const {
    return m_timezoneId;
}

bool ClockWidget::restoreTimeZone(QByteArray &timezoneId) const {
    Q_UNUSED(timezoneId)
    qWarning() << "ClockWidget::restoreTimeZone default impl invoked, which is do nothing" << __FILE__ << __LINE__;
    return false;
}

QTime ClockWidget::offset() const {
    return m_offset.toQTime();
}

void ClockWidget::setOffset(const QTime &offset)
{
    if (m_offset.toQTime() == offset)
        return;

    m_offset.hour = offset.hour();
    m_offset.minute = offset.minute();
    m_offset.second = offset.second();

    emit offsetChanged(offset);
}

void ClockWidget::setFormat(const QString &format)
{
    if (m_format == format)
        return;

    m_format = format;
    emit formatChanged(format);
}

void ClockWidget::setTimezoneId(const QByteArray &timezoneId)
{
    if (m_timezoneId == timezoneId)
        return;

    if (QTimeZone::isTimeZoneIdAvailable(timezoneId)) {
        m_timezoneId = timezoneId;

        emit timezoneIdChanged(timezoneId);
    }
}

void ClockWidget::timerEvent(QTimerEvent *event)
{
    static bool anim = false;
    QTime tm = time();
    QString str(tm.toString(m_format));

    if (anim) {
        QPushButton::setText(str.replace(QRegularExpression("~."), " "));
    } else {
        QPushButton::setText(str.replace("~", ""));
    }
    event->accept();
    anim = !anim;
    emit timeChanged(tm);
}
