#pragma once

#include <QColor>
#include <QPixmap>


class SettingsDatabase
{
private:
    struct PrivateData;
    PrivateData* m_dPtr;

    SettingsDatabase();
    virtual ~SettingsDatabase();
public:
    static const QStringList &names();
    static QString name(int index);
    static const QMap<QString, QVariantList> &values();
    static QVariantList values(const QString& key);
    static QVariant value(const QString& key, int index);


    static QVariantMap values(const QStringList& namesLst);
};
