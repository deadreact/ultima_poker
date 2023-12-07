#pragma once

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <PokerApplication.h>


enum class EMsgType
{
    Plain,
    Warning,
    Error,
    Info,
    Data,
    Event,
    Time
};

template <EMsgType type = EMsgType::Plain>
struct HtmlTemplate
{
    constexpr static const char* value = "<p><tm>%0</tm><span style = \"color: #191919;\"> %1 </span> %2</p>";
    constexpr static const int args = 3;
};

#define NO_FILELINE 0, -1
#define __FILELINE__ __FILE__, __LINE__

class Logger : public QObject
{
    Q_OBJECT
    Q_ENUMS(EMsgType)
private:
    QFile m_logfile;
public:
    Logger(const Logger&)=delete;
    Logger& operator=(const Logger&)=delete;
    Logger(QObject *parent);
    virtual ~Logger();
    virtual Logger& addMsg(const QString& msg
                         , EMsgType msgType = EMsgType::Plain
                         , const QString& postMsg="");
    // ---------------------------------------------------------
    static void msg(const QString& msg, const char* file, int line);
    static void info(const QString& msg, const char* file, int line);
    static void error(const QString& msg, const char* file, int line);
    static void event(const QString& msg, const char* file, int line);
    static void warning(const QString& msg, const char* file, int line);
};



