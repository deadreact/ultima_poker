#include <QVariantMap>
#include <precompiled.h>
#include <qdir.h>
#include <qmap.h>
#include "SettingsDatabase.h"

struct SettingsDatabase::PrivateData
{
    PrivateData()
    {
    }
};

namespace
{
    QMap<QString, QVariantList> createData()
    {
        QMap<QString, QVariantList> values;
        QVariantList lstColors;
        lstColors << QColor(0xb0bec5) << QColor(Qt::darkCyan) << QColor(Qt::darkMagenta) << QColor(Qt::darkGreen);
        values.insert(SettingsDatabase::name(0), lstColors);
        values.insert(SettingsDatabase::name(1), lstColors);

        QVariantList lstDeck;
        QVariantList lstShirt;

        for (int i = 0; i < 4; i++)
        {
            QDir dir(QString(":/textures/svg/cards_0%0/").arg(i));
            QByteArrayList deckData;
            for (const QString& filename: dir.entryList()) {
                QFile f(dir.filePath(filename));
                if (f.open(QIODevice::ReadOnly)) {
                    deckData << f.readAll();
                }
            }
            lstDeck << QVariant::fromValue(deckData);
            QFile f(QString(":/textures/svg/shirts/0%0.svg").arg(i));
            if (f.open(QIODevice::ReadOnly)) {
                lstShirt << f.readAll();
            }
        }


        values.insert(SettingsDatabase::name(2), lstDeck);
        values.insert(SettingsDatabase::name(3), lstShirt);
        values.insert(SettingsDatabase::name(4), QVariantList() << "default" << "blue");

        return values;
    }
}


SettingsDatabase::SettingsDatabase()
    : m_dPtr(new PrivateData)
{}

SettingsDatabase::~SettingsDatabase()
{
    delete m_dPtr;
}

const QStringList& SettingsDatabase::names()
{
    static const QStringList namesLst = QStringList() << "feltColor" << "background" << "deck" << "shirt" << "theme";// << "flags";
    return namesLst;
}

QString SettingsDatabase::name(int index) {
    return names().size() > index ? names().at(index) : QString();
}

const QMap<QString, QVariantList>& SettingsDatabase::values()
{
    static const QMap<QString, QVariantList> data = ::createData();
    return data;
}

QVariantList SettingsDatabase::values(const QString &key)
{
    return values().value(key);
}

QVariant SettingsDatabase::value(const QString &key, int index)
{
    return values(key).value(index, QVariant());
}

QVariantMap SettingsDatabase::values(const QStringList &namesLst)
{
    QVariantMap map;
    for (const QString& name: namesLst) {
        map.insert(name, value(name, 0));
    }
    return map;
}
