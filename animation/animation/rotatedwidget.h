#pragma once

#include "animation_global.h"
#include <basewidgets/entitywidget.h>
#ifdef DESIGNER
    #include <tools/math_tools.hpp>
#endif //DESIGNER

class ANIMATION_DLL RotatedWidget : public EntityWidget {
    Q_OBJECT
    Q_PROPERTY(qreal rotationRad READ rotationRad WRITE setRotationRad NOTIFY rotationRadChanged DESIGNABLE false)
    Q_PROPERTY(QMatrix parentMatrix READ parentMatrix WRITE setParentMatrix)
#ifdef DESIGNER
    Q_PROPERTY(int rotationDeg READ rotationDeg WRITE setRotationDeg)
public:
    inline int rotationDeg() const
    {
        return qRadiansToDegrees(rotationRad());
    }
    inline void setRotationDeg(int rotationDeg)
    {
        setRotationRad(qDegreesToRadians(static_cast<double>(rotationDeg)));
    }
#endif //DESIGNER
private:
    struct PrivateData;
    PrivateData* m_dPtr;
    bool m_rttMode;
public:
    Q_INVOKABLE RotatedWidget(QWidget* parent = nullptr);
    virtual ~RotatedWidget();
    qreal rotationRad() const;
    QMatrix parentMatrix() const;
protected:
    virtual void initPainter(QPainter *p) const override;
    virtual void resizeEvent(QResizeEvent *e) override;
    virtual void paintEvent(QPaintEvent *e) override;
    virtual void renderChildren(QPainter* p);
public slots:
    void setRotationRad(qreal rotationRad);
    void setParentMatrix(const QMatrix& matrix);

signals:
    void rotationRadChanged(qreal rotationRad);
};
