#pragma once

#include <QStringListModel>

namespace ns_model
{
    class Chat : public QStringListModel
    {
        Q_OBJECT
    public:
        explicit Chat(QObject *parent = 0);
        void addMessage(const QString& senderName, const QString& msg);
    };

} //namespace ns_model
