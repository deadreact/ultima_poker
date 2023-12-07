#pragma once

#include "basewidgets_global.h"
//#include "../types.h"
#include <QLineEdit>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <functional>

class BASEWIDGETS_DLL ValidatedLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QStringList expressions READ expressions WRITE setExpressions)
    Q_PROPERTY(QStringList styles READ styles WRITE setStyles)
    Q_PROPERTY(int acceptedExprMask READ acceptedExprMask MEMBER m_acceptedExprMask NOTIFY acceptedExprMaskChanged)

    Q_PROPERTY(QString testString1 MEMBER m_testString1)
    Q_PROPERTY(QString testString2 MEMBER m_testString2)
    Q_PROPERTY(QString testString3 MEMBER m_testString3)
public:
//    enum class State
//    {
//        Empty = 0,
//        AcceptsMask,
//        Incorrect,
//        Correct
//    };

private:
    QString m_testString1 = "";
    QString m_testString2 = "";
    QString m_testString3 = "";
    QStringList m_expressions;
    QStringList m_styles;

    int m_acceptedExprMask;

public:
    Q_INVOKABLE ValidatedLineEdit(QWidget* parent=0);

    const QStringList& expressions() const;
    const QStringList& styles() const;

    QString expression(int idx) const;
    QString style(int idx) const;

    int acceptedExprMask() const;

public slots:

    void setExpressions(const QStringList& expressions);
    void setStyles(const QStringList& styles);

    void updateAcceptedExpression();
signals:
    Q_SIGNAL void acceptedExprMaskChanged(int acceptedExprMask);
    Q_SIGNAL void focused();
    Q_SIGNAL void unfocused();
protected:
    virtual void focusInEvent(QFocusEvent *e) override;
    virtual void focusOutEvent(QFocusEvent *e) override;
};
