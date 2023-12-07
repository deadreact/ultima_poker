#pragma once

#include "styles_global.h"
#include <QWidget>
#include <QProxyStyle>
#include <basewidgets/types.h>

void STYLES_DLL setShadowOnTexts(QWidgetList &lst);

//namespace ns_styles
//{
    class STYLES_DLL ShadowedTextStyle : public QProxyStyle
    {
        Q_OBJECT

        Q_PROPERTY(QPoint shadowOffset READ shadowOffset WRITE setShadowOffset)
        Q_PROPERTY(int shadowSizeDiffW READ shadowSizeDiffW WRITE setShadowSizeDiffW)
        Q_PROPERTY(int shadowSizeDiffH READ shadowSizeDiffH WRITE setShadowSizeDiffH)
        Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor)
        Q_PROPERTY(qreal textOpacity READ textOpacity WRITE setTextOpacity)
        Q_PROPERTY(E::Blending blending READ blending WRITE setBlending NOTIFY blendingChanged)
    private:
        QPoint m_shadowOffset;
        int m_shadowSizeDiffW;
        int m_shadowSizeDiffH;
        QColor m_shadowColor;
        qreal m_textOpacity;
        E::Blending m_blending;
    public:
        ShadowedTextStyle(QStyle* style = nullptr);
        void setShadowOffset(const QPoint &shadowOffset);
        inline void setShadowOffset(int dx, int dy);
        inline void setShadowOffsetX(int dx);
        inline void setShadowOffsetY(int dy);
        void setShadowColor(const QColor &shadowColor);
        inline void setShadowColor(int r, int g, int b, int a = 255);
        inline void setShadowOpacity(qreal opacity);

        void setTextOpacity(qreal opacity);
        qreal textOpacity() const;
    public:
        virtual ~ShadowedTextStyle() {}

        virtual void drawItemText( QPainter *painter
                                 , const QRect &rect
                                 , int flags
                                 , const QPalette &pal
                                 , bool enabled
                                 , const QString &text
                                 , QPalette::ColorRole textRole/* = QPalette::NoRole*/) const override;
        QPoint shadowOffset() const;
        inline int shadowOffsetX() const;
        inline int shadowOffsetY() const;
        QColor shadowColor() const;
        inline int shadowOpacity() const;

        E::Blending blending() const;
        int shadowSizeDiffW() const;
        int shadowSizeDiffH() const;

    public slots:
        void setBlending(E::Blending blending);

        void setShadowSizeDiffW(int delta);
        void setShadowSizeDiffH(int delta);
        inline void setShadowSizeDiff(int deltaW, int deltaH);

    signals:
        void blendingChanged(E::Blending blending);
    };

    inline void ShadowedTextStyle::setShadowOffset(int dx, int dy) { setShadowOffset(QPoint(dx, dy));}
    inline void ShadowedTextStyle::setShadowOffsetX(int dx) { setShadowOffset(QPoint(dx, shadowOffsetY()));}
    inline void ShadowedTextStyle::setShadowOffsetY(int dy) { setShadowOffset(QPoint(shadowOffsetX(), dy));}
    inline void ShadowedTextStyle::setShadowColor(int r, int g, int b, int a) { setShadowColor(QColor(r, g, b, a)); }
    inline void ShadowedTextStyle::setShadowOpacity(qreal opacity) { setShadowColor(shadowColor().red(), shadowColor().green(), shadowColor().blue(), opacity * 255); }
    inline int ShadowedTextStyle::shadowOffsetX() const { return shadowOffset().x(); }
    inline int ShadowedTextStyle::shadowOffsetY() const { return shadowOffset().y(); }
    inline int ShadowedTextStyle::shadowOpacity() const { return shadowColor().alpha(); }

    inline void ShadowedTextStyle::setShadowSizeDiff(int deltaW, int deltaH) {
        setShadowSizeDiffW(deltaW);
        setShadowSizeDiffH(deltaH);
    }

    ShadowedTextStyle*
#ifdef Q_OS_WIN
    STYLES_DLL
#endif
    setTextDropShadowStyle(QWidget* w, const QPoint& offset = QPoint(1, 1), const QColor& color = QColor(0, 0, 0, 100), E::Blending blending = E::Blending::Normal);
    ShadowedTextStyle*
#ifdef Q_OS_WIN
    STYLES_DLL
#endif
    setTextDropShadowStyle(QWidget* w, const QRect& shadowArea, const QColor& color = QColor(0, 0, 0, 100), E::Blending blending = E::Blending::Normal);
//} //namespace ns_styles

