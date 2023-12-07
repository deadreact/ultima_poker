//#ifdef NOT_A_PLUGIN
//    #include <precompiled.h>
//#endif // NOT_A_PLUGIN
#include <QDebug>
#include "textwidget.h"

namespace {
    const QString empty("");
}

TextWidget::TextWidget(QWidget *parent)
    : QLabel(parent)
    , m_text("")
{
    connect(this, &TextWidget::textChanged, this, &TextWidget::updateText);
    connect(this, &TextWidget::stringsChanged, this, &TextWidget::updateText);
}

TextWidget::~TextWidget() {}

const QStringList &TextWidget::strings() const
{
    return m_strings;
}

const QString &TextWidget::string(int index) const
{
    return (index < m_strings.size()) ? m_strings.at(index) : empty;
}

void TextWidget::updateText()
{
    QLabel::setText(updatedText());
}

void TextWidget::setStrings(const QStringList &strings)
{
    if (m_strings == strings)
        return;

    m_strings = strings;

//    processText();

    emit stringsChanged(strings);
}

void TextWidget::pushBack(const QString &text)
{
    m_strings.push_back(text);
}

void TextWidget::popBack()
{
    m_strings.pop_back();
}

void TextWidget::setString(const QString &text, int index)
{
    if (index < m_strings.size() && m_strings[index] == text)
        return;

    while (index >= m_strings.size()) {
        m_strings.append("");
    }
    m_strings[index] = text;
    emit stringsChanged(m_strings);
}

QString TextWidget::text() const {
    return m_text;
}

QString TextWidget::updatedText() const
{
    QString text = m_text;    

    if (text.length() > 0)
    {
     for (const QString& str: m_strings) {
         text = text.arg(str);
     }
    }

    return text;
}

void TextWidget::setText(const QString& text)
{
    if (m_text == text)
        return;

    m_text = text;
//    processText();

    emit textChanged(text);
}


QSize TextWidget::sizeHint() const
{
    return QSize(fontMetrics().width(updatedText()), fontMetrics().height());
}
