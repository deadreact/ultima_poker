#include <precompiled.h>
#include "utils/logger.h"
#include "SharedDataService.h"
#include <QDebug>

SharedDataService &shdService()
{
    return pApp->db().sharedDataService();
}

void SharedDataService::logWarning(const QString &msg, const char *file, int line)
{
    Logger::warning(msg.toLatin1().data(), file, line);
}

SharedDataService::SharedDataService(QObject *parent)
    : m_qobject(new QObject(parent))
{
    m_qobject->setObjectName("SharedDataHelper");
}

SharedDataService::~SharedDataService()
{
//    qDebug() << "~SharedDataService()";
    delete m_qobject;
}


