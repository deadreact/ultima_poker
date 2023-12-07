#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QEasingCurve>

class QPropertyAnimation;

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT
private:
    Ui::Form *m_ui;
    QPropertyAnimation* m_eff;
    QEasingCurve::Type m_easing;
public:
    explicit Form(QWidget *parent = 0);
    ~Form();
    QRect destinationValue() const;
    void setDestinationValue(const QRect& rc) const;
protected:
    void changeEvent(QEvent *e);

private slots:
    void updateDirections();
    void updatePreview();
    void setEasing(QEasingCurve::Type);
    void onTargetGeometryChanged();
    void on_btn_go_clicked();
    void on_input_rotation_valueChanged(int value);
};

#endif // FORM_H
