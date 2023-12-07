#include <precompiled.h>
#include "model_Chat.h"
#include <qpixmap.h>

namespace ns_model
{

    Chat::Chat(QObject *parent)
        : QStringListModel(parent)
    {}

    void Chat::addMessage(const QString &senderName, const QString &msg) {
        int row = rowCount();
        insertRow(row);
        setData(index(row), QString("\"%0\": %1\n").arg(senderName, msg), Qt::DisplayRole);
    }


} //namespace ns_model
