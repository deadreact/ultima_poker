#include "validatedLineEdit.h"
#include "util.h"

#include <QMetaMethod>
#include <QPainter>
#include <qlabel.h>
#include <qsortfilterproxymodel.h>
#include <QStyledItemDelegate>

ValidatedLineEdit::ValidatedLineEdit(QWidget *parent)
    : QLineEdit(parent)
    , m_expressions()
    , m_styles()
    , m_acceptedExprMask(0)
{

}

QString ValidatedLineEdit::expression(int idx) const
{
    return m_expressions.size() > idx ? m_expressions.at(idx) : QString();
}

QString ValidatedLineEdit::style(int idx) const
{
    return m_styles.size() > idx ? m_styles.at(idx) : QString();
}

const QStringList& ValidatedLineEdit::expressions() const
{
    return m_expressions;
}

const QStringList& ValidatedLineEdit::styles() const
{
    return m_styles;
}

int ValidatedLineEdit::acceptedExprMask() const
{
    return m_acceptedExprMask;
}

void ValidatedLineEdit::setExpressions(const QStringList &expressions)
{
    m_expressions = expressions;
}

void ValidatedLineEdit::setStyles(const QStringList &styles)
{
    m_styles = styles;
}

void ValidatedLineEdit::updateAcceptedExpression()
{
    QString accumulatedStyleSheet = "";
    int idx = 0;
    for (int i = 0; i < m_expressions.size(); i++) {
        const QString& expr = m_expressions.at(i);
        if (QRegExp(expr).exactMatch(text())) {
            accumulatedStyleSheet += style(i);
            idx |= (1 << i);
        }
    }
    if (idx != m_acceptedExprMask) {
        m_acceptedExprMask = idx;
        setStyleSheet(accumulatedStyleSheet);

        emit acceptedExprMaskChanged(idx);
    }
}

void ValidatedLineEdit::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    emit focused();
}
void ValidatedLineEdit::focusOutEvent(QFocusEvent *e)
{
    QLineEdit::focusOutEvent(e);
    emit unfocused();
}
