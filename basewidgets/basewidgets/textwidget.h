#pragma once

#include "basewidgets_global.h"
#include <QLabel>

class BASEWIDGETS_DLL TextWidget : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QStringList strings READ strings WRITE setStrings NOTIFY stringsChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
private:
    QStringList m_strings;
    QString m_text;
protected slots:
public:
    Q_INVOKABLE TextWidget(QWidget* parent = nullptr);
    virtual ~TextWidget();
    const QStringList& strings() const;
    const QString& string(int index) const;
    QString text() const;
    QString updatedText() const;
    virtual QSize sizeHint() const override;
public slots:
    void setStrings(const QStringList& strings);
    void setString(const QString& text, int index);
    void pushBack(const QString& text);
    void popBack();
    void setText(const QString &text);
    void updateText();

signals:
    void stringsChanged(const QStringList& strings);
    void textChanged(const QString& text);

};
