#pragma once

#include <basewidgets/entitywidget.h>

namespace Ui {
class PasswordToolTip;
}

class PasswordToolTip : public EntityWidget
{
    Q_OBJECT

    Q_PROPERTY(int progress READ progress WRITE setProgress)
    Q_PROPERTY(QColor progressColor READ progressColor WRITE setProgressColor)
    Q_PROPERTY(int shortDescription READ shortDescription WRITE setShortDescription)
    Q_PROPERTY(QString description READ description WRITE setDescription)

public:
    explicit PasswordToolTip(QWidget *parent = 0);
    ~PasswordToolTip();
    int progress() const;
    QColor progressColor() const;
    QString shortDescriptionString() const;
    int shortDescription() const;
    QString description() const;
public slots:
    void updateStateByProgress(int value, int passlen = 7);
    void setProgress(int value);
    void setProgressColor(const QColor &color);
    void setShortDescription(int index);
    void setDescription(const QString& text);
private:
    Ui::PasswordToolTip *m_ui;
    int m_progress;
    QColor m_progressColor;
    QString m_shortDescription;
    QString m_description;

    // QWidget interface
protected:
    virtual void changeEvent(QEvent *e) override;
};
