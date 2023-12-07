#include <precompiled.h>
#include "logger.h"
#include <QDateTime>
//#include <PokerApplication.h>
#include <QDir>
#include <QRegularExpression>

namespace {
    const char* starts = "<html><head><meta charset=\"utf-8\"><title>LOG PokerClient</title>"
                         "<style>lnk {color: #00aaef; text-decoration: underline;} ev {color: #557799; text-decoration: underline;}  tm  { color: #755555; }"
                         "err { font-weight: 600; color: #fa1f10; font-style: bold;} "
                         "war { color: #f0b022; font-weight: 600; }p {font-family: 'Tahoma'; font-size: 12px;line-height: 0.2;color:#151a08;}</style></head><body>";
    QString createLink(const char* file, int line) {
        QString href("<a href=\"%0\" style=\"color: #00aeef\">%1:%2</a>");
        href = href.arg(QDir::current().absoluteFilePath(QString("../%0").arg(file))).arg(QRegularExpression("\\w+\\.\\w+$").match(file).captured()).arg(line);
        return href;
    }

    QString errTMP = "<p><tm>%0</tm><span style = \"font-weight: 600; color: #fa1f10; font-style: bold;\"> %1 </span> %2</p>";
}
static const QString GetLogPath()
{
    QString path = QDir::currentPath()+"/../logs";
    if (!QDir(path).exists())
        QDir::current().mkpath(path);
    return path;
}

Logger::Logger(QObject* parent)
    : QObject(parent)
    , m_logfile(QString("%0/%1_log.%2").arg(GetLogPath()).arg(QDateTime::currentDateTime().toString("yy_MM_dd-hh-mm")).arg("html"), this)
{
    if (m_logfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        m_logfile.write(starts);
        m_logfile.close();
    }
    addMsg("LOG CREATED.");
}

Logger::~Logger()
{
    if (!m_logfile.isOpen()) {
        m_logfile.open(QIODevice::Append | QIODevice::Text);
    }
    if (m_logfile.isOpen())
    {
        addMsg("LOG CLOSED.");
        m_logfile.write("</body></html>");
        m_logfile.close();
    }
}


Logger &Logger::addMsg(const QString &msg, EMsgType msgType, const QString& postMsg)
{
    if ( m_logfile.open(QIODevice::Append | QIODevice::Text) )
    {
        QString time = QTime::currentTime().toString("[hh:mm:ss]...............");
        switch(msgType)
        {
            case EMsgType::Plain: m_logfile.write(QString("<p><tm>%0 </tm>%1 %2</p>").arg(time).arg(msg, postMsg).toUtf8()); break;
            case EMsgType::Error: m_logfile.write(errTMP.arg(time).arg(msg, postMsg).toUtf8()); break;
            case EMsgType::Warning: m_logfile.write(QString("<p><tm>%0</tm><war>%1</war> %2</p>").arg(time).arg(msg, postMsg).toUtf8()); break;
            case EMsgType::Info: m_logfile.write(QString("<p><tm>%0</tm><info>%1</info> %2</p>").arg(time).arg(msg, postMsg).toUtf8()); break;
            case EMsgType::Data: m_logfile.write(QString("<p><tm>%0</tm><dat>%1</dat> %2</p>").arg(time).arg(msg, postMsg).toUtf8()); break;
            case EMsgType::Event: m_logfile.write(QString("<p><tm>%0</tm><ev>%1</ev> %2</p>").arg(time).arg(msg, postMsg).toUtf8()); break;
            case EMsgType::Time: m_logfile.write(QString("<p><tm>%0</tm>%1</p>").arg(msg, postMsg).toUtf8()); break;
        }
        m_logfile.close();
    }
    return *this;
}

void Logger::msg(const QString& msg, const char *file, int line)
{
    if (pApp) {
        pApp->logger().addMsg(msg, EMsgType::Plain, ((file != 0) ? createLink(file, line): "Location not specified"));
    }
}

void Logger::info(const QString& msg, const char *file, int line)
{
    if (pApp) {
        pApp->logger().addMsg(msg, EMsgType::Info, ((file != 0) ? createLink(file, line): "Location not specified"));
    }
}
void Logger::error(const QString& msg, const char *file, int line)
{
    if (pApp) {
        pApp->logger().addMsg(msg, EMsgType::Error, ((file != 0) ? createLink(file, line): "Location not specified"));
    }

}
void Logger::warning(const QString& msg, const char *file, int line)
{
    if (pApp) {
        pApp->logger().addMsg(msg, EMsgType::Warning, ((file != 0) ? createLink(file, line): "Location not specified"));
    }

}
void Logger::event(const QString& msg, const char *file, int line)
{
    if (pApp) {
        pApp->logger().addMsg(msg, EMsgType::Event, ((file != 0) ? createLink(file, line): "Location not specified"));
    }

}
