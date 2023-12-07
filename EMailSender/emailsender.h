#pragma once

#include <QFile>

namespace ns_email_sender
{
    struct message
    {
        QString text;
        QStringList attachments;
    };

    struct SMTPInfo
    {
        QString smtp;
        qint16 port;
        qint16 conn_type;
        QString user;
        QString pass;
        QString sender;
        QString recipient;
        QString subject;
        static QString lastError;

        SMTPInfo(const char* filename);
    };

//    namespace ns_pycode
//    {
//        int verification(const message* msg, const char* config = "config.ini");
//    }

    int verification(const message* msg, const char* config = "config.ini");
} // ns_email_sender

template <typename OStream>
OStream& operator<<(OStream& os, const ns_email_sender::SMTPInfo& info)
{
    static const char* pattern("SMTPInfo(smtp: %0, port: %1, user: %2, pass %3, sender: %4, recipient: %5, subject: %6");
    QString str = QString(pattern).arg(info.smtp).arg(info.port).arg(info.user, info.pass, info.sender, info.recipient, info.subject);
    if (!info.lastError.isNull() && !info.lastError.isEmpty()) {
        os << QString("SMTPError(lastError:%0)").arg(info.lastError);
    }
    os << str;
    return os;
}
