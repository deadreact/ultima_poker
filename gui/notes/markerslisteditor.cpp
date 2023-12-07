//#include <precompiled.h>
#include "markerslisteditor.h"
#include "ui_markerslisteditor.h"
#include <qevent.h>
#include <qcolordialog.h>

MarkersListEditor::MarkersListEditor(QWidget *parent)
    : EntityWidget(parent)
    , m_ui(new Ui::MarkersListEditor)
{
    m_ui->setupUi(this);

    connect(m_ui->lineEdit, &QLineEdit::textChanged, this, &MarkersListEditor::setText);
}

MarkersListEditor::~MarkersListEditor()
{
    delete m_ui;
}

QColor MarkersListEditor::color() const
{
    return m_color;
}

QString MarkersListEditor::text() const
{
    return m_text;
}

void MarkersListEditor::setColor(const QColor &color)
{
    if (m_color == color)
        return;

    static const QString pattern("QAbstractButton { background-color: rgba(%0, %1, %2, %3); }");

    m_color = color;
    m_ui->bttn_color->setStyleSheet(pattern.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
    update();
    emit colorChanged(color);
}

void MarkersListEditor::setText(const QString &text)
{
    if (m_text == text)
        return;

    m_text = text;
    if (m_ui->lineEdit->text() != text) {
        m_ui->lineEdit->setText(text);
    }
    emit textChanged(text);
}

void MarkersListEditor::moveEvent(QMoveEvent *e)
{
    if (e->pos().x() > 0) {
        move(0, e->pos().y()+2);
    }
}
void MarkersListEditor::actionEvent(QActionEvent *e)
{
    if (e && e->type() == QEvent::ActionChanged) {

    }
}

void MarkersListEditor::on_bttn_color_clicked()
{
    QColor col = QColorDialog::getColor();
    if (col.isValid()) {
        setColor(col);
    }
}
