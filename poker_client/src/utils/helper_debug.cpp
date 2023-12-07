#include <precompiled.h>
#include "helper_debug.h"
#include <QDebug>
#include <PokerApplication.h>

DebugHelper::DebugHelper(QObject *parent)
    : QObject(parent)
    , m_lastCondition(true)
{

}

DebugHelper &DebugHelper::assertX(bool condition, const char *file, int line)
{
    static bool _dumpObjectInfo = false;
    static bool _dumpObjectTree = false;
    static QString objName = "";

    DEBUG.m_lastCondition = condition;
    if (!condition)
    {
        qDebug() << "ASSERTION " << (condition ? "ok" : "triggered");
        qCritical("   Loc: [%s:%i]", file, line);
        qDebug() << "stopped here" << (__FILE__) << (__LINE__);

        if (QObject* obj = pApp->findChild<QObject*>(objName))
        {
            if (_dumpObjectInfo) {
                obj->dumpObjectInfo();
            }
            if (_dumpObjectTree) {
                obj->dumpObjectTree();
            }
        }
    }

    return DEBUG;
}
