#pragma once

#include "notes_global.h"
#include <basewidgets/entitywidget.h>

namespace Ui {
class MarkersListEditor;
}

class NOTES_DLL MarkersListEditor : public EntityWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

private:
    Ui::MarkersListEditor *m_ui;
    QColor m_color;

    QString m_text;

public:
    explicit MarkersListEditor(QWidget *parent = nullptr);
    virtual ~MarkersListEditor();

    QColor color() const;
    QString text() const;

public slots:
    void setColor(const QColor& color);
    void setText(const QString& text);

signals:
    void colorChanged(const QColor& color);
    void textChanged(const QString& text);

protected:
    virtual void moveEvent(QMoveEvent *e) override;
    virtual void actionEvent(QActionEvent *e) override;

private slots:
    void on_bttn_color_clicked();
};

