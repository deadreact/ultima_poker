#pragma once

#include <QBasicTimer>
#include <render/basic.h>
#include "render/spritebuilder.h"
#include "render/camera.h"
#include <tools/types_fwd.hpp>


#ifndef NO_OPENGL
#include <QGLWidget>
#include <QGLFunctions>
#include <QGLFramebufferObject>
#include <QGLShaderProgram>
using GLWidgetParent = QGLWidget;
class GLWidget : public GLWidgetParent, protected QGLFunctions
#else
#include <QLabel>
using GLWidgetParent = QLabel;
class GLWidget : public GLWidgetParent
#endif // !NO_OPENGL
{
    Q_OBJECT

    Q_PROPERTY(QStringList resourcePathes READ resourcePathes WRITE setResourcePathes NOTIFY resourcePathesChanged)
private:
    QBasicTimer m_timer;
#ifndef NO_OPENGL
    QGLFramebufferObject* m_fbo;
#endif // !NO_OPENGL
    Camera m_camera;
    QMatrix4x4 m_projection;
    QMap<QString, std::pair<gbp_u16, uint> > m_textureMap;
    QList<RenderedObject*> m_renderedObjects;

    QPoint m_mouseOffset;
    float m_angleX, m_angleY;
    QStringList m_resourcePathes;
    bool m_needUpdate;
    bool m_renderToFrameBuffer;
signals:
    void resourcePathesChanged(const QStringList& resourcePathes);
    void renderedToImage(const QImage&);
public:
    explicit GLWidget(QWidget *parent = 0);
    virtual ~GLWidget();

    qreal fov() const;
    qreal nearPlane() const;
    qreal farPlane() const;

    bool isRenderToFrameBuffer() const;
    QList<RenderedObject*> renderedObjects() const { return m_renderedObjects; }
public slots:
    void setResourcePathes(const QStringList& resourcePathes);
    void setFov(qreal fov);
    void setNearPlane(qreal nearPlane);
    void setFarPlane(qreal farPlane);
    void setCameraRotation(const QQuaternion& quat);
    void setCameraTranslation(const QVector3D& vec);

    void glEnableDepthTest(bool enable);
    void glEnableCullFace(bool enable);

    void setRenderToFrameBuffer(bool toFbo);
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

    void updateProjection(float aspect);
    // QOpenGLWidget interface

#ifndef NO_OPENGL
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
#endif // !NO_OPENGL
protected:
    void initBufferObjects();
    void initShaders();
    void initTextures();
    void initChildren();
public:
    void timerEvent(QTimerEvent *e);
#ifndef NO_OPENGL
    void updateTexture(const QString &path, QGLShaderProgram *shaderProgram);
#else
    void updateTexture(const QString &path);
#endif // !NO_OPENGL
    void initRenderObject(RenderedObject* rObject);
    void doRender(const RenderedObject *rObject);
    void updateContent();
    const QStringList &resourcePathes() const;


    // QWidget interface
    void updateRendering();
protected:
    virtual void paintEvent(QPaintEvent *event) override;
};

