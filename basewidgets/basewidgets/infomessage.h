#pragma once

#include "basewidgets_global.h"
#include <QLabel>

class BASEWIDGETS_DLL InfoMessage : public QLabel
{
    Q_OBJECT

    Q_PROPERTY(QStringList messages READ messages WRITE setMessages)
    Q_PROPERTY(QStringList styles READ styles WRITE setStyles)
    Q_PROPERTY(int currentMsgIndex READ currentMsgIndex WRITE setCurrentMsgIndex NOTIFY currentMsgIndexChanged RESET resetCurrentMsgIndex)
    Q_PROPERTY(int defaultMsgIndex READ defaultMsgIndex WRITE setDefaultMsgIndex)
private:
    std::pair<QStringList, QStringList> m_messagesAndStyles;
//    QStringList m_styles;

    int m_currentMsgIndex;
    int m_defaultMsgIndex;
public:
    Q_INVOKABLE InfoMessage(QWidget* parent=0);

    const QStringList& messages() const;
    const QStringList& styles() const;
    int currentMsgIndex() const;

    const QString& msg(int index) const;
    const QString& style(int index) const;

    bool isIndexValid(int index) const;
    int defaultMsgIndex() const;
protected:
    virtual void changeEvent(QEvent *) override;
protected slots:
    virtual void updateMessage();
public slots:

    void setMessages(const QStringList &infolst);
    void setStyles(const QStringList& styles);

    void setMessage(int index, const QString& msg, const QString& style=QString());
    void addMessage(const QString& msg, const QString& style=QString());
    void removeMessage(int index);
    void setCurrentMsgIndex(int currentMsgIndex);
    void resetCurrentMsgIndex();
    void setDefaultMsgIndex(int index);

signals:
    void currentMsgIndexChanged(int currentMsgIndex);
};
