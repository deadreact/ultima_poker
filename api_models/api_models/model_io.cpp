#include "model_io.hpp"
#include <qabstractitemmodel.h>
#include <qiodevice.h>
#include <qstandardpaths.h>
#include <qdir.h>
#include <QDebug>
#include <qtimer.h>

namespace
{
    QString getSettingsPath()
    {
        QString path = "";
        QStringList lst = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation);
        if (!lst.isEmpty()) {
            path = lst.first();
        }
        if (!QDir(path + QString("/usr_%0/").arg(qHash(gbp_i64(-1)))).exists()) {
            QDir().mkpath(path + QString("/usr_%0/").arg(qHash(gbp_i64(-1))));
        }
        //qDebug() << path;
        return path + "/";
    }
}

namespace ns_model
{
    gbp_i64 IOModel::sm_userId = -1;

    IOModel::IOModel(QObject *object)
        : m_destroyedMsg("destroyed instance of %0 at %1")
    {
        QTimer::singleShot(0, [this, object]{
            if (object) {
                //qDebug() << "created new instance of" << object->metaObject()->className() << "at" << QString("0x%0").arg(QString::number(quintptr(this), 16));
                m_destroyedMsg = m_destroyedMsg.arg(object->metaObject()->className()).arg(QString::number(quintptr(this), 16));
            } else {
                //qDebug() << "created new instance of IOModel at" << QString("0x%0").arg(QString::number(quintptr(this), 16));
                m_destroyedMsg = m_destroyedMsg.arg("IOModel").arg(QString::number(quintptr(this), 16));
            }
        });
    }

    IOModel::~IOModel()
    {
        //qDebug() << m_destroyedMsg;
    }

    QString IOModel::getPath()
    {
        static const QString path = getSettingsPath();
        return path + QString("usr_%0/").arg(qHash(sm_userId));
    }

    void IOModel::setUserId(gbp_i64 id) {
        if (id != sm_userId) {
            sm_userId = id;
            QDir().mkpath(getPath());
        }
    }

    gbp_i64 IOModel::userId() {
        return sm_userId;
    }
}
