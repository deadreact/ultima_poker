#include <precompiled.h>
#include "ColorMatrixEditor.h"
#include "ui_ColorMatrixEditor.h"
#include <render/renderedobject.h>
#include <core/colortransform.h>
#include <PokerApplication.h>

ColorMatrixEditor::ColorMatrixEditor(QWidget *parent) :
    QGroupBox(parent),
    m_ui(new Ui::ColorMatrixEditor)
{
    m_ui->setupUi(this);

    connect(m_ui->rR,   static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.r.R) = val;   emit changed(m_data); });
    connect(m_ui->rG,   static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.r.G) = val;   emit changed(m_data); });
    connect(m_ui->rB,   static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.r.B) = val;   emit changed(m_data); });
    connect(m_ui->rA,   static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.r.A) = val;   emit changed(m_data); });
    connect(m_ui->gR,   static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.g.R) = val;   emit changed(m_data); });
    connect(m_ui->gG,   static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.g.G) = val;   emit changed(m_data); });
    connect(m_ui->gB,   static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.g.B) = val;   emit changed(m_data); });
    connect(m_ui->gA,   static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.g.A) = val;   emit changed(m_data); });
    connect(m_ui->bR,   static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.b.R) = val;   emit changed(m_data); });
    connect(m_ui->bG,   static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.b.G) = val;   emit changed(m_data); });
    connect(m_ui->bB,   static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.b.B) = val;   emit changed(m_data); });
    connect(m_ui->bA,   static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.b.A) = val;   emit changed(m_data); });
    connect(m_ui->aR,   static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.a.R) = val;   emit changed(m_data); });
    connect(m_ui->aG,   static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.a.G) = val;   emit changed(m_data); });
    connect(m_ui->aB,   static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.a.B) = val;   emit changed(m_data); });
    connect(m_ui->aA,   static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.a.A) = val;   emit changed(m_data); });
    connect(m_ui->offR, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.off.R) = val; emit changed(m_data); });
    connect(m_ui->offG, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.off.G) = val; emit changed(m_data); });
    connect(m_ui->offB, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.off.B) = val; emit changed(m_data); });
    connect(m_ui->offA, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this,  [this](double val){ m_data.value(m_data.off.A) = val; emit changed(m_data); });


}

ColorMatrixEditor::~ColorMatrixEditor()
{
    delete m_ui;
}

const ColorTransform &ColorMatrixEditor::transform() const
{
    return m_data;
}

const QMatrix4x4 &ColorMatrixEditor::matrix() const
{
    return m_data.matrix();
}

const QVector4D& ColorMatrixEditor::offset() const
{
    return m_data.offset();
}

float ColorMatrixEditor::value(int index) const
{
    return m_data.value(index);
}
float &ColorMatrixEditor::value(int index)
{
    return m_data.value(index);
}

void ColorMatrixEditor::setColorTransform(const ColorTransform &mat)
{
    m_ui->rR->setValue(mat.value(mat.r.R));
    m_ui->rG->setValue(mat.value(mat.r.G));
    m_ui->rB->setValue(mat.value(mat.r.B));
    m_ui->rA->setValue(mat.value(mat.r.A));

    m_ui->gR->setValue(mat.value(mat.g.R));
    m_ui->gG->setValue(mat.value(mat.g.G));
    m_ui->gB->setValue(mat.value(mat.g.B));
    m_ui->gA->setValue(mat.value(mat.g.A));

    m_ui->bR->setValue(mat.value(mat.b.R));
    m_ui->bG->setValue(mat.value(mat.b.G));
    m_ui->bB->setValue(mat.value(mat.b.B));
    m_ui->bA->setValue(mat.value(mat.b.A));

    m_ui->aR->setValue(mat.value(mat.a.R));
    m_ui->aG->setValue(mat.value(mat.a.G));
    m_ui->aB->setValue(mat.value(mat.a.B));
    m_ui->aA->setValue(mat.value(mat.a.A));

    m_ui->offR->setValue(mat.value(mat.off.R));
    m_ui->offG->setValue(mat.value(mat.off.G));
    m_ui->offB->setValue(mat.value(mat.off.B));
    m_ui->offA->setValue(mat.value(mat.off.A));

    emit changed(m_data);
}

void ColorMatrixEditor::resetMatrix()
{
//    m_matrix.setToIdentity();
    m_ui->rR->setValue(1.0);
    m_ui->rG->setValue(0.0);
    m_ui->rB->setValue(0.0);
    m_ui->rA->setValue(0.0);
    m_ui->gR->setValue(0.0);
    m_ui->gG->setValue(1.0);
    m_ui->gB->setValue(0.0);
    m_ui->gA->setValue(0.0);
    m_ui->bR->setValue(0.0);
    m_ui->bG->setValue(0.0);
    m_ui->bB->setValue(1.0);
    m_ui->bA->setValue(0.0);
    m_ui->aR->setValue(0.0);
    m_ui->aG->setValue(0.0);
    m_ui->aB->setValue(0.0);
    m_ui->aA->setValue(1.0);

    emit changed(m_data);
}

void ColorMatrixEditor::clearOffset()
{
    m_ui->offR->setValue(0.0);
    m_ui->offG->setValue(0.0);
    m_ui->offB->setValue(0.0);
    m_ui->offA->setValue(0.0);

    emit changed(m_data);
}

void ColorMatrixEditor::applyTo(RenderedObject *ro)
{
    ro->setColorTransform(matrix());
    ro->setColorOffset(offset());
}
void ColorMatrixEditor::on_bttn_restore_clicked()
{
    pApp->db().testDataStream();
    if (!m_ui->input_stateName->text().isEmpty()) {
        ColorTransform mat;
        pApp->db().restoreMat(m_ui->input_stateName->text(), mat);
        setColorTransform(mat);
    }
}
