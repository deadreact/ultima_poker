#include "infomessage.h"
#include <QEvent>


//=======================================================
//=======================================================

InfoMessage::InfoMessage(QWidget *parent)
    : QLabel(parent)
    , m_currentMsgIndex(-1)
    , m_defaultMsgIndex(-1)
{
    hide();
}

const QStringList& InfoMessage::messages() const
{
    return m_messagesAndStyles.first;
}


int InfoMessage::currentMsgIndex() const
{
    return m_currentMsgIndex;
}

const QString &InfoMessage::msg(int index) const
{
    static const QString emptyString = "";
    if (messages().size() > index)
        return messages().at(index);
    //Logger::error("QStringList out of range!!", __FILE__, __LINE__);
    return emptyString;
}
const QString &InfoMessage::style(int index) const
{
    static const QString emptyString = "";
    if (messages().size() > index)
        return styles().at(index);
    //Logger::error("QStringList out of range!!", __FILE__, __LINE__);
    return emptyString;
}

bool InfoMessage::isIndexValid(int index) const
{
    return index >= 0 && index < messages().size();
}

int InfoMessage::defaultMsgIndex() const
{
    return m_defaultMsgIndex;
}

const QStringList &InfoMessage::styles() const
{
    return m_messagesAndStyles.second;
}


void InfoMessage::updateMessage()
{
    setHidden(!isIndexValid(m_currentMsgIndex));
    if (!isHidden()) {
        setStyleSheet(styles().at(m_currentMsgIndex));
        setText(messages().at(m_currentMsgIndex));
    }
}

void InfoMessage::setCurrentMsgIndex(int currentMsgIndex)
{
    if (m_currentMsgIndex == currentMsgIndex)
        return;

    m_currentMsgIndex = currentMsgIndex;
    updateMessage();
    emit currentMsgIndexChanged(currentMsgIndex);
}

void InfoMessage::resetCurrentMsgIndex()
{
    setCurrentMsgIndex(m_defaultMsgIndex);
}

void InfoMessage::setDefaultMsgIndex(int index)
{
    if (m_defaultMsgIndex != index) {
        if (m_currentMsgIndex == m_defaultMsgIndex) {
            m_defaultMsgIndex = index;
            resetCurrentMsgIndex();
        } else {
            m_defaultMsgIndex = index;
        }
    }
}

void InfoMessage::setStyles(const QStringList& styles)
{
    m_messagesAndStyles.second = styles;
    while (m_messagesAndStyles.first.size() > m_messagesAndStyles.second.size()) {
        m_messagesAndStyles.second.push_back("");
    }
    while (m_messagesAndStyles.first.size() < m_messagesAndStyles.second.size()) {
        m_messagesAndStyles.first.push_back("");
    }
}

void InfoMessage::setMessages(const QStringList& messages)
{
    m_messagesAndStyles.first = messages;
    while (m_messagesAndStyles.first.size() > m_messagesAndStyles.second.size()) {
        m_messagesAndStyles.second.push_back("");
    }
    while (m_messagesAndStyles.first.size() < m_messagesAndStyles.second.size()) {
        m_messagesAndStyles.first.push_back("");
    }
}

void InfoMessage::setMessage(int index, const QString& msg, const QString& style)
{
    while (m_messagesAndStyles.first.size() < index) {
        addMessage("", "");
    }
    if (m_messagesAndStyles.first[index] != msg)
    {
        m_messagesAndStyles.first[index] = msg;
        if (!style.isNull()) {
            m_messagesAndStyles.second[index] = style;
        }
        updateMessage();
    }
}

void InfoMessage::addMessage(const QString& msg, const QString& style)
{
    m_messagesAndStyles.first.push_back(msg);
    m_messagesAndStyles.second.push_back(style);
}

void InfoMessage::removeMessage(int index)
{
    int last = m_messagesAndStyles.first.size() - 1;
    m_messagesAndStyles.first[index] = msg(last);
    m_messagesAndStyles.second[index] = style(last);

    m_messagesAndStyles.first.pop_back();
    m_messagesAndStyles.second.pop_back();
}

void InfoMessage::changeEvent(QEvent *e)
{
    if (e != nullptr && e->type() == QEvent::LanguageChange) {
        updateMessage();
    }
}
