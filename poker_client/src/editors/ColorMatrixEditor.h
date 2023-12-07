#pragma once
#include <QGroupBox>
#include <qmatrix4x4.h>
#include <qvector4d.h>

#include <core/colortransform.h>

namespace Ui {
class ColorMatrixEditor;
}
class RenderedObject;

class ColorMatrixEditor : public QGroupBox
{
    Q_OBJECT
    Q_PROPERTY(QMatrix4x4 matrix READ matrix)
    Q_PROPERTY(QVector4D offset READ offset)
private:
    Ui::ColorMatrixEditor *m_ui;
    ColorTransform m_data;
public:
    explicit ColorMatrixEditor(QWidget *parent = nullptr);
    ~ColorMatrixEditor();
    const ColorTransform &transform() const;
    const QMatrix4x4 &matrix() const;
    const QVector4D &offset() const;
    float value(int index) const;
    float &value(int index);
public slots:
    void setColorTransform(const ColorTransform& mat);
    void resetMatrix();
    void clearOffset();
    void applyTo(RenderedObject* ro);
signals:
    void changed(const ColorTransform&);

private slots:
    void on_bttn_restore_clicked();
};

