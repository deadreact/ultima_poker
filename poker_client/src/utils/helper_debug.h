#pragma once
#include <QObject>

class DebugHelper : public QObject
{
    Q_OBJECT
    bool m_lastCondition;
public:
    explicit DebugHelper(QObject *parent = 0);

    static DebugHelper& assertX(bool condition, const char* file, int line);

    template <typename T>
    DebugHelper& operator << (const T& val) {
        if (!m_lastCondition) {
            qDebug() << val;
        }
        return *this;
    }

};
