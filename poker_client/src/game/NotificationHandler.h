#pragma once

#include <QObject>

class NotificationHandler : public QObject
{
    Q_OBJECT
public:
    NotificationHandler(QObject *parent = nullptr);
    virtual ~NotificationHandler();
private:
    void initialize();
};
