#pragma once

#include "basewidgets_global.h"
#include "types.h"
#include <QBitmap>
#include <QUrl>
#include <QWidget>

bool isScrOpacityDesignable();

class BASEWIDGETS_DLL EntityWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QSize sizeIncrement READ sizeIncrement WRITE setSizeIncrement DESIGNABLE false)
    Q_PROPERTY(QSize baseSize READ baseSize WRITE setBaseSize DESIGNABLE false)
    Q_PROPERTY(QPalette palette READ palette WRITE setPalette DESIGNABLE false)
    Q_PROPERTY(QFont font READ font WRITE setFont DESIGNABLE false)
    Q_PROPERTY(QString statusTip READ statusTip WRITE setStatusTip DESIGNABLE false)
    Q_PROPERTY(QString whatsThis READ whatsThis WRITE setWhatsThis DESIGNABLE false)
    Q_PROPERTY(QString accessibleName READ accessibleName WRITE setAccessibleName DESIGNABLE false)
    Q_PROPERTY(QString accessibleDescription READ accessibleDescription WRITE setAccessibleDescription DESIGNABLE false)
    Q_PROPERTY(bool autoFillBackground READ autoFillBackground WRITE setAutoFillBackground DESIGNABLE false)
    Q_PROPERTY(Qt::InputMethodHints inputMethodHints READ inputMethodHints WRITE setInputMethodHints DESIGNABLE false)

    Q_PROPERTY(bool fixedAspectRatio READ hasHeightForWidth WRITE setFixedAspectRatio)
    Q_PROPERTY(qreal aspectRatio READ aspectRatio WRITE setAspectRatio DESIGNABLE hasHeightForWidth)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(qreal scrOpacity READ scrOpacity WRITE setScrOpacity DESIGNABLE isScrOpacityDesignable)
    Q_PROPERTY(E::Blending blending READ blending WRITE setBlending)
    Q_PROPERTY(bool shape READ hasShape WRITE setShape)
    Q_PROPERTY(bool respondable READ isRespondable WRITE setRespondable NOTIFY respondableToggled)
    Q_PROPERTY(bool rendering READ isRendering WRITE setRendering NOTIFY renderingChanged)
    Q_PROPERTY(E::ParentPainting parentPainting READ parentPainting WRITE setParentPainting)
    Q_ENUMS(E::ParentPainting)


protected:
    QColor m_bgColor;
    qreal  m_opacity;
    qreal  m_scrOpacity;
    E::Blending m_blending;
    bool m_shape;
    bool m_rendering;
    E::ParentPainting m_parentPainting;
    bool m_fixedAspectRatio;
    qreal m_aspectRatio;
public:
    Q_INVOKABLE EntityWidget(QWidget *parent = 0);
    virtual void init() {}
    virtual bool event(QEvent *event) override;
    QColor backgroundColor() const;
    qreal opacity() const;
    E::Blending blending() const;
    bool hasShape() const;
    void setShape(bool shape);
    bool isRespondable() const;
    bool isRendering() const;

    qreal scrOpacity() const;

    E::ParentPainting parentPainting() const;
    void setParentPainting(E::ParentPainting);
signals:
    void entered();
    void leaved();
    void pressed();
    void released();
    void shown();
    void hidden();
    void focused();
    void unfocused();
    void respondableToggled(bool respondable);
    void backgroundColorChanged(const QColor& backgroundColor);
    void renderingChanged(bool rendering);

public slots:
    void setBackgroundColor(const QColor& backgroundColor);
    void setBlending(E::Blending blending);
    inline void setBlending(int index);

    virtual void setRespondable(bool);
    virtual void setRendering(bool);

    void setOpacity(qreal opacity);
    void setScrOpacity(qreal opacity);

    void setFixedAspectRatio(bool fixedAspectRatio) {
        if (m_fixedAspectRatio != fixedAspectRatio) {
            m_fixedAspectRatio = fixedAspectRatio;
            update();
        }
    }

    void setAspectRatio(qreal aspectRatio) {
        if (!qFuzzyCompare(m_aspectRatio, aspectRatio)) {
            m_aspectRatio = aspectRatio;
            update();
        }
    }

protected:
    virtual void paintEvent(QPaintEvent *e) override;

    // QPaintDevice interface
protected:
    virtual void initPainter(QPainter *p) const override;

    // QWidget interface
public:
    virtual int heightForWidth(int) const override;
    virtual bool hasHeightForWidth() const override;
    qreal aspectRatio() const;
};

inline void EntityWidget::setBlending(int index) {
    setBlending(E::Blending(index));
}
