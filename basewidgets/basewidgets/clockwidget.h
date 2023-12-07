#pragma once

#include "basewidgets_global.h"
#include <QPushButton>
#include <QBasicTimer>
#include <QTime>
//#include "types.h"

class BASEWIDGETS_DLL ClockWidget : public QPushButton
{
    Q_OBJECT

    Q_PROPERTY(QTime time READ time NOTIFY timeChanged STORED false)
    Q_PROPERTY(QTime offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(QString format READ format WRITE setFormat NOTIFY formatChanged)
    Q_PROPERTY(QByteArray timezoneId READ timezoneId WRITE setTimezoneId NOTIFY timezoneIdChanged DESIGNABLE false)

    QBasicTimer m_timer;
    // Because of QTime strange behavior. Seems it was written by gays, suka bliat'!!
    struct {
        int hour = 0, minute = 0, second = 0;
        QTime toQTime() const { return QTime(hour, minute, second); }
    } m_offset;

    QString m_format;
    QByteArray m_timezoneId;

public:
    Q_INVOKABLE ClockWidget(QWidget *parent = 0);
    virtual ~ClockWidget();

    QTime time() const;
    QTime offset() const;
    const QString &format() const;
    const QByteArray& timezoneId() const;

    virtual bool restoreTimeZone(QByteArray& timezoneId) const;
signals:
    void timeChanged(const QTime& time);
    void offsetChanged(const QTime& offset);
    void formatChanged(const QString& format);

    void timezoneIdChanged(const QByteArray& timezoneId);

public slots:
    void setOffset(const QTime& offset);
    void setFormat(const QString& format);
    void setTimezoneId(const QByteArray& timezoneId);

protected:
    virtual void timerEvent(QTimerEvent *event) override;
};

