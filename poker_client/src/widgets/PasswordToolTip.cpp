#include <precompiled.h>
#include "PasswordToolTip.h"
#include "ui_PasswordToolTip.h"

#include <QDialog>

PasswordToolTip::PasswordToolTip(QWidget *parent)
    : EntityWidget(parent)
    , m_ui(new Ui::PasswordToolTip)
{
    m_ui->setupUi(this);
    m_ui->triangle->raise();
//    setWindowFlags(/*Qt::Dialog | Qt::FramelessWindowHint*/);

//    QDialog dialog(this);
//    dialog.exec();
}

PasswordToolTip::~PasswordToolTip()
{
    delete m_ui;
}

void PasswordToolTip::setProgress(int value)
{
    if (value != m_ui->progressBar->value()) {
        m_ui->progressBar->setValue(value);

    }
}

void PasswordToolTip::setProgressColor(const QColor& color)
{
    if (m_progressColor != color) {
        static const QString styleSheetPattern = "QProgressBar::chunk { background-color: rgba(%0, %1, %2, %3);}";
        m_progressColor = color;
        m_ui->progressBar->setStyleSheet(styleSheetPattern.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
    }
}

void PasswordToolTip::setShortDescription(int index)
{
    m_ui->msg_reason->setCurrentMsgIndex(index);
}

void PasswordToolTip::setDescription(const QString &text)
{
    m_ui->desc_password_strenght->setText(text);
}

int PasswordToolTip::progress() const
{
    return m_ui->progressBar->value();
}

QColor PasswordToolTip::progressColor() const
{
    return m_progressColor;
}

QString PasswordToolTip::shortDescriptionString() const
{
    return m_ui->msg_reason->text();
}

int PasswordToolTip::shortDescription() const
{
    return m_ui->msg_reason->currentMsgIndex();
}

QString PasswordToolTip::description() const
{
    return m_ui->desc_password_strenght->text();
}

void PasswordToolTip::updateStateByProgress(int value, int passlen)
{
    setProgress(value);
    setProgressColor(QColor((100 - value)*2.55f, value*2.55f, 0, 255));
    setDescription(tr("@text_password_description"));

    if (passlen > 0 && passlen < 8) {
        value = std::min(value, 33);
        setShortDescription(0);
    }
    else if (value < 37)
    {
        if (value==0) {
            setShortDescription(1);
        } else {
            setShortDescription(2);
        }
    } else if (value < 60) {
        setShortDescription(3);
    } else if (value < 80) {
        setShortDescription(4);
    } else {
        setShortDescription(5);
    }

}


void PasswordToolTip::changeEvent(QEvent *e)
{
    if (e != nullptr && e->type() == QEvent::LanguageChange) {
        m_ui->retranslateUi(this);
        e->accept();
    }
    EntityWidget::changeEvent(e);
}
