#pragma once

#include "basewidgets_global.h"
#include <QtSvg/qsvgwidget.h>
#include <QUrl>
#include <QPixmap>
#include "entitywidget.h"

class QFile;
class QGraphicsDropShadowEffect;

class BASEWIDGETS_DLL SvgWidget : public EntityWidget
{
    Q_OBJECT
    Q_PROPERTY(QUrl sourceFile READ sourceFile WRITE setSourceFile NOTIFY sourceFileChanged RESET resetSourceFile)
    Q_PROPERTY(QByteArray sourceCode READ sourceCode WRITE setSourceCode NOTIFY sourceCodeChanged)
    Q_PROPERTY(bool shadowEnabled READ isShadowEnabled WRITE setShadowEnabled)
    Q_PROPERTY(qreal shadowBlurRadius READ shadowBlurRadius WRITE setShadowBlurRadius NOTIFY shadowBlurRadiusChanged DESIGNABLE isShadowEnabled)
    Q_PROPERTY(QPointF shadowOffset READ shadowOffset WRITE setShadowOffset NOTIFY shadowOffsetChanged DESIGNABLE isShadowEnabled)
    Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor NOTIFY shadowColorChanged DESIGNABLE isShadowEnabled)

    QPixmap m_framebuffer;
    QUrl m_sourceFile;
    QByteArray m_sourceCode;
    QFile* m_file;

    bool m_isUserSetsSourceCode;
    QScopedPointer<QSvgRenderer> m_svgRenderer;
    QScopedPointer<QGraphicsDropShadowEffect> m_eff;

public:
    Q_INVOKABLE SvgWidget(QWidget *parent = nullptr);
    virtual ~SvgWidget();
    QUrl sourceFile() const;
    QByteArray sourceCode() const;

    QSvgRenderer* renderer() const;
#ifndef NOT_A_PLUGIN
    static bool containerInEditorEnabled();
#endif // !NOT_A_PLUGIN
public slots:
    void setSourceFile(const QUrl &sourceFile);
    void resetSourceFile();

    void setSourceCode(const QByteArray &sourceCode);
    inline void setSourceCode(const char* sourceCode) { setSourceCode(QByteArray(sourceCode)); }
    inline void setSourceCode(QString sourceCode) { setSourceCode(sourceCode.toLatin1()); }
    void resetSourceCode();
    void setShadowEnabled(bool shadowEnabled);
    void setShadowBlurRadius(qreal shadowBlurRadius);
    void setShadowOffset(const QPointF& shadowOffset);
    void setShadowColor(const QColor& col);

private slots:
    void renderToFramebuffer();
protected:
    virtual void paintEvent(QPaintEvent* ev) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void initPainter(QPainter *p) const override;
signals:
    void sourceFileChanged(QUrl sourceFile);
    void sourceCodeChanged(QByteArray sourceCode);
    void doubleClicked();

    void shadowBlurRadiusChanged(qreal);
    void shadowOffsetChanged(QPointF);
    void shadowColorChanged(QColor);
public:
    virtual QSize sizeHint() const override;

    bool isShadowEnabled() const;
    qreal shadowBlurRadius() const;
    QPointF shadowOffset() const;
    QColor shadowColor() const;

protected:
};

