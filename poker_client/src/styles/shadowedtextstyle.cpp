#ifdef NOT_A_PLUGIN
    #include <precompiled.h>
#endif //NOT_A_PLUGIN
#include "shadowedtextstyle.h"
#include <QAbstractButton>
#include <QGraphicsEffect>
#include <qlabel.h>
#include <QPainter>
#include <basewidgets/util.h>

void addShadow(QWidget* w, qreal xOffset=0., qreal yOffset=0.8, qreal blurRadius=3)
{
    QGraphicsDropShadowEffect *eff = new QGraphicsDropShadowEffect(w);
    eff->setOffset(xOffset, yOffset);
    eff->setEnabled(true);
    eff->setColor(QColor(0, 0, 0, 140));
    eff->setBlurRadius(blurRadius);
    w->setGraphicsEffect(eff);
}


void setShadowOnTexts(QWidgetList &lst) {

    static auto splitBttnTextToLabel = [](QAbstractButton* bttn)
    {
        QLabel* bttn_text = new QLabel(bttn->text(), bttn);
        //            bttn_text->setFont(bttn->font());
        bttn_text->setAttribute(Qt::WA_TransparentForMouseEvents);
        bttn_text->setGeometry(bttn->geometry());
        bttn_text->setAlignment(Qt::AlignCenter);
        bttn->setText("");

        bttn->connect(bttn, &QAbstractButton::pressed, bttn_text, [bttn_text]{
            bttn_text->setStyleSheet("QLabel { color: rgba(255, 255, 255, 128) }");
            bttn_text->move(bttn_text->x(), bttn_text->y()+1);
        });

        bttn->connect(bttn, &QAbstractButton::released, bttn_text, [bttn_text]{
            bttn_text->setStyleSheet("");
            bttn_text->move(bttn_text->x(), bttn_text->y()-1);
        });

        return bttn_text;
    };

    for (QWidget* w : lst) {
        if (QAbstractButton* bttn = qobject_cast<QAbstractButton*>(w)) {
            w = splitBttnTextToLabel(bttn);
        }
        addShadow(w);
    }
}

//namespace ns_styles
//{

    ShadowedTextStyle::ShadowedTextStyle(QStyle *style)
        : QProxyStyle(style)
        , m_shadowOffset(1, 1)
        , m_shadowSizeDiffW(0)
        , m_shadowSizeDiffH(0)
        , m_shadowColor(Qt::black)
        , m_textOpacity(1.0)
        , m_blending(E::Blending::Normal)
    {}

     void ShadowedTextStyle::drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
    {
        QWidget* widget = dynamic_cast<QWidget*>(painter->device());
        if (widget && textRole != QPalette::ToolTipBase && textRole != QPalette::ToolTipText)
        {
            QPalette palShadow(pal);
            QPalette palText(pal);
            QRect rc = QRect(rect.x() + m_shadowOffset.x(), rect.y() + m_shadowOffset.y(), rect.width() + m_shadowSizeDiffW, rect.height() + m_shadowSizeDiffH);// rect.adjusted(m_shadowOffset.x(), m_shadowOffset.y(), m_shadowOffset.x(), m_shadowOffset.y());
            QColor col(shadowColor());
            if (!enabled) {
                col.setAlpha(70);
            }
            palShadow.setColor(textRole, col);
            auto compMode = painter->compositionMode();
            ns_util::setBlendingOption(m_blending, *painter);
            QProxyStyle::drawItemText(painter, rc, flags, palShadow, enabled, text, textRole);
            painter->setCompositionMode(compMode);

            QColor col2(pal.color(textRole));
            col2.setAlphaF(col2.alphaF() * m_textOpacity);
            palText.setColor(textRole, col2);
            QProxyStyle::drawItemText(painter, rect, flags, palText, enabled, text, textRole);
        } else {
            QProxyStyle::drawItemText(painter, rect, flags, pal, enabled, text, textRole);
        }
    }

    QPoint ShadowedTextStyle::shadowOffset() const
    {
        return m_shadowOffset;
    }

    QColor ShadowedTextStyle::shadowColor() const
    {
        return m_shadowColor;
    }

    E::Blending ShadowedTextStyle::blending() const
    {
        return m_blending;
    }

    int ShadowedTextStyle::shadowSizeDiffW() const
    {
        return m_shadowSizeDiffW;
    }

    int ShadowedTextStyle::shadowSizeDiffH() const
    {
        return m_shadowSizeDiffH;
    }

    void ShadowedTextStyle::setBlending(E::Blending blending)
    {
        if (m_blending == blending)
            return;

        m_blending = blending;
        emit blendingChanged(blending);
    }

    void ShadowedTextStyle::setShadowSizeDiffW(int delta)
    {
        m_shadowSizeDiffW = delta;
    }

    void ShadowedTextStyle::setShadowSizeDiffH(int delta)
    {
        m_shadowSizeDiffH = delta;
    }

    void ShadowedTextStyle::setShadowOffset(const QPoint &shadowOffset)
    {
        m_shadowOffset = shadowOffset;
    }

    void ShadowedTextStyle::setShadowColor(const QColor &shadowColor)
    {
        m_shadowColor = shadowColor;
    }

    void ShadowedTextStyle::setTextOpacity(qreal opacity)
    {
        if (!qFuzzyCompare(m_textOpacity, opacity)) {
            m_textOpacity = opacity;
        }
    }

    qreal ShadowedTextStyle::textOpacity() const
    {
        return m_textOpacity;
    }

    ShadowedTextStyle* setTextDropShadowStyle(QWidget *w, const QPoint &offset, const QColor &color, E::Blending blending)
    {
        ShadowedTextStyle* dropShadowStyle = new ShadowedTextStyle(w->style());
        dropShadowStyle->setShadowOffset(offset);
        dropShadowStyle->setShadowSizeDiff(0, 0);
        dropShadowStyle->setShadowColor(color);
        dropShadowStyle->setBlending(blending);
        w->setStyle(dropShadowStyle);

        w->connect(w, &QWidget::destroyed, dropShadowStyle, &ShadowedTextStyle::deleteLater);

        return dropShadowStyle;
    }

    ShadowedTextStyle* setTextDropShadowStyle(QWidget *w, const QRect &shadowArea, const QColor &color, E::Blending blending)
    {
        ShadowedTextStyle* dropShadowStyle = new ShadowedTextStyle(w->style());
        dropShadowStyle->setShadowOffset(shadowArea.topLeft());
        dropShadowStyle->setShadowSizeDiff(shadowArea.width() - w->width(), shadowArea.height() - w->height());
        dropShadowStyle->setShadowColor(color);
        dropShadowStyle->setBlending(blending);
        w->setStyle(dropShadowStyle);

        w->connect(w, &QWidget::destroyed, dropShadowStyle, &ShadowedTextStyle::deleteLater);

        return dropShadowStyle;
    }
//} //namespace ns_styles
