#include "svgwidget.h"
#include <qsvgwidget.h>
#include <QSvgRenderer>
#include <qevent.h>
#include <qfile.h>
#include <qgraphicseffect.h>
#include <qpainter.h>
#include <qregularexpression.h>
#include "util.h"

#ifndef NOT_A_PLUGIN
    /*static*/ bool SvgWidget::containerInEditorEnabled()
    {
        QFile f("editor_config.ini");
        if (f.exists()) {
            if (f.open(QIODevice::ReadOnly)) {
                QByteArray data = f.readAll();
                f.close();
                return !QRegularExpression("SvgWidgetContainer false").match(data.data()).hasMatch();
            }
        }
        return true;
    }
#endif // !NOT_A_PLUGIN

QFile f("pluginsConfig.ini");


SvgWidget::SvgWidget(QWidget *parent)
  : EntityWidget(parent)
  , m_sourceFile(QUrl())
  , m_sourceCode("")
  , m_file(nullptr)
  , m_isUserSetsSourceCode(true)
  , m_svgRenderer(new QSvgRenderer(this))
  , m_eff(new QGraphicsDropShadowEffect)
//  , m_opacity(1.0)
{

    connect(renderer(), &QSvgRenderer::repaintNeeded, this, &SvgWidget::renderToFramebuffer);
//    connect(this, &SvgWidget::opacityChanged, this, &SvgWidget::renderToFramebuffer);

    setGraphicsEffect(m_eff.data());
    m_eff->setBlurRadius(13);
    m_eff->setOffset(0, 0);
    m_eff->setColor(Qt::black);
    m_eff->setEnabled(true);

    connect(m_eff.data(), SIGNAL(blurRadiusChanged(qreal)), this, SIGNAL(shadowBlurRadiusChanged(qreal)));
    connect(m_eff.data(), SIGNAL(offsetChanged(QPointF)), this, SIGNAL(shadowOffsetChanged(QPointF)));
    connect(m_eff.data(), SIGNAL(colorChanged(QColor)), this, SIGNAL(shadowColorChanged(QColor)));
}

SvgWidget::~SvgWidget()
{
    if (m_file != nullptr)
    {
        if (m_file->isOpen()) {
            m_file->close();
        }
        delete m_file;
    }
}

QUrl SvgWidget::sourceFile() const {
    return m_sourceFile;
}

QByteArray SvgWidget::sourceCode() const
{
    return m_sourceCode;
}

QSvgRenderer *SvgWidget::renderer() const {
    return m_svgRenderer.data();
}

void SvgWidget::setSourceFile(const QUrl &sourceFile)
{
    if (m_sourceFile == sourceFile)
        return;

    m_sourceFile = sourceFile;

    QString path = m_sourceFile.toString().replace("qrc:/", ":/");

    if (m_file != nullptr) {
        delete m_file;
    }
    m_file = new QFile(path);
    if (m_file->open(QIODevice::ReadOnly)) {
        QByteArray data = m_file->readAll();

        m_isUserSetsSourceCode = false;
        setSourceCode(data);
        m_isUserSetsSourceCode = true;
    }
    emit sourceFileChanged(sourceFile);
}

void SvgWidget::setSourceCode(const QByteArray &sourceCode)
{
    if (m_sourceCode == sourceCode)
        return;

    m_sourceCode = sourceCode;
    renderer()->load(sourceCode);
    if (m_isUserSetsSourceCode) {
        emit sourceCodeChanged(sourceCode);
    }
}

void SvgWidget::resetSourceFile(){
    setSourceFile(QUrl());
}

void SvgWidget::resetSourceCode() {
    setSourceCode(QByteArray());
}

void SvgWidget::setShadowEnabled(bool shadowEnabled)
{
    m_eff->setEnabled(shadowEnabled);
}

void SvgWidget::setShadowBlurRadius(qreal shadowBlurRadius)
{
    m_eff->setBlurRadius(shadowBlurRadius);
}

void SvgWidget::setShadowOffset(const QPointF& shadowOffset)
{
    m_eff->setOffset(shadowOffset);
}

void SvgWidget::setShadowColor(const QColor &col)
{
    m_eff->setColor(col);
}

void SvgWidget::renderToFramebuffer()
{
    m_framebuffer = QPixmap(size());
    m_framebuffer.fill(Qt::transparent);
    QPainter p;
    p.begin(&m_framebuffer);
    renderer()->render(&p);
    p.end();

    update();
}



void SvgWidget::paintEvent(QPaintEvent *ev)
{
    if (!isRendering()) {
        ev->ignore();
        return;
    }
    if (parentPainting() == E::ParentPainting::PaintParentBefore) {
        EntityWidget::paintEvent(ev);
    }

    QPainter p(this);
    p.setOpacity(scrOpacity());
    ns_util::setBlendingOption(blending(), p);
    p.drawPixmap(rect(), m_framebuffer);

    if (parentPainting() == E::ParentPainting::PaintParentAfter) {
        EntityWidget::paintEvent(ev);
    }
}


QSize SvgWidget::sizeHint() const {
    return renderer()->defaultSize();
}

bool SvgWidget::isShadowEnabled() const
{
    return m_eff->isEnabled();
}

qreal SvgWidget::shadowBlurRadius() const
{
    return m_eff->blurRadius();
}

QPointF SvgWidget::shadowOffset() const
{
    return m_eff->offset();
}

QColor SvgWidget::shadowColor() const
{
    return m_eff->color();
}

void SvgWidget::resizeEvent(QResizeEvent *event)
{
    EntityWidget::resizeEvent(event);
    renderToFramebuffer();
}


void SvgWidget::initPainter(QPainter *p) const
{
    EntityWidget::initPainter(p);
    p->setOpacity(scrOpacity());
    ns_util::setBlendingOption(blending(), *p);
}
