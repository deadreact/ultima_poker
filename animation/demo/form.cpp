#include "form.h"
#include "ui_form.h"
#include <animation/animation.h>
#include <QPainter>
#include <QPropertyAnimation>
//#include <animation/rotatedwidget.h>

namespace
{
    QRect operator-(const QRect rc1, const QRect& rc2) {
        return QRect(rc1.x()-rc2.x(), rc1.y()-rc2.y(), rc1.width()-rc2.width(), rc1.height()-rc2.height());
    }
    QRect operator+(const QRect rc1, const QRect& rc2) {
        return QRect(rc1.topLeft()+rc2.topLeft(), rc1.size()+rc2.size());
    }
    qreal deg2rad(qreal deg) {
        return (M_PI * deg)/180.0;
    }
}

Form::Form(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::Form)
    , m_eff(nullptr)
    , m_easing(QEasingCurve::Linear)
{
    m_ui->setupUi(this);

    connect(m_ui->easingcurvecombobox, SIGNAL(easingChanged(QEasingCurve::Type)), this, SLOT(setEasing(QEasingCurve::Type)));
    onTargetGeometryChanged();

    connect(m_ui->buttonGroup, static_cast<void(QButtonGroup::*)(QAbstractButton*, bool)>(&QButtonGroup::buttonToggled), this, [this]{
        QRect diff = m_ui->target->geometry() - destinationValue();
        setDestinationValue(m_ui->target->geometry() + diff);
    });

    connect(m_ui->dest_x, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
    connect(m_ui->dest_y, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
    connect(m_ui->dest_w, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
    connect(m_ui->dest_h, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
}

Form::~Form()
{
    delete m_ui;
}

QRect Form::destinationValue() const {
    return QRect(m_ui->dest_x->value(), m_ui->dest_y->value(), m_ui->dest_w->value(), m_ui->dest_h->value());
}

void Form::setDestinationValue(const QRect &rc) const
{
    m_ui->dest_x->setValue(rc.x());
    m_ui->dest_y->setValue(rc.y());
    m_ui->dest_w->setValue(rc.width());
    m_ui->dest_h->setValue(rc.height());
}

void Form::updatePreview()
{
    m_ui->destinationTargetPreview->setGeometry(destinationValue());
}

void Form::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Form::updateDirections()
{

}

void Form::setEasing(QEasingCurve::Type easing)
{
    m_easing = easing;
}

void Form::onTargetGeometryChanged()
{
    m_ui->curr_x->setValue(m_ui->target->x());
    m_ui->curr_y->setValue(m_ui->target->y());
    m_ui->curr_w->setValue(m_ui->target->width());
    m_ui->curr_h->setValue(m_ui->target->height());
}

void Form::on_btn_go_clicked()
{
    QRect newGeometry(m_ui->dest_x->value(), m_ui->dest_y->value(), m_ui->dest_w->value(), m_ui->dest_h->value());
    if (m_ui->target->geometry() != newGeometry) {
        m_eff = ns_anim::changeGeometry(m_ui->target, newGeometry, m_ui->input_duration->value());
        m_eff->setEasingCurve(m_easing);
        m_eff->setDirection(m_ui->bttn_forward->isChecked() ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
        connect(m_eff, &QPropertyAnimation::valueChanged, this, &Form::onTargetGeometryChanged);

        connect(m_eff, &QPropertyAnimation::finished, this, [this]{
            m_eff->targetObject()->setProperty(m_eff->propertyName(), m_eff->endValue());
            onTargetGeometryChanged();
            m_eff->deleteLater();
            m_eff = nullptr;
        });

        m_eff->start();
    }
}

void Form::on_input_rotation_valueChanged(int value) {
    if (value == -1) {
        m_ui->input_rotation->setValue(359);
    } else if (value == 360) {
        m_ui->input_rotation->setValue(0);
    } else {
        m_ui->target->setRotationRad(deg2rad(value));
    }
}
